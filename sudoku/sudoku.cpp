#include <iostream>
#include <chrono>
#include <fstream>
#include "Solver.h"


using namespace std::chrono;

int main()
{	
	auto start = high_resolution_clock::now();			//Duration count

	Solver s;
	
	int sudoku[N][N];

	s.setGridFromFile(sudoku, "sudoku4.txt");			//Sets sudoku from file 
	s.printGrid(sudoku);

	if (s.checkContradictions(sudoku)) {				//Checks if there are any contradictions
		if (s.solveSudoku(sudoku) == false) {			//Recursively solves the sudoku if it can be solved
			cout << "Sudoku is not solvable" << endl;
		}
		s.printGrid(sudoku);
		//s.writeGridToFile(sudoku);
	}

	auto stop = high_resolution_clock::now(); //Duration count

	auto duration = duration_cast<microseconds>(stop - start);
	cout << "Total time: " << duration.count() << " microseconds." << endl;		//Writes the total time
}

