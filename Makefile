#----------------------------------------------------
#
# Makefile --- Makefile for Smash
#
# Usage
# 	make all	Build the released product
# 	make clean	Remove build artifiacts
# 	make debug	Add debugging options
#
# Author
# 	Casey Lewis
#
# ----------------------------------------------------


	OBJ = history.o smash.o parse.o commands.o
	
	HDRS = history.h parse.h smash.h

	EXE = smash
	
	CFLAGS = -Wall -std=c99
	
	CC = gcc

all:	$(EXE) rules.d

rules.d: $(wildcard *.c) $(wildcard *.h)
	gcc -MM $(wildcard *.c) >rules.d

-include rules.d

$(EXE):	$(OBJ)
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f $(OBJ) $(EXE) rules.d

debug:	CFLAGS += -g -O0
debug:	$(EXE)
