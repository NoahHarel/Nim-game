# makefile
CC = gcc
OBJS = main.o Board.o Game.o DataStructs.o Edit.o ListUtils.o Solve.o Utilities.o Gurobi.o Init.o
EXEC = sudoku-console
COMP_FLAG = -ansi -Wall -Wextra -Werror -pedantic-errors
GUROBI_COMP = -I/usr/local/lib/gurobi563/include
GUROBI_LIB = -L/usr/local/lib/gurobi563/lib -lgurobi56

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(OBJS) $(GUROBI_LIB) -o $@ -lm
main.o: main.c Gurobi.h
	$(CC) $(COMP_FLAG) $(GUROBI_COMP) -c $*.c
Board.o: Board.c
	$(CC) $(COMP_FLAG) $(GUROBI_COMP) -c $*.c
Game.o: Game.c
	$(CC) $(COMP_FLAG) $(GUROBI_COMP) -c $*.c
DataStructs.o: DataStructs.c
	$(CC) $(COMP_FLAG) $(GUROBI_COMP) -c $*.c
Edit.o: Edit.c
	$(CC) $(COMP_FLAG) $(GUROBI_COMP) -c $*.c	
ListUtils.o: ListUtils.c
	$(CC) $(COMP_FLAG) $(GUROBI_COMP) -c $*.c
Solve.o: Solve.c
	$(CC) $(COMP_FLAG) $(GUROBI_COMP) -c $*.c
Utilities.o: Utilities.c
	$(CC) $(COMP_FLAG) $(GUROBI_COMP) -c $*.c
Gurobi.o: Gurobi.c
	$(CC) $(COMP_FLAG) $(GUROBI_COMP) -c $*.c
Init.o: Init.c
	$(CC) $(COMP_FLAG) $(GUROBI_COMP) -c $*.c
clean:
	rm -f	$(OBJS) $(EXEC)