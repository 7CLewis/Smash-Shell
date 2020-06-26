#define _GNU_SOURCE
#define EXIT_COMMAND_STATUS -22
#define COMMAND_NOT_FOUND -23
#include "history.h"
#include "parse.h"

#include <stdio.h>    //Standard I/O functions and definitions
#include <stdlib.h>   //Standard library (includes exit function)
#include <string.h>   //String manipulation functions
#include <fcntl.h>    //File descriptors 
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>   //chdir, fork

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
 * Adds the NULL pointer as the last argument 
 */
int separateArgs(char* currArgs[], char* cmd) {
	char *token = strtok(cmd, " ");
	int argCount = 0;
	while(token != NULL) {
		currArgs[argCount++] = token;
		token = strtok(NULL, " ");
	}
	currArgs[argCount] = '\0';
	return argCount;
}

int locateInputRedirect(char* args[]) {
	return 0;
}

int locateOutputRedirect(char *args[]) {
	return 0;
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

		//Create the array of file descriptors for each pipe
		int pipeFileDescriptors[10][2];
		int currentPipe = 0;

		// Parse each command
		do {
			//Parse for current command
			char *currArgs[50];

			//Split the command up into individual pieces
			int count = separateArgs(currArgs, args[currentCommand]);

			//Search for '>' and '<'
			int input = locateInputRedirect(currArgs);
			int output = locateOutputRedirect(currArgs);

			//Create a pipe if necessary 
			if (commandCount > 1) {
				if(pipe(pipeFileDescriptors[currentPipe]) < 0){
					perror("Error: Could not create pipe");
					exit(-1);
				}
			}

			//Execute the command
			exitStatus = executeCommand(currArgs, count);

			//If the exit status is the exit command's status,
			//free the duplicated string and exit the program
			if(exitStatus == EXIT_COMMAND_STATUS) {
				free(strCopy);
				exit(0);
			} else if (exitStatus == COMMAND_NOT_FOUND) {
				int pid = fork();
				//Arrange file descriptors

				if(pid == 0) {
					exitStatus = executeExternalCommand(currArgs[0], currArgs);
				} else {
					int originalExitStatus;
					wait(&originalExitStatus);   

					//Adjust the exit status
					exitStatus = WEXITSTATUS(originalExitStatus);
    			}
			}

			currentPipe++;
			currentCommand++;
			commandCount--;
		} while (commandCount > 0);

		add_history(str, exitStatus);

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