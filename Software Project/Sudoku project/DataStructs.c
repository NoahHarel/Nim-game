#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include "Board.h"

typedef struct StackNode {
    int data;
    struct StackNode* next;
} StackNode;

struct StackNode* newNode(int data)
{
    struct StackNode* stackNode = (struct StackNode*)malloc(sizeof(struct StackNode));
    stackNode->data = data;
    stackNode->next = NULL;
    return stackNode;
}

int isEmpty(struct StackNode* root)
{
    return !root;
}

void push(struct StackNode** root, int data)
{
    struct StackNode* stackNode = newNode(data);
    stackNode->next = *root;
    *root = stackNode;

}

int pop(struct StackNode** root)
{
	struct StackNode* temp;
	int popped;
    if (isEmpty(*root))
        return INT_MIN;
    temp = *root;
    *root = (*root)->next;
    popped = temp->data;
    free(temp);
    return popped;
}

int peek(struct StackNode* root)
{
    if (isEmpty(root))
        return INT_MIN;
    return root->data;
}

int goNext(int* col, int* row, int N) {
	(*col)++;
	if ((*col) == N && (*row) == N) {
		return 2;
	} else if ((*col) == N) {
		(*col) = 0;
		(*row)++;
	}
	return 1;
}

void findNextEmpty(struct Board board, int *col, int *row) {
	while (board.values[(*row)][(*col)] != 0) {
		goNext(col, row, board.N);
	}
}

int cellsLeftToFill(struct Board *board) {
	int i = 0;
	int j = 0;
	int empty_cells = 0;
	int N = board->N;
	while (i < N) {
		j = 0;
		while (j < N) {
			if (board->values[i][j] == 0) {
				empty_cells++;
			}
			j++;
		}
		i++;
	}
	return empty_cells;
}


void update_3_stacks(int push_flag, StackNode **game, StackNode **rows, StackNode **cols, int *i, int *j, int *num,
		int *blanks, int *size) {
	/*Takes three stacks: one for row, one for col and one for elements. Updates all three at once.
	 * if push_flag == 1, func pushes onto; else - it pops.
	 */
	if (push_flag == 1) {
		push(game, *num);
		push(rows, *i);
		push(cols, *j);
		(*size)++;
		(*blanks)--;
	} else {
		*num = pop(game);
		*i = pop(rows);
		*j = pop(cols);
		(*num)++;
		(*size)--;
		(*blanks)++;
	}

}

int backtrack_stack(struct Board board) {
	int i = 0;
	int j = 0;
	int N = board.N;
	int success_counter = 0;
	int stack_size = 0;
	int curr_num = 1;
	int legal_rowcol;
	int legal_square;
	int end_phase_one = 0;
	int end_phase_two = 0;
	int val_of_cell_before;
	int back_to_phase_one = 0;
	int cells_to_fill = cellsLeftToFill(&board);

	/*Init 3 stacks for vals, rows and col*/
	StackNode *backStack;
	StackNode *rowStack;
	StackNode *colStack;
	int full_board = isBoardFull(&board);
	while (full_board == 0) {
		/*Phase one: this part only adds to the stack*/
		back_to_phase_one = 0;
		findNextEmpty(board, &j, &i);
		while (curr_num <= N && end_phase_one== 0) {
			legal_rowcol = isLegalInRowCol(&board, j+1, i+1, curr_num);
			legal_square = isLegalInSquare(&board, j+1, i+1, curr_num);
			if (legal_rowcol == 1 && legal_square == 1) {
				/*If we can place the val, it's a success. If we're done, raise the counter.
				 * Reset elem back to 1 and continue to Phase Two*/
				end_phase_one = 1;
				if (cells_to_fill == 1) {
					success_counter++;
				} else {
					board.values[i][j] = curr_num;
					update_3_stacks(1, &backStack, &rowStack, &colStack, &i, &j, &curr_num, &cells_to_fill, &stack_size);
				}
				curr_num = 1;
			}
			if (end_phase_one == 0) {
				/*If we couldn't put it, continue phase one*/
				curr_num++;
				continue;
			}
		}
		end_phase_one = 0;
		val_of_cell_before = board.values[i][j];
		if (val_of_cell_before != 0) {
			continue;
		}
		/*Phase two: In this part, pushing and popping occur. Takes care of the success cases and
		 * fail cases (all vals didn't work for cell)*/
		while (stack_size != 0 && back_to_phase_one == 0) {
			end_phase_two = 0;
			update_3_stacks(0, &backStack, &rowStack, &colStack, &i, &j, &curr_num, &cells_to_fill, &stack_size);
			board.values[i][j] = 0;
			while (curr_num <= N && end_phase_two == 0) {
				legal_rowcol = isLegalInRowCol(&board, j+1, i+1, curr_num);
				legal_square = isLegalInSquare(&board, j+1, i+1, curr_num);
				if (legal_rowcol == 1 && legal_square == 1) {
					end_phase_two = 1;
					if (cells_to_fill == 1) {
						success_counter++;
					} else {
						update_3_stacks(1, &backStack, &rowStack, &colStack, &i, &j, &curr_num, &cells_to_fill, &stack_size);
						board.values[i][j] = curr_num;
						back_to_phase_one = 1;
						curr_num = 1;
					}
				}
				if (end_phase_two == 0) {
					curr_num++;
					continue;
				}
			}

		}
		if (stack_size == 0) {
			break;
		}
	}
	return success_counter;
}

