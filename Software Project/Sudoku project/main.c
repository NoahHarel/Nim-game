#include <stdio.h>
#include <stdlib.h>
#include "SPBufferset.h"
#include "Board.h"
#include "Utilities.h"
#include "Game.h"
#include "Edit.h"
#include "Init.h"
#include "ListUtils.h"
#include "gurobi_c.h"
#include "Gurobi.h"

int main() {
	struct Board gameBoard;
	SP_BUFF_SET();
	printf("#################\n");
	printf("S-U-D-O-K-U-!-!-!\n");
	printf("-----------------\n");

	InitLoop(&gameBoard);

	return 0;
}
