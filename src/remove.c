#include "inc.h"

void remove_name(char *name) {
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

    file = fopen("conf", "w");
    if (file == NULL)
        errExit("fopen-conf");
    for (int i = 0; i < cnt; i++) {
        if (strcmp(lines[i], name))
            fprintf(file, "%s\n", lines[i]);
    }
    fclose(file);
}

void remove_disk(char *argv[]) {
    char cmd[1024];
    snprintf(cmd, 1024, "rm disk/%s", argv[0]);
    if (system(cmd))
        errExit("dd");

    remove_name(argv[0]);
}

