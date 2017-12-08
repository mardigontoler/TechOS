#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <ctype.h>
#include <unistd.h>
#include <time.h>
#include <sys/stat.h>
#include <dirent.h>

#include "datetime.h"
#include "help.h"
#include "queue.h"
#include "security.h"
#include "history.h"


// Windows definitions
// Linux definitions

#define MAXTOKENS (25)
#define MAXINPUTSIZE (300)
#define RUN (1) // not a command, controls main loop
#define STOP (0)

#define SYSTEM_VIEWDIR "ls"
#define DIR_MAIN "TOS"

// prototypes
int COMHAN(int, char**);
void dispatchReady();

User main_user;


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

    /* Security */
    LoadUsers();

    // set up queues
    initQueues();

    InitDate(); // sets date to today's datedir

    if (initLogFile() == NULL) {
        return 0;
    }


    // DEBUG for ls
    chdir(DIR_MAIN);

    int user_id = UserLogin();
    if (user_id == -1) {
        printf("\nAccess Denied\n");
        return -1;
    }

    main_user = GetUser(user_id);

    printf("\nWelcome, %s!\n", main_user.username);

    while(running){
        printf("\n" BLUECOLOR "TechOS >" DEFAULTCOLOR);
        numTokens = 0;
        if(fgets(input, MAXINPUTSIZE - MAXTOKENS - 1, stdin) != NULL)
        {
            saveCommand(input);

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

// char* getName(char *namePtr, int length) {
//         fgets(namePtr, length, stdin);
// }

char* findName(int numTokens, char **tokens){
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
            return name;
        }
        else{
            printf("ERROR: You must specify a name.\n");
        }
    }
    return NULL;
}


