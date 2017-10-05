#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <ctype.h>
#include <unistd.h>

#include "datetime.h"
#include "help.h"
#include "queue.h"

#define MAXTOKENS (25)
#define MAXINPUTSIZE (300)
#define RUN (1) // not a command, controls main loop
#define STOP (0)


// prototypes
int COMHAN(int, char**);

int main(int argc, char **argv)
{
    
    // get input in a loop
    int running = RUN;
    char input[MAXINPUTSIZE];
    char *tokens[MAXTOKENS]; // array of tokens (array of pointers to strings)
    char *token;
    int numTokens;

    // set up queues
    initQueues();
    
    InitDate(); // sets date to today's date

    while(running){
	printf("\n" BLUECOLOR "TechOS >" DEFAULTCOLOR);
	numTokens = 0;
	if(fgets(input, MAXINPUTSIZE - MAXTOKENS - 1, stdin) != NULL)
        {
	    // convert input into tokens for parsing options
	    token = strtok(input, " ");
	    while(token != NULL && numTokens < MAXTOKENS){
		if(!isspace(token[0]) && token[0] != '\0'){
		    numTokens += 1;
		    // strip off possible newline
		    if(strlen(token) > 1 && token[strlen(token) - 1] == '\n')
			token[strlen(token) - 1] = '\0';
		    tokens[numTokens - 1] = token;
		}
		token = strtok(NULL, " ");
	    }	    
	    int validCommand = 0;
	    if(isValidCommand(tokens[0])){
		// let the command handler figure out the command and deal with the options
		running = COMHAN(numTokens, tokens);
	    }
	    else if(tokens[0] != NULL && tokens[0][0] != '\n'){
		printf("\nERROR: command not recognized. Try \'help\'.\n");
	    }
	}
    }
    puts("\n");
    return 0;
}




/**
   Gets pointer to the pcb from the command arguments
**/
pcb* findFromArgName(int numTokens, char **tokens){
    optind = 1; // help out getopt function
    int c;
    char *name;
    int valid = 0;
    while((c = getopt(numTokens, tokens, "n:")) != -1){
	switch(c){
	case 'n':
	    name = optarg;
	    valid = 1;
	    break;
	default:;
	}
	if(valid){
	    return FindPCB(name);
	}
    }
    return NULL;
}



