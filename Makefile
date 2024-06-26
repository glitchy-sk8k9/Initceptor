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

debug: 
	$(CC) $(SRCS) $(CFLAGS) $(DFLAGS) -o $(EXEC)

clean:
	rm -f $(OBJS) $(EXEC)

compile:
	$(CC) $(SRCS) $(CFLAGS) -o $(EXEC)

run: compile 
	./$(EXEC)

sudo: compile
	sudo ./$(EXEC)

.PHONY: all debug clean compile run sudo
