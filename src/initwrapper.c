#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include <time.h>
#include <crypt.h>
#include <libgen.h>
#include "../headers/user_hash_utils.h"


// Settings
#define USERNAME            "initceptor"
#define LOG_FILE            "/var/log/custom_boot_hook.log"
#define INIT_SYSTEM         "/lib/systemd/systemd" // TODO: make a "change init system" menu. Ncurses maybe?
#define SHELL               "/bin/sh"
#include                    "../headers/lang/en.h" // Set your language here!

// Flags, comment out to disable
// #define PUBLIC_LOG_FILE //                                                                      DEFAULT: off
#define USE_SHELL_SCRIPT // Warning, a bad actor could utilize this.                            DEFAULT: off
#define PROMPT_PASSWORD //  Do you need this? If so, keep it on.                                DEFAULT: on

// Default is normal, toggled is debug
// #define SILENCE_FLAG_INFO // Turns off "X flag is disabled, skipping..." messages,           DEFAULT: on
//#define SILENCE_CHMOD // Removes --verbose from chmod                                           DEFAULT: on
//#define DEBUG //                                                                                DEFAULT: off

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
        fprintf(fp, "[%s] %s", timestamp, message);
        #ifdef DEBUG
            printf("[LOG] %s", message);
        #endif
        fclose(fp);
    } else {
        perror(STRING_ERROR_LOG_FILE_OPEN_FAIL);
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

        fprintf(stderr, STRING_FORMAT_BUFFER_OVERFLOW, STRING_COMMAND);
        snprintf(response, sizeof(response), STRING_BAILING_BUFFER_OVERFLOW_FORMAT, STRING_COMMAND);
        log_message(response);
        return -1;
    }

	int result = system(command);
	if ( (long unsigned int)
            snprintf(response, sizeof(response), 
            STRING_SET_PERMISSIONS_EXECUTED,
            permissions, filepath, flags, result) >= sizeof(response)){

        fprintf(stderr, STRING_FORMAT_BUFFER_OVERFLOW, STRING_RESPONSE);
        snprintf(response, sizeof(response), STRING_BAILING_BUFFER_OVERFLOW_FORMAT, STRING_RESPONSE);
        log_message(response);
        return -1;
    } else {
        log_message(response);
    }
	return result;
}



// Function to handle if user/password combination matches
int password_works(char *password) {
    int result = hash_matches_for_user(USERNAME, password);
    #ifdef DEBUG
        printf(STRING_DEBUG_PASSWORD_WORKS_RESULT, result);
    #endif
    return result;
}

// Function to prompt for a password
void prompt_password() {
    char entered_password[256];
    int attempts = 3;
    int initial_check = password_works("1111");
    char response[4096];
    bool exit_because_failed = false;
    
    // if (DEBUG){ printf("%b", initial_check); }
    // Thank you for your help. The fly has been swatted.

    // Make sure the account actually exists :P
    if (initial_check == -1){
	    fprintf(stderr, STRING_ERROR_USER_NONEXISTENT_PROMPT_PASSWORD);
	    snprintf(response, sizeof(response), STRING_FORMAT_ERROR_PASSWORD_WORKS_RETURNED, STRING_CHECK_IF_USER_EXISTS);
	    exit_because_failed = true;
    }
    
    if (initial_check == -2 && !exit_because_failed){
	    fprintf(stderr, "%s", STRING_ERROR_HASH_FAILED_PROMPT_PASSWORD);
	    snprintf(response, sizeof(response), STRING_FORMAT_ERROR_PASSWORD_WORKS_RETURNED, STRING_PLEASE_DEBUG_HASHING_PROCESS);
        exit_because_failed = true;
    }
    if (initial_check == -3 && !exit_because_failed){
	    fprintf(stderr, STRING_ERROR_SHADOW_FILE_OPEN_FAIL);
	    snprintf(response, sizeof(response), STRING_FORMAT_ERROR_PASSWORD_WORKS_RETURNED, STRING_FORMAT_DOES_SHADOW_FILE_EXIST);
	    exit_because_failed = true;
    }

    if (exit_because_failed) {
        exit(EXIT_FAILURE);
    }

    struct termios old_term, new_term;
    tcgetattr(STDIN_FILENO, &old_term);
    new_term = old_term;
    new_term.c_lflag &= ~(ECHO);  // Turn off echoing
    tcsetattr(STDIN_FILENO, TCSANOW, &new_term);


    while (attempts > 0) {
        printf(STRING_ENTER_BOOT_PASSWORD);
        fflush(stdout);
        char* result = fgets(entered_password, sizeof(entered_password), stdin); // Yes gcc i am ignoring this screw you
	    (void) result;
        entered_password[strcspn(entered_password, "\n")] = '\0';
         
        if (password_works(entered_password) == 0) {
            printf(STRING_CORRECT_PASSWORD);
            log_message(STRING_CORRECT_PASSWORD);
            tcsetattr(STDIN_FILENO, TCSANOW, &old_term); // Restore terminal settings
            return;
        } else {
            printf(STRING_WRONG_PASSWORD);
            log_message(STRING_LOG_WRONG_PASSWORD);
            attempts--;

            if (attempts == 0) {
                log_message(STRING_LOG_USER_TOO_MANY_ATTEMPTS);
                printf(STRING_HALT_SYSTEM_TOO_MANY_ATTEMPTS);
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
            printf(STRING_FORMAT_FLAG_DISABLED_SKIPPING, "USE_SHELL_SCRIPT");
        #endif
		return -1; // git -tf out at any rate
	#endif
	// Lets be safe here, we're going to set the permissions to the shell script to zeros on all (if we're not debugging, we do not want to mess up permissions in git builds)
    #ifndef DEBUG
        set_permissions(SHELL_SCRIPT_PATH, "0000");
    #endif
    log_message(STRING_LOG_USING_SHELL_SCRIPT);
    snprintf(command, sizeof(command), "%s %s", SHELL, SHELL_SCRIPT_PATH);
    int result = system(command); // Aight so i dont understand but why in the actual heck does github have more extra warnings than my host
    #ifdef DEBUG
    snprintf(response, sizeof(response), STRING_FORMAT_DEBUG_SYSTEM_CALL_RETURNED, result);
    #endif
    log_message(response);
	
	return result;
}

void pre_boot_tasks(){
	// Tasks here
	use_shell_script();
	// Tasks are done, let's log then boot
	
	log_message(STRING_LOG_PRE_BOOT_TASKS_DONE);
}

int main() {
    log_message(STRING_LOG_MAIN_ENTERED);
	#ifdef PUBLIC_LOG_FILE
		set_permissions(LOG_FILE, "0666");
    #else
        set_permissions(LOG_FILE, "0000");
	#endif


    #ifdef PROMPT_PASSWORD // If this is off we're skipping like 67+ lines of code
	prompt_password();
    #endif

	log_message(STRING_LOG_EXECUTING_PRE_BOOT_TASKS);
	pre_boot_tasks();

	// Finally, exec our init system if we're not debugging
    #ifndef DEBUG
        char *init_system_name = basename(INIT_SYSTEM);
	    execl(INIT_SYSTEM,init_system_name, NULL);
    #endif
	// In case exec somehow fails
	log_message(STRING_DID_NOT_EXEC_INIT);

	return 0;
}

