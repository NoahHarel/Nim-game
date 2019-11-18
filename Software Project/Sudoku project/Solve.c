/*
 * Solve.c
 *
 *  Created on: Sep 8, 2019
 *      Author: Itamar
 */


#include <stdio.h>
#include <stdlib.h>
#include "Board.h"
#include "Utilities.h"
#include "Edit.h"
#include "Game.h"
#include "ListUtils.h"
#include "DataStructs.h"
#include "gurobi_c.h"
#include "Gurobi.h"

int solve_command_selector(char command[256]) {
	int autofill_command = isAutofillCommand(command);
	int set_command = isSetCommand(command);
	int undo_command = isUndoCommand(command);
	int redo_command = isRedoCommand(command);
	int reset_command = isResetCommand(command);
	int edit_command = isEditCommand(command);
	int mark_errors_command = isMark_ErrorsCommand(command);
	int print_board_command = isPrintBoardCommand(command);
	int exit_command = isExitCommand(command);
	int numsol_command = isNumSolutionCommand(command);
	int solve_command = isSolveCommand(command);
	int guess_command = isGuessCommand(command);
	int hint_command = isHintCommand(command);
	int gint_command = isGuessHintCommand(command);
	int save_command = isSaveCommand(command);
	int validate_command = isValidateCommand(command);
	if (autofill_command == 1) {
		return 1;
	} else if (set_command == 1) {
		return 2;
	} else if (undo_command == 1) {
		return 3;
	} else if (redo_command == 1) {
		return 4;
	} else if (reset_command == 1) {
		return 5;
	} else if (edit_command == 1) {
		return 6;
	} else if (edit_command == 2) {
		return 7;
	} else if (mark_errors_command == 1) {
		return 8;
	} else if (print_board_command == 1) {
		return 9;
	} else if (numsol_command == 1) {
		return 10;
	} else if (exit_command == 1) {
		return 11;
	} else if (solve_command == 1) {
		return 12;
	} else if (guess_command == 1) {
		return 13;
	} else if (hint_command == 1) {
		return 14;
	} else if (gint_command == 1) {
		return 15;
	} else if (save_command==1) {
		return 16;
	} else if (validate_command == 1) {
		return 17;
	}
	return 0;
}


