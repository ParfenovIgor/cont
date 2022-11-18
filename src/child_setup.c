#include "inc.h"

int get_number() {    
    FILE *file = fopen("run", "r");
    if (file == NULL)
        errExit("fopen-run");
    char *line = NULL;
    size_t len;
    ssize_t nread;
    int cnt = 0;
    nread = getline(&line, &len, file);
    line[nread - 1] = '\0';
    fclose(file);
    
    int d = atoi(line);
    return d;
}

int child_main(void *arg) {
    unshare(CLONE_NEWNS);

    char **args = arg;
    char new_root[1023];
    snprintf(new_root, 1023, "mnt/%s", args[0]);
    const char *put_old = "/oldrootfs";
    char path[1024];

    if (mount(NULL, "/", NULL, MS_REC | MS_PRIVATE, NULL) == -1)
        errExit("mount-MS_PRIVATE");
    if (mount(new_root, new_root, "bind", MS_BIND | MS_REC, "") == -1)
        errExit("mount-MS_BIND");
    snprintf(path, 1024, "%s/%s", new_root, put_old);
    mkdir(path, 0755);
    if (syscall(SYS_pivot_root, new_root, path) == -1)
        errExit("pivot_root");
    chdir("/");
    umount2(put_old, MNT_DETACH);
    rmdir(put_old);

    if (mount("proc", "/proc", "proc", 0, NULL) == -1)
        errExit("mount-proc");
    if (mount("sysfs", "/sys", "sysfs", 0, NULL) == -1)
        errExit("mount-sysfs");
    if (mount("none", "/tmp", "tmpfs", 0, NULL) == -1)
        errExit("mount-tmp");
    if (mount("none", "/dev", "tmpfs", MS_NOSUID | MS_STRICTATIME, NULL) == -1)
        errExit("mount-dev");
    
    char cmd[1024];
    sethostname(args[0], strlen(args[0]));
    for (int i = 0; i < 10; i++) {
        sleep(1);
        snprintf(cmd, 1024, "ip link set veth1%s up", args[0]);
        if (system(cmd) == 0)
            break;
        if (i == 9)
            errExit("ip link");
    }
    int d = get_number();
    snprintf(cmd, 1024, "ip addr add 172.16.0.%03d/24 dev veth1%s", d + 100, args[0]);
    printf("%s\n", cmd);
    system(cmd);
    snprintf(cmd, 1024, "route add default gw 172.16.0.100 veth1%s", args[0]);
    system(cmd);
    
    execvp(args[1], &args[1]);
    
    return 0;
};

