
#include "help.h"
#include <stdio.h>
#include <string.h>

char *validCommands[NUMCOMMANDS] = {
    HELPCOMMAND,
    VERSIONCOMMAND,
    SETDATECOMMAND,
    TIMECOMMAND,
    TERMINATECOMMAND,
    SHOWDATECOMMAND
};

void displayVersion(){
    printf("Russell Short, Mardigon Toler\nTechOS\nVersion: %s\n%s", VERSION, COMPLETIONDATE);
}


int isValidCommand(char *command)
{
    for(int i = 0; i < NUMCOMMANDS; i++){
	if(matches(command, validCommands[i]))
	    return 1;
    }
    return 0;
}
    
// returns 1 when string lhs matches string rhs
int matches(char *lhs, char *rhs)
{
    static int maxCmp = 100;
    //  printf("b");
    return (strncmp(lhs,rhs,maxCmp) == 0)? 1 : 0;
}


// when they ask for help, COMHAN gives help() the command,
// and help() shows the usage
void help(char *command, int showAll){
    if(showAll){
	printf("\nTechOS provides you with the following commands:");
	for(int i = 0; i < NUMCOMMANDS; i++){
	    printf("\n%s", validCommands[i]);
	}
	printf("\n\nThe help command can tell you how to use the other commands!");
	printf(HELPUSAGE);
    }
    else{
	// search for a command that they want the usage of
	if(matches(HELPCOMMAND,      command))printf(HELPUSAGE);
	if(matches(VERSIONCOMMAND,   command))printf(VERSIONUSAGE);
	if(matches(SHOWDATECOMMAND,  command))printf(DATEUSAGE);
	if(matches(SETDATECOMMAND,   command))printf(SETDATEUSAGE);
	if(matches(TIMECOMMAND,      command))printf(TIMEUSAGE);
	if(matches(TERMINATECOMMAND, command))printf(TERMINATEUSAGE);
    }
}
