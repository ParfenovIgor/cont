#include "inc.h"

int run(char *argv[]);
void setup_disk(char *argv[]);
void remove_disk(char *argv[]);
void pull(char *argv[]);

void help() {
    char *text =
        "Usage: cont [command]\n"
        "Commands:\n"
        "  list                    List all local containers\n"
        "  setup <name> <size>     Create disk file for mounting with given name and size in Mb\n"
        "  remove <name>           Remove disk file with given name\n"
        "  pull <name> <image>     Pull given docker image into given disk file name\n"
        "  run <name> <command>    Run given command in local container with given name\n";
    printf("%s", text);
}

void list() {
    printf("Disks:\n\n");
    FILE *file = fopen("conf", "r");
    if (file == NULL)
        errExit("fopen-conf");
    char *line;
    size_t len;
    ssize_t nread;
    while ((nread = getline(&line, &len, file)) != -1) {
        line[nread - 1] = '\0';
        printf("%s\n", line);
    }
    fclose(file);
}

int main(int argc, char *argv[]) {
    if (argc == 1) {
        help();
    }
    else if (strcmp(argv[1], "list") == 0) {
        if (argc != 2) {
            help();
            return 0;
        }
        list();
    }
    else if (strcmp(argv[1], "setup") == 0) {
        if (argc != 4) {
            help();
            return 0;
        }
        setup_disk(argv + 2);
    }
    else if (strcmp(argv[1], "remove") == 0) {
        if (argc != 3) {
            help();
            return 0;
        }
        remove_disk(argv + 2);
    }
    else if (strcmp(argv[1], "pull") == 0) {
        if (argc != 4) {
            help();
            return 0;
        }
        pull(argv + 2);
    }
    else if (strcmp(argv[1], "run") == 0) {
        if (argc != 4) {
            help();
            return 0;
        }
        run(argv + 2);
    }
    else {
        help();
    }
    
    return 0;
}
