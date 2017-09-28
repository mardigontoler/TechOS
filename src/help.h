#ifndef HELP_H
#define HELP_H


#define COMPLETIONDATE ("10/2/2017")
#define VERSION ("2.0")

#define NUMCOMMANDS (6)

#define HELPCOMMAND ("help")
#define VERSIONCOMMAND ("version")
#define SHOWDATECOMMAND ("date")
#define SETDATECOMMAND ("setdate")
#define TIMECOMMAND ("time")
#define TERMINATECOMMAND ("exit")
// R2 commands
#define SUSPENDCOMMAND ("suspend")
#define RESUMECOMMAND ("resume")
#define SETPRIORITYCOMMAND ("priority")
#define SHOWPCBCOMMAND ("pcb")
#define SHOWPROCESSESCOMMAND ("processes")
#define SHOWREADYPROCESSESCOMMAND ("ready")
#define SHOWBLOCKEDPROCESSESCOMMAND ("blocked")
#define CREATEPCBCOMMAND ("create")
#define DELETEPCBCOMMAND ("delete")
#define BLOCKPCBCOMMAND ("block")
#define UNBLOCKPCBCOMMAND ("unblock")


#define SETDATEUSAGE ("\nUsage for setdate: "\
		      "\nsetdate -m <month> -d <day> -y <year>\n")

#define TIMEUSAGE ("\nUsage for time "\
	   "\ntime (-t | -T | -S)\n\nExample:\ntime -T\n")

#define TERMINATEUSAGE ("\nUsage for exit:"\
	   "\nexit\nYou will be asked if you're sure you want to exit. "\
	   "Enter the letter \"y\" if you are sure.")

#define DATEUSAGE ("\nUsage for date: "\
	"\ndate (-d | -D | -f | -F | -g | -G | -m | -s | -y)")

#define HELPUSAGE ("\nUsage for help:"\
	   "\nhelp [-c <command>]\n\nExample:\nhelp -c time\n")

#define VERSIONUSAGE ("\nUsage for version:\nversion\n")



int isValidCommand(char *);
int matches(char *, char *);
void help(char *, int);
void displayVersion();

#endif
