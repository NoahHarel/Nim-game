#include <stdio.h>
#include <stdlib.h>
#include "Board.h"
#include "Utilities.h"
#include "SPBufferset.h"
#include "Game.h"
#include "ListUtils.h"
#include "DataStructs.h"
#include "gurobi_c.h"
#include "Gurobi.h"


int whatIsTheCommand(char command[256]) {
	int set_option = isSetCommand(command);
	int edit_option = isEditCommand(command);
	int redo_option = isRedoCommand(command);
	int undo_option = isUndoCommand(command);
	int reset_option = isResetCommand(command);
	int save_option = isSaveCommand(command);
	int solve_option = isSolveCommand(command);
	int print_command = isPrintBoardCommand(command);
	int numsol_command = isNumSolutionCommand(command);
	int exit_command = isExitCommand(command);
	int generate_command = isGenerateCommand(command);
	int validate_command = isValidateCommand(command);
	if (set_option == 1) {
		return 1;
	} else if (edit_option == 2) {
		return 2;
	} else if (edit_option == 1) {
		return 3;
	} else if (redo_option == 1) {
		return 4;
	} else if (undo_option == 1) {
		return 5;
	} else if (reset_option == 1) {
		return 6;
	} else if (save_option == 1) {
		return 7;
	} else if (solve_option == 1) {
		return 8;
	} else if (print_command == 1) {
		return 9;
	} else if (numsol_command == 1) {
		return 10;
	} else if (exit_command == 1) {
		return 11;
	} else if (generate_command == 1) {
		return 12;
	} else if (validate_command == 1) {
		return 13;
	} else {
		return 0;
	}

}


