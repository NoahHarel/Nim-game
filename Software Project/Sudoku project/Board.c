#include <stdio.h>
#include <stdlib.h>
#include "SPBufferset.h"
#include "Board.h"
#include "Game.h"

void fillWithZeros(struct Board *board, int N) { /*just to help initialize. not sure will be in use eventually*/
	int i;
	int j;
	for (i=0; i<N;i++){
		for (j=0; j<N;j++){
			board->values[i][j]= 0;
		}
	}
}

void setDimensions(struct Board *board, int n, int m) {
	int i;
	int N = n*m;
	board->m = m;
	board->n = n;
	board->N = N;
	board->values = malloc(N*sizeof(int*));
	for(i=0;i<N;i++){
		board->values[i] = malloc(N*sizeof(int));
	}
	fillWithZeros(board, board->N);
}

void separator(int N, int m) {
	char ch = '-';
	int i = 0, s = 0;
	s = (4*N)+1+m;
	while (i < s) {
		putchar(ch);
		i++;
	}
	printf("\n");
}

/*values1 is current board.values and values2 is fixed cells*/
void printCell(int row, int col, int N, int** values1, int** values2, int showErrors, int isErroneous){
	printf (" ");
	if (values1[row][col] >= 1 && values1[row][col] <= N){ /*legal value in cell*/
		printf("%2d", values1[row][col]);

		if (values1[row][col]==values2[row][col]){ /*cell is fixed*/
			printf (".");
		}

		else {
			if (showErrors == 1 && isErroneous == 1){
				printf ("*");
			}
			else {
				printf (" ");
			}
		}
	}
	else{
		printf ("   ");
	}
}

void printSquare(struct Board *board, int col, int row) {
	int n = board->n;
	int m = board->m;
	int i = 0;
	int j = 0;
	int currRow;
	int curr_square_row;
	int currCol;
	int curr_square_col;
	currCol = ((col-1)/n)*n;
	currRow = ((row-1)/m)*m;
	printf("for %d < m=%d, %d < n=%d :\n", currRow, m, currCol, n);
	while (i < m) {
		j = 0;
		while (j < n) {
			curr_square_row = currRow + i;
			curr_square_col = currCol + j;
			printf("<%d,%d> ", curr_square_row, curr_square_col);
			printf("%d ", board->values[curr_square_row][curr_square_col]);
			j++;
		}
		printf("\n");
		i++;
	}
}



int isLegalInSquare(struct Board *board, int col, int row, int num) {
	int n = board->n;
	int m = board->m;
	int i = 0;
	int j = 0;
	int currRow;
	int curr_square_row;
	int currCol;
	int curr_square_col;
	/*printSquare(board, col, row);*/
	currCol = ((col-1)/n)*n;
	currRow = ((row-1)/m)*m;
	while (i < m) {
		j = 0;
		while (j < n) {
			curr_square_row = currRow + i;
			curr_square_col = currCol + j;
			if (board->values[curr_square_row][curr_square_col] == num) {
				return 0;
			}
			j++;
		}
		i++;
	}
	return 1;
}


int isLegalInSquarePrint(struct Board *board, int col, int row, int num) {
	int n = board->n;
		int m = board->m;
		int i = 0;
		int j = 0;
		int currRow;
		int curr_square_row;
		int currCol;
		int curr_square_col;
		currCol = ((col-1)/n)*n;
		currRow = ((row-1)/m)*m;
		while (i < m) {
			j = 0;
			while (j < n) {
				curr_square_row = currRow + i;
				curr_square_col = currCol + j;
				if (board->values[curr_square_row][curr_square_col] == num && curr_square_row!=(row-1) && curr_square_col!=(col-1) ) {
					return 0;
				}
				j++;
			}
			i++;
		}
		return 1;
}

int returnNumOfEmptyCells(struct Board *gameBoard) {
	int N = gameBoard->N;
	int i=0;
	int j;
	int counter = 0;
	while (i < N) {
		j = 0;
		while (j < N) {
			if (gameBoard->values[i][j] == 0) {
				counter++;
			}
			j++;
		}
		i++;
	}
	return counter;
}


int isLegalInRowCol(struct Board *board, int col, int row, int num) {
	int i = 0;
	int j = 0;
	int N = board->N;
	int currCol = col - 1;
	int currRow = row -1;
	while (i < N) {
		if (board->values[currRow][i] == num) {
			return 0;
		}
		i++;
	}
	while (j < N) {
		if (board->values[j][currCol] == num) {
			return 0;
		}
		j++;
	}
	return 1;
}

int isLegalInRowColPrint(struct Board *board, int col, int row, int num) {
	int i = 0;
	int j = 0;
	int N = board->N;
	int currCol = col - 1;
	int currRow = row - 1;
	while (i < N) {
		if (i!= currCol && board->values[currRow][i] == num) {
			return 0;
		}
		i++;
	}
	while (j < N) {
		if (j!= currRow && board->values[j][currCol] == num) {
			return 0;
		}
		j++;
	}
	return 1;
}

