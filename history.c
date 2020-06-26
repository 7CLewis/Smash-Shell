//#define _BSD_SOURCE
#define _DEFAULT_SOURCE
#include "history.h"  //Header file for this class
#include <stdio.h>    //Standard I/O functions and definitions
#include <stdlib.h>   //Standard library (includes exit function)
#include <string.h>   //String manipulation functions
#include <unistd.h>   //chdir

/* 
 * history.c - store the last 10 smash            
 *             commands.
 *
 * @author - Casey Lewis
 */

struct Cmd *cmds[10];
int count = 0;
int start = 0;

//Builds data structures for recording cmd history
void init_history(void)
{
	for(int i = 0; i < 10; i++) {
		cmds[i] = malloc(sizeof(struct Cmd));
	}
}

//Adds an entry to the history
void add_history(char *cmd, int exitStatus)
{
	char *cmdCopy = strdup(cmd);

	if(count >= 10) {
		slide_history();
		start++;
	}
	
	cmds[count - start]->cmd = cmdCopy;
	cmds[count - start]->exitStatus = exitStatus;
		
	count++;
}

void slide_history() 
{
	free(cmds[0]->cmd);
	//cmds[0] = malloc(sizeof(struct Cmd));
	for(int i = 0; i < 9; i++) {
		cmds[i]->cmd = cmds[i+1]->cmd;
		cmds[i]->exitStatus = cmds[i+1]->exitStatus;
		
	}
}

//Frees all malloc’d memory in the history
void clear_history(void) 
{
	for(int i = 0; i < 10; i++)
 	{	
		free(cmds[i]->cmd);
		free(cmds[i]);
	}
}

//Prints the history to stdout
void print_history(int firstSequenceNumber) 
{
	for(int i = firstSequenceNumber; i < count; i++) {
		if(cmds[i-start]->cmd != NULL) {
			printf("%d [%d] %s\n", i+1, cmds[i-start]->exitStatus, cmds[i-start]->cmd);
		}
	}
}
