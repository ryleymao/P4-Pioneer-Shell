# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -std=gnu99 -Wno-unused -Werror=vla -g

# Source files
SRC = pish.c pish_history.c

# Object files
OBJ = $(SRC:.c=.o)

# Executable
EXE = pish

all: $(EXE)

$(EXE): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(EXE)

clean:
	rm -f $(OBJ) $(EXE)