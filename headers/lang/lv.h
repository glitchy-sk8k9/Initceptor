// Generic strings

static const char STRING_COMMAND[]                                  =   "kommanda";
static const char STRING_RESPONSE[]                                 =   "atbilde";

// Strings related to flags
static const char STRING_FORMAT_FLAG_DISABLED_SKIPPING[]            =   "%s izslēkts, izlaižam...\n";

// Strings related to logging functions
static const char STRING_ERROR_LOG_FILE_OPEN_FAIL[]                 =   "Nevarējām atvērt žurnālfailu!\n";

//Strings related to buffer overflows
static const char STRING_FORMAT_BUFFER_OVERFLOW[]                   =   "%s bufera pārplūde!\n";
static const char STRING_BAILING_BUFFER_OVERFLOW_FORMAT[]           =   "Bufera pārplūde noķertā, bēgam ārā no %s!\n";

//Strings related to set_permissions
static const char STRING_SET_PERMISSIONS_EXECUTED[]                 =   "Set_permissions izsaukts ar parametriem\n\tAtļavjas = %s\n\tFailu vieta = %s\n\tKarodziņi = %s\n\tAtbildēja ar = %d\n";

// Strings related to prompt_password and password_works

static const char STRING_ERROR_USER_NONEXISTENT_PROMPT_PASSWORD[]   =   "Kļūda, lietotājs no kā dabūt parole nēexistē. Lūdzu sazinies ar sistēmas administrātoru. Sistēma izslēgsies.\n";
static const char STRING_ERROR_HASH_FAILED_PROMPT_PASSWORD[]        =   "Kļūda, jaukšanas process neizdevās! Lūdzu sazinies ar sistēmas administratoru. Sistēma izslēgsies.\n";
static const char STRING_ERROR_SHADOW_FILE_OPEN_FAIL[]              =   "Kļūda, atveršana no 'shadow' faila neizdevās! Lūdzu sazinies ar sistēmas administrātoru! Sistēma izslēgsies.\n";
static const char STRING_FORMAT_ERROR_PASSWORD_WORKS_RETURNED[]     =   "password_works atbildēja ar %s.\n";
static const char STRING_CHECK_IF_USER_EXISTS[]                     =   "-1, vari apskatīt vai lietotājs existē?\n";
static const char STRING_PLEASE_DEBUG_HASHING_PROCESS[]             =   "-2, šis nozīmē ka ir kļūda jaukšanas processā.\n";
static const char STRING_FORMAT_DOES_SHADOW_FILE_EXIST[]            =   "-3, šis nozīmē ka ir vainu tiesība kļūda vai specificēts SHADOW_FILE nēexistē.\n";
static const char STRING_ENTER_BOOT_PASSWORD[]                      =   "Please enter boot password: ";
static const char STRING_CORRECT_PASSWORD[]                         =   "Pareizā parole.. Ieslēdzam...\n";
static const char STRING_WRONG_PASSWORD[]                           =   "Nepareizā parole. Meiģini atkal.\n";
static const char STRING_LOG_WRONG_PASSWORD[]                       =   "! Lietotājs ievadija nepareizo paroli.\n";
static const char STRING_LOG_USER_TOO_MANY_ATTEMPTS[]               =   "Lietotājs ievadija pārāk daudzas nepareizas meiģinājumus. Sistēma izslēgsies.\n";
static const char STRING_HALT_SYSTEM_TOO_MANY_ATTEMPTS[]            =   "Pārāk daudz nepareize meiģinājumi.\nSistēma izslēgsies.\nAtā.\n";

// Strings related to use_shell_script

static const char STRING_LOG_USING_SHELL_SCRIPT[]                   =   "Čaulas scripts tiek izpildīta...\n";
static const char STRING_FORMAT_DEBUG_SYSTEM_CALL_RETURNED[]        =   "system() izsaukums atbildeja ar %d\n";

// Strings related to main() and pre_boot_tasks
static const char STRING_LOG_EXECUTING_PRE_BOOT_TASKS[]             =   "Executing pre-boot tasks...\n";
static const char STRING_LOG_PRE_BOOT_TASKS_DONE[]                  =   "Pirms-ieslēgšanas darbi izdarīti!\n";
static const char STRING_LOG_MAIN_ENTERED[]                         =   "Initceptors iesākās.\n";
static const char STRING_DID_NOT_EXEC_INIT[]                        =   "Init neiesākās!\n";

// Debug strings
static const char STRING_DEBUG_PASSWORD_WORKS_RESULT[]             =   "password_works resulāts: %d\n";