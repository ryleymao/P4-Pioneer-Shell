Project 4: Pioneer Shell (Pish)

This project implements a simplified command-line shell in C, named the Pioneer Shell (or pish). It emulates the basic behavior of shells like bash, enabling users to interactively or programmatically execute commands, navigate directories, and manage processes.

The shell supports both interactive mode (where users type commands) and batch mode (where commands are read from a script file). It practices key concepts in systems programming including string parsing, command-line argument handling, file I/O, and process management.

Learning Goals
Use of C string and I/O library functions

Parsing input and command-line arguments

Managing child processes using fork(), exec(), and wait()

Implementing shell-like behavior with built-in commands

File reading/writing and error handling

Incremental debugging and memory management with tools like Valgrind

Features
Interactive Mode
In interactive mode, the shell:

Displays a prompt

Reads a line of input from the user

Tokenizes the input into a command and arguments

Executes the command, either by invoking a built-in or by running an external program

Batch Mode
If run with a script file as an argument, the shell reads and executes commands from the file line by line, without displaying a prompt.

Example:

bash
Copy
Edit
./pish script.sh
Input Parsing
User input is parsed into a command (e.g., ls) and a list of arguments using strtok(), with whitespace and newline delimiters. The shell is robust to arbitrary spaces and tabs between arguments.

Built-In Commands
The shell supports three built-in commands:

exit — Exits the shell. Accepts no arguments.

cd <path> — Changes the current working directory. Uses chdir().

history — Prints a numbered list of all past commands entered in interactive mode, stored in ~/.pish_history.

History Feature
Commands entered in interactive mode are saved to a hidden file at ~/.pish_history

Empty or whitespace-only commands are not recorded

The history command reads and prints this file with line numbers

Example output:

bash
Copy
Edit
1 pwd
2 ls -a
3 history
Running External Programs
If a command is not a built-in, the shell uses fork() to spawn a child process and execvp() to run the specified program. The parent process waits for the child to complete using wait().

Error Handling
The shell handles errors from various system calls using perror() with descriptive messages:

open: when file opening fails

cd: when changing directories fails

pish: when a program fails to execute

fork: when forking fails

It also uses usage_error() for incorrect built-in usage, e.g. cd without an argument or exit 1.

File Structure
pish.c: Core shell logic, including parsing, execution, and interactive loop

pish_history.c: Functions for reading and writing command history

pish.h: Shared types and function declarations, including struct pish_arg

Makefile: Builds the shell

ref-pish: Reference implementation for testing (uses ~/.ref_pish_history)

Compilation and Usage
Compile the shell with:

go
Copy
Edit
make
Run in interactive mode:

bash
Copy
Edit
./pish
Run a script:

bash
Copy
Edit
./pish script.sh
Command Structure
The shell parses input into a struct pish_arg defined as:

c
Copy
Edit
#define MAX_ARGC 64
struct pish_arg {
  int argc;
  char *argv[MAX_ARGC];
};
Development Suggestions
Recommended order of implementation:

Basic command parsing and external execution (e.g., ls)

Input tokenization using strtok()

Implement exit and cd commands with error handling

Handle arguments and arbitrary whitespace

Support script input from files

Implement history functionality

Use valgrind to ensure proper memory handling if you use malloc.

Limitations
The shell does not support:

Arrow-key navigation or autocomplete

Tilde (~) expansion

Input/output redirection (>, <)

Piping (|) These features are out of scope for this simplified project.
