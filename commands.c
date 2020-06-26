#define _GNU_SOURCE
#define EXTERNAL_COMMAND -23
#define EXIT_COMMAND "exit"
#define EXIT_COMMAND_LENGTH 5
#define EXIT_COMMAND_STATUS -22
#define CD_COMMAND "cd"
#define CD_COMMAND_LENGTH 3
#define HISTORY_COMMAND "history"
#define HISTORY_COMMAND_LENGTH 8
#include "history.h"
#include "smash.h"

#include <stdio.h>    //Standard I/O functions and definitions
#include <stdlib.h>   //Standard library (includes exit function)
#include <string.h>   //String manipulation functions
#include <unistd.h>   //chdir
#include <sys/wait.h> //Used for wait() to retrieve exit statuses
#include <errno.h> 	  //Retrieve error codes
#include <fcntl.h>	  //Manipulating file descriptors

//Execute an external command - not directly supported by smash
void executeExternalCommand(char* prog, char* argsArr[]) {

    execvp(prog, argsArr);
    
    //if these lines are reached, execvp did not execute properly
    perror("Execvp failed");
	exit(-1);

}

// Execute the command the user passed in
int executeCommand(char *args[], int argsCount, int sequenceCount) {
	
    int exitStatus = 0;
    if (argsCount == 0) {
        return exitStatus;
    }
    char *program = args[0];

    //exit - exit the smash shell
    if(strncmp(program, EXIT_COMMAND, EXIT_COMMAND_LENGTH) == 0) {
        clear_history();
        return EXIT_COMMAND_STATUS;
    }

    //cd - change directories
    else if (strncmp(program, CD_COMMAND, CD_COMMAND_LENGTH) == 0) {
        if(args[1] != NULL) {
            if(chdir(args[1]) != 0) {
                perror(args[0]);
                exitStatus = 1;
            } 
        }
    }

    //history - print the 10 most recent commands, along with their exit status
    else if (strncmp(program, HISTORY_COMMAND, HISTORY_COMMAND_LENGTH) == 0) {
        if(sequenceCount > 10) {
            print_history(sequenceCount-10);
        } else {
            print_history(0);
        }
    }

    //Any command that isn't one of the ones above
    else {
        sequenceCount++;
        return EXTERNAL_COMMAND;
    }

    sequenceCount++;
    return exitStatus;
}

/* Redirect stdin to read from the specified file
 * Return the file descriptor that was created
 */
int openInputFile(char *fileName) {
    return 0;
}

/*
 * Open a file of the given file name to write to from stdout.
 * Return the file descriptor that was created.
 */
int openOutputFile(char *fileName) {
    return 0;
}

void dupProper(int pipeType) {

	
}

void closeProper(int pipeType) {

}
