#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include <time.h>
#include <crypt.h>
#include "user_hash_utils.h"


// Settings, best to leave this at default unless you know what you're doing.
#define USERNAME "initceptor"
#define LOG_FILE "/var/log/custom_boot_hook.log"
#define DEBUG true

// Flags, comment out to disable
#define SET_LOG_FILE_AS_0666 // I tried to keep it obvious :>
#define USE_SHELL_SCRIPT // Warning, a bad actor could utilize this.
// #define SILENCE_FLAG_INFO // Turns off "X flag is disabled, skipping..." messages, yeah you should turn this on
// #define SILENCE_CHMOD // Removes --verbose from chmod

// Function to log messages with timestamp
void log_message(const char *message) {
    time_t rawtime;
    struct tm *timeinfo;
    char timestamp[20]; // Enough to hold "YYYY-MM-DD HH:MM:SS"

    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", timeinfo);

    FILE *fp = fopen(LOG_FILE, "a");

    if (fp != NULL) {
        fprintf(fp, "[%s] %s\n", timestamp, message);
        fclose(fp);
    } else {
        perror("Failed to open log file");
    }
}


// A function i wrote but it's kinda useless :|
void write_to_file(const char *filepath, const char *data) {
	FILE *fp = fopen(filepath, "w");
	if (fp != NULL){
		fprintf(fp, "%s", data);
		fclose(fp);
		log_message("Writing worked");
	} else {
		log_message("Error in write_to_file!");
		log_message("Error data below");
		log_message("File path:");
		log_message(filepath);
		log_message("Data:");
		log_message(data);
	}
}

// Obvious?
int set_log_permissions(){
	char command[256];
	char flags[10];
	#ifndef SILENCE_CHMOD
    		strcpy(flags, "--verbose");
	#else
    		flags[0] = '\0'; 
	#endif
	snprintf(command, sizeof(command), "chmod 0666 %s %s", LOG_FILE, flags);
	int result = system(command);
	if (DEBUG){
		printf("Set_log_permissions, chmod returned code %d\n", result);
	}
	return result;
}

// Function to handle if user/password combination matches
int password_works(char *password) {
    int result = hash_matches_for_user(USERNAME, password);
    if (DEBUG) {
        printf("password_works result: %d\n", result);
    }
    return result;
}

// Function to prompt for a password
void prompt_password() {
    char entered_password[256];
    int attempts = 3;
    int initial_check = password_works("Placeholder");
    
    // if (DEBUG){ printf("%b", initial_check); }
    // Thank you for your help. The fly has been swatted.

    // Make sure the account actually exists :P
    if (initial_check == -1){
	    fprintf(stderr, "%s", "Error, user to get password from is not found. Please contact your system administrator. System halted.");
	    log_message("password_works returned -1");
	    exit(EXIT_FAILURE);
    }
    
    if (initial_check == -2){
	    fprintf(stderr, "%s", "Error, the hash failed. Please contact your system administrator. System halted.");
	    log_message("password_works returned -2");
	    exit(EXIT_FAILURE);
    }
    if (initial_check == -3){
	    fprintf(stderr, "%s", "Error, opening the file failed. Please contact your system administrator. System halted.");
	    log_message("password_works somehow failed with -3, does SHADOW_FILE exist?");
	    exit(EXIT_FAILURE);
    }

    struct termios old_term, new_term;
    tcgetattr(STDIN_FILENO, &old_term);
    new_term = old_term;
    new_term.c_lflag &= ~(ECHO);  // Turn off echoing
    tcsetattr(STDIN_FILENO, TCSANOW, &new_term);

    while (attempts > 0) {
        printf("Enter boot password: ");
        fflush(stdout);
        fgets(entered_password, sizeof(entered_password), stdin);
        entered_password[strcspn(entered_password, "\n")] = '\0'; // Remove newline character
         
        if (password_works(entered_password) == 0) {
            printf("Password correct. Booting...\n");
            log_message("Password correct. Booting...");
            tcsetattr(STDIN_FILENO, TCSANOW, &old_term); // Restore terminal settings
            return;
        } else {
            printf("Incorrect password. Try again.\n");
            log_message("Incorrect password entered.");
            attempts--;

            if (attempts == 0) {
                log_message("Too many failed attempts. System will halt.");
                printf("Halting system.\n\nGoodbye.");
                tcsetattr(STDIN_FILENO, TCSANOW, &old_term); // Restore terminal settings
                exit(EXIT_FAILURE);
            }
        }
    }

    tcsetattr(STDIN_FILENO, TCSANOW, &old_term); // Restore terminal settings
}

int use_shell_script(){
	#ifndef USE_SHELL_SCRIPT
        #ifndef SILENCE_FLAG_INFO
            printf("USE_SHELL_SCRIPT is off, skipping");
        #endif
		return -1; // git -tf out at any rate
	#endif
	
	printf("Not implemented");
	return 0;
}

void pre_boot_tasks(){
	// Tasks here
	
	// Tasks are done, let's log then boot
	
	log_message("Pre-boot tasks done.");
}

int main() {
	#ifdef SET_LOG_FILE_AS_0666
		set_log_permissions();
	#endif

	// Call the password prompt function
	log_message("Initialization script started.");
	prompt_password();

	log_message("Executing pre-boot tasks...");
	pre_boot_tasks();

	// Finally, exec systemd
	execl("/lib/systemd/systemd", "systemd", NULL);

	// In case exec somehow fails
	log_message("Oh hello there, this is NOT how stuff is supposed to work 0_0");

	return 0;
}

