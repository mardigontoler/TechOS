#ifndef HELP_H
#define HELP_H

#include "queue.h"

#define COMPLETIONDATE "10/2/2017"
#define VERSION "2.0"

#define BLUECOLOR "\e[34m"
#define YELLOWCOLOR "\e[93m"
#define MAGENTACOLOR "\e[95m"
#define DEFAULTCOLOR "\e[39m"
#define REDCOLOR "\e[91m"

#define NUMCOMMANDS (18)
#define MAXPROCESSNAMESIZE (8)

#define HELPCOMMAND "help"
#define VERSIONCOMMAND "version"
#define SHOWDATECOMMAND "date"
#define SETDATECOMMAND "setdate"
#define TIMECOMMAND "time"
#define TERMINATECOMMAND "exit"
// R2 commands
#define SETPRIORITYCOMMAND "priority"
#define SHOWPCBCOMMAND "pcb"
#define SHOWPROCESSESCOMMAND "processes"
#define SHOWREADYPROCESSESCOMMAND "ready"
#define SHOWBLOCKEDPROCESSESCOMMAND "blocked"
#define CREATEPCBCOMMAND "create"
#define DELETEPCBCOMMAND "delete"
#define BLOCKPCBCOMMAND "block"
#define UNBLOCKPCBCOMMAND "unblock"
#define SUSPENDPCBCOMMAND "suspend"
#define RESUMEPCBCOMMAND "resume"
#define DISPATCHCOMMAND "dispatch"

#define DISPATCHUSAGE "\nUsage for " DISPATCHCOMMAND	\
    "\n " DISPATCHCOMMAND "\nBegins all processes that are ready.\n"

#define SETDATEUSAGE "\nUsage for " SETDATECOMMAND	\
    ":\n" SETDATECOMMAND " -m <month> -d <day> -y <year>\n"

#define TIMEUSAGE "\nUsage for " TIMECOMMAND		\
    ":\n" TIMECOMMAND " (-t | -T | -S)\n\nExample:\ntime -T\n"

#define TERMINATEUSAGE "\nUsage for " TERMINATECOMMAND			\
    ":\n" TERMINATECOMMAND "\nYou will be asked if you're sure you want to exit. "   	\
    "Enter the letter \"y\" if you are sure.\n"

#define DATEUSAGE "\nUsage for " SHOWDATECOMMAND		\
    ":\n" SHOWDATECOMMAND " (-d | -D | -f | -F | -g | -G | -m | -s | -y)\n"

#define HELPUSAGE "\nUsage for " HELPCOMMAND		\
    ":\n" HELPCOMMAND " [-c <command>]\n\nExample:\nhelp -c time\n" 

#define VERSIONUSAGE "\nUsage for " VERSIONCOMMAND ":\n" VERSIONCOMMAND

#define SETPRIORITYUSAGE "\nUsage for " SETPRIORITYCOMMAND	\
    ":\n" SETPRIORITYCOMMAND " -n <name> -p <priority>\n"

#define SHOWPCBUSAGE "\nUsage for " SHOWPCBCOMMAND	\
    ":\n" SHOWPCBCOMMAND " -n <name>"

#define SHOWPROCESSESUSAGE "\nUsage for " SHOWPROCESSESCOMMAND	\
    ":\n" SHOWPROCESSESCOMMAND

#define SHOWREADYPROCESSESUSAGE "\nUsage for " SHOWREADYPROCESSESCOMMAND \
    ":\n" SHOWREADYPROCESSESCOMMAND

#define SHOWBLOCKEDPROCESSESUSAGE "\nUsage for " SHOWBLOCKEDPROCESSESCOMMAND \
    ":\n" SHOWBLOCKEDPROCESSESCOMMAND

#define CREATEPCBUSAGE "\nCreates a PCB\nUsage for " CREATEPCBCOMMAND	\
    ":\n" CREATEPCBCOMMAND " -n <name> -p<priority> -c<class>\n" \
    " \nThe priority is a number from 0 to 9."							\
    "\nThere is no space between the -p and the priority or -c and the class.\n" 

#define DELETEPCBUSAGE "\nUsage for " DELETEPCBCOMMAND	\
    ":\n" DELETEPCBCOMMAND " -n <name>"

#define BLOCKPCBUSAGE "\nUsage for " BLOCKPCBCOMMAND	\
    ":\n" BLOCKPCBCOMMAND " -n <name>"

#define UNBLOCKPCBUSAGE "\nUsage for " UNBLOCKPCBCOMMAND	\
    ":\n" UNBLOCKPCBCOMMAND " -n <name>"

#define SUSPENDPCBUSAGE "\nUsage for " SUSPENDPCBCOMMAND	\
    ":\n" SUSPENDPCBCOMMAND " -n <name>"

#define RESUMEPCBUSAGE "\nUsage for " RESUMEPCBCOMMAND	\
    ":\n" RESUMEPCBCOMMAND " -n <name>"

int isValidCommand(char *);
int matches(char *, char *);
void help(char *, int);
void displayVersion();
int isValidInt(char* str);


#endif
