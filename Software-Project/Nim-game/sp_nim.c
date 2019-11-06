int nimSum(int *sizes, int length){ /*returns the current turn's nim sum*/
	int currentNimSum=0,s;
	for (s=0;s<length;s++){
		currentNimSum^=sizes[s];
	}
	return currentNimSum;
}
/*----------------------------------------------------------------------------------------*/
int heapNimSum(int *sizes, int length, int heapNum){/*returns the input heap's current turn's nim sum*/
	int heapNimSum, turnNimSum;
	turnNimSum = nimSum(sizes, length);
	heapNimSum = turnNimSum^sizes[heapNum];
	return heapNimSum;
}
/*----------------------------------------------------------------------------------------*/
int winningHeap(int *sizes, int length, int heapNum){/*returns 1 if heapNum is a winning heap, 0 otherwise*/
	int result, size;
	result = heapNimSum(sizes, length, heapNum);
	size = sizes[heapNum];
	if (result<size){
		return 1;
	}
	return 0;
}
/*----------------------------------------------------------------------------------------*/
int winnerHeap(int *sizes, int length){/*returns minimal heap number h s.t h is a winning heap*/
	int h;
	for (h=0;h<length;h++){
		if (winningHeap(sizes, length, h)==1){
			return h;
		}
	}

	return -1;
}
