#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Board.h"
#include "Utilities.h"
#include "Gurobi.h"

/*Auxiliary function: copies the command to another array for actions to be set on it*/
void copyCommand(char src[256], char dst[256]) {
	int i = 0;
		while (i < 256) {
			dst[i] = src[i];
			i++;
		}

}

/*Auxiliary function: determines if the current string is an integer*/
int isNum(char* str) {
    int i = 0;
    while (str[i] != '\0') {
        if (str[i] > '9' || str[i] < '0') {
            return 0;
        }
        i++;
    }
    return 1;
}

/*Autofill: goes through each cell, and calls an aux function to check it legal values. If they exist,
 * they are added within it*/
int autoFill(struct Board *gameBoard) {
	struct Board dummyBoard;
	int N = gameBoard->N;
	int n = gameBoard->n;
	int m = gameBoard->m;
	int i = 0;
	int j = 0;
	setDimensions(&dummyBoard, n, m);
	copyBoard(gameBoard, &dummyBoard);
	while (i < N) {
		j = 0;
		while (j < N) {
			howManyValidForCell(&dummyBoard, gameBoard, i+1, j+1);
			j++;
		}
		i++;
	}
	if (compareBoards(&dummyBoard, gameBoard) == 0) {
/*		printf("Autofill: No cells could be auto-filled!\n")*/
		freeBoardMemory(&dummyBoard);
		return 0;
	}
	freeBoardMemory(&dummyBoard);
	return 1;
}


/*Determines if the current char array from fgets is "set" command"*/
int isSetCommand(char command[256]) {
	char seperators[] = " \t\n\r\f\v";
	char currCommand[256];
	char* stok;
	copyCommand(command, currCommand);
	stok = strtok(currCommand, seperators);
	if (strcmp(stok, "set") != 0) {
		return 0;
	}
	stok = strtok(NULL, seperators);
	return 1;
}

/*Determines if the current char array from fgets is "exit" command"*/
int isExitCommand(char command[256]) {
	char seperators[] = " \t\n\r\f\v";
	char currCommand[256];
	char* stok;
	copyCommand(command, currCommand);
	stok = strtok(currCommand, seperators);
	if (strcmp(stok, "exit") != 0) {
		return 0;
	}
	stok = strtok(NULL, seperators);
	return 1;
}

/*Determines the length of the command, for user input reasons*/
int findOfCommand(char command[256]) {
	int i = 0;
	while (i<512) {
		if (command[i] == '\0' || command[i] =='\n') {
			break;
		}
		i++;
	}
	return i;
}

/*Parses the input found to be "set" command and makes sure it follows the following rules:
 * number of params is enough and all params are numbers. Pops
 * an error and fails if they don't.
 */
int parseSetCommand(char command[256], int params[3]) {
	int retVal = 1;
    int i = 0;
    int j = 0;
    int tmp_num;
	char seperators[] = " \n\t\v\f";
	char *stok = strtok(command, seperators);
	stok = strtok(NULL, seperators);
	while (stok != NULL && j < 3) {
	    if (isNum(stok)) {
	        sscanf(stok, "%d", &tmp_num);
	        params[i] = tmp_num;
    	    i++;
    	    j++;
	    } else {
	    	printf("ERROR: value number %d isn't a valid number to input. Try again!\n", i+1);
	    	j++;
	        retVal = 0;
	        return retVal;
	    }
	    stok = strtok(NULL, seperators);
	}
	if (i < 3) {
		printf("ERROR: only %d values were good for input. Try again\n", i);
		retVal = 0;
		return retVal;
	}
	if (stok != NULL) {
		printf("ERROR: Your command is too long...\n");
		retVal = 0;
		return retVal;
	}
	return retVal;
}

/*Actually applies "set" command, after validating the range for each param (that found to be a valid
 * from the parse function). Success == 1, failure == 0.
 */
int validateSetCommand(char command[256], struct Board *board, struct Board *fixedBoard, int solve) {
	int x,y,z;
	int N = board->N;
	int success = 0;
	int params[3];
	char currCommand[256];
	copyCommand(command, currCommand);
	success = parseSetCommand(currCommand, params);
	if (success == 0) {
		return 0;
	}
	x = params[1];
	y = params[0];
	z = params[2];
	if (y <= 0 || y > N) {
		printf("Whoops! Your first parameter X is out of range! Remember: Range is 1-%d\n",N);
		return 0;
	}
	if (x <= 0 || x > N) {
		printf("Whoops! Your second parameter Y is out of range! Remember: Range is 1-%d\n",N);
		return 0;
	}
	if (z < 0 || z > N) {
		printf("Whoops! Your third parameter Z is out of range! Remember: Range is 1-%d\n",N);
		return 0;
	}
	if (fixedBoard->values[x-1][y-1] != 0 && solve == 1) {
		printf("Yikes! That's a fixed cell! Can't place there...\n");
		return 0;
	}
	board->values[x-1][y-1] = z;
	return 1;
}

