# Compiler
CC = gcc


# Directories
SRCDIR = ./src
BINDIR = ./bin
CDIR   = ./config
RELEASE_CONFIGDIR = /usr/share/initceptor_config
INSTALL_LOCATION = /sbin/init

# Compiler flags
CFLAGS = -Wall -I$(SRCDIR) -lcrypt -O2

# Also flags for IDE debugging
DFLAGS = -g

# And then also the strict flags
SFLAGS = -Wall -Werror -Wextra

# Source files
SRCS = $(wildcard $(SRCDIR)/*.c) 

# Object files
OBJS = $(SRCS:$(SRCDIR)/%.c=$(BINDIR)/%.o)

# Executable name
EXEC = $(BINDIR)/main

# Default target
all: run

foldersdamnit: # Apparently we have github's shenanigans :/
	-mkdir $(SRCDIR)
	-mkdir $(BINDIR)
	-mkdir $(CDIR)
	-touch $(CDIR)

install:
	-sudo mkdir -p $(RELEASE_CONFIGDIR)
	-sudo touch $(RELEASE_CONFIGDIR)/commands.sh
	-sudo cp $(CDIR)/commands.sh $(RELEASE_CONFIGDIR)/commands.sh
	@echo "This will overwrite $(INSTALL_LOCATION)"
	@echo "Do you want to continue? (y/n)"
	@read answer; \
	if [ "$$answer" = "y" ]; then \
		echo "Compiling..." && \
		make strict && \
		echo "Copying $(EXEC) to $(INSTALL_LOCATION)..." && \
		sudo rm $(INSTALL_LOCATION) && \
		sudo cp $(EXEC) $(INSTALL_LOCATION) && \
		echo "Adding user..." && \
		if sudo useradd -r -M -N -c "Account for Initceptor" initceptor; then \
			echo "Please enter a new password for initceptor." && \
			sudo passwd initceptor; \
		else \
			echo "User already exists."; \
		fi && \
		echo "Installation complete!"; \
	else \
		echo "Installation aborted."; \
	fi



$(BINDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@
	-make foldersdamnit

debug: $(OBJS) 
	$(CC) $(OBJS) $(CFLAGS) $(DFLAGS) -o $(EXEC)

clean:
	rm -fv $(OBJS) $(EXEC)

compile: $(OBJS)
	$(CC) $(OBJS) $(CFLAGS) -o $(EXEC)

run: strict
	./$(EXEC)

sudo: strict
	sudo ./$(EXEC)

strict:
	# How parents see chores:
	$(CC) $(SRCS) $(CFLAGS) $(SFLAGS) -o $(EXEC)

.PHONY: all debug clean compile run sudo strict foldersdamnit
