#include <ctype.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include "pish.h"

static char pish_history_path[1024] = {'\0'};

static void initialize_history_path() {
    struct passwd *pw = getpwuid(getuid());
    if (pw) {
        snprintf(pish_history_path, sizeof(pish_history_path), "%s/.pish_history", pw->pw_dir);
    }
}

void add_to_history(const char *command) {
    if (!pish_history_path[0]) {
        initialize_history_path();
    }

    if (command == NULL || strlen(command) == 0 || isspace(command[0])) {
        return; // Ignore empty or whitespace-only commands
    }

    FILE *file = fopen(pish_history_path, "a");
    if (!file) {
        perror("open");
        return;
    }

    fprintf(file, "%s", command);
    fclose(file);
}

void show_history() {
    if (!pish_history_path[0]) {
        initialize_history_path();
    }

    FILE *file = fopen(pish_history_path, "r");
    if (!file) {
        perror("open");
        return;
    }

    char line[1024];
    int count = 1;
    while (fgets(line, sizeof(line), file)) {
        printf("%d %s", count++, line);
    }

    fclose(file);
}
