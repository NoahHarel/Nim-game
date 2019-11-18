# makefile
CC = gcc
OBJS = main.o Parser.o Game.o Solver.o
EXEC = sudoku
COMP_FLAG = -ansi -Wall -Wextra -Werror -pedantic-errors

$(EXEC): $(OBJS)
	$(CC) $(OBJS) -o $@
main.o: main.c
	$(CC) $(COMP_FLAG) -c $*.c
Parser.o: Parser.c
	$(CC) $(COMP_FLAG) -c $*.c
Game.o: Game.c
	$(CC) $(COMP_FLAG) -c $*.c
Solver.o: Solver.c
	$(CC) $(COMP_FLAG) -c $*.c
clean:
	rm -f $(OBJS) $(EXEC)
	echo Clean done