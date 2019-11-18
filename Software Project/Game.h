/*
 * Game.h
 *
 *  Created on: Jun 10, 2019
 *      Author: Itamar
 */

#ifndef GAME_H_
#define GAME_H_

int receiveNumber();
char* getCommand();
void printBoard(int fixedBoard[9][9], int currBoard[9][9]);
void separator();
void reassignCommand(char command[1024]);



#endif /* GAME_H_ */
typedef int make_iso_compilers_happier;
