
#include "help.h"
#include "queue.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char *validCommands[NUMCOMMANDS] = {
    HELPCOMMAND,
    VERSIONCOMMAND,
    SETDATECOMMAND,
    TIMECOMMAND,
    TERMINATECOMMAND,
    SHOWDATECOMMAND,

    // R2
    SETPRIORITYCOMMAND,
    SHOWPCBCOMMAND,
    SHOWPROCESSESCOMMAND,
    SHOWREADYPROCESSESCOMMAND,
    SHOWBLOCKEDPROCESSESCOMMAND,
    //CREATEPCBCOMMAND,
    //DELETEPCBCOMMAND,
    //BLOCKPCBCOMMAND,
    //UNBLOCKPCBCOMMAND,
    //SUSPENDPCBCOMMAND,
    //RESUMEPCBCOMMAND,
    DISPATCHCOMMAND,

    // R3
    LOADCOMMAND,

    //R4
    VIEWDIRCOMMAND,
    CHANGEDIRCOMMAND,
    CREATEFOLDERCOMMAND,
    REMOVEFOLDERCOMMAND,
    REMOVEFILECOMMAND,
    CREATEFILECOMMAND
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

int isValidInt(char* str){
    // atoi returns 0 if conversion failed
    // But the string might have actually been 0
    if(atoi(str) == 0 && str[0] != '0')
	return 0;
    return 1;
}


// returns 1 when string lhs matches string rhs
int matches(char *lhs, char *rhs)
{
    static int maxCmp = 100;
    if(lhs != NULL){
	return (strncmp(lhs,rhs,maxCmp) == 0)? 1 : 0;
    }
    return 0;
}


// when they ask for help, COMHAN gives help() the command,
// and help() shows the usage
void help(char *command, int showAll){
    printf(YELLOWCOLOR);
    if(showAll){
	printf("\nTechOS provides you with the following commands:\n");
	for(int i = 0; i < NUMCOMMANDS; i++){
	    char *b = (i % 2 == 0) ? "\n" : "\t\t\t"; // alternate tabs and newlines
	    printf("%s%s", b, validCommands[i]);
	}
	printf("\n\nThe help command can tell you how to use the other commands!");
	printf(HELPUSAGE);
    }
    else{
	// search for a command that they want the usage of
	if(matches(HELPCOMMAND,                 command))printf(HELPUSAGE);
	if(matches(VERSIONCOMMAND,              command))printf(VERSIONUSAGE);
	if(matches(SHOWDATECOMMAND,             command))printf(DATEUSAGE);
	if(matches(SETDATECOMMAND,              command))printf(SETDATEUSAGE);
	if(matches(TIMECOMMAND,                 command))printf(TIMEUSAGE);
	if(matches(TERMINATECOMMAND,            command))printf(TERMINATEUSAGE);
	// R2
	if(matches(SETPRIORITYCOMMAND,          command))printf(SETPRIORITYUSAGE);
	if(matches(SHOWPCBCOMMAND,              command))printf(SHOWPCBUSAGE);
	if(matches(SHOWPROCESSESCOMMAND,        command))printf(SHOWPROCESSESUSAGE);
	if(matches(SHOWREADYPROCESSESCOMMAND,   command))printf(SHOWREADYPROCESSESUSAGE);
	if(matches(SHOWBLOCKEDPROCESSESCOMMAND, command))printf(SHOWBLOCKEDPROCESSESUSAGE);
	//if(matches(CREATEPCBCOMMAND,            command))printf(CREATEPCBUSAGE);
	//if(matches(DELETEPCBCOMMAND,            command))printf(DELETEPCBUSAGE);
	//if(matches(BLOCKPCBCOMMAND,             command))printf(BLOCKPCBUSAGE);
	//if(matches(UNBLOCKPCBCOMMAND,           command))printf(UNBLOCKPCBUSAGE);
	if(matches(VIEWDIRCOMMAND,              command))printf(VIEWDIRUSAGE);
	if(matches(CHANGEDIRCOMMAND,            command))printf(CHANGEDIRUSAGE);
	if(matches(CREATEFOLDERCOMMAND,         command))printf(CREATEFOLDERUSAGE);
	if(matches(REMOVEFOLDERCOMMAND,         command))printf(REMOVEFOLDERUSAGE);
	if(matches(CREATEFILECOMMAND,           command))printf(CREATEFILEUSAGE);
	if(matches(REMOVEFILECOMMAND,           command))printf(REMOVEFILEUSAGE);
    if(matches(LOADCOMMAND,                 command))printf(LOADUSAGE);
	if(matches(DISPATCHCOMMAND,             command))printf(DISPATCHUSAGE);
    }
    printf(DEFAULTCOLOR);
}