/**
   Chooses the appropriate TechOS call after
   parsing the input for arguments for that call
**/
int COMHAN(int numTokens, char **tokens)
{
    int opt;
    int c;
    int valid = 1;
    int parsing = 1;
    char* command = tokens[0];
    char *cvalue = NULL; // used to hold onto the optarg for the help function
    // use getopt, usually used to parse argv

    // every time COMHAN is called we want to reuse getopt, so we help it out with its index
    optind = 1;
    if(matches(command,SETDATECOMMAND)){
	int month = -1, day = -1, year = -1;
	while((c = getopt(numTokens, tokens, "m:d:y:")) != -1){
	    switch(c){
	    case 'm':
		month = atoi(optarg); // atoi returns 0 if can't be parsed. Covered by setdate call
		break;
	    case 'd':
		day = atoi(optarg);
		break;
	    case 'y':
		year = atoi(optarg);
		break;
	    case '?':
		printf("\nERROR: Missing argument");
		break;
	    default:
		printf("\nERROR: incorrect option");
		break;
	    }
	}
	// call setDate if all required options are there
	if(month > 0 && day > 0 && year > 0){
	    SetDate(month, day, year);
	}
	else{printf(SETDATEUSAGE);}
    }
    else if(matches(command, TIMECOMMAND)){
	char timeOption = 't';
	while((c = getopt(numTokens, tokens, "tTS")) != -1){
	    timeOption = c;
	}
	printf("\n%s",GetTime(timeOption));

    }
    else if(matches(command, HELPCOMMAND)){
	int helped = 0;
	while((c = getopt(numTokens, tokens, "c:")) != -1){
	    switch(c){
	    case 'c':
		cvalue = optarg;
		help(cvalue, 0);
		helped = 1;
		break;
	    }
	}
	if(!helped){
	    help("\0",1); // show all commands
	}
    }
    else if(matches(command, SHOWDATECOMMAND)){
	char dateOption = 'F';
	while((c = getopt(numTokens, tokens, "dDfFgGmsy")) != -1){
	    dateOption = c;
	}
	printf("\n%s\n",GetDate(dateOption));
    }
    else if(matches(command, VERSIONCOMMAND)){
	displayVersion();
    }
    else if(matches(command, TERMINATECOMMAND)){
	char prompt[10];
	printf(REDCOLOR "\n\nYou are about to exit. Type \"y\" if you are sure:  " DEFAULTCOLOR);
	fgets(prompt, 8, stdin);
	if(prompt[0] == 'y')
	    return STOP;
    }
    else if(matches(command, SETPRIORITYCOMMAND)){
	char *name;
	int priority = 0, prioritySet = 0, nameSet = 0;
	while((c = getopt(numTokens, tokens, "n:p:")) != -1){
	    switch(c){
	    case 'n':
		name = optarg;
		nameSet = 1;
		break;
	    case 'p':
		priority = atoi(optarg);
		prioritySet = isValidInt(optarg);
	    }
	}
	if(nameSet && prioritySet){
	    pcb* ptr;
	    if((ptr=findFromArgName(numTokens, tokens)) != NULL){
		ptr->priority = priority;
	    }
	    else{
		printf(REDCOLOR "\nERROR: Couldn't set that priority. Exiting..." DEFAULTCOLOR);
		exit(1);
	    }
	}
	else{
	    printf("\n%s\n", SETPRIORITYUSAGE);
	}
    }
    else if(matches(command, SHOWPCBCOMMAND)){
	pcb* ptr;
	if((ptr = findFromArgName(numTokens, tokens)) != NULL){
	    printPCB(ptr);
	}
	else{
	    printf("\n%s\n", SHOWPCBUSAGE);
	}
    }
    else if(matches(command, SHOWPROCESSESCOMMAND)){
	printAllProcesses();// show processes
    }
    else if(matches(command, SHOWREADYPROCESSESCOMMAND)){
	// show ready processes
	printReadyProcesses();
    }
    else if(matches(command, SHOWBLOCKEDPROCESSESCOMMAND)){
	// show blocked processes
	printBlockedProcesses();
    }
    else if(matches(command, CREATEPCBCOMMAND)){
	char *name;
	unsigned char class = 0;
	int priority = 0;
	int nameSet = 0, classSet = 0, prioritySet = 0;
	//optind = 1;
	while((c = getopt(numTokens, tokens, "n:c:p:")) != -1){
	    switch(c){
	    case 'n':
		name = optarg;
		name[MAXPROCESSNAMESIZE] = '\0';
		nameSet = 1;
		break;
	    case 'c':
		class = optarg[0];
		classSet = 1; // TODO validate correct process class entered
		break;
	    case 'p':
		prioritySet = isValidInt(optarg);
		if(prioritySet)priority = atoi(optarg); // only set priority var if valid
		break;
	    case '?':
		printf("\nMissing argument.");
		break;
	    default:
		valid -= 1;
		break;		
	    }
	}
	if(nameSet == 0 || prioritySet == 0 || classSet == 0)
	    printf("\n%s\n", CREATEPCBUSAGE);
	else{
	    printf("\nCreating process %s with priority %d and class %c.", name, priority, class);
	    // create pcb
	    pcb* ptr = SetupPCB(name, class, priority);
	    if(ptr == NULL){
		printf(REDCOLOR "\nERROR: Could not create the process. Exiting..." DEFAULTCOLOR);
		exit(1);
	    }
	    else{
		InsertPCB(ptr);
	    }
	}	    
    }
    else if(matches(command, DELETEPCBCOMMAND)){
        pcb *ptr;
	if((ptr = findFromArgName(numTokens, tokens)) != NULL){
	    RemovePCB(ptr);
	    FreePCB(ptr);
	}
	else{
	    printf("\n%s\n", DELETEPCBUSAGE);
	}
    }
    else if(matches(command, BLOCKPCBCOMMAND)){
        pcb* ptr;
	if((ptr = findFromArgName(numTokens, tokens)) != NULL){
	    RemovePCB(ptr);
	    ptr->running_state = BLOCKED;
	    InsertPCB(ptr); // will now get inserted into the other queue
	}
	else{
	    printf("\n%s\n", BLOCKPCBUSAGE);
	}
    }
    else if(matches(command, UNBLOCKPCBCOMMAND)){
	pcb* ptr;
	if((ptr = findFromArgName(numTokens, tokens)) != NULL){
	    RemovePCB(ptr);
	    ptr->running_state = READY;
	    InsertPCB(ptr); // will now get inserted into the other queue
	}
	else{
	    printf("\n%s\n", UNBLOCKPCBUSAGE);	    
	}
    }
    
    return RUN;
}

