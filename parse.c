#define _GNU_SOURCE
#include "history.h"
#include "smash.h"

#include <stdio.h>    //Standard I/O functions and definitions
#include <stdlib.h>   //Standard library (includes exit function)
#include <string.h>   //String manipulation functions
#include <fcntl.h>    //File descriptors 
#include <sys/types.h>
#include <sys/wait.h>

// Execute the command the user passed in
int execute(char *str) {

	if(strnlen(str, 1) != 0) {
	
		// Turn off Disable buffering in stdout and stderr	
		setvbuf(stdout,NULL,_IONBF,0);

		char *strCopy = strndup(str, strlen(str));

		char *args[50];

		int exitStatus = 0;

		int commandCount = 0;

		// Parse the string and separate the commands by the pipes
		char *token = strtok(str, "|");
		while(token != NULL) {
			args[commandCount++] = token;
			token = strtok(NULL, " ");
		}
		// Parse each command
		

		free(strCopy);

		return exitStatus;
	}
	return 0;
}







































// /* Redirect stdin to read from the specified file
//  * Return the file descriptor that was created
//  */
// int openInputFile(char *fileName) {
//     return 0;
// }

// /*
//  * Open a file of the given file name to write to from stdout.
//  * Return the file descriptor that was created.
//  */
// int openOutputFile(char *fileName) {
//     return 0;
// }

// void dupProper(int pipeType) {

	
// }

// void closeProper(int pipeType) {

// }