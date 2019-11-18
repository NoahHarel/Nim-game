#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "SPBufferset.h"
#include "Board.h"
#include "gurobi_c.h"
#include "Gurobi.h"
#include "Utilities.h"


/*struct for result board*/
typedef struct results {
	int** solved;
	int optimstatus;
	double objval;
}RESULTS;
RESULTS results;


void solver(struct Board *board){
	/*Create variables*/
	GRBenv  *env;
	GRBmodel *model;
	int error;
	int optimstatus, count,i,j,k, ig,jg;
	int** gurobiBoard;
	int** solved;
	double objval;
	int *ind;
	double *sol;
	double *val;
	double *lb;
	char *vtype;
	int N = board->N;
	int n = board->n; /*width*/
	int m = board->m; /*height*/
	ind =malloc(N * sizeof(int));
	sol = malloc(N*N*N* sizeof(double));
	val = malloc(N * sizeof(double));
	lb = malloc(N*N*N*sizeof(double));
	vtype = malloc(N * N * N * sizeof(char));
	results.solved = NULL;
	results.optimstatus = 0;
	results.objval = 0.0;

	/*Check for memory error*/
	if (ind == NULL || val == NULL || lb == NULL || vtype == NULL || sol == NULL) {
		memError();
		return;
	}

	env = NULL;
	model = NULL;
	error = 0;

	/*copy board into gurobiBoard*/
	gurobiBoard = boardMem(N);
	for (i=0;i<N;i++){
		for (j = 0; j < N; j++){
			if (board->values[i][j]==0){
				gurobiBoard[i][j] = -1;
			}
			else{
				gurobiBoard[i][j] = board->values[i][j]-1;
			}

		}
	}

	/*Set variables and lower bounds*/
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			for (k = 0; k < N; k++) {
				if (gurobiBoard[i][j] == k) {
					lb[i * N * N + j * N + k] = 1;
				}
				else {
					lb[i * N * N + j * N + k] = 0;
				}
				vtype[i * N * N + j * N +k] = GRB_BINARY;
			}
		}
	}

	/* Create environment */
	error = GRBloadenv(&env, "sudoku.log");
	if (error==1){
		quit(env, error);
	 }

	/*No log prints during optimizing*/
	error = GRBsetintparam(env, GRB_INT_PAR_LOGTOCONSOLE, 0);
	if (error == 1) {
		printf("Error in intParam: %d", error);
		quit(env, error);
	}

	/* Create new model */
	error = GRBnewmodel(env, &model, "sudoku", N*N*N, NULL, lb, NULL, vtype, NULL);
	if (error==1){
			quit(env, error);
	}

	/* Each cell gets a value */
	for (i = 0; i < N; i++) {
	  for (j = 0; j < N; j++) {
	    for (k = 0; k < N; k++) {
	      ind[k] = i*N*N + j*N + k;
	      val[k] = 1.0;
	    }

	    error = GRBaddconstr(model, N, ind, val, GRB_EQUAL, 1.0, NULL);

	    if (error==1){
	      quit(env, error);
	     }
	   }
	 }

	/* Each value must appear once in each row */
	for (k = 0; k < N; k++) {
	    for (j = 0; j < N; j++) {
	      for (i = 0; i < N; i++) {
	        ind[i] = i*N*N + j*N + k;
	        val[i] = 1.0;
	      }
	     error = GRBaddconstr(model, N, ind, val, GRB_EQUAL, 1.0, NULL);
	     if (error == 1) {
	    	 quit(env, error);
	     }

	    }
	}


	/* Each value must appear once in each column */
	 for (k = 0; k < N; k++) {
	   for (i = 0; i < N; i++) {
	     for (j = 0; j < N; j++) {
	       ind[j] = i*N*N + j*N + k;
	       val[j] = 1.0;
	     }
	     error = GRBaddconstr(model, N, ind, val, GRB_EQUAL, 1.0, NULL);
	     if (error == 1) {
	    	 quit(env, error);
	     }
	   }
	 }


  /* Each value must appear once in each box */
	 for (k = 0; k < N; k++) {
	 		for (ig = 0; ig < n; ig++) {
	 			for (jg = 0; jg < m; jg++) {
	 				count = 0;
	 				for (i = ig * m; i < (ig + 1)*m; i++) {
	 					for (j = jg * n; j < (jg + 1)*n; j++) {
	 						ind[count] = i*N*N + j*N + k;
	 						val[count] = 1.0;
	 						count++;
	 					}
	 				}
	 			error = GRBaddconstr(model, N, ind, val, GRB_EQUAL, 1.0, NULL);
	 			if (error == 1) {
	 				quit(env, error);
	 			}
	 		}
	 	}
	 }



	 /* Optimize model */
	 error = GRBoptimize(model);
	 if (error == 1) {
	 	 quit(env, error);
	 }

	 /* Capture solution information */
	 error = GRBgetintattr(model, GRB_INT_ATTR_STATUS, &optimstatus);
	 if (error == 1) {
	 	 quit(env, error);
	 	}

	 if (optimstatus == GRB_OPTIMAL) {
	 	error = GRBgetdblattr(model, GRB_DBL_ATTR_OBJVAL, &objval);
	 	if (error == 1) {
	 		 	 quit(env, error);
	 		 	 }
		error = GRBgetdblattrarray(model, GRB_DBL_ATTR_X, 0, N*N*N, sol);
		if (error == 1) {
			quit(env, error);
			}
		/*copy solved board to results*/
		i=0;
		j=0;
		k=0;
		solved = boardMem(N);
		for (i = 0; i < N; i++) {
	 		for (j = 0; j < N; j++) {
	 			for (k = 0; k < N; k++) {
	 				if (sol[i*N* N + j * N + k] == 1) {
	 					solved[i][j] = k + 1;
	 				}
	 			}
	 		}
	 	}
		results.solved = solved;
		results.optimstatus = optimstatus;
		results.objval = objval;
	 }/*optimal ends*/

	/*Free model */
	GRBfreemodel(model);
	/* Free environment */
	GRBfreeenv(env);
	free(sol);
	free(ind);
	free(lb);
	free(val);
	free(vtype);
	i=0;
		if (gurobiBoard != NULL) {
			for (i = 0; i < board->N; i++) {
				if (gurobiBoard[i] != NULL) {
					free(gurobiBoard[i]);
				}
			}
		free(gurobiBoard);
		}

}


