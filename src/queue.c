//
// Created by Russell on 10/1/2017.
//

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "queue.h"
#include "help.h"

/**
 * You have two queues or four queues.
 * You must have a Ready queue that holds all PCBs that are in the ready state.
 * The PCBs in this queue should be stored in priority order, with the highest priority
 * at the head of the queue. You must have a Blocked queue that holds all PCBs in the blocked
 * state. The PCBs in this queue should be stored in FIFO order.
 *
 * You may also crete separate queues for suspended PCBs. This would be the two
 * additional queues previously mentioned: A Suspended Ready queue and a Suspended Blocked queue.
 *
 */


struct queue {
    int count;
    pcb* head;
    pcb* tail;
};

struct queue ready_queue;   // Holds all processes in the ready state, sorted by priority.
struct queue blocked_queue; // Holds all processes in the blocked state, sorted FIFO.

/***
 * Initializes all queues for use.
 */
void initQueues() {
    ready_queue.count = 0;
    ready_queue.head = NULL;
    ready_queue.tail = NULL;
    blocked_queue.count = 0;
    blocked_queue.head = NULL;
    blocked_queue.tail = NULL;
}


/**
 * Allocates memory for a new PCB.
 * Returns either a pointer to the new PCB or NULL if there is an error during allocation.
 */
pcb* AllocatePCB() {
    pcb* alloc_process = malloc(sizeof(pcb));
    return alloc_process;
}

/**
 * Frees all memory asscoated with a given PCB.
 * Take a pointer to the PCB as a Parameter.
 */
int FreePCB(pcb* process) {
    free(process);
}

/**
 * Calls AllocatePCB() to create an empty PCB.
 * After, the PCB information should be initialized, state to ready and not suspended.
 * Three params: Process name, process class, and process priority
 * Returns pointer to pcb or null if an error occurs
 */
pcb* SetupPCB(char* process_name, unsigned char process_class, int priority) {
    pcb* new_pcb = AllocatePCB();

    if(strlen(process_name)>8){
        printf("ERROR: The name of this process is larger than 8 characters.\n");
        return NULL; 
    }
    if(priority > 9 || priority < 0){
        printf("ERROR: The priority isn't between 0 and 9.\n");
        return NULL;
    }
    strcpy(new_pcb->process_name, process_name);
    new_pcb->process_class = process_class;
    new_pcb->priority = priority;
    new_pcb->running_state = READY;
    new_pcb->suspension_state = NOTSUSPENDED;
    return new_pcb;
}

/**
 * Searches all queues for a process with a given name.
 * Can be used to make sure a process name is unique.
 * @return - Pointer to the found pcb or NULL if not found.
 */
pcb* FindPCB(char* name){
    pcb* find_pcb;

    find_pcb = ready_queue.head;
    if(find_pcb == NULL){
	return NULL;
    }
    do {
	
        if(strcmp(name, find_pcb->process_name) == 0) {
            printf("Found it!\n");
            return find_pcb;
        }
        find_pcb = find_pcb->next_pcb;
    } while(find_pcb != NULL);

    find_pcb = blocked_queue.head;
    if(find_pcb == NULL){
	return NULL;
    }
    do {
        if(strcmp(name, find_pcb->process_name) == 0) {
            printf("Found it!\n");
            return find_pcb;
        }
        find_pcb = find_pcb->next_pcb;
    } while(find_pcb != NULL);
    printf("Did not find process %s", name);
    return NULL;
}

/**
 * Inserts a PCB into the appropriate queue.
 * Ready queue is a priority queue.
 * Blocked queue is a FIFO queue.
 * @param process - pcb to be inserted.
 */
