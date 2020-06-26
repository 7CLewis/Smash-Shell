#define _GNU_SOURCE
#define MAXLINE 4096
#include "smash.h"
#include "history.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>    //File descriptors 
#include <sys/wait.h> //wait() function call
#include <unistd.h>   //fork() function call

/**
 * In case of a keyboard interrupt (Ctrl-C),
 * Kill the current process and return to smash
 */
void sig_handler(int signal) {
  if(signal == SIGINT) {
    fputs("\n$ ", stderr);
  }
}

int main(int argc, char **argv) 
{
    int exitStatus;
	setvbuf(stdout, NULL, _IONBF, 0);

 	init_history();
	signal(SIGINT, sig_handler);

    char bfr[MAXLINE];

	fputs("$ ", stderr); //Output the first prompt
	while(fgets(bfr, MAXLINE, stdin) != NULL) {
		char *p = strchr(bfr, '\n');
		
		if(p == NULL) {
			fputs("The line you entered was too long", stderr);
			exit(1);
		}
		
		bfr[strlen(bfr) - 1] = '\0'; //Replace newline with NULL
		exitStatus = execute(bfr);
		fputs("$ ", stderr);
	}

	return exitStatus;
}