/*returns 1 if valid (exists a solution), 0 otherwise*/
int validate(struct Board *board) { /*call it after checking that the board is not erroneous (if it is, error message and don't call validate)*/
	int i, j;
	struct Board validateBoard;
	setDimensions(&validateBoard, board->n,board->m);
/*copy board to values*/
		for (i = 0; i < board->N; i++) {
				for (j = 0; j < board->N; j++) {
	/*				if (board->values[i][j] == 0) {
						validateBoard.values[i][j] = 0;
					}
					else {*/
						validateBoard.values[i][j] = board->values[i][j];
				/*	}*/
				}
			}
/*solver on the copied board*/
		solver(&validateBoard);


	if (results.optimstatus == GRB_OPTIMAL) {
		/*free results.solved and validateBoard.values*/
		if (results.solved != NULL) {
			for (i = 0; i < board->N; i++) {
				if (results.solved[i] != NULL) {
					free(results.solved[i]);
						}
					}
			free(results.solved);
		}

		if (validateBoard.values != NULL){
			for (i = 0; i < board->N; i++) {
						if (validateBoard.values[i] != NULL) {
							free(validateBoard.values[i]);
								}
							}
			if (validateBoard.values != NULL){
					free(validateBoard.values);
			}
		}
		return 1;
		}

		else {
			/*free results.solved and validateBoard.values*/
			if (results.solved != NULL) {
				for (i = 0; i < board->N; i++) {
					if (results.solved[i] != NULL) {
						free(results.solved[i]);
						}
					else{
					}
				}
			}

			if (results.solved != NULL){
				free(results.solved);
			}

			if (validateBoard.values != NULL){
			for (i = 0; i < board->N; i++) {
						if (validateBoard.values[i] != NULL) {
							free(validateBoard.values[i]);
								}
							}
					free(validateBoard.values);
			}
			return 0;
		}
}





