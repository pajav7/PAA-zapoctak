#include "Solver.h"



Solver::Solver()
{
}


Solver::~Solver()
{
}

bool Solver::solveSudoku(int grid[N][N])
{
	int row;
	int collumn;

	if (!findEmpty(grid, row, collumn)) {
		return true;
	}

	for (int number = 1; number <= 9; number++) {
		if (isOk(grid, row, collumn, number)) {
			grid[row][collumn] = number;

			if (solveSudoku(grid)) {
				return true;
			}
			else {
				grid[row][collumn] = EMPTY;
			}
		}
	}
	return false;
}

void Solver::printGrid(int grid[N][N])
{
	for (int r = 0; r < N; r++) {
		for (int c = 0; c < N; c++) {
			cout << grid[r][c] << " ";
		}
		cout << endl;
	}
}

bool Solver::findEmpty(int grid[N][N], int& row, int& collumn)
{
	for (row = 0; row < N; row++) {
		for (collumn = 0; collumn < N; collumn++) {
			if (grid[row][collumn] == EMPTY)
				return true;
		}
	}
	return false;
}

bool Solver::isUsedInRow(int grid[N][N], int row, int number)
{
	for (int c = 0; c < N; c++) {
		if (grid[row][c] == number)
			return true;
	}
	return false;
}

bool Solver::isUsedInCollumn(int grid[N][N], int collumn, int number)
{
	for (int r = 0; r < N; r++) {
		if (grid[r][collumn] == number)
			return true;
	}
	return false;
}

bool Solver::isUsedInBox(int grid[N][N], int boxRow, int boxCollumn, int number)
{
	for (int row = boxRow; row < boxRow + 3; row++) {
		for (int col = boxCollumn; col < boxCollumn + 3; col++) {
			if (grid[row][col] == number)
				return true;
		}
	}
	return false;
}

bool Solver::isOk(int grid[N][N], int row, int collumn, int number)
{
	return !isUsedInRow(grid, row, number) && 
		!isUsedInCollumn(grid, collumn, number) && 
		!isUsedInBox(grid, row - row % 3, collumn - collumn%3, number);
}