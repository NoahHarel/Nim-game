/*
 * Parser.h
 *
 *  Created on: Jun 10, 2019
 *      Author: Itamar
 */

#ifndef PARSER_H_
#define PARSER_H_

int isCommandLegal(char* command) ;
int isSetCommand(char command[]) ;
int extractFromSet(char command[], int answer[3]);
int applySet(int currSolution[9][9], int table[9][9], char command[]) ;
void setCommand(char* command, char* answer);
int isSetAnswerValid(char* answer) ;
int isHintCommand(char* command);
void applyHint(int solution[9][9], char command[]);
int isValidateCommand(char command[]);
void applyValidate(int currentBoard[9][9], int solution[9][9]);
int isRestartCommand(char command[]);
int isExitCommand(char command[]);
void applyExit();

#endif /* PARSER_H_ */

typedef int make_iso_compilers_extremely_happy;
