#include <iostream>
#include <chrono>
#include <fstream>
#include "Solver.h"

using namespace std::chrono;

int main()
{	
	auto start = high_resolution_clock::now(); //Duration count

	Solver s;

	/*
	int sudoku[N][N];
	int sudoku[N][N] = {{0, 3, 4, 0, 8, 2, 0, 0, 0},
						{5, 0, 0, 0, 4, 0, 9, 0, 0},
						{0, 9, 0, 0, 7, 0, 0, 1, 0},
						{0, 0, 3, 4, 0, 0, 0, 5, 0},
						{7, 0, 0, 0, 0, 0, 0, 0, 8},
						{0, 6, 0, 0, 0, 7, 3, 0, 0},
						{0, 2, 0, 0, 1, 0, 0, 9, 0},
						{0, 0, 7, 0, 9, 0, 0, 0, 2},
						{0, 0, 0, 7, 2, 0, 4, 8, 0} };*/

	s.setGridFromFile();
	s.printGrid();

	if (s.checkContradictions()) {
		if (s.solveSudoku() == false) {
			cout << "Sudoku is not solvable" << endl;
		}
		s.printGrid();
		s.writeGridToFile();
	}

	auto stop = high_resolution_clock::now(); //Duration count

	auto duration = duration_cast<microseconds>(stop - start);
	cout << "Total time: " << duration.count() << " microseconds." << endl;
}