/*Determines if the current char array from fgets is "redo" command"*/
int isRedoCommand(char command[256]) {
	char seperators[] = " \t\n\r\f\v";
	char currCommand[256];
	char* stok;
	copyCommand(command, currCommand);
	stok = strtok(currCommand, seperators);
	if (strcmp(stok, "redo") != 0) {
		return 0;
	}
	stok = strtok(NULL, seperators);
	return 1;
}

/*Determines if the current char array from fgets is "undo" command"*/
int isUndoCommand(char command[256]) {
	char seperators[] = " \t\n\r\f\v";
	char currCommand[256];
	char* stok;
	copyCommand(command, currCommand);
	stok = strtok(currCommand, seperators);
	if (strcmp(stok, "undo") != 0) {
		return 0;
	}
	stok = strtok(NULL, seperators);
	return 1;
}

/*Determines if the current char array from fgets is "edit" command"*/
int isEditCommand(char command[256]) {
	char seperators[] = " \t\n\r\f\v";
	char currCommand[256];
	char* stok;
	int checkIfGood = 0;
	copyCommand(command, currCommand);
	stok = strtok(currCommand, seperators);
	if (strcmp(stok, "edit") != 0) {
		return 0;
	}
	stok = strtok(NULL, seperators);
	if (stok != NULL) {
		checkIfGood = 2;
	}
	stok = strtok(NULL, seperators);
	if (stok != NULL) {
		printf("ERROR: You gave too many args for this command. Try again!\n");
		return 0;
	}
	if (checkIfGood == 2) {
		return 2;
	} else {
		return 1;
	}
}

/*Determines if the current char array from fgets is "reset" command"*/

int isResetCommand(char command[256]) {
	char seperators[] = " \t\n\r\f\v";
	char currCommand[256];
	char* stok;
	copyCommand(command, currCommand);
	stok = strtok(currCommand, seperators);
	if (strcmp(stok, "reset") != 0) {
		return 0;
	}
	stok = strtok(NULL, seperators);
	return 1;
}

/*Determines if the current char array from fgets is "save" command"*/
int isSaveCommand(char command[256]) {
	char seperators[] = " \t\n\r\f\v";
	char currCommand[256];
	char* stok;
	copyCommand(command, currCommand);
	stok = strtok(currCommand, seperators);
	if (strcmp(stok, "save") != 0) {
		return 0;
	}
	stok = strtok(NULL, seperators);
	if (stok == NULL) {
		printf("ERROR: you gotta give us some name if you want to save, no?\n");
		return 0;
	}
	return 1;
}

/*Determines if the current char array from fgets is "solve" command"*/
int isSolveCommand(char command[256]) {
	char seperators[] = " \t\n\r\f\v";
	char currCommand[256];
	char* stok;
	copyCommand(command, currCommand);
	stok = strtok(currCommand, seperators);
	if (strcmp(stok, "solve") != 0) {
		return 0;
	}
	stok = strtok(NULL, seperators);
	return 1;
}

/*Determines if the current char array from fgets is "autofill" command"*/
int isAutofillCommand(char command[256]) {
	char seperators[] = " \t\n\r\f\v";
	char currCommand[256];
	char* stok;
	copyCommand(command, currCommand);
	stok = strtok(currCommand, seperators);
	if (strcmp(stok, "autofill") != 0) {
		return 0;
	}
	stok = strtok(NULL, seperators);
	return 1;
}

/*Determines if the current char array from fgets is "mark_errors" command"*/

int isMark_ErrorsCommand(char command[256]) {
	char seperators[] = " \t\n\r\f\v";
	char currCommand[256];
	char* stok;
	copyCommand(command, currCommand);
	stok = strtok(currCommand, seperators);
	if (strcmp(stok, "mark_errors") != 0) {
		return 0;
	}
	stok = strtok(NULL, seperators);
	if (stok == NULL) {
		printf("ERROR: Not enough parameters given. Try again!\n");
		return 0;
	}
	return 1;
}

/*Parses the mark_errors command. If value is good, changes it; otherwise, returns previous one*/
int getMarkErrorsValue(char command[256], int prev_mark) {
	char currCommand[256];
	char *stok;
	char seperators[] = " \t\n\r\v\f";
	int tmpNum;
	int success;
	copyCommand(command, currCommand);
	stok = strtok(currCommand, seperators);
	stok = strtok(NULL, seperators);
	if (stok == NULL) {
		printf("ERROR: No parameter supplied. Needs to be 0 or 1\n");
	}
	if (isNum(stok) == 1 && strlen(stok) == 1) {
		success = sscanf(stok, "%d", &tmpNum);
		if (success == 0) {
			printf("ERROR: Could not validate the number given.\n");
			return 0;
		}
	} else {
		printf("ERROR: That's not a valid number for Mark_Errors!\n");
		return prev_mark;
	}
	if (tmpNum != 0 && tmpNum != 1) {
		printf("ERROR: That's not a valid number for Mark_Errors!\n");
		return prev_mark;
	}
	stok = strtok(NULL, seperators);
	if (stok != NULL) {
		printf("That mark_errors command was not valid - you put extra stuff! Try Again\n");
		return prev_mark;
	}
	return tmpNum;
}

