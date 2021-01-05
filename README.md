# PAA-zapoctak

This is a sudoku solver using a backtracking algorithm in C++.

In folder `sudoku` a one thread solver is implemented.

In folder `sudokuParalel` a multithread solver is implemented using MPI.

## Setup

To run the multithread program, use command: 

`$ mpirun -np x sudokuParalel.exe`

in command line in the folder `sudokuParalel\Debug` where x is the number of threads you want to use.
