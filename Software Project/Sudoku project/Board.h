/*
 * Board.h
 *
 *  Created on: Aug 17, 2019
 *      Author: Itamar
 */

#ifndef BOARD_H_
#define BOARD_H_


typedef struct Board{
	int m; /*columns of blocks, rows of cells*/
	int n; /*rows of blocks, columns of cells*/
	int N; /* N=mXn */
	int **values;
} Board ;

void setDimensions(struct Board *board, int n, int m);

void fillWithZeros(struct Board *board, int N);

void separator(int N, int m);

int isLegalInSquare(struct Board *board, int col, int row, int num);

int returnNumOfEmptyCells(struct Board *gameBoard);

int isLegalInRowCol(struct Board *board, int col, int row, int num);

void printCell(int row, int col, int N, int** values1, int** values2, int showErrors, int isErroneous);

int isCellLegal(struct Board *board, int row, int col);

void printCellsRow(struct Board *board, struct Board *fixedBoard, int row, int showErrors);

void printBoard(struct Board *currBoard, struct Board *fixedBoard, int showErrors, int editMode);

void funPrint(struct Board *board);

int isBoardFull(struct Board *board);

int isBoardIllegal(struct Board *board);

int howManyValidForCell(struct Board *dummyBoard, struct Board *gameBoard, int row, int col);

int freeBoardMemory(struct Board *board);

int compareBoards(struct Board *board1, struct Board *board2);

int isCellLegalPrint(struct Board *board, int row, int col);

int isLegalInSquarePrint(struct Board *board, int row, int col, int num);

int isLegalInRowColPrint(struct Board *board, int row, int col, int num);


#endif /* BOARD_H_ */