int generate(int x, int y, struct Board *board) { /*receives a fresh board, fill x cells, clear all but y cells*/
	int isSolved, i, j;
	struct Board copy;
	setDimensions(&copy, board->n, board->m);
	isSolved = 0;
	copyBoard(board, &copy);

	if (createRand(copy.values, x ,board->N,board->n,board->m) != NULL) { /*works fine*/
			solver(&copy);
		if (results.optimstatus == GRB_OPTIMAL) {

			clearAllBut(results.solved, y ,board->N);
			i=0;
			j=0;

			for (i = 0; i < board->N; i++) {
					for (j = 0; j < board->N; j++) {
						board->values[i][j] = results.solved[i][j];
					}
			}


			isSolved = 1;
		}
	} /*end if rand was good*/

	i=0;
	/*free results.solved*/
	if (results.solved != NULL) {
		for (i = 0; i < board->N; i++) {
			if (results.solved[i] != NULL) {
				free(results.solved[i]);
			}
		}
		if (results.solved != NULL) {

		free(results.solved);
		}
	}

	/*free copyBoard*/
	if (copy.values != NULL) {
			for (i = 0; i < board->N; i++) {
				if (copy.values[i] != NULL) {
					free(copy.values[i]);
				}
			}
			if (copy.values != NULL) {

		free(copy.values);
			}
	}

	return isSolved;
}


int hint(int x, int y, struct Board *board){ /*doesn't check if board is erroneous or cell is fixed. just runs ILP. give it x-1,y-1 already calculated (0 based) */
	int val=0;
	int i=0;
	solver(board);
	if (results.optimstatus == GRB_OPTIMAL) {
		val = results.solved[x][y];
	/*free results.solved*/
		if (results.solved != NULL) {
			for (i = 0; i < board->N; i++) {
				if (results.solved[i] != NULL) {
					free(results.solved[i]);
					}
				}
			free(results.solved);
			}
			return val;
		}
		else {
			/*free results.solved*/
			if (results.solved != NULL) {
				for (i = 0; i < board->N; i++) {
					if (results.solved[i] != NULL) {
						free(results.solved[i]);
					}
				}
			free(results.solved);
			}
			return 0;
		}
	return 0;
}

int** boardMem(int N) {
	int i;
	void* temp;
	int** allocated;
	temp = (malloc((sizeof(int*)) * N));
	if (temp == NULL) {
		memError();
		return 0;
	}
	else {
		allocated = (int**)temp;
	}
	for (i = 0; i < N; i++) {
		temp = malloc(sizeof(int)*N);
		if (temp == NULL) {
			memError();
			return 0;
		}
		allocated[i] = (int*)temp;
	}
	return allocated;
}

void quit(GRBenv *env, int error) {
	if (error==1) {
		printf("ERROR: %s\n", GRBgeterrormsg(env));
		exit(1);
	}
}
void memError() {
	printf("Error: Memory allocation failed\n");
	printf("Exiting");
	exit(0);
}

int** createRand(int** values,int x, int N,int n,int m) {
	int row, col, num, count, i, j, decided;
	int* choice;
	j = 0;
	while(j < x){
		row = rand() % N;
		col = rand() % N;
		decided = 0;
		if (values[row][col] == 0) {
			choice = malloc(N * sizeof(int));
			for (i = 0; i < N; i++) {
				choice[i] = 1;
			}
			for (count = 0; count < N; count++) {
				do {
					num = rand() % N + 1;
				} while (choice[num - 1] == 0);

				if (isValid(values,row,col,num,N,n,m)==1) {
					values[row][col] = num;
					j++;
					decided = 1;
					break;
				}
				else {
					choice[num - 1] = 0;
				}
			}
			/*free(choice);*/
			if (decided!=1) {
				return NULL;
			}
		}
	}
	return values;
}

