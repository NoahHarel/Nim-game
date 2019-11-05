int numOfHeaps(); /*gets user input for number of heaps*/
int heapSizes(int *sizes, int length); /*gets user input for sizes of heaps*/
int heapEmpty(int *sizes, int length); /*checks if heaps are all empty*/
void userTurn(int *sizes, int length); /*plays a user turn*/
void computerTurn(int *sizes, int length); /*plays a computer turn using computer's strategy*/
void printSizes(int *sizes, int length, int turn); /*prints heaps' sizes*/
