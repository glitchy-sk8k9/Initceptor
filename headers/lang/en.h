// Generic strings

static const char STRING_COMMAND[]                                  =   "command";
static const char STRING_RESPONSE[]                                 =   "response";

// Strings related to flags
static const char STRING_FORMAT_FLAG_DISABLED_SKIPPING[]            =   "%s disabled, skipping...\n";

// Strings related to logging functions
static const char STRING_ERROR_LOG_FILE_OPEN_FAIL[]                 =   "Failed to open log file!\n";

//Strings related to buffer overflows
static const char STRING_FORMAT_BUFFER_OVERFLOW[]                   =   "%s buffer overflow!\n";
static const char STRING_BAILING_BUFFER_OVERFLOW_FORMAT[]           =   "Buffer overflow detected, bailing out of %s!\n";

//Strings related to set_permissions
static const char STRING_SET_PERMISSIONS_EXECUTED[]                 =   "Set_permissions executed with parameters\n\tPermissions = %s\n\tFilepath = %s\n\tFlags = %s\n\tReturned code %d\n";

// Strings related to prompt_password and password_works

static const char STRING_ERROR_USER_NONEXISTENT_PROMPT_PASSWORD[]   =   "Error, user to get password from is not found. Please contact your system administrator. System halted.\n";
static const char STRING_ERROR_HASH_FAILED_PROMPT_PASSWORD[]        =   "Error, the hash failed. Please contact your system administrator. System halted.\n";
static const char STRING_ERROR_SHADOW_FILE_OPEN_FAIL[]              =   "Error, opening the shadow file failed. Please contact your system administrator. System halted.\n";
static const char STRING_FORMAT_ERROR_PASSWORD_WORKS_RETURNED[]     =   "password_works returned %s.\n";
static const char STRING_CHECK_IF_USER_EXISTS[]                     =   "-1, check if user exists\n";
static const char STRING_PLEASE_DEBUG_HASHING_PROCESS[]             =   "-2, this means you have a bug in the hashing process\n";
static const char STRING_FORMAT_DOES_SHADOW_FILE_EXIST[]            =   "-3, this is either a permission error or the specified shadow file does not exist.\n";
static const char STRING_ENTER_BOOT_PASSWORD[]                      =   "Please enter boot password: ";
static const char STRING_CORRECT_PASSWORD[]                         =   "Correct password. Booting...\n";
static const char STRING_WRONG_PASSWORD[]                           =   "Wrong password. Try again.\n";
static const char STRING_LOG_WRONG_PASSWORD[]                       =   "! User entered incorrect password!\n";
static const char STRING_LOG_USER_TOO_MANY_ATTEMPTS[]               =   "User entered too many incorrect passwords. System halting...\n";
static const char STRING_HALT_SYSTEM_TOO_MANY_ATTEMPTS[]            =   "Too many incorrect attempts.\nSystem halting.\nGoodbye.\n";

// Strings related to use_shell_script

static const char STRING_LOG_USING_SHELL_SCRIPT[]                   =   "Shell script loading...\n";
static const char STRING_FORMAT_DEBUG_SYSTEM_CALL_RETURNED[]        =   "System call returned %d\n";

// Strings related to main() and pre_boot_tasks
static const char STRING_LOG_EXECUTING_PRE_BOOT_TASKS[]             =   "Executing pre-boot tasks...\n";
static const char STRING_LOG_PRE_BOOT_TASKS_DONE[]                  =   "Pre-boot tasks done.\n";
static const char STRING_LOG_MAIN_ENTERED[]                         =   "Initceptor has started execution!\n";
static const char STRING_DID_NOT_EXEC_INIT[]                        =   "Did not execute init!\n";

// Debug strings
static const char STRING_DEBUG_PASSWORD_WORKS_RESULT[]             =   "password_works result: %d\n";