int Solve(struct Board *gameBoard, struct Board *fixedBoard) {
	char input[512];
	char command[256];
	int xy[2];
	char *command_pointer;
	int set_validator;
	float guess_validator;
	int hint_validator;
	int selector;
	int mark_errors = 1;
	int verify_len;
	int full_board_indictaor;
	int board_checker_indicator;
	int hintNum;
	ELEM UndoRedoList;
	/*SET BASE BOARD: this will be the "first board" of the game, for further uses*/

	/*INITIALIZE UNDOREDOLIST: We create the "list" object as an element pointer that points to
	 * current list place. */
	UndoRedoList = initializeElem(gameBoard);
	printBoard(gameBoard, fixedBoard, mark_errors, 0);
	while (1) {
		/*Check if feof*/
		if (feof(stdin)) {
			beKindRewind(&UndoRedoList);
			KillListElements(&UndoRedoList);
			freeBoardMemory(gameBoard);
			freeBoardMemory(fixedBoard);
			freeBoardMemory(&(UndoRedoList->board));
			UndoRedoList = NULL;
		}

		/*Current gameboard needs to match current board of list pointer*/
		copyBoard(&(*UndoRedoList).board, gameBoard);
		/*Checks if board is full. If so, checks board; if good, returns 0 for Init to start again;
		 * else, allows continuing the game for undoing or anything that will be good.*/
		full_board_indictaor = isBoardFull(gameBoard);
		if (full_board_indictaor == 1) {
			board_checker_indicator = isBoardIllegal(gameBoard);
			if (board_checker_indicator == 1) {
				printf("ERROR: Tsk-Tsk. Your Board is bad. Start undoing and think about what you did.\n");
			} else {
				printf("Congratulations! You completed the Board!\n");
				printf("You are now being directed to the start. Feel free to start again!\n\n");
				return 0;
			}
		}
		printf("Enter your command:\n");
		command_pointer = fgets(input, 512, stdin);
		/*Checks if command input succeeded*/
		if (command_pointer == NULL) {
			continue;
		}
		if (findLenOfCommand(input) == 0) {
			continue;
		}
		verify_len = commandLenVerifier(input);
		if (verify_len == 0 || command_pointer == NULL) {
			printf("ERROR: Command is more than 256 characters. Try again!\n");
			fflush(stdin);
			continue;
		} else {
			copyFromInputToCommand(input, command);
		}
		selector = solve_command_selector(command);
		if (selector == 1) {
			/*Case meaning: Autofill. Checks for no params and if board legal, then executes*/
			if (checkNoParam(command) == 1) {
				if (isBoardIllegal(gameBoard) == 1) {
					printf("ERROR: Board is erroneous, and has no solutions! Try again\n");
					continue;
				} else {
					if (autoFill(gameBoard) == 1) {
						/*If autofill succeeded, we add to the list*/
						createAndConnect(&UndoRedoList, gameBoard);
						copyBoard(&(*UndoRedoList).board, gameBoard);
						printBoard(gameBoard, fixedBoard, mark_errors, 0);
					}
					else {
						copyBoard(&(*UndoRedoList).board, gameBoard);
						printBoard(gameBoard, fixedBoard, mark_errors, 0);
					}
				}
			} else {
				continue;
			}


		} else if (selector == 2) {
			/*Case meaning: Set. Parses set according to Game.c func.*/
			set_validator = validateSetCommand(command, gameBoard, fixedBoard, 1);
			if (set_validator == 1) {
				createAndConnect(&UndoRedoList, gameBoard);
				printBoard(gameBoard, fixedBoard, mark_errors, 0);
			}


		} else if (selector == 3) {
			/*Case meaning: Undo*/
			if (checkNoParam(command) == 1) {
				Undo(&UndoRedoList, 1);
				copyBoard(&(*UndoRedoList).board, gameBoard);
				printBoard(gameBoard, fixedBoard, mark_errors, 0);
			} else {
				continue;
			}
		}
		else if (selector == 4) {
			/*Case meaning: Redo*/
			if (checkNoParam(command) == 1) {
				Redo(&UndoRedoList, 1);
				copyBoard(&(*UndoRedoList).board, gameBoard);
				printBoard(gameBoard, fixedBoard, mark_errors, 0);
			} else {
				continue;
			}
		}

		else if (selector == 5) {
			/*Case meaning: Reset*/
			if (checkNoParam(command) == 1) {
				beKindRewind(&UndoRedoList);
				printf("Board Reset!\n");
				copyBoard(&(*UndoRedoList).board, gameBoard);
				printBoard(gameBoard, fixedBoard, mark_errors, 0);
			} else {
				continue;
			}
		}

		else if (selector == 6) {
			/*Case meaning: edit without [x]. Deletes boards so far, initialize new 9x9, and return 1 to go
			 * back to Edit in InitLoop.
			 */
			if (checkNoParam(command) == 1) {
				freeBoardMemory(gameBoard);
				freeBoardMemory(fixedBoard);
				setDimensions(gameBoard, 3, 3);
				setDimensions(fixedBoard,3,3);
				beKindRewind(&UndoRedoList);
				KillListElements(&UndoRedoList);
				return 1;
			} else {
				continue;
			}
		}

		else if (selector == 7) {
			/*Case meaning: Edit had [x]. Load it similarly,return 1 to go to Edit in InitLoop*/
			if (LoadBoardProcedureEdit(command, gameBoard, fixedBoard, 1) == 0) {
				continue;
			}
			beKindRewind(&UndoRedoList);
			KillListElements(&UndoRedoList);
			return 1;

		} else if (selector == 8) {
			/*Case Meaning: Mark errors. Checks if the param is 1 or 0.*/
			mark_errors = getMarkErrorsValue(command, mark_errors);
		}

		else if (selector == 9) {
			/*Case meaning: Print_board.*/
			if (checkNoParam(command) == 1) {
				printBoard(&(UndoRedoList->board), fixedBoard, mark_errors, 0);
			} else {
				continue;
			}

		} else if (selector == 10) {
			/*Case meaning: num_solutions. Calls the stack backtrack*/
			if (checkNoParam(command) == 1) {
				if (isBoardIllegal(gameBoard) == 1) {
					printf("ERROR: The board's illegal, so 0 solutions. Fix the values before running this command.\n");
				} else {
					printf("Number of solutions found: %d\n", backtrack_stack(*gameBoard));
				}
			} else {
				continue;
			}
		} else if (selector == 11) {
			/*Case meaning: EXIT. frees resources, exits on 0*/
			if (checkNoParam(command) == 1) {
				printf("See ya!\n");
				beKindRewind(&UndoRedoList);
				KillListElements(&UndoRedoList);
				freeBoardMemory(gameBoard);
				freeBoardMemory(fixedBoard);
				UndoRedoList = NULL;
				/*printf("SUCCESS?\n");*/
				/*free(startPointer);*/
				exit(0);
			} else {
				continue;
			}
		}


		else if (selector == 12) {
			/*Case meaning: Solve. Gameboard resets to new value, list destroyed, start over*/
			if (validateLoadCommandAndPath(command) == 0) {
				continue;
			}
			beKindRewind(&UndoRedoList);
			KillListElements(&UndoRedoList);
			LoadBoardProcedureSolve(command, gameBoard, fixedBoard, 1);
			UndoRedoList = initializeElem(gameBoard);
		}



		else if (selector == 13) {
			/*Case meaning: Guess. parses the relevant elements and passes them to guess under constraints*/
			guess_validator = parse_guess_command(command, gameBoard);
			if (guess_validator != -1) {
				if (guess(gameBoard, guess_validator) == 1) {
					createAndConnect(&UndoRedoList, gameBoard);
				}
				printBoard(gameBoard, fixedBoard, mark_errors, 0);
			}
			xy[0] = -1;
			xy[1] = -1;
		} else if (selector == 14) {
			/*Case meaning: hint. parses the values and passes them under constraints (if-elses)*/
			hint_validator = parse_hint_command(command, xy, gameBoard->N);
			if (hint_validator == 0){
				if (isBoardIllegal(gameBoard) == 0){
					if (fixedBoard->values[xy[1]-1][xy[0]-1]==0){ /*cell is fixed*/
						if (gameBoard->values[xy[1]-1][xy[0]-1]==0){
							hintNum = hint((xy[1]-1), (xy[0]-1), gameBoard);
							if (hintNum>0 && hintNum<=gameBoard->N){
								printf("Hint for cell %d,%d: try %d\n",xy[0],xy[1], hintNum);
								}
							else {
								printf("ERROR: Board found to be unsolvable.\n");
							}
							}
						else{
							printf("ERROR: Cell %d,%d already has a value\n",xy[1],xy[0]);
							}
						}
					else{
						printf("ERROR: Cell %d,%d is fixed\n",xy[1],xy[0]);
					}
				}
				else{
					printf("ERROR: The board is erroneous. No hint possible.\n");
				}
			}
			xy[0] = -1;
			xy[0] = -1;

		} else if (selector == 15) {
			/*Case meaning: guess_hint. parse_hint returns the same values.*/
			hint_validator = parse_hint_command(command, xy, gameBoard->N); /*It literally needs the same function. Easy!*/
			if (hint_validator == 0){
				if (isBoardIllegal(gameBoard) == 0){
					if (fixedBoard->values[xy[1]-1][xy[0]-1]==0){ /*cell is fixed*/
						if (gameBoard->values[xy[1]-1][xy[0]-1]==0){
							hintNum = guessHint(gameBoard, (xy[1]-1), (xy[0]-1));
							if (hintNum==0){
								printf("ERROR: Board found to be unsolvable.\n");
								}
							}
						else{
							printf("ERROR: Cell %d,%d already has a value\n",xy[1],xy[0]);
							}
						}
					else{
						printf("ERROR: Cell %d,%d is fixed\n",xy[1],xy[0]);
					}
				}
				else{
					printf("ERROR: The board is erroneous. No guess_hint possible.\n");
				}
			}
			xy[0] = -1;
			xy[0] = -1;
		} else if (selector == 16) {
			/*Case meaning: Save. Parses the command and executes save*/
			if (isBoardIllegal(gameBoard) == 1) {
				printf("ERROR: Your board is erroneous. Cannot save erroneous boards. Fix it and try again!\n");
			} else {
				parseSaveBoardCommandAndExec(command, gameBoard, fixedBoard, 0);
			}
		} else if (selector == 17) {
			/*Case meaning: validate*/
			if (checkNoParam(command) == 1) {
				if (isBoardIllegal(gameBoard) == 0) {
					if (validate(gameBoard)){
						printf("Validation success! The board is solvable.\n");
					}
					else{
						printf("Validation fail! The board is unsolvable.\n");
					}
				} else {
					printf("ERROR: Board has erroneous values! Cannot validate.\n");
					continue;
				}
			} else {
				continue;
			}

		} else {
			printf("ERROR: Invalid Command!\n");
		}
	}
	return 0;
}


