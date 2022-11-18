#include "inc.h"

#define CHILD_STACK_SIZE 1024 * 1024
static char child_stack[CHILD_STACK_SIZE];

int child_main(void *arg);
void mount_disk(char *argv[]);
void umount_disk(char *argv[]);

char* get_link() {
    FILE *file = fopen("link", "r");
    if (file == NULL)
        errExit("fopen-link");
    char *line = NULL;
    size_t len;
    ssize_t nread;
    int cnt = 0;
    nread = getline(&line, &len, file);
    line[nread - 1] = '\0';
    fclose(file);
    return line;
}

void setup_net(char *argv[]) {
    char cmd[1024];
    char *link = get_link();
    
    snprintf(cmd, 1024, "ip link add veth0%s type veth peer name veth1%s", argv[0], argv[0]);
    system(cmd);
    
    snprintf(cmd, 1024, "ip link set veth0%s up", argv[0]);
    system(cmd);
    
    snprintf(cmd, 1024, "brctl addbr br0%s", argv[0]);
    system(cmd);
    
    snprintf(cmd, 1024, "ip addr add dev br0%s 172.16.0.100/24", argv[0]);
    system(cmd);
    
    snprintf(cmd, 1024, "ip link set br0%s up", argv[0]);
    system(cmd);
    
    snprintf(cmd, 1024, "iptables -A FORWARD -i %s -o br0%s -j ACCEPT", link, argv[0]);
    system(cmd);
    
    snprintf(cmd, 1024, "iptables -A FORWARD -o %s -i br0%s -j ACCEPT", link, argv[0]);
    system(cmd);
    
    system("iptables -t nat -A POSTROUTING -s 172.16.0.0/16 -j MASQUERADE");
    
    snprintf(cmd, 1024, "brctl addif br0%s veth0%s", argv[0], argv[0]);
    system(cmd);
}

void remove_net(char *argv[]) {
    char cmd[1024];
    char *link = get_link();
    
    snprintf(cmd, 1024, "ip link set br0%s down", argv[0]);
    system(cmd);
    snprintf(cmd, 1024, "iptables -D FORWARD -i %s -o br0%s -j ACCEPT", link, argv[0]);
    system(cmd);
    snprintf(cmd, 1024, "iptables -D FORWARD -o %s -i br0%s -j ACCEPT", link, argv[0]);
    system(cmd);
    snprintf(cmd, 1024, "brctl delbr br0%s", argv[0]);
    system(cmd);
}

void connect_net(int pid, char *argv[]) {
    char ip_link_set[4096];
    snprintf(ip_link_set, 4096, "ip link set veth1%s netns %d", argv[0], pid);
    system(ip_link_set);
}

void push_number(int x) {    
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
    sprintf(line, "%d", d + x);
    
    file = fopen("run", "w");
    if (file == NULL)
        errExit("fopen-run");
    fprintf(file, "%s\n", line);
    fclose(file);
}

int run(char *argv[]) {
    setup_net(argv);
    mount_disk(argv);
    push_number(+1);
    
    int flags =
        CLONE_NEWNS |
        CLONE_NEWUTS |
        CLONE_NEWPID |
        CLONE_NEWIPC |
        CLONE_NEWNET |
        SIGCHLD;
    
    int pid = clone(child_main, child_stack + CHILD_STACK_SIZE, flags, argv);
    
    connect_net(pid, argv);
    
    waitpid(pid, NULL, 0);
    
    push_number(-1);
    umount_disk(argv);
    remove_net(argv);
    
    return 0;
}
