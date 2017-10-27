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
#define RUN (1) // not a command, controls main loop
#define STOP (0)


// prototypes
int COMHAN(int, char**);
void dispatchReady();


int main(int argc, char **argv)
{
    
    // get input in a loop
    int running = RUN;
    char input[MAXINPUTSIZE];
    char *tokens[MAXTOKENS]; // array of tokens (array of pointers to strings)
    char *token;
    int numTokens;

    time_t t;
    localtime(&t);
    srand(t);
    
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
    
    /* Time */
    else if(matches(command, TIMECOMMAND)){
	char timeOption = 't';
	while((c = getopt(numTokens, tokens, "tTS")) != -1){
	    timeOption = c;
	}
	printf("\n%s",GetTime(timeOption));
    }
    
    /* Help */
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
    
    /* Date */
    else if(matches(command, SHOWDATECOMMAND)){
	char dateOption = 'F';
	while((c = getopt(numTokens, tokens, "dDfFgGmsy")) != -1){
	    dateOption = c;
	}
	printf("\n%s\n",GetDate(dateOption));
    }
    
    /* Version */
    else if(matches(command, VERSIONCOMMAND)){
	displayVersion();
    }
    
    /* Exit */
    else if(matches(command, TERMINATECOMMAND)){
	char prompt[10];
	printf(REDCOLOR "\n\nYou are about to exit. Type \"y\" if you are sure:  " DEFAULTCOLOR);
	fgets(prompt, 8, stdin);
	if(prompt[0] == 'y')
	    return STOP;
    }
    
    /* Priority */
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
	    // Find the PCB, remove it from its queue, change its priority, and put back in queue
	    pcb* pcb_ptr = FindPCB(name);
	    if(pcb_ptr != NULL){
		RemovePCB(pcb_ptr);
		pcb_ptr->priority = priority;
		InsertPCB(pcb_ptr);
		
	    }
	    else{
		printf(REDCOLOR "The process was not found, so no priorities were changed.\n" DEFAULTCOLOR);
	    }
	}
	else{
	    printf(REDCOLOR "\nERROR: Couldn't set that priority." DEFAULTCOLOR);
	    printf("\n%s\n", SETPRIORITYUSAGE);
	}
    }
    
    /* PCB */
    else if(matches(command, SHOWPCBCOMMAND)){
	pcb* ptr;
	if((ptr = findFromArgName(numTokens, tokens)) != NULL){
	    printPCB(ptr);
	}
	else{
	    printf("\n%s\n", SHOWPCBUSAGE);
	}
    }
    
    /* Processes */
    else if(matches(command, SHOWPROCESSESCOMMAND)){
	printAllProcesses();// show processes
    }
    
    /* Ready */
    else if(matches(command, SHOWREADYPROCESSESCOMMAND)){
	// show ready processes
	printReadyProcesses();
    }
    
    /* Blocked */
    else if(matches(command, SHOWBLOCKEDPROCESSESCOMMAND)){
	// show blocked processes
	printBlockedProcesses();
    }
    
 //    /* Create */
 //    else if(matches(command, CREATEPCBCOMMAND)){
	// char name[MAXPROCESSNAMESIZE];
	// unsigned char class = 0;
	// int priority = 0;
	// int nameSet = 0, classSet = 0, prioritySet = 0;
	// //optind = 1;
	// while((c = getopt(numTokens, tokens, "n:c:p:")) != -1){
	//     switch(c){
	//     case 'n':
	// 	strcpy (name, optarg);
	// 	nameSet = 1;
	// 	break;
	//     case 'c':
	// 	class = optarg[0];
	// 	classSet = 1; // TODO validate correct process class entered
	// 	break;
	//     case 'p':
	// 	prioritySet = isValidInt(optarg);
	// 	if(prioritySet)priority = atoi(optarg); // only set priority var if valid
	// 	break;
	//     case '?':
	// 	printf("\nMissing argument.");
	// 	break;
	//     default:
	// 	valid -= 1;
	// 	break;		
	//     }
	// }
	// if(nameSet == 0 || prioritySet == 0 || classSet == 0)
	//     printf("\n%s\n", CREATEPCBUSAGE);
	// else{
	//     printf("\nCreating process %s with priority %d and class %c.", name, priority, class);
	//     pcb* ptr = SetupPCB(name, class, priority);
	//     if(ptr == NULL){
	// 	printf(REDCOLOR "\nERROR: Could not create the process. Exiting..." DEFAULTCOLOR);
	// 	exit(1);
	//     }
	//     else{
	// 	InsertPCB(ptr);
	//     }
	// }	    
 //    }
    
    /* Delete */
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

 //    /* Block */
 //    else if(matches(command, BLOCKPCBCOMMAND)){
	// pcb* ptr;
	// if((ptr = findFromArgName(numTokens, tokens)) != NULL){
	//     RemovePCB(ptr);
	//     ptr->running_state = BLOCKED;
	//     InsertPCB(ptr); // will now get inserted into the other queue
	// }
	// else{
	//     printf("\n%s\n", BLOCKPCBUSAGE);
	// }
 //    }

 //     Unblock 
 //    else if(matches(command, UNBLOCKPCBCOMMAND)){
	// pcb* ptr;
	// if((ptr = findFromArgName(numTokens, tokens)) != NULL){
	//     RemovePCB(ptr);
	//     ptr->running_state = READY;
	//     InsertPCB(ptr); // will now get inserted into the other queue
	// }
	// else{
	//     printf("\n%s\n", UNBLOCKPCBUSAGE);	    
	// }
 //    }

 //    /* Suspend */
 //    else if(matches(command, SUSPENDPCBCOMMAND)){
	//    pcb* ptr;
	//    if((ptr = findFromArgName(numTokens, tokens)) != NULL){
	//        RemovePCB(ptr);
	//        ptr->suspension_state = SUSPENDED;
	//        InsertPCB(ptr);
	//    }
	//    else{
	//        printf(REDCOLOR "ERROR: Could not find a process with that name.\n" DEFAULTCOLOR);
 //           printf("\n%s\n", SUSPENDPCBUSAGE);	    
	//    }	
 //    }

 //    /* Resume */
 //    else if(matches(command, RESUMEPCBCOMMAND)){
	// pcb* ptr;
	// if((ptr = findFromArgName(numTokens, tokens)) != NULL){
	//     RemovePCB(ptr);
	//     ptr->suspension_state = NOTSUSPENDED;
	//     InsertPCB(ptr);
	// }
	// else{
	//     printf(REDCOLOR "ERROR: Could not find a process with that name.\n" DEFAULTCOLOR);
 //        printf("\n%s\n", RESUMEPCBUSAGE);	    
	// }	
 //    }

    /* Dispatch */
    else if(matches(command, DISPATCHCOMMAND)){
	dispatchReady();
    }

    /* Load */
    else if(matches(command, LOADCOMMAND)){
        char name[MAXPROCESSNAMESIZE];
        char file_name[128];
        unsigned char class = 0;
        int priority = 0;
        int nameSet = 0, filenameSet = 0, prioritySet = 0;
        //optind = 1;
        while((c = getopt(numTokens, tokens, "n:p:f:")) != -1){
            switch(c){
            case 'n':
                strcpy (name, optarg);
                nameSet = 1;
                break;
            case 'p':
                prioritySet = isValidInt(optarg);
                if(prioritySet)priority = atoi(optarg);
                break;
            case 'f':
                strcpy(file_name, optarg);
                filenameSet = 1;
                break;
            case '?':
                printf("\nMissing argument.");
                break;
            default:
                valid -= 1;
                break;          
            }
        }
        if(nameSet == 0 || prioritySet == 0 || filenameSet == 0){
            printf("\nname:%s", name);
            printf("\npriority:%d", priority);
            printf("\nfilename:%s", file_name);
            printf("\n%s\n", LOADUSAGE);
        }
        else{
            printf("\nCreating process %s with priority %d from file %s", name, priority, file_name);
            pcb* ptr = LoadProcess(name, priority, file_name);
        }           
    }

    return RUN;
}

