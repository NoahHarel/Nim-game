

#ifndef LISTUTILS_H_
#define LISTUTILS_H_


typedef struct UndoRedoElem {
	struct UndoRedoElem *next;
	struct UndoRedoElem *prev;
	struct Board board;

} UndoRedoElem;

typedef UndoRedoElem *ELEM;

ELEM initializeElem(struct Board* board);

void connectToNext(ELEM *base_elem, ELEM *next_elem);

void Undo(ELEM *curr_elem, int show_diff);

void Redo(ELEM *curr_elem, int show_diff);

void KillListElements(ELEM *start_elem);

void beKindRewind(ELEM *elem);

void createAndConnect(ELEM *base_elem, struct Board *board);

void print_diff(ELEM curr_elem, int undo);

void killElem(ELEM elemToKill);

void beGoodForward(ELEM *elem);

void KillAllElems(ELEM* start_elem);


#endif /* LISTUTILS_H_ */
