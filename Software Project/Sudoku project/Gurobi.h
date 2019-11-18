#ifndef GUROBI_H_
#define GUROBI_H_
#include "time.h"
#include "gurobi_c.h"

int** clearAllBut(int** values, int y, int N);
void solver(struct Board *board);
int** boardMem(int N);
void quit(GRBenv *env, int error);
void memError();
int isValid(int** values, int row, int col, int num, int N, int n, int m);
int validBlock(int** values, int row, int col, int num,int m, int n);
int** createRand(int** values,int x, int N,int n,int m);
int hint(int x, int y, struct Board *board);
int generate(int x, int y, struct Board *board);
int validate(struct Board *board);
int solverLP(struct Board *board, double ***valueProbability);
void impossiblesCell(struct Board *board, int row, int col, int *possiblesCell);
void impossiblesRow(struct Board *board, int row, int val, int *possiblesCell);
void impossiblesCol(struct Board *board, int col, int val, int *possiblesCell);
void impossiblesBlock(struct Board *board, int row, int col, int val, int *possiblesCell);
int guess(struct Board *board, float x);
void probThresholdX(struct Board *board, float x, double ***probabilities);
double possibleVal(struct Board *board, float x, double *probabilities);
int guessHint(struct Board *board, int row, int col);
void getValueProbabilities(struct Board *board, double ***valueProbability, double *solutions, int *possiblesCell);
int colValuesConstr(GRBenv *env, GRBmodel *model, int* possiblesCell, int *ind, double *val, struct Board *board);
int blockValuesConstr(GRBenv *env, GRBmodel *model, int* possiblesCell, int *ind, double *val, struct Board *board);
int rowValuesConstr(GRBenv *env, GRBmodel *model, int* possiblesCell, int *ind, double *val, struct Board *board);
int cellValuesConstr(GRBenv *env, GRBmodel *model, int* possiblesCell, int *ind, double *val, struct Board *board);
int cellConstraints(GRBenv *env, GRBmodel *model, int* possiblesCell, struct Board *board);
int setModel(GRBenv **env, GRBmodel **model, int possibilities, double *obj, char *vtype);
int probabilityConstraints(GRBenv *env, GRBmodel *model, int* possiblesCell, struct Board *board);
int startModel(GRBenv *env, GRBmodel *model, int possibilities, int *optimstatus, double *sol);






#endif /* GUROBI_H_ */
