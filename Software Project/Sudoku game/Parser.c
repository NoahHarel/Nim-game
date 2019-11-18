#include <stdlib.h>
#include <stdio.h>
#include "Game.h"
#include "Solver.h"
#include <string.h>
#define MaxLen = 1024;


int isCommandLegal(char* command) ;
int isSetCommand(char command[]) ;
int extractFromSet(char command[], int answer[3]);
int applySet(int currSolution[9][9], int table[9][9], char command[]) ;
void setCommand(char* command, char* answer);
int isSetAnswerValid(char* answer) ;
int isHintCommand(char* command);
void applyHint(int solution[9][9], char command[]) ;
int isValidateCommand(char command[]);
void applyValidate(int currentBoard[9][9], int solution[9][9]);
int isRestartCommand(char command[]);
int isExitCommand(char command[]);
void applyExit();


int isCommandLegal(char* command) {
	int i = 0;
	int flag = 0;
	int len = sizeof(command);
	while (i < len && i <= 1024) {
		if (command[i] != ' ') {
			flag = 1;
		}
		i++;
	}
	if (flag == 1) {
		return 1;
	} else {
		return -1;
	}
}


int isSetCommand(char command[]) {
	char str[] = "set";
	if (strstr(command, str)) {
		return 1;
	}
	return 0;
}

int extractFromSet(char command[], int answer[3]) {
	int counter = 0;
	int currNum = 0;
	int validator = 0;
	int i = 2;
	while (i < 1024) {
		currNum = (int)command[i] - 48;
		if ((command[i] != ' ' && command[i-1] == ' ') && (currNum >= 0 && currNum <= 9)) {
			answer[counter] = currNum;
			counter++;
			if (counter == 3) {
				break;
			}
		}
		i++;
	}
	while (validator++ < 3) {
		if (answer[validator] == 0) {
			return -1;
		}
	}
	return 1;
}

int applySet(int currSolution[9][9], int table[9][9], char command[]) {
	int x;
	int y;
	int z;
	int answer[3] = {-1,-1,-1};
	extractFromSet(command, answer);
	x = answer[0] - 1;
	y = answer[1] - 1;
	z = answer[2];
	if (x == -1 || y == -1 || z == -1) {
		printf("Error: invalid command\n");
		return -1;
	}
	else if (currSolution[y][x] != 0) {
		printf("Error: cell is fixed\n");
		return -1;
	} else if (z < 0) {
		printf("Error: invalid command\n");
		return -1;
	}
	else if (isLegal(table, y, x, z) != 1) {
		printf("Error: value is invalid\n");
		return -1;
	} else {
		table[y][x] = z;
		return 1;
	}

}

void setCommand(char* command, char* answer) {
	int i = 3;
	int j = 0;
	while (i < 1024) {
		if (command[i] == ' ') {
			i++;
			continue; /*if it's a whitespace, move on to the next index*/
		} else if (command[i] != ' ' && command[i-1] == ' ') {
			answer[j] = command[i]; /*if it's not whitespace preceded by whitespace, add to answer*/
			j++;
		}
		if (j == 3) {
			break;
		}
	}
}


int isSetAnswerValid(char* answer) {
	int i = 0;
	while (i<3) {
		if (answer[i] == '0') {
			return -1;
		}
		i++;
	}
	return 1;
}




int isHintCommand(char* command) {
	int i = 0;
	int comm_len = sizeof(command);
	int j = 0;
	char hint_comm[] = {'h','i','n','t', ' '};
	while (i < comm_len && j < 5) {
		if (command[i] != hint_comm[j]) {
			return -1;
		}
		i++;
		j++;
	}
	return 1;
	}


void applyHint(int solution[9][9], char command[]) { /*receives the original generated board and the X,Y values of the cell requested and prints out the hint*/
	int cellValue;
	int row=0;
	int col=0;
	int hintFlag = 0;
	int i=0;
	while (i<1024 && row == 0) {
		if (command[i] == 't') {
			hintFlag = 1;
		}
		if (hintFlag == 1){
			if (command[i] != ' ' && command[i-1] == ' '){
				if (col == 0){
					col = (int)command[i] - 48;
				}
				else if (row == 0){
					row = (int)command[i] - 48;

				}
			}
		}

		i++;

	}
	if (row <= 0 || col <= 0 || row > 9 || col > 9){
		printf("Error: invalid command\n");

	}
	else{
		cellValue = solution[row-1][col-1];
		printf("Hint: set cell to %d\n",cellValue);

	}
}


int isValidateCommand(char command[]) {
	char str[] = "validate";
	if (strstr(command, str)) {
		return 1;
	}
	return 0;
}


void applyValidate(int currentBoard[9][9], int solution[9][9]) { /*assumes a solver function exists in Solver*/
	int solvable;
	solvable = solver(currentBoard, solution);
	if (solvable == -1){
		printf("Validation failed: board is unsolvable\n");
	}
	else {
		printf("Validation passed: board is solvable\n");
	}

}


int isRestartCommand(char command[]) {
	char str[] = "restart";
	if (strstr(command, str)) {
		return 1;
	}
	return 0;
}


int isExitCommand(char command[]) {
	char str[] = "exit";
	if (strstr(command, str)) {
		return 1;
	}
	return 0;
}

void applyExit(){
	printf("Exiting...\n");
	exit(0);
}

