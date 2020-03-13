#pragma once

#include "sudoku.cpp"


class Solver
{
public:
	Solver();
	~Solver();
	bool solveSudoku(int grid[N][N]);
	void printGrid(int grid[N][N]);

	bool findEmpty(int grid[N][N], int& row, int& collumn);
	
	bool isUsedInRow(int grid[N][N], int row, int number);
	bool isUsedInCollumn(int grid[N][N], int collumn, int number);
	bool isUsedInBox(int grid[N][N], int boxRow, int boxCollumn, int number);
	bool isOk(int grid[N][N], int row, int collumn, int number);

};