void InsertPCB(pcb* process) {
    if (FindPCB(process->process_name) != NULL) {
        printf("ERROR: Process Exists! Process not added.");
        return;
    }

    // BLOCKED QUEUE
    if (process->running_state == BLOCKED) {
        if (blocked_queue.count != 0) {
            blocked_queue.tail->next_pcb = process;
            blocked_queue.tail = process;
            blocked_queue.count++;
        }
        else {
            blocked_queue.head = process;
            blocked_queue.tail = process;
	    process->next_pcb = NULL;
            blocked_queue.count++;
        }

    // READY QUEUE
    }
    else {
        if (ready_queue.count != 0) {
            if (process->priority > ready_queue.head->priority){
                process->next_pcb = ready_queue.head;
                ready_queue.head = process;
                ready_queue.count++;
            }
	    //else if(ready_queue.count == 1){
		
	    //}
            else {
                pcb* store_temp_pcb = ready_queue.head;
                pcb* temp_pcb = ready_queue.head;//->next_pcb; casuing problem when count is 1?
		
                while (temp_pcb != NULL && temp_pcb->priority > process->priority) {
                    store_temp_pcb = temp_pcb;
                    temp_pcb = temp_pcb->next_pcb;
                }
                if (temp_pcb->next_pcb == NULL){
                    temp_pcb->next_pcb = process;
		    process->next_pcb = NULL;
                    ready_queue.tail = process;
                    ready_queue.count++;
                }
                else {
                    process->next_pcb = temp_pcb;
                    store_temp_pcb->next_pcb = process;
                    ready_queue.count++;
                }
            }
        }
        else { // queue was empty
            ready_queue.head = process;
            ready_queue.tail = process;
	    process->next_pcb = NULL;
            ready_queue.count++;
        }
    }
}

/**
 * Removes a PCB from the queue in which it is currently stored.
 * Should not free the memory associated with the pcb.
 * @return success or error code.
 * @param process - process to be removed
 */
int RemovePCB(pcb* process){
    pcb* index;
    if (blocked_queue.head == process){
        blocked_queue.head = process->next_pcb;
        process->next_pcb = NULL;
        blocked_queue.count--;
        if(blocked_queue.tail == process) {
            blocked_queue.tail = NULL;
        }
	return 1;
    }
    index = blocked_queue.head;
    while(index != NULL && index->next_pcb != NULL){
        if (index->next_pcb == process) {
            if (blocked_queue.tail == process) {
                blocked_queue.tail = index->next_pcb;
            }
            index->next_pcb->next_pcb = process->next_pcb;
            process->next_pcb = NULL;
            blocked_queue.count--;
            return 1;
        }
	index = index->next_pcb;
    }
    index = ready_queue.head;
    // case when the process is at the head
    if(ready_queue.head == process){
	if(ready_queue.tail == process){
	    ready_queue.tail = NULL;
	}
	ready_queue.head = process->next_pcb;
	process->next_pcb = NULL;
	ready_queue.count--;
	return 1;
    }

    while(index != NULL && index->next_pcb != NULL){
        if (index->next_pcb == process) {
            if (ready_queue.tail == process) {
                ready_queue.tail = index;
            }
            index->next_pcb = process->next_pcb;
            process->next_pcb = NULL;
            ready_queue.count--;
            return 1;
        }
	index = index->next_pcb;
    }
    printf("\nERROR: Did not find process to be removed.");
    return 0;
}

void printPCB(pcb* process){
    printf(MAGENTACOLOR);
    printf("\nProcess name: %s ", process->process_name);
    printf("\nProcess class: %c", process->process_class);
    printf("\nRunning state: %d", process->running_state);
    printf("\nPriority:  %d\n", process->priority);
    printf(DEFAULTCOLOR);
}

void printReadyProcesses(){
    pcb* index = ready_queue.head;
    while(index != NULL) {
        printf("-------------\n");
        printPCB(index);
        printf("-------------\n");
        index = index->next_pcb;
    }
    printf("\n");
}

void printBlockedProcesses(){
    pcb* index = blocked_queue.head;
    while(index != NULL) {
        printf("-------------");
        printPCB(index);
        printf("-------------\n");
        index = index->next_pcb;
    }
    printf("\n");    
}

void printAllProcesses(){
    printf("Ready Processes:\n");
    printReadyProcesses();
    printf("Blocked Processes:\n");
    printBlockedProcesses();
}