/*Determines if the current char array from fgets is "print_board" command"*/
int isPrintBoardCommand(char command[256]) {
	char seperators[] = " \t\n\r\f\v";
	char currCommand[256];
	char* stok;
	copyCommand(command, currCommand);
	stok = strtok(currCommand, seperators);
	if (strcmp(stok, "print_board") != 0) {
		return 0;
	}
	return 1;
}

/*Determines if the current char array from fgets is "num_solutions" command"*/
int isNumSolutionCommand(char command[256]) {
	char seperators[] = " \t\n\r\f\v";
	char currCommand[256];
	char* stok;
	copyCommand(command, currCommand);
	stok = strtok(currCommand, seperators);
	if (strcmp(stok, "num_solutions") != 0) {
		return 0;
	}
	stok = strtok(NULL, seperators);
	if (stok != NULL) {
		return 0;
	}
	return 1;
}

/*Determines if the current char array from fgets is "guess" command"*/
int isGuessCommand(char command[256]) {
	char seperators[] = " \t\n\r\f\v";
	char currCommand[256];
	char* stok;
	copyCommand(command, currCommand);
	stok = strtok(currCommand, seperators);
	if (strcmp(stok, "guess") != 0) {
		return 0;
	}
	stok = strtok(NULL, seperators);
	if (stok == NULL) {
		printf("ERROR: Not enough params found for Guess command!\n");
		return 0;
	}
	return 1;
}

/*Parses guess command and checks if it has a correct number of params + a valid float*/
float parse_guess_command(char command[256], struct Board *board) {
	char seperators[] = " \t\n\r\f\v";
	char currCommand[256];
	char* stok;
	int success;
	float tmpNum;
	board->values[0][0]--;
	board->values[0][0]++;
	copyCommand(command, currCommand);
	stok = strtok(currCommand, seperators);
	/*printf("String first is %s\n", stok)*/;
	stok = strtok(NULL, seperators);
	/*printf("String later is %s\n", stok)*/;
	success = sscanf(stok, "%f", &tmpNum);
	if (success == 0) {
		printf("ERROR: That's not a valid Float number! Try again\n.");
		return -1;
	}
	stok = strtok(NULL, seperators);
	if (stok != NULL) {
		printf("ERROR: You put too many args. Try again!\n");
		return -1;
	}
	if (tmpNum > 1 || tmpNum < 0) {
		printf("ERROR: Threshold must be between 0 and 1.\n");
	}
	return tmpNum;
}

/*Determines if the current char array from fgets is "generate" command"*/
int isGenerateCommand(char command[256]) {
	char seperators[] = " \t\n\r\f\v";
	char currCommand[256];
	char* stok;
	copyCommand(command, currCommand);
	stok = strtok(currCommand, seperators);
	if (strcmp(stok, "generate") != 0) {
		return 0;
	}
	stok = strtok(NULL, seperators);
	if (stok == NULL) {
		printf("ERROR: Not enough params found for Guess command!\n");
		return 0;
	}
	return 1;
}

/*Parses generate command; if successful, returns the two parameters given in an array. Otherwise,
 * the array remains {-1,-1}*/
int parse_generate_command(char command[256], int xy[2], struct Board board) {
	char seperators[] = " \t\n\v\r\f\v";
	char currCommand[256];
	char* stok;
	int tmpNum;
	int failFlag = 0;
	int N_sq = (board.N)*(board.N);
/*	printf("%d, %d\n", board.N, N_sq);*/
	copyCommand(command, currCommand);
	stok = strtok(currCommand, seperators);
	stok = strtok(NULL, seperators);
	if (isNum(stok) != 1) {
		printf("ERROR: First param is not a valid int!\n");
		failFlag = 1;
	}
	if (failFlag == 0) {
		sscanf(stok, "%d", &tmpNum);
		if (tmpNum > N_sq || tmpNum < 0) {
			printf("ERROR: Number is too big! keep in range between 0 and %d\n", N_sq);
			return 0;
		} else {
			xy[0] = tmpNum;
		}
	}
	stok = strtok(NULL, seperators);
	if (stok == NULL) {
		printf("ERROR: Not enough parameters supplied!\n");
		return 0;
	}
	if (isNum(stok) != 1) {
		printf("ERROR: Second param is not a valid int!\n");
		failFlag = 1;
		return 0;
	}
	if (failFlag == 0) {
		sscanf(stok, "%d", &tmpNum);
		if (tmpNum > N_sq || tmpNum < 0) {
			printf("ERROR: Number is too big! keep in range between 0 and %d\n", N_sq);
			return 0;
		} else {
			xy[1] = tmpNum;
		}
	}
	return 1;
}


