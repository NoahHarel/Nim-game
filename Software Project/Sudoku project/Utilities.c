

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Board.h"
#include "Game.h"
#include "gurobi_c.h"
#include "Gurobi.h"


void copyBoard(struct Board *board_src, struct Board *board_dst) {
	/*Copies the board from src to dst, one value elem at the time*/
	int n = board_src->n;
	int m = board_src->m;
	int N = m*n;
	int i = 0;
	int j = 0;
	while (i < N) {
		j = 0;
		while (j < N) {
			board_dst->values[i][j] = board_src->values[i][j];
			j++;
		}
		i++;
	}
}


void copyBoardByPointer(struct Board *board_src, struct Board **board_dst) {
	int n = board_src->n;
	int m = board_src->m;
	int N = m*n;
	int i = 0;
	int j = 0;
	while (i < N) {
		j = 0;
		while (j < N) {
			(*board_dst)->values[i][j] = board_src->values[i][j];
			j++;
		}
		i++;
	}
}

int validatIntOfChar(char currChar, int N) {
	if ((int)currChar - 48 < N && (int)currChar - 48 > 0) {
		return 1;
	}
	return 0;
}

int saveBoardToFile(struct Board *board, struct Board *fixedBoard, int inEdit, char path[256]) {
		int i = 0;
		int j = 0;
		FILE *saveFile;
		saveFile = fopen(path,"w");
		/*Check If the path is okay*/
		if (saveFile == NULL) {
			printf("ERROR: Path given is erroneous. Try again\n");
			return 0;
		}
		fprintf(saveFile, "%d %d\n", board->m, board->n);

		/*The save in accordance with the format rules start here*/
		while (i < board->N) {
			j = 0;
			while (j < board->N) {
				if (inEdit == 1) {
					if (board->values[i][j] != 0) {
						fprintf(saveFile, "%d. ", board->values[i][j]);
					} else {
						fprintf(saveFile, "%d ", board->values[i][j]);
					}
				} else {
					if ((board->values[i][j] != 0) && (board->values[i][j] == fixedBoard->values[i][j])) {
						fprintf(saveFile, "%d. ", board->values[i][j]);
					} else {
						fprintf(saveFile, "%d ", board->values[i][j]);
					}
				}
				j++;
			}
			fprintf(saveFile, "\n");
			i++;
		}
		if (fclose(saveFile) != 0) {
			/*For any case of error when closing the flie*/
			printf("ERROR: File problem encountered.\n");
			return 0;
		}
		return 1;
}

int parseSaveBoardCommandAndExec(char command[256], struct Board *board, struct Board *fixedBoard, int edit) {
	char currCommand[256];
	char seperator[] = " \t\n\v\f\r";
	char *stok;
	char path[256];
	int i = 0;
	int len;
	char *point;
	copyCommand(command, currCommand);
	stok = strtok(currCommand, seperator);
	stok = strtok(NULL, seperator);
	if (stok == NULL) {
		printf("ERROR: You didn't supply a path! Try again\n;");
		return 0;
	}

	point = stok;
	len = strlen(stok);
	while (i < len) {;
		path[i] = *point;
		point++;
		i++;
	}
	/*Makes sure the file ends with .txt*/
	path[i] = '.';
	path[i+1] = 't';
	path[i+2] = 'x';
	path[i+3] = 't';
	path[i+4] = '\0';
	stok = strtok(NULL, seperator);
	if (stok != NULL) {
		/*Check for extra args but the path*/
		printf("ERROR: You supplied too many args to this command. Try again!\n");
		return 0;
	}
	saveBoardToFile(board, fixedBoard, edit, path);
	printf("Board saved successfuly to path %s.\n", path);
	return 1;
}



