/*
 * Utilities.h
 *
 *  Created on: Aug 17, 2019
 *      Author: Itamar
 */

#ifndef UTILITIES_H_
#define UTILITIES_H_

struct actionList {
	struct actionList *next;
	struct actionList *prev;
	struct Board board;
};

void saveCurrentBoardToFile(struct Board *board, struct Board *originalBoard);

int loadBoardFromFile(struct Board *board, struct Board *originalBoard, char path[256]);

int loadTest(struct Board *board, struct Board *originalBoard);

int initializeUndoRedoList(struct actionList *list, struct Board board);

int setNextForList(struct actionList *listCurr, struct actionList *listNext);

int undo(struct actionList *listCurr);

int redo(struct actionList *listCurr);

void copyBoard(struct Board *board_src, struct Board *board_dst);

void copyBoardByPointer(struct Board *board_src, struct Board **board_dst);

int validateLoadCommandAndPath(char command[256]);

int parseSaveBoardCommandAndExec(char command[256], struct Board *board, struct Board *fixedBoard, int edit);

void saveBoardToFile(struct Board *board, struct Board *fixedBoard, int inEdit, char path[256]);

int applySolveWithLoad(char command[256], struct Board *board, struct Board *solutionBoard);

int applEdityWithLoad(char command[256], struct Board *board, struct Board *solutionBoard);

int LoadBoardProcedureEdit(char command[256], struct Board *board, struct Board *solutionBoard, int inGame);

int LoadBoardProcedureSolve(char command[256], struct Board *board, struct Board *solutionBoard, int inGame);

#endif /* UTILITIES_H_ */
