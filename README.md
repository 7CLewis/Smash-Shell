# Smash Shell
* Author: Casey Lewis
* Project description and test cases were created by Boise State University Professors.
* I have modified the test cases, but all credit goes to the BSU Professors.
* 27 June 2020

## Overview
This program generates a "smash" shell - a simplified version of
the Linux "bash" shell. This program runs 3 commands - "exit", "cd",
and "history" internally, and calls on the bash programs for all other
commands. 

## Compiling and Using
This program must be run in a Unix/Linux environment with a /bin/sh program directory.

To compile this program, simply type "make" or "make all" into the command line. To run
in debug mode, use the command "make clean" instead. This will create the build files 
and the executable program, "smash". Once everything is compiled, run the executable using
"./smash". This will open a smash shell within your bash shell, with a line starting with
the "$" symbol.

Now that smash is running, it will wait for you to enter a command. If you are using this
for the first time, enter the command "ls" and see the results. You can continue entering
commands until you kill the program (using Ctrl-D), or exit by typing the command "exit".

## Smash vs. Bash
Smash has a few notable differences and missing features when compared to bash. Each of these
are discussed below.

### Similarities
* Smash supports basic commands with up to 50 arguments (Example: "ls -a -l").

* Smash supports command piping (Example: "ls -l | fgrep foo | wc -l")

* Smash supports basic input and output redirection (Examples: "ls -l > outputfile.txt", 
"wc -l < inputfile.txt")

### Differences
* External commands in smash (anything other than "exit", "cd", or "history") that are either
not found (example: "nonExistentCommand") or return an error (Example: "ls blah")
will return an exit status of 127. Bash will return more specific error codes for various command types.

* Internal and external commands in smash that execute properly will return an exit status of 0. Internal
commands that have any error (example: "cd nonExistentDirectory") will return an exit status of 1.

* Smash does not support wildcards (Example: "ls *.c")

* Smash does not support more complicated input and output redirection (Examples: "ls -l >> outputfile.txt")

* Smash's "history" command displays only the 10 most recent commands, along with their sequence number
and exit status.

* Smash does not support quotation marks for files with spaces (Example: "ls -l > 'file.txt'")

* Smash does not support shell variables (Example: "echo $?")

## Discussion
While smash is more simplistic than bash, it is a great way to learn about many important
C functions. Smash uses various important C features, including fork(), wait(), dup2(), open(), close(),
malloc(), free(), pipe(), execvp(), and various string functions (strncmp(), strndup(), strtok(), etc).

This program also takes memory leakage into consideration. No command usage with proper exiting should
result in any amount of memory leakage. You can test this by making smash and then running it using: 

valgrind --leak-check=full --show-leak-kinds=all --child-silent-after-fork=yes --track-origins=yes --trace-children=no ./smash