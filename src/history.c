
#include <stdio.h>
#include <stdlib.h>
#include "history.h"

int _HISTORY_NUM_COMMANDS_SAVED = 0;

int initLogFile() {
    // deletes old file if it exists
    FILE* logFilePtr = fopen(LOG_FILE_NAME, "w");
    if (logFilePtr == NULL) {
        return NULL;
    }
}

int saveCommand(char* command) {
    FILE* logFilePtr = fopen(LOG_FILE_NAME, "a");

    if (logFilePtr == NULL) {
        return 0;
    }

    fprintf(logFilePtr, "%s\n", command);
    _HISTORY_NUM_COMMANDS_SAVED++;

    fclose(logFilePtr);
}

char* getNthCommand(int n, int maxLineSize) {
    
    FILE* logFilePtr = fopen(LOG_FILE_NAME, "r");

    if (logFilePtr == NULL) {
        return 0;
    }

    char* result = malloc((maxLineSize + 1) * sizeof(char));

    int i = 0;
    for (; i < n; i++) {
        // loop to get down to the last command saved
        fgets(result, maxLineSize, logFilePtr);
    }
    
    return result;

}


int getNumCommandsSaved() {
    return _HISTORY_NUM_COMMANDS_SAVED;
}