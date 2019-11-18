A command-line program which consists of two parts; initialization and the game itself. It
receives one command-line parameter "seed" for randomization.
In the initialization part, the board is partially filled with random digits (from 1 to 9) in the
following way.
The program randomly generates a solved board, and then clears all cells except several cells chosen at random.
The user chooses the number of these cells.
After the initialization, the game begins. For each move, the board is printed and the game waits for user input.
The game continues in this manner until the puzzle is solved and all cells are filled with correct values.
To generate the board, solve the board, and give hints to the user, the backtracking algorithm is used.
This algorithm attempts to solve a Sudoku board by filling in its empty cells.
It may be possible that there is no solution for a given board.
Two variants of the backtracking algorithm will be used, randomized and deterministic.
Implemented in collaboration with classmate Itamar Tzafrir.
