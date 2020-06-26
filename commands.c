#define _GNU_SOURCE
#include "history.h"
#include "smash.h"

#include <stdio.h>    //Standard I/O functions and definitions
#include <stdlib.h>   //Standard library (includes exit function)
#include <string.h>   //String manipulation functions
#include <unistd.h>   //chdir
#include <fcntl.h>    //File descriptors 
#include <sys/wait.h> //Used for wait() to retrieve exit statuses
#include <errno.h> 	  //Retrieve error codes
#include <fcntl.h>	  //Manipulating file descriptors


// Execute the command the user passed in
int executeCommand(char *str) {
	
    int exitStatus = 0;

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