int isValid(int** values, int row, int col, int num, int N, int n, int m) {
	int i;
	if (validBlock(values,row,col,num,m,n)!=1) {
		return 0;	}
	for (i = 0; i < N; i++) {
		if (values[row][i] == num) {
			return 0;		}
	}
	for (i = 0; i < N; i++) {
		if (values[i][col] == num) {
			return 0;		}
	}
	return 1;
}

int** clearAllBut(int** values, int y, int N) {
	int row;
	int col;
	int delete;
	int i=0;
	delete = N * N - y;
	while (i<delete){
		row = rand()%N;
		col = rand()%N;
		if (values[row][col] != 0 && values[row][col]!=-1) {
			values[row][col] = 0;
			i++;
		}
	}
	return values;
}

int validBlock(int** values, int row, int col, int num,int m, int n) {
	int i, j, blockRows, blockCols;
	blockRows = row/m; /*m=height*/
	blockCols = col/n; /*n = width*/
	for (i = (m*blockRows); i < m*(blockRows + 1); i++) {
		for (j = (n*blockCols); j < n*(blockCols + 1); j++) {
			if (values[i][j] == num) {
				return 0;
			}
		}
	}
	return 1;

}

int solverLP(struct Board *board, double ***valueProbability){
	GRBenv  *env;
	GRBmodel *model;
	int optimstatus,i,j,k, possibilities;
	int *possiblesCell;
	int res;
	int modelSet = 0, cellConstraintsAdded = 0, probConstraintsAdded = 0, modelDone = 0;
	double *sol;
	double *obj;
	char *vtype;
	int N = board->N;
	possibilities = N*N*N;
	possiblesCell = (int*)calloc(possibilities, sizeof(int));

	for(i=0; i<N; i++){
	    for(j=0; j<N; j++){
	        if(board->values[i][j] != 0 && board->values[i][j] != -1){
				impossiblesCell(board,i,j,possiblesCell);
				impossiblesRow(board,i,board->values[i][j]-1, possiblesCell);
				impossiblesCol(board,j,board->values[i][j]-1, possiblesCell);
				impossiblesBlock(board,i,j,board->values[i][j]-1,possiblesCell);
	        }
	     }
	 }

	possibilities = 0;
	    for(i=0; i<board->N; i++){
	        for(j=0; j<board->N; j++){
	            for(k=0; k<board->N; k++){
	                if(possiblesCell[i*board->N*board->N + j*board->N + k] != -1){
	                	possiblesCell[i*board->N*board->N + j*board->N + k] = possibilities;
	                	possibilities++;
	                }
	            }
	        }
	    }

	  sol = (double*)malloc(sizeof(double) * possibilities);
	  obj = (double*)malloc(sizeof(double) * possibilities);
	  vtype = (char*)malloc(sizeof(char) * possibilities);

	  for(i=0; i<possibilities; i++){
	         obj[i] = rand() % (board->N) + 1;
	         vtype[i] = GRB_CONTINUOUS;
	     }

	env = NULL;
	model = NULL;

	modelSet = setModel(&env, &model, possibilities, obj, vtype);
	cellConstraintsAdded = cellConstraints(env, model, possiblesCell, board);
	probConstraintsAdded = probabilityConstraints(env, model, possiblesCell, board);
	modelDone = startModel(env, model, possibilities, &optimstatus, sol);

	if (modelSet == 1 && cellConstraintsAdded == 1 && probConstraintsAdded ==1 && modelDone==1 ){
		res=1;
	}
	if(res==1 && optimstatus == GRB_OPTIMAL){
	   getValueProbabilities(board, valueProbability, sol, possiblesCell);
	    }
	else {
	   res= 0;
	    }
	free(sol);
	free(obj);
	free(vtype);
	free(possiblesCell);
	GRBfreemodel(model);
	GRBfreeenv(env);
	return res;
}

