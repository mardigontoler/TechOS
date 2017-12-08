#include "history.h"
#include <stdlib.h>
#include <stdio.h>

#define _CRT_SECURE_NO_WARNINGS

int main(void) {

    if (initLogFile() == NULL) {
        return 0;
    }

    char com1[] = "test command number 1";
    char com2[] = "test com num 2";

    saveCommand(com1);
    saveCommand(com2);

    int maxLineSize = 300;
    int x = getNumCommandsSaved();
    char* lastFromFile = getNthCommand(x - 1, maxLineSize);
    printf("Last command is: %s\n", lastFromFile);

    free(lastFromFile);
    getc(stdin);

    return 0;
}