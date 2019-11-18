/*
 * main.c
 *
 *  Created on: May 21, 2019
 *      Author: Itamar
 */
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "Game.h"
#include "Solver.h"
#include "Parser.h"
#include "SPBufferset.h"


int main(int argc, char *argv[] ) {
	int seed;
	int isDone = 0;
	int newSudo[9][9];
	int game[9][9];
	int hint_num;
	int restartFlag;
	int firstSolution[9][9];
	int setVerify;
	char command[1024];
	if (argc ==2){
		seed = atoi(argv[1]);
		srand(seed);
	}
	else{
		srand(5);
	}
	while (1) {
		restartFlag = 0;
		isDone = 0;
		printf("Please enter the number of cells to fill [0-80]:\n");
		hint_num = receiveNumber();
		if (hint_num == -1) {
			printf("Exiting...\n");
			exit(0);
		}
		getchar();
		initialize(newSudo);
		initialize(game);
		randBacktrack(newSudo);
		createPuzzle(newSudo, game, hint_num);
		copyBoard(game, firstSolution);
		printBoard(firstSolution, game);
		while (1) {
			reassignCommand(command);
			if (feof(stdin)) {
				printf("Exiting...\n");
				exit(0);
			}
			fgets(command, 1024, stdin);
			if (command[0] == '\n' || command[0] == 'a') {
				continue;
			}
			if (isCommandLegal(command) == 1) {
				if (isSetCommand(command) == 1 && isDone==0) {
					setVerify = applySet(firstSolution, game, command);
					if (setVerify == 1) {
						printBoard(firstSolution, game);
					}
					if (isFull(game) == 1) {
						printf("Puzzle solved successfully\n");
						isDone = 1;
					}
					reassignCommand(command);
					continue;
				} else if (isHintCommand(command) == 1 && isDone == 0) {
					applyHint(newSudo, command);
					reassignCommand(command);
					continue;
				} else if (isValidateCommand(command) && isDone == 0) {
					applyValidate(game, newSudo); /* Apply validate needs the current solution in order to change it, if necessary */
					reassignCommand(command);
				} else if (isRestartCommand(command)) {
					restartFlag = 1;
					break;
				} else if (isExitCommand(command)) {
					applyExit();
				}
				else{
					printf("Error: invalid command\n");
				}
			}
			else{
				printf("Error: invalid command\n");
			}
		}
		if (restartFlag == 1) {
			continue;
		}
		return 0;
	}
	return 0;
}


