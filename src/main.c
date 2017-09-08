#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <ctype.h>
#include <unistd.h>

#include "datetime.h"

#define VERSION ("0.1")

#define MAXTOKENS (25)
#define MAXINPUTSIZE (300)
#define RUN (1) // not a command, controls main loop
#define STOP (0)

#define HELPCOMMAND ("help")
#define VERSIONCOMMAND ("version")
#define SHOWDATECOMMAND ("date")
#define SETDATECOMMAND ("setdate")
#define TIMECOMMAND ("time")
#define TERMINATECOMMAND ("exit")
#define NUMCOMMANDS (6)

char *validCommands[NUMCOMMANDS] = {
    HELPCOMMAND,
    VERSIONCOMMAND,
    SETDATECOMMAND,
    TIMECOMMAND,
    TERMINATECOMMAND,
    SHOWDATECOMMAND
};

// prototypes
int COMHAN(int, char**);
int isValidCommand(char*);
int matches(char*, char*);
void setDateUsage();
void helpUsage();
void versionUsage();
void timeUsage();
void terminateUsage();
void displayVersion();
void help(char *optarg, int showAll);

int main(int argc, char **argv)
{
    // get input in a loop
    int running = RUN;
    char input[MAXINPUTSIZE];
    char *tokens[MAXTOKENS]; // array of tokens (array of pointers to strings)
    char *token;
    int numTokens;

    while(running){
	printf("\nTechOS >");
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


// Chooses the appropriate TechOS call after
// parsing the input for arguments for that call
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
	    default:
		printf("\nERROR: incorrect option");
		break;
	    }
	}
	// call setDate if all required options are there
	if(month > 0 && day > 0 && year > 0){
	    SetDate(month, day, year);
	}
	else{setDateUsage();}
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
	printf("\n%s",GetDate(dateOption));
    }
    else if(matches(command, VERSIONCOMMAND)){
	displayVersion();
    }
    else if(matches(command, TERMINATECOMMAND)){
	printf("\n\nTerminating ...");
	return STOP;
    }
    while(optind < numTokens){
	printf("\nUnrecognized option: %s", tokens[optind++]);
    }

    return RUN;

}


void setDateUsage()
{
    printf("\nUsage for setdate: "\
	   "\nsetdate -m <month> -d <day> -y <year>\n");
}

void timeUsage(){
    printf("\nUsage for time "\
	   "\ntime (-t | -T | -S)\n\nExample:\ntime -T\n");
}

void terminateUsage(){
    printf("\nUsage for exit:"\
	   "\nexit\n");
}

void dateUsage(){
    printf("\nUsage for date: "\
	"\ndate (-d | -D | -f | -F | -g | -G | -m | -s | -y");
}

void helpUsage(){
    printf("\nUsage for help:"\
	   "\nhelp [-c <command>]\n\nExample:\nhelp -c time\n");
}

void versionUsage(){
    printf("\nUsage for version:\nversion\n");
}

void displayVersion(){
    printf("\nCS 450 Project\nTechOS\nVersion = %s\n", VERSION);
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
	helpUsage();
    }
    else{
	// search for a command that they want the usage of
	if(matches(HELPCOMMAND,      command))helpUsage();
	if(matches(VERSIONCOMMAND,   command))versionUsage();
	if(matches(SHOWDATECOMMAND,  command))dateUsage();
	if(matches(SETDATECOMMAND,   command))setDateUsage();
	if(matches(TIMECOMMAND,      command))timeUsage();
	if(matches(TERMINATECOMMAND, command))terminateUsage();
    }
}
