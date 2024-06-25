# Compiler
CC = gcc

# Directories
SRCDIR = src
BINDIR = bin

# Compiler flags
CFLAGS = -Wall -I$(SRCDIR) -lcrypt

# Also flags for IDE debugging

DFLAGS = -g

# Source files
SRCS = $(wildcard $(SRCDIR)/*.c) 

# Object files
OBJS = $(SRCS:%.c=%.o)

# Executable name
EXEC = $(BINDIR)/main

# Default target
all: run

debug: $(OBJS)
	$(CC) $^ $(CFLAGS) $(DFLAGS) -o $(EXEC)

clean:
	rm -f $(OBJS) $(EXEC)

compile: $(OBJS)
	$(CC) $^ $(CFLAGS) -o $(EXEC)

run: $(EXEC)
	./$(EXEC)


.PHONY: all debug clean compile run
