#include "inc.h"

void mount_disk(char *argv[]) {
    char cmd[1024];

    snprintf(cmd, 1024, "mkdir mnt/%s", argv[0]);
    system(cmd);
    
    snprintf(cmd, 1024, "losetup -f > loop/%s", argv[0]);
    if (system(cmd))
        errExit("losetup");

    snprintf(cmd, 1024, "loop/%s", argv[0]);
    char *loop_name = NULL;
    FILE *file = fopen(cmd, "r");
    size_t len;
    ssize_t nread;
    int cnt = 0;
    nread = getline(&loop_name, &len, file);
    loop_name[nread - 1] = '\0';
    fclose(file);
    
    snprintf(cmd, 1024, "losetup %s disk/%s", loop_name, argv[0]);
    if (system(cmd))
        errExit("losetup");
    snprintf(cmd, 1024, "mount -o loop %s mnt/%s", loop_name, argv[0]);
    if (system(cmd))
        errExit("mount");
}

void umount_disk(char *argv[]) {
    char cmd[1024];

    snprintf(cmd, 1024, "loop/%s", argv[0]);
    char *loop_name = NULL;
    FILE *file = fopen(cmd, "r");
    size_t len;
    ssize_t nread;
    int cnt = 0;
    nread = getline(&loop_name, &len, file);
    loop_name[nread - 1] = '\0';
    fclose(file);

    snprintf(cmd, 1024, "rm loop/%s", argv[0]);
    system(cmd);

    snprintf(cmd, 1024, "umount mnt/%s", argv[0]);
    if (system(cmd))
        errExit("umount");
    snprintf(cmd, 1024, "losetup -d %s", loop_name);
    if (system(cmd))
        errExit("losetup");
    
    snprintf(cmd, 1024, "rmdir mnt/%s", argv[0]);
    system(cmd);
}