int startModel(GRBenv *env, GRBmodel *model, int possibilities, int *optimstatus, double *sol){
    int error=0;
    int optimized = 0, written = 0, getInt = 0, getDbl = 0;
    optimized = GRBoptimize(model);
    written = GRBwrite(model, "sudoku.lp");
    getInt = GRBgetintattr(model, GRB_INT_ATTR_STATUS, optimstatus);
    getDbl = GRBgetdblattrarray(model, GRB_DBL_ATTR_X, 0,possibilities, sol);
    if (optimized==1||written==1||getInt==1||getDbl==1){
    	error =1;
    }

    if(error && *optimstatus != GRB_INF_OR_UNBD && *optimstatus!= GRB_INFEASIBLE ){
        printf("ERROR %d occurred when executing model: %s\n", error, GRBgeterrormsg(env));
        return  0;
    }
    return 1;
}

int probabilityConstraints(GRBenv *env, GRBmodel *model, int* possiblesCell, struct Board *board){
    int i,j,k, error, ind[1];
    double val[1] = {1};
    for(i=0; i < board->N; i++){
        for(j=0; j<board->N; j++){
            for(k=0; k<board->N; k++){
                if(possiblesCell[i*board->N*board->N + j*board->N + k] != -1){
                    ind[0] = possiblesCell[i*board->N*board->N + j*board->N + k];
                    if( (error = GRBaddconstr(model, 1, ind, val, GRB_GREATER_EQUAL, 0, NULL))
                    || (error = GRBaddconstr(model, 1, ind, val, GRB_LESS_EQUAL, 1, NULL))){
                        printf("ERROR %d occurred when adding probability constraint(): %s\n", error, GRBgeterrormsg(env));
                        return 0;
                    }
                }
            }
        }
    }
    return 1;
}

int setModel(GRBenv **env, GRBmodel **model, int possibilities, double *obj, char *vtype){
    int error;
    error = GRBloadenv(env, "sudoku.log")
            || GRBsetintparam(*env, GRB_INT_PAR_LOGTOCONSOLE, 0)
            || GRBnewmodel(*env, model, "sudoku", 0, NULL, NULL, NULL, NULL, NULL)
            || GRBaddvars(*model, possibilities, 0, NULL, NULL, NULL, obj, NULL, NULL, vtype, NULL)
            || GRBsetintattr(*model, GRB_INT_ATTR_MODELSENSE, GRB_MAXIMIZE)
            || GRBupdatemodel(*model);
    if(error)
    {
        printf("ERROR %d occurred when setting the model: %s\n", error, GRBgeterrormsg(*env));
        return  0;
    }

    return 1;
}

int cellConstraints(GRBenv *env, GRBmodel *model, int* possiblesCell, struct Board *board){
    int *ind, i;
    double *val;
    int result;

    val = (double*)malloc(sizeof(double) * board->N);
    ind = (int*)malloc(sizeof(int) * board->N);

    for(i=0; i< board->N; i++)
        val[i] = 1.0;

    result = cellValuesConstr(env, model, possiblesCell, ind, val, board)
    && colValuesConstr(env, model, possiblesCell, ind, val, board)
    && rowValuesConstr(env, model, possiblesCell, ind, val, board)
    && blockValuesConstr(env, model, possiblesCell, ind, val, board);

    free(val);
    free(ind);
    return result;
}

int cellValuesConstr(GRBenv *env, GRBmodel *model, int* possiblesCell, int *ind, double *val, struct Board *board){
    int count, i, j, k, error;
    for(i=0; i < board->N; i++){
        for(j=0; j < board->N; j++){
            count = 0;
            for (k = 0; k < board->N; k++) {
                if(possiblesCell[i*board->N*board->N + j*board->N + k] != -1){
                    ind[count] = possiblesCell[i*board->N*board->N + j*board->N + k];
                    count++;
                }
            }
            if(count > 0 && (error = GRBaddconstr(model, count, ind, val, GRB_EQUAL, 1, NULL))){
                printf("ERROR %d occurred when adding cell constraint(): %s\n", error, GRBgeterrormsg(env));
                return 0;
            }
        }
    }
    return 1;
}

