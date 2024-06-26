#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include <time.h>
#include <crypt.h>
#include "../headers/user_hash_utils.h"


// Settings, best to leave this at default unless you know what you're doing.
#define USERNAME            "initceptor"
#define LOG_FILE            "/var/log/custom_boot_hook.log"
#define INIT_SYSTEM         "/usr/lib/systemd/systemd" // TODO: make a "change init system" menu. Ncurses maybe?
#define INIT_SYSTEM_NAME    "systemd" // TODO: Make this dynamic. I do not want this to have execl hold magic values tho. Priorities.

// Flags, comment out to disable
// #define PUBLIC_LOG_FILE //                                                                      DEFAULT: off
#define USE_SHELL_SCRIPT // Warning, a bad actor could utilize this.                            DEFAULT: off

// Default is normal, toggled is debug
// #define SILENCE_FLAG_INFO // Turns off "X flag is disabled, skipping..." messages,           DEFAULT: on
//#define SILENCE_CHMOD // Removes --verbose from chmod                                           DEFAULT: on
#define DEBUG //                                                                                DEFAULT: off

#ifdef DEBUG
#define SHELL_SCRIPT_PATH "./config/commands.sh"
#else
#define SHELL_SCRIPT_PATH "/usr/share/initceptor_config/commands.sh"
#endif


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
        #ifdef DEBUG
            printf("[LOG] %s\n", message);
        #endif
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
int set_permissions(const char *filepath, const char *permissions){ // Do not ask why param 2 is char.
	char command[256];
    char response[4096];
	char flags[10];
	#ifndef SILENCE_CHMOD
    		strcpy(flags, "--verbose");
	#else
    		flags[0] = '\0'; 
	#endif
    if ( (long unsigned int)
            snprintf(command, sizeof(command), "chmod %s %s %s", permissions, filepath, flags)
            >= sizeof(command)) {

        fprintf(stderr, "Command buffer overflow\n");
        log_message("Warning, buffer overflow got detected, bailing out of command.");
        return -1;
    }

	int result = system(command);
	if ( (long unsigned int)
            snprintf(response, sizeof(response), 
            "Set_permissions executed with parameters\n\tPermissions = %s\n\tFilepath = %s\n\tFlags = %s\n\nReturned code %d",
            permissions, filepath, flags, result) >= sizeof(response)){

        fprintf(stderr, "Response buffer overflow\n");
        log_message("Warning, buffer overflow got detected, bailing out of response.");
        return -1;
    }
	return result;
}



// Function to handle if user/password combination matches
int password_works(char *password) {
    int result = hash_matches_for_user(USERNAME, password);
    #ifdef DEBUG
        printf("password_works result: %d\n", result);
    #endif
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
	    fprintf(stderr, "%s", "Error, user to get password from is not found. Please contact your system administrator. System halted.\n");
	    log_message("password_works returned -1");
	    exit(EXIT_FAILURE);
    }
    
    if (initial_check == -2){
	    fprintf(stderr, "%s", "Error, the hash failed. Please contact your system administrator. System halted.\n");
	    log_message("password_works returned -2");
	    exit(EXIT_FAILURE);
    }
    if (initial_check == -3){
	    fprintf(stderr, "%s", "Error, opening the file failed. Please contact your system administrator. System halted.\n");
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
        char* result = fgets(entered_password, sizeof(entered_password), stdin); // Yes gcc i am ignoring this
	(void) result;
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
    char command[512];
    char response[512];
	#ifndef USE_SHELL_SCRIPT
        #ifndef SILENCE_FLAG_INFO
            printf("USE_SHELL_SCRIPT is off, skipping");
        #endif
		return -1; // git -tf out at any rate
	#endif
	// Lets be safe here, we're going to set the permissions to the shell script to zeros on all (if we're not debugging, we do not want to mess up permissions in git builds)
    #ifndef DEBUG
        set_permissions(SHELL_SCRIPT_PATH, "0000");
    #endif
    log_message("Aight so we're starting the shell script now.");
    snprintf(command, sizeof(command), "/bin/sh %s", SHELL_SCRIPT_PATH);
    int result = system(command); // Aight so i dont understand but why in the actual heck does github have more extra warnings than my host
    snprintf(response, sizeof(response), "System call returned %d", result);
    log_message(response);
	
	return result;
}

void pre_boot_tasks(){
	// Tasks here
	use_shell_script();
	// Tasks are done, let's log then boot
	
	log_message("Pre-boot tasks done.");
}

int main() {
	#ifdef PUBLIC_LOG_FILE
		set_permissions(LOG_FILE, "0666");
    #else
        set_permissions(LOG_FILE, "0000");
	#endif


	// Call the password prompt function
	log_message("Initialization script started.");
	prompt_password();

	log_message("Executing pre-boot tasks...");
	pre_boot_tasks();

	// Finally, exec our init system if we're not debugging
    #ifndef DEBUG
	    execl(INIT_SYSTEM, INIT_SYSTEM_NAME, NULL);
    #endif
	// In case exec somehow fails
	log_message("Oh hello there, this is NOT how stuff is supposed to work 0_0");

	return 0;
}

