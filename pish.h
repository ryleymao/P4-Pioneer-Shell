#ifndef __PISH_HISTORY_H__
#define __PISH_HISTORY_H__

#define MAX_ARGC 64

struct pish_arg {
    int argc;
    char *argv[MAX_ARGC];
};

void add_to_history(const char *command);
void show_history(void);
void parse_input(const char *input, struct pish_arg *args);
void execute_command(struct pish_arg *args);
void usage_error(const char *command);

#endif
