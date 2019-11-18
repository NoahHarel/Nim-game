/*
 * Game.c
 *
 *  Created on: May 21, 2019
 *      Author: Itamar
 */
#include <stdio.h>
#include <stdlib.h>

void separator() {
	char ch = '-';
	int i = 0;
	while (i < 34) {
		putchar(ch);
		i++;
	}
	printf("\n");
}

void printBoard(int fixedBoard[9][9], int currBoard[9][9]) {
	int row = 0;
	int column = 0;
	separator();
	while (row < 9) {
		column = 0;
		printf("|");
		while (column < 9) {
			printf(" ");
			if (currBoard[row][column] >= 1 && currBoard[row][column] <= 9) { /*if the cell is not empty - change to match our empty cell indicator (0? -1?) */
				if (currBoard[row][column] == fixedBoard[row][column]) { /*fixed cell */
					printf(".%d", currBoard[row][column]);
				} else {
					printf(" %d", currBoard[row][column]);
				}
			}

			else {
				printf("  ");
			}

			if (column == 2 || column == 5) {
				printf(" |");
			}

			column++;
		}
		printf(" |\n");
		if (row % 3 == 2) {
			separator();
		}
		row++;
	}
}

int receiveNumber() {  /*Function returns the number of hints between [0,80]*/
	int flag = 0;
	int numOfCells;
	int valid;
	while (flag == 0) { /*Will loop until valid option will set flag = 1*/
		if (feof(stdin)) { /*EOF will force exit through the loop; main.c sees -1 as termination*/
			return -1;
		}
		valid = scanf("%d", &numOfCells);
		if (valid == 0) {
			printf("Error: not a number\n");
			flag = 1;
			return -1;
		}
		else if (valid < 0 || valid == EOF) {
					flag = 1;
					return -1;
				}
		else if (numOfCells < 0 || numOfCells > 80) {
			printf("Error: invalid number of cells to fill (should be between 0 and 80)\n");
			printf("Please enter the number of cells to fill [0-80]:\n");
			continue;
		} else {
			flag = 1;
		}
	}
	return numOfCells;
}

char* getCommand(char command[]) {
	fgets(command, 1024, stdin);
	return command;
}


void reassignCommand(char command[1024]) {
	int i = 0;
	while (i < 1024) {
		command[i] = 'a';
		i++;
	}
}
