/*
 * Game.h
 *
 *  Created on: Aug 25, 2019
 *      Author: Itamar
 */

#ifndef GAME_H_
#define GAME_H_
#include "Board.h"
int findCommandLen(char command[256]);
int findCommandStart(char command[256]);
int isLegalInSquare(struct Board *board, int row, int col, int num);
int isLegalInRowCol(struct Board *board, int row, int col, int num);
int isSetCommand(char command[256]);
int validateSetCommand(char command[256], struct Board *board, struct Board *fixedBoard, int solve);
int returnNumOfEmptyCells(struct Board *gameBoard);
int isEditCommand(char command[256]);
int isUndoCommand(char command[256]);
int isRedoCommand(char command[256]);
int isResetCommand(char command[256]);
int isSaveCommand(char command[256]);
int isSolveCommand(char command[256]);
int isAutofillCommand(char command[256]);
int isExitCommand(char command[256]);
int isGuessCommand(char command[256]);
int isGuessHintCommand(char command[256]);
float parse_guess_command(char command[256], struct Board *board);
int isGenerateCommand(char command[256]);
int isHintCommand(char command[256]);
int parse_hint_command(char command[256], int xy[2], int N);
void parse_generate_command(char command[256], int xy[2], struct Board board);
int autoFill(struct Board *gameBoard);
int findLenOfCommand(char command[512]);
int isMark_ErrorsCommand(char command[256]);
int getMarkErrorsValue(char command[256], int prev_mark);
int commandLenVerifier(char command[512]);
void copyFromInputToCommand(char src[512], char dst[256]);
int isPrintBoardCommand(char command[256]);
int isNum(char *str);
int isNumSolutionCommand(char command[256]);
void copyCommand(char src[256], char dst[256]);
int isValidateCommand(char command[256]);
int checkNoParam(char command[256]);

#endif /* GAME_H_ */
