#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <ctype.h>
#include <unistd.h>
#include <time.h>

#include "datetime.h"
#include "help.h"
#include "queue.h"

#define MAXTOKENS (25)
#define MAXINPUTSIZE (300)
#define RUN (1)
#define STOP (0)


int COMHAN(int, char**);
void dispatchReady();

int main(int argc, char **argv)
{
	int running = RUN;
	char input[MAXINPUTSIZE];
	char *tokens[MAXTOKENS];
	char *token;
	int numTokens;

	time_t t;
	localtime (&t);
	srand(t);

	initQueue();
	InitDate();

	while (running){
		pritnf("\n" BLUECOLOR "TechOS >" DEFAULTCOLOR);
		numTokens = 0;
		if (fgets(input, MAXINPUTSIZE - MAXTOKENS - 1, stdin) != NULL) {
			// convert input into tokens for parsing options
			token = strtok(input, " ");
			while (token != NULL && numTokens < MAXTOKENS) {
				if (!isspace(token[0]) && token[0] != '\0') {
					numTokens += 1;
					// strip off possible newline
					if (strlen(token) > 1 && token[strlen(token) - 1] == '\n')
						token[strlen(token) - 1] = '\0';
					tokens[numTokens - 1] = token;
				}
				token = strtok(NULL, " ");
			}
			int validCommand = 0;
			if (isValidCommand(tokens[0])) {
				// let the command handler figure out the command and
				// deal with the options
				running = COMHAN(numTokens, tokens);
			} else if (tokens[0] != NULL && tokens[0][0] != '\n') {
				printf("\nERROR: command not recognized. Try \'help\'.\n");
			}
		}
	}
	puts("\n");
	return 0;
}

/* Gets pointer to the pcb from the command arguments
 *
 */
pcb* findFromArgName(int numTokens, char **tokens)
{
	optind = 1; // help out getopt fn
	int c;
	char *name;
	int valid = 0;
	while ((c = getopt(numTokens, tokens, "n:")) != -1) {
		switch (c) {
		case 'n':
			name = optarg;
			valid = 1;
			break;
		default:;
		}
		if (valid) {
			return FindPCB(name);
		}
	}
	return NULL;
}

/* Chooses the appropriate TechOS call after parsing the input for arguments
 *
 */
int COMHAN(int numTokens, char **tokens)
{
	int opt;
	int c;
	int valid = 1;
	int parsing = 1;
	char* command = tokens[0];
	char* cvalue = NULL; // used to hold onto the optarg for the help fn
	// use getopt, usually used to parse argv

	// every time COMHAN is called we want to reuse get opt, so we help it out with its index
	optind = 1;
	if (matches(command, SETDATECOMMAND)) {
		int month = -1, day = -1, year = -1;
		while 
	}
}