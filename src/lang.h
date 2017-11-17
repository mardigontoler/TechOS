// This file is used for saving the output strings that will be used by TechOS.
// Maybe it can be used as a sort of lang file. Maybe it'll work.

/* Version Information */
#define TOS_DATE    "11/10/2017"
#define TOS_VERSION "4.0"

/* Text Decoration */
#define COLOR_BLUE      "\e[34m"
#define COLOR_YELLOW    "\e[93m"
#define COLOR_MAGENTA   "\e[95m"
#define COLOR_RED       "\e[91m"
#define COLOR_DEFAULT   "\e[39m"

/* Commands */
#define COMMANDINFO_NUM         13
#define COMMANDINFO_NAMESIZE    8
#define COMMAND_HELP            "help"
#define COMMAND_VERSION         "version"
#define COMMAND_SHOWDATE        "date"
#define COMMAND_SETDATE         "setdate"
#define COMMAND_TIME            "time"
#define COMMAND_TERMINATE       "exit"
#define COMMAND_SETPRIORITY     "priority"
#define COMMAND_SHOWPCB         "pcb"
#define COMMAND_SHOWPROCESSES   "processes"
#define COMMAND_SHOWREADYPCB    "ready"
#define COMMAND_SHOWBLOCKEDPCB  "blocked"
// #define COMMAND_CREATEPCB       "create"     //removed in R3
#define COMMAND_DELETEPCB       "delete"
// #define COMMAND_BLOCKPCB        "block"      //removed in R3
// #define COMMAND_UNBLOCKPCB      "unblock"    //removed in R3
// #define COMMAND_SUSPENDPCB      "suspend"    //removed in R3
// #define COMMAND_RESUMEPCB       "resume"     //removed in R3
#define COMMAND_LOADPCB         "load"
#define COMMAND_DISPATCH        "dispatch"

/* Command Usages */
    //TODO Export these into their repective main files.

// "help"
#define USAGE_HELP \
    "\n" COMMAND_HELP ": " COMMAND_HELP " [-c <command>]" \
    "\n\tDisplays usage of partiulcar commands, or list of available commands." \
    "\n\tArguments:" \
    "\n\t  COMMAND\tSpecific command to get information."
// "version"
#define USAGE_VERSION \
    "\n" COMMAND_VERSION ": " COMMAND_VERSION \
    "\n\tDisplays current version information, release date, and author information."
// "date"
#define USAGE_SHOWDATE \
    "\n" COMMAND_SHOWDATE ": " COMMAND_SHOWDATE " [-d | -D | -f | -F | -g | -G | -m | -s | -y]" \
    "\n\tDisplays the current date in several different formats. \nCan be changed by running \"" COMMAND_SETDATE "\"."
// "setdate"
#define USAGE_SETDATE \
    "\n" COMMAND_SETDATE ": " COMMAND_SETDATE " -m <month> -d <day> -y <year>" \
    "\n\tChanges current saved date." \
    "\n\tArguments:" \
    "\n\t MONTH\tNumber of the month to be set." \
    "\n\t  DAY\tDay to be set." \
    "\n\t  YEAR\tYear to be set."
// "time"
#define USAGE_TIME \
    "\n" COMMAND_TIME ": " COMMAND_TIME " [-t | -T | -S)" \
    "\n\tDisplays current time in several different formats."
// "exit"
#define USAGE_TERMINATE \
    "\n" COMMAND_TERMINATE ": " COMMAND_TERMINATE \
    "\n\tTerminates the program after a prompt."
// "priority"
#define USAGE_SETPRIORITY \
    "\n" COMMAND_SETPRIORITY ": " COMMAND_SETPRIORITY " -n <name> -p <priority>" \
    "\n\tChanges the priority of an existing process." \
    "\n\tArguments:" \
    "\n\t  NAME\tName of process to be modified." \
    "\n\t  PRIORITY\tNew priority to be set. (Between 1 and 9)" 
// "pcb" prints pcb information
#define USAGE_SHOWPCB \
    "\n" COMMAND_SHOWPCB ": " COMMAND_SHOWPCB " -n <name>" \
    "\n\tDisplays informaiton about a process." \
    "\n\tArguments:" \
    "\n\t  NAME\tName of process to display."
// "processes"
#define USAGE_SHOWPROCESSES \
    "\n" COMMAND_SHOWPROCESSES ": " COMMAND_SHOWPROCESSES \
    "\n\tDisplays all loaded processes."
// "ready"
#define USAGE_SHOWREADYPCB \
    "\n" COMMAND_SHOWREADYPCB ": " COMMAND_SHOWREADYPCB \
    "\n\tDisplays all ready processes."
// "blocked"
#define USAGE_SHOWBLOCKEDPCB \
    "\n" COMMAND_SHOWBLOCKEDPCB ": " COMMAND_SHOWBLOCKEDPCB \
    "\n\tDisplays all blocked processes."
// "delete"
#define USAGE_DELETEPCB \
    "\n" COMMAND_DELETEPCB ": " COMMAND_DELETEPCB " -n <name>" \
    "\n\tDeletes a loaded process." \
    "\n\tArguments:" \
    "\n\t  NAME\tName of the process to be deleted."
// "load"
#define USAGE_LOADPCB \
    "\n" COMMAND_LOADPCB ": " COMMAND_LOADPCB " -n <name> -p <priority> -f <filename>" \
    "\n\tLoads a new process into TechOS." \xzzzzzzzzzz
    "\n\tArguments:" \
    "\n\t  NAME\tName of new process." \
    "\n\t  PRIORITY\tPriority of the new process." \
    "\n\t  FILENAME\tPath to the file holding the process information."
// "dispatch"
#define USAGE_DISPATCH \
    "\n" COMMAND_DISPATCH ": " COMMAND_DISPATCH \
    "\n\tExecutes all ready processes."