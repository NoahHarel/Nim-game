#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "SPBufferset.h"
#include "sp_nim.h"
#include "main_aux.h"
int numOfHeaps();
int heapSizes(int *sizes, int length);
int heapEmpty(int *sizes, int length);
void printSizes(int *sizes, int length, int turn);
void userTurn(int *sizes, int length);
void computerTurn(int *sizes, int length);
#define MAXARRAY 32
/*functions:*/
/*--------------------------------------------------------------------------------------*/
int main(){ /*main function*/
int heaps, heapArr[MAXARRAY], negativeHeap, isEmpty, turn;
SP_BUFF_SET();
/*get number of heaps*/
heaps = numOfHeaps();
if (heaps==-1){
    exit(0);
	}
/*get heap sizes*/
negativeHeap = heapSizes(heapArr, heaps);
if (negativeHeap <0){
	exit(0);
	}
/*turn starts*/
turn = 1;
/* check if heaps are empty:*/
isEmpty = heapEmpty(heapArr, heaps);
if (isEmpty == 1){
	if (turn%2==1){ /*user turn*/
		printf("You win!\n");
		}
	else{ /*computer turn*/
		printf("Computer wins!\n");
		}
	exit(0);
	}
/*heaps are not empty*/
while (isEmpty != 1){/*while heaps are not empty - if turn is even play user turn else if odd play computer turn */
	/*print heap sizes*/
	printSizes(heapArr, heaps, turn);
	/*play turn*/
	if (turn%2==1){ /*play user turn*/
		userTurn(heapArr, heaps);
		/*check if heaps are empty*/
		isEmpty = heapEmpty(heapArr, heaps);
			if (isEmpty == 1){
				printf("You win!\n");
				exit(0);
				}
			}
		else{ /*play computer turn*/
			computerTurn(heapArr, heaps);
/* ............................check if heaps are empty.............................................*/
			isEmpty = heapEmpty(heapArr, heaps); /* check if heaps are empty:*/
						if (isEmpty == 1){
							printf("Computer wins!\n");
							exit(0);
						}
			}
	turn++;
} /*end while turn*/
return 0;
/*end main function*/
}
