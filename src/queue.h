//
// Created by Russell on 10/1/2017.
//

/* RUNNING STATES */

#ifndef QUEUE_H
#define QUEUE_H

#define READY 1
#define RUNNING 2
#define BLOCKED 3

/* SUSPENSION STATES */
#define SUSPENDED 1
#define NOTSUSPENDED 0

typedef struct pcb {
    char process_name[9];           // Name of process
    //unsigned char process_class;    // Id of process (application or process)
    int priority;                   // Integer between 0 and 9.
    int running_state;              // Ready, Running, or Blocked
    int suspension_state;           // Check if suspended or not
    struct pcb* next_pcb;           // Pointer to next pcb
    char file_name[128];				// R3
    int offset;					
} pcb;

struct queue {
    int count;
    pcb* head;
    pcb* tail;
};

void initQueues();
void printPCB(pcb* process);
pcb* AllocatePCB();
int FreePCB(pcb* process);
pcb* SetupPCB(char* process_name, int priority, char* file_name);
pcb* FindPCB(char* name);
void InsertPCB(pcb* process);
int RemovePCB(pcb* process);
pcb* LoadProcess(char* process_name, int priority, char* file_name);
void printAllProcesses();
void printReadyProcesses();
void printBlockedProcesses();
pcb* GetNextReady();
pcb* GetNextReadyNotSuspended();
pcb* GetNextReadySuspended();
pcb* GetNextBlockedNotSuspended();
pcb* GetNextBlockedSuspended();

#endif

