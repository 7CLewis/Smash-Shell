#define _GNU_SOURCE
#define EXIT_COMMAND_STATUS -22
#define COMMAND_NOT_FOUND -23
#define O_PERMISSIONS S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH
#define O_FLAGS O_WRONLY | O_CREAT
#define I_FLAGS O_RDONLY
#include "history.h"
#include "parse.h"

#include <stdio.h>    //Standard I/O functions and definitions
#include <stdlib.h>   //Standard library (includes exit function)
#include <string.h>   //String manipulation functions
#include <fcntl.h>    //File descriptors 
#include <sys/wait.h> //wait() function call
#include <unistd.h>   //fork() function call

int sequenceCount = 0; //Keep track of how many total commands have been entered

/**
 * Chop a piped command into individual commands
 * Example: "ls -l | wc" will yield args[0] = "ls -l", args[1] = "wc"
 * A single command, i.e., "cd example", will yield args[0] = "cd example"
 */
int chop_commands(char* args[], char* strCopy) {
		// Parse the string and separate the commands by the pipes
		char *token = strtok(strCopy, "|");

		int count = 0;
		while(token != NULL) {
			args[count++] = token;
			token = strtok(NULL, "|");
		}
		return count;
}

/**
 * Separate a single command into its individual arguments
 * Example: "ls -l" will yield currArgs[0] = "ls", currArgs[1] = "-l"
 * Adds the NULL pointer as the last argument 
 */
int separate_args(char* currArgs[], char* cmd) {
	char *token = strtok(cmd, " ");
	int argCount = 0;
	while(token != NULL) {
		currArgs[argCount++] = token;
		token = strtok(NULL, " ");
	}
	currArgs[argCount] = '\0';
	return argCount;
}

/**
 * Locate the input redirection symbol, '<',
 * and it's corresponding file, and open that file,
 * if it exists
 */
int locate_input_redirect(char* args[], int argCount) {
	int inputFD = 0;
	if (argCount > 1) {
		if(strncmp(args[argCount-2], "<", 1) == 0) {
			inputFD = open(args[argCount-1], I_FLAGS);
			args[argCount-1] = '\0';
			args[argCount-2] = '\0';
		} else if(strncmp(args[argCount-1], "<", 1) == 0) {
			args[argCount-1]++;
			inputFD = open(args[argCount-1], I_FLAGS);
			args[argCount-1] = '\0';
		}
	}

	return inputFD;
}

/**
 * Locate the output redirection symbol, '>',
 * and it's corresponding file, and open that file,
 * if it exists
 */
int locate_output_redirect(char *args[], int argCount) {
	int outputFD = 0;
	if (argCount > 1) {
		if(strncmp(args[argCount-2], ">", 1) == 0) {
			outputFD = open(args[argCount-1], O_FLAGS, O_PERMISSIONS);
			args[argCount-2] = '\0';
		} else if(strncmp(args[argCount-1], ">", 1) == 0) {
			args[argCount-1]++;
			outputFD = open(args[argCount-1], O_FLAGS, O_PERMISSIONS);
			args[argCount-1] = '\0';
		}
	}

	return outputFD;
}

/**
 * Properly arrange the file descriptors according to the current command
 * being executed.
 */
void arrange_file_descriptors(int pipeFileDescriptors[][2], int currCommand, int totalCommands, int in, int out) {
	if(in != 0) {
		dup2(in, STDIN_FILENO);
	}
	if(out != 0) {
		dup2(out, STDOUT_FILENO);
	}
	if(totalCommands > 1) {
		if(currCommand+1 == 1) {
			dup2(pipeFileDescriptors[currCommand][1],1);
		} else if(totalCommands == currCommand+1) {
			dup2(pipeFileDescriptors[currCommand-1][0], 0);
		} else {
			dup2(pipeFileDescriptors[currCommand-1][0], 0);
			dup2(pipeFileDescriptors[currCommand][1],1);
		}
	}
}

/**
 * Properly close the file descriptors according to way they were arranged
 * by the current command
 */
void close_file_descriptors(int pipeFileDescriptors[][2], int currCommand, int totalCommands, int in, int out) {
	if(in != 0) {
		close(in);
	}
	if(out != 0){
		close(out);
	}
	if(totalCommands > 1) {
		if(currCommand+1 == 1) {
			close(pipeFileDescriptors[currCommand][1]);
		} else if(totalCommands == currCommand+1) {
			close(pipeFileDescriptors[currCommand-1][0]);
		} else {
			close(pipeFileDescriptors[currCommand-1][0]);
			close(pipeFileDescriptors[currCommand][1]);
		}
	}
}

/** 
 * Execute the command the user passed in.
 * Wait for and retrieve the exit status, and return to smash when completed
 */
int execute(char *str) {

	if(strnlen(str, 1) != 0) {
	
		// Turn off Disable buffering in stdout and stderr	
		setvbuf(stdout,NULL,_IONBF,0);

		char *strCopy = strndup(str, strlen(str));
		char *args[50];
		int exitStatus = 0;

		//Chop the string into separate commands
		int commandCount = chop_commands(args, strCopy);
		int commandHolder = commandCount;
		int currentCommand = 0;

		//Create the array of file descriptors for each pipe
		int pipeFileDescriptors[commandCount-1][2];
		int currentPipe = 0;

		// Parse each command
		do {
			//Parse for current command
			char *currArgs[50];

			//Split the command up into individual pieces
			int count = separate_args(currArgs, args[currentCommand]);

			//Search for '>' and '<'
			int input = locate_input_redirect(currArgs, count);
			int output = locate_output_redirect(currArgs, count);

			//Create a pipe if necessary 
			if (commandCount > 1) {
				if(pipe(pipeFileDescriptors[currentPipe]) < 0){
					perror("Error: Could not create pipe");
					exit(-1);
				}
			}

			//Execute internal command, if the current command is internal
			exitStatus = execute_command(currArgs, count, sequenceCount);

			//If the exit status is the exit command's status,
			//free the duplicated string and exit the program
			if(exitStatus == EXIT_COMMAND_STATUS) {
				free(strCopy);
				exit(0);
			} else if (exitStatus == COMMAND_NOT_FOUND) {
				int pid = fork();

				if(pid == 0) {
					//Arrange file descriptors
					arrange_file_descriptors(pipeFileDescriptors, currentCommand, commandHolder, input, output);

					execute_external_command(currArgs[0], currArgs);
					exit(-2);

				} else {
					//Close file descriptors
					close_file_descriptors(pipeFileDescriptors, currentCommand, commandHolder, input, output);

					//Reset input and output values for the next command
					input = 0;
					output = 0;
    			}
			}

			//Update values for the next loop
			currentPipe++;
			currentCommand++;
			commandCount--;
		} while (commandCount > 0);

		//Retrive all exit statuses (if any)
		if(exitStatus == COMMAND_NOT_FOUND) {
			exitStatus = 0;
			for(int i = 0; i < commandHolder; i++) {
				int originalExitStatus;
				wait(&originalExitStatus);   

				//Adjust the exit status
				int adjustedStatus = WEXITSTATUS(originalExitStatus);
				if(adjustedStatus != 0) {
					exitStatus = 127;
				}
			}
		}


		//Add the command to history and update the count
		add_history(str, exitStatus);
		sequenceCount++;

		//Free allocated string
		free(strCopy);

		return exitStatus;
	}

	//If a blank line was entered, simply return 0
	return 0;
}