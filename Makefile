# Compiler
CC = gcc

# Directories
SRCDIR = ./src
BINDIR = ./bin

# Compiler flags
CFLAGS = -Wall -I$(SRCDIR) -lcrypt -O2

# Also flags for IDE debugging
DFLAGS = -g

# And then also the checking flags

CHFLAGS = -Wall -Werror -Wextra

# Source files
SRCS = $(wildcard $(SRCDIR)/*.c) 

# Object files
OBJS = $(SRCS:$(SRCDIR)/%.c=$(BINDIR)/%.o)

# Executable name
EXEC = $(BINDIR)/main

# Default target
all: run

foldersdamnit: # Apparently we have github's shenanigans :/

	-@mkdir $(SRCDIR)
	-@mkdir $(BINDIR)
	

$(BINDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

debug: $(OBJS) 
	$(CC) $(OBJS) $(CFLAGS) $(DFLAGS) -o $(EXEC)

clean:
	rm -fv $(OBJS) $(EXEC)

compile: $(OBJS)
	$(CC) $(OBJS) $(CFLAGS) -o $(EXEC)

run: compile
	./$(EXEC)

sudo: compile
	sudo ./$(EXEC)

check:
	# Use this to make sure stuff actually compiles without errors
	$(CC) $(SRCS) $(CFLAGS) $(CHFLAGS) -o $(EXEC)
	make clean

.PHONY: all debug clean compile run sudo check
