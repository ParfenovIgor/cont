#include "inc.h"

void mount_disk(char *argv[]);
void umount_disk(char *argv[]);

void pull(char *argv[]) {
    char cmd[1024];

    if (system("rm -rf tmp"))
        errExit("rm");
    if (system("mkdir tmp"))
        errExit("mkdir");

    snprintf(cmd, 1024, "docker export $(docker create %s) | tar -C tmp -xvf -", argv[1]);
    if (system(cmd))
        errExit("docker");

    mount_disk(argv);
    
    snprintf(cmd, 1024, "cp -rf tmp/. mnt/%s", argv[0]);
    if (system(cmd))
        errExit("cp");

    if (system("rm -rf tmp"))
        errExit("rm");
    if (system("mkdir tmp"))
        errExit("mkdir");

    umount_disk(argv);
}

