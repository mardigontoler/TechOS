#pragma once

#define LOG_FILE_NAME "history.log"

// Get the log file ready.
// Essentially deletes the old one as of right now
int initLogFile();


// appends the string to a line in the log file
// Before using this to write to a file, make sure that the
// string's length is less than the value you'll be passing to
// getLastCommand()
// I think in main we have MAXINPUTSIZE defined, 
// so MAKE SURE that command is smaller than that before passing it to this function
int saveCommand(char* command);


// this will allocate space for the command it reads, so be 
// sure to free() the memory pointed to by what this returns.
char* getNthCommand(int n, int maxLineSize);


// returns how many commands have been saved.
// Useful for getNthCommand()
// For example you can use the following to get the very
// last command:
//
// int x = getNumCommandsSaved();
// char* last = getNthCommand(x, MAXLINESIZE);
int getNumCommandsSaved();