int isCellLegal(struct Board *board, int row, int col){
	int legalInSquare, legalRowCol, num;
	num = board->values[row][col];
	legalInSquare=1;
	legalRowCol=1;
	legalInSquare = isLegalInSquare(board, col+1, row+1, num);
	legalRowCol = isLegalInRowCol(board, col+1, row+1, num);
	if (legalInSquare && legalRowCol){
		return 1;
	}
	return 0;

}

int isCellLegalPrint(struct Board *board, int row, int col){
	int legalInSquare, legalRowCol, num;
	num = board->values[row][col];
	legalInSquare=1;
	legalRowCol=1;
	legalInSquare = isLegalInSquarePrint(board, col+1, row+1, num);
	legalRowCol = isLegalInRowColPrint(board, col+1, row+1, num);
	if (legalInSquare && legalRowCol){
		return 1;
	}
	return 0;

}

int freeBoardMemory(struct Board *board) {
	int i = 0;
	int N = board->N;
	for(i = 0; i<N; i++){
		free(board->values[i]);
		board->values[i] = NULL;
	}
	free(board->values);
	board->values = NULL;
	return 1;
}


void printCellsRow(struct Board *board, struct Board *fixedBoard, int row, int showErrors){
	int isErroneous = 0, col;
	printf("|");
	for (col = 0; col < board->N; col++){
		/*check if cell is erroneous*/
		if (board->values[row][col]==0){
			 isErroneous = 0;
		}
		else if (board->values[row][col] >= 1 && board->values[row][col] <= board->N){
			isErroneous = !(isCellLegalPrint(board, row, col));
		}

		else {
			 isErroneous = 1;
		}
		printCell(row, col, board->N, board->values, fixedBoard->values, showErrors, isErroneous);
		if ((col+1)%board->n == 0){
			printf("|");
		}
	}
	printf("\n");
}



/*prints board, marks fixed cells according to fixedBoard with a dot '.' */
/*and erroneous cells with asterisk '*' if showErrors == 1 or editMode ==1 */

void printBoard(struct Board *currBoard, struct Board *fixedBoard, int showErrors, int editMode){
	int rowOfBlocks, i, row = 0; /*up to n*/
	int showErrs = 0;
	separator(currBoard->N, currBoard->m);

	if (editMode==1||showErrors==1){
		showErrs = 1;
	}
	for (rowOfBlocks=0; rowOfBlocks < currBoard->n; rowOfBlocks++){
		for (i = 0; i<currBoard->m; i++){
			printCellsRow(currBoard, fixedBoard, row, showErrs);
			row ++;
		}
		separator(currBoard->N, currBoard->m);
	}
}

void funPrint(struct Board *board) {
	int i = 0;
	int j = 0;
	while (i < board->N) {
		j = 0;
		while (j < board->N) {
			printf("%d", board->values[i][j]);
			j++;
		}
		printf("\n");
		i++;
	}
}

int isBoardFull(struct Board *board) {
	int i = 0;
	int j = 0;
	int N = board->N;
	while (i < N) {
		j = 0;
		while (j < N) {
			if (board->values[i][j] == 0) {
				return 0;
			}
			j++;
		}
		i++;
	}
	return 1;
}


int isBoardIllegal(struct Board *board) {
	int N = board->N;
	int i = 0;
	int j = 0;
	int curr_num_to_check;
	int good_rowcol;
	int good_square;
	int IllegalFlag = 0;
	while (i < N) {
		j = 0;
		while (j < N) {
			curr_num_to_check = board->values[i][j];
			board->values[i][j] = 0;
			good_rowcol = isLegalInRowCol(board, j+1, i+1, curr_num_to_check);
			good_square = isLegalInSquare(board, j+1, i+1, curr_num_to_check);
			if (curr_num_to_check != 0 && (good_rowcol == 0 || good_square == 0)) {
				IllegalFlag = 1;
				board->values[i][j] = curr_num_to_check;
				return IllegalFlag;
			}
			board->values[i][j] = curr_num_to_check;
			j++;
		}
		i++;
	}
	return IllegalFlag;
}

int howManyValidForCell(struct Board *dummyBoard, struct Board *gameBoard, int row, int col) {
	int N = gameBoard->N;
	int counter = 0;
	int curr_num = 1;
	int last_num;
	while (curr_num <= N) {
		if (dummyBoard->values[row-1][col-1] == 0
				&&isLegalInRowCol(dummyBoard, col, row, curr_num) == 1
				&& isLegalInSquare(dummyBoard, col, row, curr_num) == 1) {
			last_num = curr_num;
			counter++;
		}
		curr_num ++;
	}
	if (counter == 1) {
		printf("Autofill: following change was made to cell <%d,%d>: %d -> %d\n", col, row,
				gameBoard->values[row-1][col-1], last_num);
		gameBoard->values[row-1][col-1] = last_num;
	}
	return 1;
}


int compareBoards(struct Board *board1, struct Board *board2) {
	int i = 0;
	int j = 0;
	int N = board1->N;
	while (i < N) {
		j = 0;
		while (j < N) {
			if (board1->values[i][j] != board2->values[i][j]) {
				return 1;
			}
			j++;
		}
		i++;
	}
	return 0;
}


