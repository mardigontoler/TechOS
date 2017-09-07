#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <ctype.h>
#include <unistd.h>

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


int main(int argc, char **argv){
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
    }
    puts("\n");
    return 0;
}




int isValidCommand(char *command){
    for(int i = 0; i < NUMCOMMANDS; i++){
	if(matches(command, validCommands[i]))
	    return 1;
    }
    return 0;
}
    
// returns 1 when string lhs matches string rhs
int matches(char *lhs, char *rhs){
    static int maxCmp = 100;
    //  printf("b");
    return (strncmp(lhs,rhs,maxCmp) == 0)? 1 : 0;
}


// Chooses the appropriate TechOS call after
// parsing the input for arguments for that call
int COMHAN(int numTokens, char **tokens){
    int opt;
    int c;
    int valid = 1;
    int parsing = 1;
    char* command = tokens[0];
    // use getopt, usually used to parse argv
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
		setDateUsage();
		break;
	    }
	}
    }
    else if(matches(command, TIMECOMMAND)){
	displayTime();
    }
    else if(matches(command, HELPCOMMAND)){
	displayHelp();
    }
    else if(matches(command, SHOWDATECOMMAND)){
	showDate();
    }
    else if(matches(command, VERSIONCOMMAND)){
	showVersion();
    }
    else if(matches(command, TERMINATECOMMAND)){
	printf("\n\nTerminating ...");
	return STOP;
    }
    
    
    while(optind < numTokens){
	printf("\nProblem with: %s", tokens[optind++]);
    }

    return RUN;

}


void setDateUsage(){
    printf("\nUsage for setdate: "\
	   "\nsetdate -m <month> -d <day> -y <year>");
}
