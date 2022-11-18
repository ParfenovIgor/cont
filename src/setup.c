#include "inc.h"

void append_name(char *name) {
    char *lines[1024];
    
    FILE *file = fopen("conf", "r");
    if (file == NULL)
        errExit("fopen-conf");
    char *line = NULL;
    size_t len;
    ssize_t nread;
    int cnt = 0;
    while ((nread = getline(&line, &len, file)) != -1) {
        if (nread == 1) continue;
        line[nread - 1] = '\0';
        lines[cnt] = malloc(nread);
        strcpy(lines[cnt], line);
        cnt++;
    }
    fclose(file);
    
    lines[cnt] = name;
    cnt++;
    file = fopen("conf", "w");
    if (file == NULL)
        errExit("fopen-conf");
    for (int i = 0; i < cnt; i++) {
        fprintf(file, "%s\n", lines[i]);
    }
    fclose(file);
}

void setup_disk(char *argv[]) {
    char cmd[1024];
    snprintf(cmd, 1024, "dd if=/dev/zero of=disk/%s bs=%sM count=1", argv[0], argv[1]);
    if (system(cmd))
        errExit("dd");
    snprintf(cmd, 1024, "mkfs.ext4 disk/%s", argv[0]);
    if (system(cmd))
        errExit("mkfs.ext4");
    
    append_name(argv[0]);
}