char* getName(char *namePtr, int length){
    printf("Enter the name:\n");

    fgets(namePtr, length, stdin);
    // replace newline character with null terminator
    if(namePtr != NULL){
        for(int i = 0; i < length; i++){
            if(namePtr[i] == '\n'){
                namePtr[i] = '\0';
                if(i == 0){
                    namePtr = NULL;
                    return NULL;
                }
                return namePtr;
            }
        }
    }
    return NULL;
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
            //printf("\nname:%s", name);
            //printf("\npriority:%d", priority);
            //printf("\nfilename:%s", file_name);
            //printf("\n%s\n", LOADUSAGE);
        }
        else{
            //printf("\nCreating process %s with priority %d from file %s", name, priority, file_name);
            pcb* ptr = LoadProcess(name, priority, file_name);
        }
    }

    /* ls */
    else if(matches(command, VIEWDIRCOMMAND)){
        char name[1000];
        strcpy(name, ""); // clears the name buffer each time
        int pathSet = 0;
        int showSizes = 0;
        char sysArgs[1002];
        while((c = getopt(numTokens, tokens, "ps")) != -1){
            switch(c){
            case 'p':
                //strncpy(name, optarg, 999);
                getName(name, 999);
                if(name == NULL){
                    strcpy(name, "");
                }
                else{
                    pathSet = 1;
                }
                break;
            case 's':
                showSizes = 1;
            default:
                break;
            }
        }
        printf("\n");
        if(pathSet == 0)
            strcpy(name, ".");
        if(showSizes == 1)
            sprintf(sysArgs, SYSTEM_VIEWDIR " '%s' -s", name);
        else
            sprintf(sysArgs, SYSTEM_VIEWDIR " '%s'", name);
        system(sysArgs);
    }

    /* cd */
    else if(matches(command, CHANGEDIRCOMMAND)){
        char name[1000];
        if(getName(name, 999) == NULL){
            printf(REDCOLOR "Invalid path.\n" DEFAULTCOLOR);
        }
        else if(chdir(name) == -1){
            printf("\nERROR: Could not find directory %s.", name);
            strcpy(name, "");
        }
    }

    else if(matches(command, CREATEFOLDERCOMMAND)){
        char name[1000];
        if(getName(name, 999) == NULL){
            printf(REDCOLOR "Invalid path.\n" DEFAULTCOLOR);
        }
        // check first if directory already exists before creating it
        // S_IFREG indicates a REGULAR file
        else if(mkdir(name, S_IFREG) != 0){
            printf(REDCOLOR "ERROR: This directory already exists or you lack permission to create it.\n");
        }
        else{
            printf("Successfully created directory %s.\n",name);
        }
    }

    /* rmdir */
    else if(matches(command, REMOVEFOLDERCOMMAND)){
        char name[1000];
        getName(name, 999);
        if(rmdir(name) != 0){
            printf(REDCOLOR "ERROR: This directory is not empty.\n");
        }
        else{
            printf( "Successfully removed %s\n",name);
        }
    }

    /* mkfile */
    else if(matches(command, CREATEFILECOMMAND)){
        char name[1000];
        FILE* f;
        if(getName(name, 999) == NULL){
            printf(REDCOLOR "Invalid path.\n" DEFAULTCOLOR);
        }
        // Check if file exists
        else if((f = fopen(name, "r")) != NULL){
            printf(REDCOLOR "ERROR: That file already exists.");
            fclose(f);
        }
        else{
            f = fopen(name, "w");
            fclose(f);
            printf("Successfully created file %s.\n",name);
        }
    }

    /* rm file */
    else if(matches(command, REMOVEFILECOMMAND)){
        char name[1000];
        if(getName(name, 999) == NULL){
            printf(REDCOLOR "Invalid path.\n" DEFAULTCOLOR);
        }
        else if(unlink(name) != 0 ){
            printf(REDCOLOR "ERROR: Could not remove %s.\n", name);
        }
        else{
            printf("Successfully removed file %s.\n", name);
        }
    }

    /* Create User */
    else if(matches(command, CREATEUSERCOMMAND)) {
        char username[16];
        char password[16];
        int adminflag = 0;
        int unflag = 0;
        int pwflag = 0;
        while((c = getopt(numTokens, tokens, "n:p:a")) != -1) {
            switch(c) {
                case 'n':
                    unflag = 1;
                    strcpy(username, optarg);
                    break;
                case 'p':
                    pwflag = 1;
                    strcpy(password, optarg);
                    break;
                case 'a':
                    adminflag = 1;
                    break;
                default:
                    break;
            }
        }
        if (adminflag == 1 && unflag == 1 && pwflag == 1){
            CreateUser(username, password, ADMIN, main_user.authority);
        }
        else if (adminflag == 0 && unflag == 1 && pwflag == 1){
            CreateUser(username, password, BASIC, main_user.authority);
        }
        else {
            printf(CREATEUSERUSAGE);
        }
    }

    /* Remove User */
    else if(matches(command, REMOVEUSERCOMMAND)) {
        char username[16];
        while((c = getopt(numTokens, tokens, "n:")) != -1) {
            switch(c) {
                case 'n':
                    strcpy(username, optarg);
                    break;
                default:
                    break;
            }
        }
        if (username != NULL){
            RemoveUser(username,  main_user.authority);
        }
        else {
            printf(REMOVEUSERUSAGE);
        }
    }

    /* Change Password */
    else if(matches(command, CHANGEPASSWORDCOMMAND)) {
        char username[16];
        while((c = getopt(numTokens, tokens, "n:")) != -1) {
            switch(c) {
                case 'n':
                    strcpy(username, optarg);
                    break;
                default:
                    break;
            }
        }
        if (username != NULL){
            ChangePassword(username, main_user.username);
        }
        else {
            printf(CHANGEPASSWORDUSAGE);
        }
    }

    /* Add Admin */
    else if(matches(command, ADDADMINCOMMAND)) {
        char username[16];
        while((c = getopt(numTokens, tokens, "n:")) != -1) {
            switch(c) {
                case 'n':
                    strcpy(username, optarg);
                    break;
                default:
                    break;
            }
        }
        if (username != NULL){
            AddAdmin(username, main_user.authority);
        }
        else {
            printf(ADDADMINUSAGE);
        }
    }

    /* Remove Admin */
    else if(matches(command, REMOVEADMINCOMMAND)) {
        char username[16];
        while((c = getopt(numTokens, tokens, "n:")) != -1) {
            switch(c) {
                case 'n':
                    strcpy(username, optarg);
                    break;
                default:
                    break;
            }
        }
        if (username != NULL){
            RemoveAdmin(username, main_user.authority);
        }
        else {
            printf(REMOVEADMINUSAGE);
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
        char* executeCommand = "./execute";

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
        //printf("\n%d", interrupt);
        if(interrupt == 0){
            printf("\n[%d] Process %s is completed.", interrupt, oldHead->process_name);
            FreePCB(oldHead);
        }
        else{	    // there was an interrupt
            // block and suspend
            oldHead->offset = interrupt/256;
            printf("\n[%d] Suspending %s.",interrupt, oldHead->process_name);
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
