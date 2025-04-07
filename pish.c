#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_ARGC 64

// Structure for handling parsed command-line arguments
struct pish_arg {
    int argc;
    char *argv[MAX_ARGC];
};

// Function to parse input into command arguments
void parse_input(const char *input, struct pish_arg *args) {
    char *token;
    char *input_copy = strdup(input); // Duplicate input to preserve the original
    args->argc = 0;

    // Use strtok to break input into tokens, based on space, tab, and newline
    token = strtok(input_copy, " \t\n");
    while (token != NULL && args->argc < MAX_ARGC) {
        args->argv[args->argc++] = strdup(token); // Store argument
        token = strtok(NULL, " \t\n"); // Continue tokenizing
    }
    args->argv[args->argc] = NULL; // Null-terminate argv for execvp
    free(input_copy); // Clean up the duplicated input
}

// Function to execute the parsed command
void execute_command(struct pish_arg *args) {
    if (args->argc == 0) return; // Ignore empty commands

    // Handle built-in commands like 'exit'
    if (strcmp(args->argv[0], "exit") == 0) {
        // Clean up and exit the program
        for (int i = 0; i < args->argc; i++) {
            free(args->argv[i]);
        }
        exit(EXIT_SUCCESS);
    }

    // Handle 'echo' command
    if (strcmp(args->argv[0], "echo") == 0) {
        for (int i = 1; i < args->argc; i++) {
            printf("%s", args->argv[i]);
            if (i < args->argc - 1) printf(" "); // Print space between arguments
        }
        printf("\n");
        return;
    }

    // External commands (use execvp)
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
    } else if (pid == 0) { // Child process
        if (execvp(args->argv[0], args->argv) == -1) {
            perror("execvp");
            exit(EXIT_FAILURE);
        }
    } else { // Parent process
        wait(NULL); // Wait for the child process to finish
    }

    // Free dynamically allocated argv strings
    for (int i = 0; i < args->argc; i++) {
        free(args->argv[i]);
    }
}

// Interactive mode (handles commands entered by the user in real-time)
void interactive_mode() {
    char input[1024];
    struct pish_arg args;

    while (1) {
        // Print the prompt before accepting input
        printf("▶  ");
        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("\n");
            exit(EXIT_SUCCESS); // Handle EOF (Ctrl+D)
        }

        parse_input(input, &args); // Parse the command
        if (args.argc > 0) {
            execute_command(&args); // Execute the command
        }
    }
}

// Batch mode (handles commands from a file)
void batch_mode(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    char input[1024];
    struct pish_arg args;

    // Read each line in the file
    while (fgets(input, sizeof(input), file)) {
        input[strcspn(input, "\n")] = 0; // Remove trailing newline

        if (strlen(input) > 0) { // Ignore empty lines
            printf("▶  "); // Print the prompt before each command
            parse_input(input, &args); // Parse the command
            if (args.argc > 0) {
                execute_command(&args); // Execute the command
            }
        }
    }

    fclose(file); // Close the file after processing
    printf("▶\n"); // Final prompt at the end of batch mode
}

// Main function that controls mode selection (interactive or batch)
int main(int argc, char **argv) {
    if (argc > 2) {
        fprintf(stderr, "Usage: %s [batch_file]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if (argc == 2) {
        batch_mode(argv[1]); // Run in batch mode using the provided file
    } else {
        interactive_mode(); // Run in interactive mode (default)
    }

    return 0;
}