int findLenOfCommand(char command[512]) {
	int i = 0;
	while (i<512) {
		if (command[i] == '\0' || command[i] =='\n') {
			break;
		}
		i++;
	}
	return i;
}

/*Aux function: determines the length of the current command*/
int commandLenVerifier(char command[512]) {
	int len = findLenOfCommand(command);
	if (len > 256) {
		return 0;
	} else {
		return 1;
	}
}

/*Determines if the current char array from fgets is "hint" command"*/
int isHintCommand(char command[256]) {
	char seperators[] = " \t\n\r\f\v";
	char currCommand[256];
	char* stok;
	copyCommand(command, currCommand);
	stok = strtok(currCommand, seperators);
	if (strcmp(stok, "hint") != 0) {
		return 0;
	}
	stok = strtok(NULL, seperators);
	if (stok == NULL) {
		printf("ERROR: Not enough params found for Guess command!\n");
		return 0;
	}
	return 1;
}


/*
 * Checks if the current char array follows the rules for a hint command - finding 2 numbers
 * following the "hint" string and making sure they're within range of N.
 */
int parse_hint_command(char command[256], int xy[2], int N) {
	char currCommand[256];
	char seperators[] = " \t\n\r\v\f";
	char *stok;
	int errVal = 0;
	int tmpNum;
	copyCommand(command, currCommand);
	stok = strtok(currCommand, seperators);
	stok = strtok(NULL, seperators);
	if (isNum(stok) != 1) {
		printf("ERROR: Your first parameter is not a number!\n");
		errVal = 1;
	} else {
		sscanf(stok, "%d", &tmpNum);
		if (tmpNum > N || tmpNum <= 0) {
			printf("ERROR: First parameter out of range! Remember: range is 1-%d\n", N);
			errVal = 1;
		} else {
			xy[0] = tmpNum;
		}
	}
	stok = strtok(NULL, seperators);
	if (stok == NULL) {
		printf("ERROR: You don't have enough parameters!\n");
		xy[0] = -1;
		return 0;
	}
	if (isNum(stok) != 1) {
		printf("ERROR: Your Second parameter is not a number!\n");
		errVal = 1;
	} else {
		sscanf(stok, "%d", &tmpNum);
		if (tmpNum > N || tmpNum <= 0) {
			printf("ERROR: Second parameter out of range! Remember: range is 1-%d\n", N);
			errVal = 1;
		} else {
			xy[1] = tmpNum;
		}
	}
	stok = strtok(NULL, seperators);
	if (stok != NULL) {
		printf("ERROR: You entered too many parameters!\n");
		xy[0] = -1;
		xy[1] = -1;
		return 0;
	}
	return errVal;

}


/*Determines if the current char array from fgets is "guess_hint" command"*/
int isGuessHintCommand(char command[256]) {
	char seperators[] = " \t\n\r\f\v";
	char currCommand[256];
	char* stok;
	copyCommand(command, currCommand);
	stok = strtok(currCommand, seperators);
	if (strcmp(stok, "guess_hint") != 0) {
		return 0;
	}
	stok = strtok(NULL, seperators);
	if (stok == NULL) {
		printf("ERROR: Not enough params found for Guess command!\n");
		return 0;
	}
	return 1;
}


/*Determines if the current char array from fgets is "validate" command"*/
int isValidateCommand(char command[256]) {
	char seperators[] = " \t\n\r\f\v";
	char currCommand[256];
	char *stok;
	copyCommand(command, currCommand);
	stok = strtok(currCommand, seperators);
	if (strcmp(stok, "validate") != 0) {
		return 0;
	}
	stok = strtok(NULL, seperators);
	return 1;

}

/*aux function to help with buffer overflow in certain cases*/
void copyFromInputToCommand(char src[512], char dst[256]) {
	int i = 0;
	while (i < 256) {
		dst[i] = src[i];
		i++;
	}
}

/*Checks that commands with no parameters indeed have no parameters in them*/
int checkNoParam(char command[256]) {
	char seperators[] = " \t\n\r\f\v";
	char *stok;
	char currCommand[256];
	copyCommand(command, currCommand);
	stok = strtok(currCommand, seperators);
	stok = strtok(NULL, seperators);
	if (stok != NULL) {
		printf("ERROR: No parameters should be given to this command.\n");
		return 0;
	}
	return 1;
}

