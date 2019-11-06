int nimSum(int *sizes, int length); /*returns the current turn's nim sum*/
int heapNimSum(int *sizes, int length, int heapNum);/*returns the input heap's current turn's nim sum*/
int winningHeap(int *sizes, int length, int heapNum);/*returns 1 if heapNum is a winning heap, 0 otherwise*/
int winnerHeap(int *sizes, int length);/*returns minimal heap number h s.t h is a winning heap*/
