/*
 * Solver.c
 *
 *  Created on: May 21, 2019
 *      Author: Itamar
 */

/******************************************************************************

 Online C Compiler.
 Code, Compile, Run and Debug C program online.
 Write your code in this editor and press "Run" button to compile and execute it.

 *******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "Game.h"
int isFull(int sudo[9][9]);
int* findEmpty(int sudo[9][9], int rowcol[2]);
int isLegalInBox(int sudo[9][9], int row, int col, int num);
int isLegal(int sudo[9][9], int row, int col, int num);
int findLenWithBreak(int arr[9]);
void initialize(int sudo[9][9]);
void listInvalidsFromRowCol(int sudo[9][9], int invalids[9], int row, int col);
void listInvalidsFromSquare(int sudo[9][9], int invalids[9], int row, int col);
void listAllValids(int sudo[9][9], int left[9], int row, int col);
void rearrangeLeft(int left[9]);
int randBacktrack(int sudo[9][9]);
void createPuzzle(int fullSudo[9][9], int gameSudo[9][9], int hints);
int deterBacktrack(int sudo[9][9]);
int solver(int currentBoard[9][9], int solution[9][9]);
void copyBoard(int src[9][9], int dst[9][9]);

int isFull(int sudo[9][9]) {
	int i1 = 0;
	int i2 = 0;
	while (i1 < 9) {
		i2 = 0;
		while (i2 < 9) {
			if (sudo[i1][i2] == 0) {
				return -1;
			}
			i2++;
		}
		i1++;
	}
	return 1;
}

int* findEmpty(int sudo[9][9], int rowcol[2]) {
	int i1 = 0;
	int i2 = 0;
	while (i1 < 9) {
		i2 = 0;
		while (i2 < 9) {
			if (sudo[i1][i2] == 0) {
				rowcol[0] = i1;
				rowcol[1] = i2;
				return rowcol;
			}
			i2++;
		}
		i1++;
	}
	return rowcol;
}

int isLegalInBox(int sudo[9][9], int row, int col, int num) {
	int curr_r = 3 * (row / 3);
	int curr_c = 3 * (col / 3);
	int i = 0;
	int j = 0;
	while (i < 3) {
		j = 0;
		while (j < 3) {
			if (row != (curr_r + i) && col != (curr_c + j) && sudo[curr_r + i][curr_c + j] == num) {
				return -1;
			}
			j++;
		}
		i++;
	}
	return 1;
}

int isLegal(int sudo[9][9], int row, int col, int num) {
	int i1 = 0;
	int i2 = 0;
	if (num == 0) {
		return 1;
	}
	while (i1 < 9) {
		if (i1 != col && sudo[row][i1] == num) {
			return -1;
		}
		i1++;
	}
	while (i2 < 9) {
		if (i2 != row && sudo[i2][col] == num) {
			return -1;
		}
		i2++;
	}
	if (isLegalInBox(sudo, row, col, num) == -1) {
		return -1;
	}
	return 1;
}


int findLenWithBreak(int arr[9]) {
	int i = 0;
	if (arr[0] == 0) {
		return 0;
	}
	while (arr[i] != '\0' && i < 9) {
		i++;
	}
	return i;
}

void initialize(int sudo[9][9]) {
	int i = 0;
	int j = 0;
	while (i < 9) {
		j = 0;
		while (j < 9) {
			sudo[i][j] = 0;
			j++;
		}
		i++;
	}
}

void listInvalidsFromRowCol(int sudo[9][9], int invalids[9], int row, int col) {
	int i = 0;
	int j = 0;
	while (i < 9) {
		if (sudo[row][i] != 0) {
			invalids[sudo[row][i] - 1] = sudo[row][i];
		}
		i++;
	}
	while (j < 9) {
		if (sudo[j][col] != 0) {
			invalids[sudo[j][col] - 1] = sudo[j][col];
		}
		j++;
	}
}

void listInvalidsFromSquare(int sudo[9][9], int invalids[9], int row, int col) {
	int curr_r = 3 * (row / 3);
	int curr_c = 3 * (col / 3);
	int i = 0;
	int j = 0;
	while (i < 3) {
		j = 0;
		while (j < 3) {
			if (sudo[curr_r + i][curr_c + j] != 0) {
				invalids[sudo[curr_r + i][curr_c + j] - 1] =
						sudo[curr_r + i][curr_c + j];
			}
			j++;
		}
		i++;
	}
}

void listAllValids(int sudo[9][9], int left[9], int row, int col) {
	int i = 0;
	int j = 0;
	int invalids[9] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	listInvalidsFromRowCol(sudo, invalids, row, col);
	listInvalidsFromSquare(sudo, invalids, row, col);
	while (i < 9) {
		if (invalids[i] == 0) {
			left[j] = i + 1;
			j++;
		}
		i++;
	}
	left[j] = '\0';
}

void rearrangeLeft(int left[9]) {
    int newLeft[9];
    int i = 0;
    int j = 0;
    int h = 0;
    while (i < 9) {
        if (left[i] != 0 && left[i] != '\0') {
            newLeft[j] = left[i];
            j++;
        }
        i++;
    }
    while (h < j) {
        left[h] = newLeft[h];
        h++;
    }
	left[h] = '\0';
}


int randBacktrack(int sudo[9][9]) {
	int row;
	int col;
	int currRand;
	int chosen;
	int len;
	int rowcol[2];
	int left[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
	if (isFull(sudo) == 1) {
		return 1;
	}
	findEmpty(sudo, rowcol);
	row = rowcol[0];
	col = rowcol[1];
	listAllValids(sudo, left, row, col);
	len = findLenWithBreak(left);
	while (len != 0) {
		len = findLenWithBreak(left);
		if (len == 0 || left[0] == '\0') {
			break;
		}
		if (len == 1) {
			currRand = 0;
			chosen = left[0];
		} else {
			currRand = rand() % len;
			chosen = left[currRand];
		}
		if (isLegal(sudo, row, col, chosen) == 1 && isLegalInBox(sudo, row, col, chosen) == 1) {
			sudo[row][col] = chosen;
			if (randBacktrack(sudo) == 1) {
				return 1;
			}
			sudo[row][col] = 0;
		}
		left[currRand] = 0;
		rearrangeLeft(left);
	}
	return -1;
}

int deterBacktrack(int sudo[9][9]) {
	int row;
	int col;
	int rowcol[2];
	int i = 1;
	if (isFull(sudo) == 1) {
		return 1;
	}
	findEmpty(sudo, rowcol);
	row = rowcol[0];
	col = rowcol[1];
	while (i <= 9) {
		sudo[row][col] = i;
		if (isLegal(sudo, row, col, i) == 1 && isLegalInBox(sudo, row, col, i) == 1) {
			if (deterBacktrack(sudo) == 1) {
				return 1;
			}
		}
		i++;
	}
	sudo[row][col] = 0;
	return -1;
}

void createPuzzle(int fullSudo[9][9], int gameSudo[9][9], int hints) {
	int i = 0;
	int row;
	int col;
	while (i < hints) {
		col = rand() % 9;
		row = rand() % 9;
		if (gameSudo[row][col] == 0) {
			gameSudo[row][col] = fullSudo[row][col];
			i++;
		}
	}
}

void copyBoard(int src[9][9], int dst[9][9]) {
	int i = 0;
	int j = 0;
	while (i < 9) {
		j = 0;
		while (j < 9) {
			dst[i][j] = src[i][j];
			j++;
		}
		i++;
	}
}

int solver(int currentBoard[9][9], int solution[9][9]) {/*returns 1 if solvable or -1 if unsolvable. if solvable and the solution is different to the original generated board,  changes the solution board to match the new solution*/
	int isSolvable;
	int currentBoardVerifier[9][9];
	copyBoard(currentBoard, currentBoardVerifier); /*No reference, let it work on a copy*/
	isSolvable = deterBacktrack(currentBoardVerifier);
	if (isSolvable == 1) {
		copyBoard(currentBoardVerifier, solution);
		return 1;
	}
	return -1;
}