void saveCurrentBoardToFile(struct Board *board, struct Board *originalBoard) {
	char path[1024];
	int i = 0;
	int j = 0;
	FILE *saveFile;
	fgets(path, 1024, stdin);
	saveFile = fopen(path,"w");
	while (saveFile == NULL) {
		printf("ERROR: You gave a bad path buddy. Try another one:\n");
		fgets(path, 1024, stdin);
		saveFile = fopen(path, "w");
	}
	fprintf(saveFile, "%d %d\n", board->m, board->n);
	while (i < board->N) {
		j = 0;
		while (j < board->N) {
			if ((board->values[i][j] != 0) && (board->values[i][j] == originalBoard->values[i][j])) {
				fprintf(saveFile, "%d. ", board->values[i][j]);
			} else {
				fprintf(saveFile, "%d ", board->values[i][j]);
			}
			j++;
		}
		fprintf(saveFile, "\n");
		i++;
	}
	if (fclose(saveFile) != 0) {
		printf("ERROR: File problem encountered.\n");
	}
}


int loadBoardFromFile(struct Board *board, struct Board *originalBoard, char path[256]) {
	FILE *loadFile;
	int n, m;
	int N;
	int i = 0;
	int j = 0;
	int basic;
	char currChar = '0';

	int fscanf_success;
	loadFile = fopen(path, "r");
	if (loadFile == NULL) {
		/*Checks for loading errors*/
		printf("ERROR: That's not really a file...\n");
		return 0;
	}
/*	printf("Loaded!\n");*/
	fscanf_success = fscanf(loadFile, "%d %d\n", &m, &n);
	if (fscanf_success != 2) {
		/*checks for format of n, m*/
		printf("ERROR: The file was there, but not in the right format. Try another.\n");
		return 0;
	}
	if (m*n > 99) {
		/*checks limit for n*m*/
		printf("ERROR: n*m must be between 0 and 99.\n");
		return 0;
	}
	if (m <= 0 || n <=0) {
		/*Checks n,m are positive*/
		printf("ERROR: both n and m need to be positive.\n");
		return 0;
	}
	setDimensions(board, n, m);
	setDimensions(originalBoard, n, m);
	N = n * m;
	/*Parsing the format*/
	while (i < N) {
		j = 0;
		while (j < N) {
			if (feof(loadFile)) {
				printf("ERROR: Your file's illegal. Try another file.\n");
				freeBoardMemory(board);
				freeBoardMemory(originalBoard);
				return 0;
			}
			basic = fscanf(loadFile, "%d", &board->values[i][j]);
			if (basic != 1) {
				printf("ERROR: The file's in the wrong format...\n");
				freeBoardMemory(board);
				freeBoardMemory(originalBoard);
				return 0;
			}
			if (board->values[i][j] > N || board->values[i][j] < 0) {
				printf("ERROR: The numbers in your file are out of range.\n");
				freeBoardMemory(board);
				freeBoardMemory(originalBoard);
				return 0;
			}
			currChar = fgetc(loadFile);
			if (currChar == '.') {
				originalBoard->values[i][j] = board->values[i][j];
			}
			j++;
		}
		i++;
	}
	if (fclose(loadFile) != 0) {
		printf("ERROR: File error occurred.\n");
		freeBoardMemory(board);
		freeBoardMemory(originalBoard);
		return 0;
	}
	return 1;
}

void extractPathFromCommand(char command[256], char path_addr[256]) {
	/*Transfers the path arg from the command to another array.*/
	char *stok;
	int i = 0;
	char currCommand[256];
	char seperators[] = " \t\n\v\r\f";
	copyCommand(command, currCommand);
	stok = strtok(currCommand, seperators);
	stok = strtok(NULL, seperators);
	while (stok[i] != '\0' && i < 256) {
		path_addr[i] = stok[i];
		i++;
	}
	path_addr[i] = '\0';
	/*printf("%s\n", path_addr);*/
}



int applyEditWithLoad(char command[256], struct Board *board, struct Board *solutionBoard) {
	/*Middle func for Load - extracts the path for the Load command*/
	char path_addr[256];
	int load_success;
	extractPathFromCommand(command, path_addr);
	load_success = loadBoardFromFile(board, solutionBoard, path_addr);
	if (load_success == 0) {
		return 0;
	}
	return 1;
}