int rowValuesConstr(GRBenv *env, GRBmodel *model, int* possiblesCell, int *ind, double *val, struct Board *board){
    int count, i, j, k, error;
    for(k=0; k < board->N; k++){
        for (j = 0; j < board->N; j++){
            count = 0;
            for(i=0; i < board->N; i++) {
                if(possiblesCell[i*board->N*board->N + j*board->N + k] != -1){
                    ind[count] = possiblesCell[i*board->N*board->N + j*board->N + k];
                    count++;
                }
            }
            if(count > 0 && (error = GRBaddconstr(model, count, ind, val, GRB_EQUAL, 1, NULL))){
                printf("ERROR %d occurred when adding row constraint(): %s\n", error, GRBgeterrormsg(env));
                return 0;
            }
        }
    }
    return 1;
}



int blockValuesConstr(GRBenv *env, GRBmodel *model, int* possiblesCell, int *ind, double *val, struct Board *board){
    int count, i, j, k, t,p, error;

    for(k=0; k < board->N; k++){ /*for each value*/
        for (i = 0; i < board->n; i++) { /*iterate on sub grids by rows*/
            for (j = 0; j < board->m; j++) { /*iterate on sub grid by  cols*/
                count = 0;
                for (t = i * board->m; t < (i + 1) * board->m; t++) {
                    for (p = j * board->n; p < (j + 1) * board->n; p++) {
                        if (possiblesCell[t*board->N*board->N + p*board->N + k] != -1) {
                            ind[count] = possiblesCell[t*board->N*board->N + p*board->N + k];
                            count++;
                        }
                    }
                }
                if(count > 0 && (error = GRBaddconstr(model, count, ind, val, GRB_EQUAL, 1, NULL))){
                    printf("ERROR %d occurred when adding block constraint(): %s\n", error, GRBgeterrormsg(env));
                    return 0;
                }
            }
        }
    }
    return 1;
}


int colValuesConstr(GRBenv *env, GRBmodel *model, int* possiblesCell, int *ind, double *val, struct Board *board){
    int count, i, j, k, error;
    for(i=0; i < board->N; i++){
        for(k=0; k < board->N; k++){
            count=0;
            for (j = 0; j < board->N; j++){
                if(possiblesCell[i*board->N*board->N + j*board->N + k] != -1){
                    ind[count] = possiblesCell[i*board->N*board->N + j*board->N + k];
                    count++;
                }
            }
            if(count > 0 && (error = GRBaddconstr(model, count, ind, val, GRB_EQUAL, 1, NULL))){
                printf("ERROR %d occurred when adding column constraint(): %s\n", error, GRBgeterrormsg(env));
                return 0;
            }
        }
    }
    return 1;
}

void impossiblesCell(struct Board *board, int row, int col, int *possiblesCell){
    int val;
    for(val = 0; val < board->N; val++){
    	possiblesCell[row*board->N*board->N + col*board->N + val] = -1;
    }
}

void impossiblesRow(struct Board *board, int row, int val, int *possiblesCell){
    int col;
    for(col = 0; col < board->N; col++){
    	possiblesCell[row*board->N*board->N + col*board->N + val] = -1 ;
    }
}

void impossiblesCol(struct Board *board, int col, int val, int *possiblesCell){
    int row;
    for(row = 0; row < board->N; row++)
    	possiblesCell[row*board->N*board->N + col*board->N + val] = -1;
}

void impossiblesBlock(struct Board *board, int row, int col, int val, int *possiblesCell){
    int i, j;
    for(i = (row/board->m) * board->m; i < (row/board->m) * board->m + board->m; i++){
        for(j=(col/board->n) * board->n; j < (col/board->n) * board->n + board->n; j++){
        	possiblesCell[i*board->N*board->N + j*board->N + val] = -1;
        }
    }
}

