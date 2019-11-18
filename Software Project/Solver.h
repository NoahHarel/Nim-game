/*
 * Solver.h
 *
 *  Created on: Jun 10, 2019
 *      Author: Itamar
 */

#ifndef SOLVER_H_
#define SOLVER_H_

int isFull(int sudo[9][9]);
int* findEmpty(int sudo[9][9], int prevRow, int rowcol[2]);
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

#endif /* SOLVER_H_ */

typedef int make_iso_compilers_really_happy;
