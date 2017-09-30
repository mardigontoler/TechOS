#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <ctype.h>
#include <unistd.h>

#include "datetime.h"
#include "help.h"

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
    
    InitDate(); // sets date to today's date

    while(running){
	printf("\n\e[34mTechOS >\e[39m");
	numTokens = 0;
	fgets(input, MAXINPUTSIZE - MAXTOKENS - 1, stdin);

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
	else{
	    printf("\nERROR: command not recognized. Try \'help\'.\n");
	}
    }
    puts("\n");
    return 0;
}




/**
   Gets the name of a process from the command options
   and calls the appropriate function
   Returns 0 if not successful
   Returns 1 otherwise
**/
int callWithProcessName(void (*func)(char*), int numTokens, char **tokens){
    optind = 1;
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
	    (func)(name);
	    return 1;
	}
	else return 0;
    }
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
	printf("\n\nYou are about to exit. Type \"y\" if you are sure:  ");
	fgets(prompt, 8, stdin);
	if(prompt[0] == 'y')
	    return STOP;
    }
    else if(matches(command, SUSPENDCOMMAND)){

    }
    else if(matches(command, RESUMECOMMAND)){
    }
    else if(matches(command, SETPRIORITYCOMMAND)){
    }
    else if(matches(command, SHOWPCBCOMMAND)){
    }
    else if(matches(command, SHOWPROCESSESCOMMAND)){
    }
    else if(matches(command, SHOWREADYPROCESSESCOMMAND)){
    }
    else if(matches(command, SHOWBLOCKEDPROCESSESCOMMAND)){
    }
    else if(matches(command, CREATEPCBCOMMAND)){
	char *name;
	int class = 0, priority = 0;
	int valid = 1;
	while((c = getopt(numTokens, tokens, "n:c:p:")) != -1){
	    switch(c){
	    case 'n':
		name = optarg;
		name[MAXPROCESSNAMESIZE] = '\0';
		break;
	    case 'c':
		class = atoi(optarg);
		valid -= isValidInt(optarg);
		break;
	    case 'p':
		priority = atoi(optarg);
		valid -= isValidInt(optarg);
		break;
	    case '?':
		printf("\nMissing argument.");
		break;
	    default:
		valid -= 1;
		break;		
	    }
	}
	if(valid <= 0 )
	    printf("\n%s\n", CREATEPCBUSAGE);
	else{
	    // create pcb
	}	
    }
    else if(matches(command, DELETEPCBCOMMAND)){
	if(callWithProcessName(&deletePcb, numTokens, tokens) == 0)
	    printf("\n%s\n",DELETEPCBUSAGE);
    }
    else if(matches(command, BLOCKPCBCOMMAND)){
	
    }
    else if(matches(command, UNBLOCKPCBCOMMAND)){
    }
    while(optind < numTokens){
	printf("\nUnrecognized option: %s", tokens[optind++]);
    }

    return RUN;

}

