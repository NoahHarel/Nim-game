#include <stdio.h>
#include <stdlib.h>
#include "Board.h"
#include "Utilities.h"

typedef struct UndoRedoElem {
	struct UndoRedoElem *next;
	struct UndoRedoElem *prev;
	struct Board board;
} UndoRedoElem;

typedef UndoRedoElem *ELEM;


ELEM initializeElem(struct Board *board) {
	ELEM elem = malloc(sizeof(UndoRedoElem));
	struct Board elem_board;
	int n = board->n;
	int m = board->m;
	elem->prev = NULL;
	elem->next = NULL;
	setDimensions(&elem_board, n, m);
	copyBoard(board, &elem_board);
	elem->board = elem_board;
	return elem;
}


/*Rewinds the list back to start position.*/
void beKindRewind(ELEM *elem) {
	while ((*elem)->prev != NULL) {
		*elem = (*elem)->prev;
	}
}

/*procedure to kill a list elements, with the board attached to it included*/
void killElem(ELEM elemToKill)
{
	int i=0;
	int N = elemToKill->board.N;
	struct Board boardToKill = elemToKill->board;
	for(i = 0; i<N; i++){
		free(boardToKill.values[i]);
		boardToKill.values[i] = NULL;
	}
	free(boardToKill.values);
	boardToKill.values = NULL;

	/* ELEM is now D-E-D dead*/
/*	free(elemToKill);*/
/*	elemToKill = NULL;*/
}

/*Function to go over each element in the list from start position and free it*/

void KillListElements(ELEM *start_elem) {
	ELEM *tmp;
	/*int stopper;*/
	while ((*start_elem)->next != NULL) {
		tmp = (&((*start_elem)->next));
		killElem(*start_elem);
		start_elem = tmp;
	}
	killElem(*start_elem);
}


/*Function that receives two list elements and connects them together (next & previous)*/

void connectToNext(ELEM *base_elem, ELEM *next_elem) {
	if ((*base_elem)->next != NULL) {
		KillListElements(&((*base_elem)->next));
	}
	(*base_elem)->next = *next_elem;
	(*next_elem)->prev = *base_elem;
	*base_elem = *next_elem;
}

/*"Cover" function for connect to next - since a new elem needs to be created, it creates it*/
void createAndConnect(ELEM *base_elem, struct Board *board) {
	ELEM next_elem = initializeElem(board);
	connectToNext(base_elem, &next_elem);
}

/*Function for undo/redo - if 1, compare board to next; else, compare board to previous and print
 * the differences*/

void print_diff(ELEM curr_elem, int undo) {
	int i = 0;
	int j = 0;
	int N = curr_elem->board.N;
	int curr_old = 0;
	int curr_new = 0;
	ELEM other;
	if (undo == 1) {
		other = curr_elem->prev;
	} else {
		other = curr_elem->next;
	}
	while (i < N) {
		j = 0;
		while (j < N) {
			curr_old = (*curr_elem).board.values[i][j];
			curr_new = (*other).board.values[i][j];
			if (curr_old != curr_new) {
				printf("Cell <%d,%d> changed: %d -> %d\n", j+1, i+1, curr_old, curr_new);
			}
			j++;
		}
		i++;
	}
}

/*Changes the UndoRedoElem pointer to the one before; shows an error if it doesn't exist*/
int Undo(ELEM *curr_elem, int show_diff) {
	if ((*curr_elem)->prev == NULL) {
		printf("ERROR: No more Undos left!\n");
		return 0;
	}
	if (show_diff == 1) {
		print_diff(*curr_elem, 1);
	}
	*curr_elem = (*curr_elem)->prev;
	return 1;
}


/*Changes the UndoRedoElem pointer to the next one; shows an error if it doesn't exist*/
int Redo(ELEM *curr_elem, int show_diff) {
	if ((*curr_elem)->next == NULL) {
		printf("ERROR: No Redo is possible!\n");
		return 0;
	}
	if (show_diff == 1) {
		print_diff(*curr_elem, 0);
	}
	*curr_elem = (*curr_elem)->next;
	return 1;
}
