#include <stdio.h>
#include <stdlib.h>
#include "Board.h"
#include "Utilities.h"
#include "Edit.h"
#include "Game.h"
#include "Solve.h"
#include "DataStructs.h"


int InitLoop(struct Board *gameBoard){
	struct Board fixedBoard;
	int currentModeSelector;
	char command[256];
	char input[512];
	int verify_len = 0;
	char *command_pointer;
	int isEditChosen;
	int isSolveChosen;
	int isExitChosen;
	int solve_answer;
	int opt_2_load_success;
	while (1) {

		/*An Outer Loop for the entire game. That's the "Init loop" */
		while (1) {
			/* Inner loop #1: Waits for valid user input.*/
			printf("Enter your command - Edit, Solve or Exit:\n");
			command_pointer = fgets(input, 512, stdin);

			/*Checks if command is not empty or problematic*/
			if (command_pointer == NULL) {
				continue;
			}
			if (findLenOfCommand(input) == 0) {
				continue;
			}
			verify_len = commandLenVerifier(input);

			/*Checks for command length.*/
			if (verify_len == 0 || command_pointer == NULL) {
				printf("ERROR: Command is more than 256 characters. Try again!\n");
				fflush(stdin);
				continue;
			} else {
				copyFromInputToCommand(input, command);
			}
			opt_2_load_success = 0;
			isEditChosen = isEditCommand(command);
			isSolveChosen = isSolveCommand(command);
			isExitChosen = isExitCommand(command);
			/*Conditions that sets the game loop in the mode relevant - uses the CurrentModeSelector*/
			if (isEditChosen == 2) {
				/*Edit had [x]*/
				opt_2_load_success = LoadBoardProcedureEdit(command, gameBoard, &fixedBoard, 0);
				if (opt_2_load_success == 0) {
					continue;
				}
				currentModeSelector = 1;
				break;
			} else if (isEditChosen == 1) {
				/*Edit didn't have [x] - initialize 9x9 blank*/
				setDimensions(gameBoard, 3, 3);
				setDimensions(&fixedBoard, 3,3);
				currentModeSelector = 1;
				break;
			} else if (isSolveChosen == 1) {
				/*Solve chosen*/
				solve_answer = LoadBoardProcedureSolve(command, gameBoard, &fixedBoard, 0);
				if (solve_answer != 0) {
					currentModeSelector = 2;
					break;
				}
				continue;

			} else if (isExitChosen == 1) {
				printf("See ya!\n");
				exit(0);
			} else {
				printf("That was an invalid command. Try again.\n");
			}
		}
		while (1) {
			/*Inner Loop #2: While game runs, allows dynamic switching between Edit Mode and Solve Mode*/
			if (currentModeSelector == 1) {
				currentModeSelector = EditMode(gameBoard, &fixedBoard);
			} else if (currentModeSelector == 2) {
				currentModeSelector = Solve(gameBoard, &fixedBoard);
			} else if (currentModeSelector == 0) {
				break;
			}
		}
	}
	return 0;
}
