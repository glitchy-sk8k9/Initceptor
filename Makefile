# Compiler
CC = gcc


# Directories
SRCDIR = ./src
BINDIR = ./bin
RELEASE_CONFIGDIR = /usr/share/initceptor_config/
INSTALL_LOCATION = /sbin/init

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

install:
	-@sudo mkdir -p $(RELEASE_CONFIGDIR)
	-@su -c "cat /dev/null > $(RELEASE_CONFIGDIR)commands.sh"
	@echo "This will overwrite $(INSTALL_LOCATION)"
	@echo "Do you want to continue? (y/n)"
	@read answer; \
	if [ "$$answer" = "y" ]; then \
		make check && \
		make compile && \
		sudo cp $(EXEC) $(INSTALL_LOCATION); \
	else \
		echo "Installation aborted."; \
	fi

$(BINDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@
	-@make foldersdamnit

debug: $(OBJS) 
	$(CC) $(OBJS) $(CFLAGS) $(DFLAGS) -o $(EXEC)

clean:
	rm -fv $(OBJS) $(EXEC)

compile: $(OBJS)
	$(CC) $(OBJS) $(CFLAGS) -o $(EXEC)

run: check
	./$(EXEC)

sudo: compile
	sudo ./$(EXEC)

check:
	# Use this to make sure stuff actually compiles without errors
	$(CC) $(SRCS) $(CFLAGS) $(CHFLAGS) -o $(EXEC)

.PHONY: all debug clean compile run sudo check foldersdamnit
