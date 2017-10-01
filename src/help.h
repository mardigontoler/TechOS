#ifndef HELP_H
#define HELP_H


#define COMPLETIONDATE "10/2/2017"
#define VERSION "2.0"

#define BLUECOLOR "\e[34m"
#define YELLOWCOLOR "\e[93m"
#define DEFAULTCOLOR "\e[39m"
#define REDCOLOR "\e[91m"

#define NUMCOMMANDS (17)
#define MAXPROCESSNAMESIZE (8)

#define HELPCOMMAND "help"
#define VERSIONCOMMAND "version"
#define SHOWDATECOMMAND "date"
#define SETDATECOMMAND "setdate"
#define TIMECOMMAND "time"
#define TERMINATECOMMAND "exit"
// R2 commands
#define SUSPENDCOMMAND "suspend"
#define RESUMECOMMAND "resume"
#define SETPRIORITYCOMMAND "priority"
#define SHOWPCBCOMMAND "pcb"
#define SHOWPROCESSESCOMMAND "processes"
#define SHOWREADYPROCESSESCOMMAND "ready"
#define SHOWBLOCKEDPROCESSESCOMMAND "blocked"
#define CREATEPCBCOMMAND "create"
#define DELETEPCBCOMMAND "delete"
#define BLOCKPCBCOMMAND "block"
#define UNBLOCKPCBCOMMAND "unblock"


#define SETDATEUSAGE "\nUsage for " SETDATECOMMAND	\
    "\n" SETDATECOMMAND " -m <month> -d <day> -y <year>\n"

#define TIMEUSAGE "\nUsage for " TIMECOMMAND		\
    "\n" TIMECOMMAND " (-t | -T | -S)\n\nExample:\ntime -T\n"

#define TERMINATEUSAGE "\nUsage for " TERMINATECOMMAND			\
    "\n" TERMINATECOMMAND "\nYou will be asked if you're sure you want to exit. "   	\
    "Enter the letter \"y\" if you are sure."

#define DATEUSAGE "\nUsage for " SHOWDATECOMMAND		\
    "\n" SHOWDATECOMMAND " (-d | -D | -f | -F | -g | -G | -m | -s | -y)"

#define HELPUSAGE "\nUsage for " HELPCOMMAND		\
    "\n" HELPCOMMAND " [-c <command>]\n\nExample:\nhelp -c time\n" 

#define VERSIONUSAGE "\nUsage for " VERSIONCOMMAND "\n" VERSIONCOMMAND

#define SUSPENDUSAGE "\nUsage for " SUSPENDCOMMAND	\
    "\n"SUSPENDCOMMAND " -n <name>"

#define RESUMEUSAGE "\nUsage for " RESUMECOMMAND	\
    "\n" RESUMECOMMAND " -n <name>"

#define SETPRIORITYUSAGE "\nUsage for " SETPRIORITYCOMMAND	\
    "\n" SETPRIORITYCOMMAND " -n <name> -p <priority>"

#define SHOWPCBUSAGE "\nUsage for " SHOWPCBCOMMAND	\
    "\n" SHOWPCBCOMMAND " -n <name>"

#define SHOWPROCESSESUSAGE "\nUsage for " SHOWPROCESSESCOMMAND	\
    "\n" SHOWPROCESSESCOMMAND

#define SHOWREADYPROCESSESUSAGE "\nUsage for " SHOWREADYPROCESSESCOMMAND \
    "\n" SHOWREADYPROCESSESCOMMAND

#define SHOWBLOCKEDPROCESSESUSAGE "\nUsage for " SHOWBLOCKEDPROCESSESCOMMAND \
    "\n" SHOWBLOCKEDPROCESSESCOMMAND

#define CREATEPCBUSAGE "\nUsage for " CREATEPCBCOMMAND	\
    "\n" CREATEPCBCOMMAND " -n <name> -p <priority> -c <class>\n"

#define DELETEPCBUSAGE "\nUsage for " DELETEPCBCOMMAND	\
    "\n" DELETEPCBCOMMAND " -n <name>\n"

#define BLOCKPCBUSAGE "\nUsage for " BLOCKPCBCOMMAND	\
    "\n" BLOCKPCBCOMMAND " -n <name>"

#define UNBLOCKPCBUSAGE "\nUsage for " UNBLOCKPCBCOMMAND	\
    "\n" UNBLOCKPCBCOMMAND " -n <name>"


int isValidCommand(char *);
int matches(char *, char *);
void help(char *, int);
void displayVersion();
int isValidInt(char* str);

/**
   Temporary fake implementations of queue pcb ops
**/
void deletePcb(char *name);

#endif
