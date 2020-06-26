#define _GNU_SOURCE
#include "history.h"
#include "parse.h"

#include <stdio.h>    //Standard I/O functions and definitions
#include <stdlib.h>   //Standard library (includes exit function)
#include <string.h>   //String manipulation functions
#include <fcntl.h>    //File descriptors 
#include <sys/types.h>
#include <sys/wait.h>

/*
 * Chop a piped command into individual commands
 * Example: "ls -l | wc" will yield args[0] = "ls -l", args[1] = "wc"
 * A single command, i.e., "cd example", will yield args[0] = "cd example"
 */
int chopCommands(char* args[], char* strCopy) {
		// Parse the string and separate the commands by the pipes
		char *token = strtok(strCopy, "|");
		int count = 0;
		while(token != NULL) {
			args[count++] = token;
			token = strtok(NULL, " ");
		}
		return count;
}

/*
 * Separate a single command into its individual arguments
 * Example: "ls -l" will yield currArgs[0] = "ls", currArgs[1] = "-l" 
 */
int separateArgs(char* currArgs[], char* cmd) {
	char *token = strtok(cmd, " ");
	int argCount = 0;
	while(token != NULL) {
		currArgs[argCount++] = token;
		token = strtok(NULL, " ");
	}
	return argCount;
}


// Execute the command the user passed in
int execute(char *str) {

	if(strnlen(str, 1) != 0) {
	
		// Turn off Disable buffering in stdout and stderr	
		setvbuf(stdout,NULL,_IONBF,0);

		char *strCopy = strndup(str, strlen(str));

		char *args[50];

		int exitStatus = 0;

		//Chop the string into separate commands
		int commandCount = chopCommands(args, strCopy);
		
		int currentCommand = 0;
		// Parse each command
		do {
			//Parse for current command
			char *currArgs[50];

			//Split the command up into individual pieces
			int count = separateArgs(currArgs, args[currentCommand]);

			//TODO: Parse for '>' or '<'
			//TODO: Pipes
			//TODO: Forks

			//Execute the command
			executeCommand(currArgs, count);
			currentCommand++;
			commandCount--;
		} while (commandCount > 0);

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