#define MAXLINE 4096
#include "smash.h"
#include "history.h"
#include "parse.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#define _POSIX_C_SOURCE 200809L

int main(int argc, char **argv) 
{
    int exitStatus;
	setvbuf(stdout, NULL, _IONBF, 0);

 	init_history();

    char bfr[MAXLINE];

	fputs("$ ", stderr); //Output the first prompt
	while(fgets(bfr, MAXLINE, stdin) != NULL) {
		char *p = strchr(bfr, '\n');
		
		if(p == NULL) {
			fputs("line too long", stderr);
			exit(1);
		}
		
		bfr[strlen(bfr) - 1] = '\0'; //Replace newline with NULL
		exitStatus = execute(bfr);
		fputs("$ ", stderr);
	}

	return exitStatus;
}