/**
 * Use the execute binary to run all processes in the ready queue
 * Processes should change from ready to running as they are dispatched
 * A nonzero return value from the system call indicates that the process
 * has experienced an interupt
 **/
void dispatchReady(){
    pcb* oldHead = GetNextReadyNotSuspended();    
    if(oldHead == NULL){
	printf(REDCOLOR "ERROR: No ready processes to dispatch!\n");
	return;
    }
    while(oldHead != NULL){
	// it is now running, remove it from whatever queue it was in and reset its properties
	RemovePCB(oldHead);
	oldHead->running_state = RUNNING;
	oldHead->suspension_state = NOTSUSPENDED;
        //InsertPCB(oldHead);
	char systemArgument[100];
	char* executeCommand = "./execute ";
        
        sprintf(systemArgument, "%s %s %d", executeCommand, oldHead->file_name, oldHead->offset+1);
	//system("./execute " oldHead->path offsetString);
	// need to copy info into systemArgument and use that to cal system()

	// strcat(systemArgument, executeCommand);
	// strcat(systemArgument, oldHead->file_name);
	// strcat(systemArgument, offsetString);

	// run the 'execute' binary with the PPCB's offset + 1
	// the return value will indicate whether there was an interrupt.
	// 0 indicates that the process is finished and should be removed
	// Otherwise there was an interrupt. The interrupt value should be divided by 256
	// and stored as the offset of the process's PCBdispa
	//printf("\nDEBUG systemArgument %s\n",systemArgument);
	int interrupt = system(systemArgument);
        printf("\n%d", interrupt);
	if(interrupt == 0){
	    FreePCB(oldHead);
	}
	else{	    // there was an interrupt
	    // block and suspend
	    oldHead->offset = interrupt/256;
            printf("\nRemoving %s", oldHead->process_name);
	    RemovePCB(oldHead);
	    oldHead->suspension_state = SUSPENDED;
	    oldHead->running_state = BLOCKED;
	    InsertPCB(oldHead);
	}
	// we need to choose the next process to dispatch.
	// If the ready queue is empty, just unblock a process in the blocked queue
	// and unsuspend it and run it.
	// If neither queue is empty, it's 50/50 whether we run the next ready process
	// or the next blocked process

	// THIS IS ASSUMING (as per module 3 instructoins) THAT ALL READY PROCESSES
	// WILL NOT BE SUSPENDED AND THAT ALL BLOKCED PROCESSES WILL BE SUSPENDED

	// Check if there are no ready processes but there are blocked processes
	if(GetNextReadyNotSuspended() == NULL && GetNextBlockedSuspended() != NULL){
	    oldHead = GetNextBlockedSuspended();
	}
	//check if there are ready processes but no blocked processes
	else if(GetNextReadyNotSuspended() != NULL && GetNextBlockedSuspended() == NULL){
	    oldHead = GetNextReadyNotSuspended();
	}
	// check if both are not empty
	else if(GetNextReadyNotSuspended() != NULL && GetNextBlockedSuspended() != NULL){
	    // choose blocked or unblocked 50/50
	    if(rand()%10 >= 5){
		oldHead = GetNextReadyNotSuspended();
	    }
	    else{
		oldHead = GetNextBlockedSuspended();
	    }
	}
	// otherwise, there are no more processes remaining in either queue
	else{
	    oldHead = NULL;
	}
    }
}