void getValueProbabilities(struct Board *board, double ***valueProbability, double *solutions, int *possiblesCell){
    int i,j,k;
    for(i=0; i<board->N; i++){
        for(j=0; j<board->N; j++){
            for(k=0; k<board->N; k++){
                if(possiblesCell[i*board->N*board->N + j*board->N + k] == -1)
                    valueProbability[i][j][k] = 0;
                else
                    valueProbability[i][j][k] = solutions[possiblesCell[i*board->N*board->N + j*board->N + k]];
            }
            if(board->values[i][j] != 0 && board->values[i][j] != -1)
                valueProbability[i][j][board->values[i][j]-1] = 1;
        }
    }
}
int guess(struct Board *board,float x){
	double ***probabilities;
	int hinted,i,j;
	struct Board copy;
	setDimensions(&copy, board->n, board->m);
	copyBoard(board, &copy);
	probabilities = (double ***) malloc(board->N * sizeof(double **));
	    for (i = 0; i < board->N; i++) {
	    	probabilities[i] = (double **) malloc(board->N * sizeof(double *));
	        for (j = 0; j < board->N; j++) {
	        	probabilities[i][j] = (double *) malloc(board->N * sizeof(double));
	        }
	    }

	if (solverLP(board, probabilities)==0) {
	     hinted = 0;
	}

	else {
	        probThresholdX(&copy,x, probabilities);
	        copyBoard(&copy, board);
	        hinted = 1;
	    }
	/*free copyBoard*/
	if (copy.values != NULL) {
		for (i = 0; i < board->N; i++) {
			if (copy.values[i] != NULL) {
				free(copy.values[i]);
			}
		}
	free(copy.values);
	}

	/*free probabilities board*/
	if (probabilities != NULL) {
		for (i = 0; i < board->N; i++) {
			if (probabilities[i] != NULL) {
				free(probabilities[i]);
			}
		}
	free(probabilities);
	}

	return hinted;
}

void probThresholdX(struct Board *board, float x, double ***probabilities) {
    int i, j, val;
    for (i = 0; i <board->N; i++) {
        for (j = 0; j < board->N; j++) {
            if (board->values[i][j] != 0 && board->values[i][j]!=-1) continue;
            val = possibleVal(board, x, probabilities[i][j]);
            if (val > 0) {
                board->values[i][j] = val;
                if (isBoardIllegal(board)==1) {
                	board->values[i][j] = 0;
                }
            }
        }
    }
}

double possibleVal(struct Board *board, float x, double *probabilities) {
    int i, count;
    double probability, countProbability;
    do {
        count = 0;
        countProbability = 0;
        probability = (double) (rand() % 100) / 100.0;
        for (i = 0; i < board->N; i++) {
            if (probabilities[i] >= x) {
                ++count;
                if (probability > countProbability && probability <= countProbability + probabilities[i])
                    return i + 1;
                countProbability += probabilities[i];
            }
        }
    } while (count > 0);
    return -1.0;
}

int guessHint(struct Board *board, int x, int y){/*give it already calculated row-1, col-1*/
	double ***probabilities;
	int res=0,i,j;
	 if ((x<0)||(x>board->N-1)||(y<0)||(y>board->N-1)) {
		 res=0;
	    } else if (isBoardIllegal(board)==1) {
	    	 res=0;
	    } else if (board->values[x][y]!= -1 && board->values[x][y]!=0) {
	    	 res=0;
	    } else {

	        probabilities = (double ***) malloc(board->N * sizeof(double **));
		    for (i = 0; i < board->N; i++) {
		    	probabilities[i] = (double **) malloc(board->N * sizeof(double *));
		        for (j = 0; j < board->N; j++) {
		        	probabilities[i][j] = (double *) malloc(board->N * sizeof(double));
		        }
		    }
	        if ((res = solverLP(board, probabilities))==1) {
	            printf("Hint: ");
	            for (i = 0; i < board->N; i++) {
	                if (probabilities[x][y][i] > 0) {
	                    printf("%d : %.3f \t", i + 1, probabilities[x][y][i]);
	                }
	            }
	            printf("\n");
	        }
	        /*free probabilities board*/
	               if (probabilities != NULL) {
	               	for (i = 0; i < board->N; i++) {
	               		if (probabilities[i] != NULL) {
	               			free(probabilities[i]);
	               		}
	               	}
	               free(probabilities);
	               }
	    }
    return res;
}



