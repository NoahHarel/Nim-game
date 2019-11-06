#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "SPBufferset.h"
#include "sp_nim.h"
int numOfHeaps(){

      int heaps=-1;
      SP_BUFF_SET();
      printf("Enter the number of heaps:\n");
      fflush(stdout);
      if (scanf("%d",&heaps)!=1 || heaps<1 || heaps>32){
    	  if (feof(stdin)){
    		exit(0);
    	  }
      	printf("Error: the number of heaps must be between 1 and 32.\n");
      	fflush(stdout);
        exit(0);
      }
        return heaps;
    }
/*----------------------------------------------------------------------------------------*/

int heapSizes(int *sizes, int length){
	int i;
	 printf("Enter the heap sizes:\n");
	      fflush(stdout);
	      for (i=0;i<length;i++) {
	    	  if (scanf("%d", &sizes[i])!=1 ||sizes[i]<=0){
	    		  if (feof(stdin)){
	    		     		exit(0);
	    		     	  }
	    		  printf("Error: the size of heap %d should be positive.\n",i+1);
	    		  return -1;
	    	  }
	      }
	      return 0;
    }
/*----------------------------------------------------------------------------------------*/
int heapEmpty(int *sizes, int length){
	int j, isEmpty = 1;
	for (j=0;j<length;j++) {
		   if (sizes[j]>0){
			   isEmpty = 0;
			   return isEmpty;
		   }
		}
	return isEmpty;
}
/*----------------------------------------------------------------------------------------*/
void userTurn(int *sizes, int length){ /*user turn*/
	int heapNum, objNum;
	 printf("Your turn: please enter the heap index and the number of removed objects:\n");

	 if(scanf("%d%d", &heapNum,&objNum)!=2 || (heapNum!=0 && heapNum/heapNum!=1)|| (objNum!=0 && objNum/objNum!=1) ){ /*input invalid*/
		 if (feof(stdin)){
		    		exit(0);
		    	  }
		 printf("Error: Invalid input.\n");
		 exit(0);
	 }
	 /*Input is valid. Check if legal*/
	 while (heapNum>length || heapNum<1||objNum<1 || objNum>sizes[heapNum-1]){ /*input valid but illegal*/
		 printf("ERROR: Invalid input.\nPlease enter again the heap index and the number of removed objects:\n");

		if(scanf("%d%d", &heapNum,&objNum)!=2){ /*input invalid*/
			 if (feof(stdin)){
			    		exit(0);
			    	  }
		 		 printf("Error: Invalid input.\n");
		 		 exit(0);
		 }

	 }
	 /*If input is valid and legal*/
	 sizes[heapNum-1]-=objNum;
	 printf("You take %d objects from heap %d.\n",objNum,heapNum);
}
/*----------------------------------------------------------------------------------------*/
void computerTurn(int *sizes, int length){
	int objNum, heapNum, nim, i, winner;
	/*Computer's strategy*/
	nim = nimSum(sizes, length);
	if (nim!=0){
		winner = winnerHeap(sizes,length);
		objNum = sizes[winner]-heapNimSum(sizes, length, winner);
		heapNum = winner;
	}
	else{
		for (i=0;i<length;i++){
			if (sizes[i]>0){
				heapNum=i;
				break;
			}
		}
		objNum = 1;
	}

	sizes[heapNum]-=objNum;
	printf("Computer takes %d objects from heap %d.\n",objNum, heapNum+1);

}
/*----------------------------------------------------------------------------------------*/
void printSizes(int *sizes, int length, int turn){
	int t;
	printf("In turn %d heap sizes are: ",turn);
	for (t=0;t<length;t++) {
		if (t == length-1){
		printf("h%d=%d.\n",t+1,sizes[t]);
		}
		else{
		printf("h%d=%d ",t+1,sizes[t]);
			}
	}

}
/*----------------------------------------------------------------------------------------*/