int EditMode(struct Board *gameBoard, struct Board *originalBoard) {

	/*INITIALIZE VARIABLES: declaring all necessary variables for each Edit Iteration.
	 */
	char input[512];
	char command[256];
	int xy_for_generate[2] = {-1,-1};
	int setValidation;
	int command_selector;
	int solve_return;
	int verify_len;
	int gen_counter = 0;
	char *command_pointer;
	ELEM UndoRedoList;

	/*INITIALIZE UNDOREDOLIST: We create the "list" object as an element pointer that points to
	 * current list place. */
	UndoRedoList = initializeElem(gameBoard);
	printBoard(gameBoard, originalBoard, 1, 1);

	/*Loop to continue running until either an EOF happens or a "Solve X" command shows up.*/
	while (1) {

		/*INITIZLIZE ACTIONS: Print the board in the current List Element.
		 * Then, process the input; check the command in accordance with all command parsers
		 */
		if (feof(stdin)) {
			beKindRewind(&UndoRedoList);
			KillListElements(&UndoRedoList);
			freeBoardMemory(gameBoard);
			freeBoardMemory(originalBoard);
			freeBoardMemory(&(UndoRedoList->board));
			UndoRedoList = NULL;
		}
		copyBoard(&(*UndoRedoList).board, gameBoard);
		printf("Enter your command:\n");
		command_pointer = fgets(input, 512, stdin);
		if (command_pointer == NULL || findLenOfCommand(input) == 0) {
			continue;
		}
		verify_len = commandLenVerifier(input);
		if (verify_len == 0 || command_pointer == NULL) {
			printf("Uh-oh! Command is more than 256 characters. Try again!\n");
			fflush(stdin);
			continue;
		} else {
			copyFromInputToCommand(input, command);
		}
		command_selector = whatIsTheCommand(command);
		if (command_selector == 1) {

			/*We check if the input to set is legal (TODO: Not completed, needed rules). If so, place
			 * it on the board in the desired place.*/
			/*printf("Gotcha! %s\n", command);*/
			setValidation = validateSetCommand(command, gameBoard, originalBoard, 0);
			if (setValidation == 1) {
				/*Creating the next element in the list with the updated board*/
				createAndConnect(&UndoRedoList, gameBoard);
				/*printf("Done\n");*/
				printBoard(gameBoard, originalBoard, 1, 1);

			}
		}

		else if (command_selector == 2) {

			/*Case meaning: edit had [x] and it was good. Load it, remove/free undo/redo list,
			 * and start all over.
			 */
			beKindRewind(&UndoRedoList);
			KillListElements(&UndoRedoList);
			LoadBoardProcedureEdit(command, gameBoard, originalBoard, 1);
			UndoRedoList = initializeElem(gameBoard);
			printBoard(gameBoard, originalBoard, 1, 1);


		} else if (command_selector == 3) {

			/*Case meaning: edit didn't have [x]. Initialize 3x3 as usual*/
			freeBoardMemory(gameBoard);
			freeBoardMemory(originalBoard);
			setDimensions(gameBoard, 3, 3);
			setDimensions(originalBoard,3,3);
			beKindRewind(&UndoRedoList);
			KillListElements(&UndoRedoList);
			UndoRedoList = initializeElem(gameBoard);
			printBoard(gameBoard, originalBoard, 1, 1);
		}

		else if (command_selector == 4) {
			/*Case meaning: Redo. calling Redo func to move the pointer back, update gameboard
			 * Also: check if no extra parameters received.*/
			if (checkNoParam(command) == 1) {
				Redo(&UndoRedoList, 1);
				copyBoard(&(*UndoRedoList).board, gameBoard);
				printBoard(gameBoard, originalBoard, 1, 1);
			} else {
				continue;
			}
		}

		else if (command_selector == 5) {
			/*Case meaning: Undo. Same as case 4, only with Undo func.*/
			if (checkNoParam(command) == 1) {
				Undo(&UndoRedoList, 1);
				copyBoard(&(*UndoRedoList).board, gameBoard);
				printBoard(gameBoard, originalBoard, 1, 1);
			} else {
				continue;
			}
		}

		else if (command_selector == 6) {
			/*Case meaning: Reset. UndoRedo list rewinds back to first elem, gameboard updated.
			 * extra parameters checked.
			 */
			if (checkNoParam(command) == 1) {
				beKindRewind(&UndoRedoList);
				printf("Board has been reset!\n");
				copyBoard(&(*UndoRedoList).board, gameBoard);
				printBoard(gameBoard, originalBoard, 1, 1);
			} else {
				continue;
			}


		} else if (command_selector == 7) {
			/*Case meaning: Save. Only allows save in Edit if board has answer*/
			if (validate(gameBoard) == 0) {
				printf("ERROR: Board has no solution. Fix the board and try again!\n");
			} else {
				parseSaveBoardCommandAndExec(command, gameBoard, gameBoard, 1);
			}
		}


		else if (command_selector == 8) {
			/*Case meaning: Solve. If solve command structure valid, updates gameboard and fixed board
			 * and returns 2. The loop in Init now enters solve with updated boards.*/
			solve_return = validateLoadCommandAndPath(command);
			if (solve_return == 0) {
				continue;
			} else {
				if (LoadBoardProcedureSolve(command, gameBoard, originalBoard, 1) == 1) {
					return 2;
				}
			}


		} else if (command_selector == 9) {
			/*Case meaning: Print Board. Board is printed if no other params are added.*/
			if (checkNoParam(command) == 1) {
				printBoard(&(UndoRedoList->board),originalBoard, 1, 1);
			} else {
				continue;
			}


		} else if (command_selector == 10) {
			/*Case meaning: num_solutions. Calls the stack backtrack if board not erroneous.*/
			if (checkNoParam(command) == 1) {
				if (isBoardIllegal(gameBoard) == 0) {
					if (validate(gameBoard) == 0) {
						printf("ERROR: The board is erroneous, so 0 solutions. Fix the values and try again!\n");
					} else {
						printf("Number of solutions found: %d\n", backtrack_stack(*gameBoard));
					}
				} else {
					printf("ERROR: Board is erroneous! Can't use validate until you fix it.\n");
				}
			} else {
				continue;
			}


		} else if (command_selector == 11) {
			/*Case meaning: EXIT. Frees all resources, exits on 0*/
			if (checkNoParam(command) == 1) {
				printf("See ya!\n");
				beKindRewind(&UndoRedoList);
				KillListElements(&UndoRedoList);
				freeBoardMemory(gameBoard);
				freeBoardMemory(originalBoard);
				exit(0);
			} else {
				continue;
			}


		} else if (command_selector == 12) {
			/*Case meaning: generate. Parses the command for the two params, which are then placed
			 * in xy_for_generate. If works, return 1 and generate is activated - in accordance with all
			 * other rules for generate execution.
			 */
			parse_generate_command(command, xy_for_generate, *gameBoard);
			if (xy_for_generate[0] != -1 && xy_for_generate[1] != -1) {
				if (returnNumOfEmptyCells(gameBoard) < xy_for_generate[0]) {
					printf("ERROR: You wanted %d empty cells, but only %d exist.\n", xy_for_generate[0], returnNumOfEmptyCells(gameBoard));
					continue;
				} else {
					while (gen_counter <= 1000) {
						if (generate(xy_for_generate[0], xy_for_generate[1], gameBoard) == 1) {
							createAndConnect(&UndoRedoList, gameBoard);
							xy_for_generate[0] = -1;
							xy_for_generate[1] = -1;
							printBoard(gameBoard, originalBoard, 1, 1);
							break;
						}
						gen_counter++;
						if (gen_counter == 1001) {
							break;
						}
					}

					continue;
				}
				continue;
			} else {
				xy_for_generate[0] = -1;
				xy_for_generate[1] = -1;

			}



		} else if (command_selector == 13) {
			/*Case meaning: validate. Calling the Gurobi func validate(), answers accordingly.*/
			if (checkNoParam(command) == 1) {
				if (validate(gameBoard) == 0) {
					printf("BOARD HAS NO SOLUTIONS!\n");
				} else {
					printf("Board validated - solution exists!\n");
				}
			} else {
				continue;
			}
		}
		else {
			printf("ERROR: Invalid Command!\n");
		}
	}
}