int applySolveWithLoad(char command[256], struct Board *board, struct Board *solutionBoard) {
	/*Middle func for Load - extracts the path for the Load command*/
	char path_addr[256];
	int load_success;
	extractPathFromCommand(command, path_addr);
	load_success = loadBoardFromFile(board, solutionBoard, path_addr);
	if (load_success == 0) {
		return 0;
	}
	return 1;
}

int validateLoadCommandAndPath(char command[256]) {
	/*Validates the command given is correct, with number of args and valid path*/
	char seperators[] = " \t\n\r\v\f";
	char *stok;
	char currCommand[256];
	int i = 0;
	char path[256];
	FILE *checker;
	copyCommand(command, currCommand);
	stok = strtok(currCommand, seperators);
	stok = strtok(NULL, seperators);
	if (stok == NULL) {
		printf("ERROR: No argument passed to solve. Try again!\n");
		return 0;
	}
	while (i < 256 && stok[i] != '\0') {
		path[i] = stok[i];
		i++;
	}
	path[i] = '\0';
	stok = strtok(NULL, seperators);
	if (stok != NULL) {
		printf("ERROR: You have supplied too many args for this command. Try again!\n");
		return 0;
	}
	checker = fopen(path, "r");
	if (checker == NULL) {
		printf("ERROR: Path given is erroneous. Try again!\n");
		return 0;
	}
	if (fclose(checker) != 0) {
		printf("ERROR: File error occurred. Try again\n");
		return 0;
	}
	return 1;
}

int LoadBoardProcedureEdit(char command[256], struct Board *gameBoard, struct Board *solutionBoard, int inGame) {
	/*Load File for Edit, creates semi-boards. If they're good, continue*/
	int success_param;
	struct Board tmpBoardGame;
	struct Board tmpBoardSolution;

	success_param = applyEditWithLoad(command, &tmpBoardGame, &tmpBoardSolution);
	if (success_param == 0) {
		return 0;
	} else {
		if (inGame == 1) {
			freeBoardMemory(gameBoard);
			freeBoardMemory(solutionBoard);
		}
		setDimensions(gameBoard, tmpBoardGame.n, tmpBoardGame.m);
		setDimensions(solutionBoard, tmpBoardSolution.n, tmpBoardSolution.m);
	}
	copyBoardByPointer(&tmpBoardGame, &gameBoard);
	return 1;
}

int LoadBoardProcedureSolve(char command[256], struct Board *gameBoard, struct Board *solutionBoard, int inGame) {
	char seperators[] = " \t\n\r\v\f";
	int success_param;
	char currCommand[256];
	char *stok;
	/* TODO:: These *might* be a problem. */
	struct Board tmpBoardGame;
	struct Board tmpBoardSolution;
	copyCommand(command, currCommand);
	stok = strtok(currCommand, seperators);
	stok = strtok(NULL, seperators);
	if (stok == NULL) {
		printf("ERROR: No path supplied.\n");
		return 0;
	}
	success_param = applySolveWithLoad(command, &tmpBoardGame, &tmpBoardSolution);
	if (success_param == 0) {
		return 0;
	} else {
		if (isBoardIllegal(&tmpBoardSolution) == 1) {
			/*Makes sure the fixed cells aren't erroneous*/
			freeBoardMemory(&tmpBoardGame);
			freeBoardMemory(&tmpBoardSolution);
			return 0;
		}
		if (inGame == 1) {
			freeBoardMemory(gameBoard);
			freeBoardMemory(solutionBoard);
		}
		setDimensions(gameBoard, tmpBoardGame.n, tmpBoardGame.m);
		setDimensions(solutionBoard, tmpBoardSolution.n, tmpBoardSolution.m);
	}
	copyBoardByPointer(&tmpBoardGame, &gameBoard);
	copyBoardByPointer(&tmpBoardSolution, &solutionBoard);
	freeBoardMemory(&tmpBoardGame);
	freeBoardMemory(&tmpBoardSolution);
	return 1;
}

