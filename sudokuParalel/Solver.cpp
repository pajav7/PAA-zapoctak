#include "Solver.h"
#include <iostream>
#include <fstream>
#include <string>
#include <mpi.h>

using namespace std;


Solver::Solver()		//Constructor
{
};


Solver::~Solver()		//Destructor
{
};

void Solver::setGridFromFile(int grid [N][N], string filename)	//Sets a sudoku grid from the file "filename", 
{																//checks if the input is ok (not if it satisfies conditions for sudoku)
	string st;
	ifstream inFile(filename);
	if (inFile.is_open()) {
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				if (j < N - 1) {
					getline(inFile, st, ' ');
				}
				else {
					getline(inFile, st, '\n');
					if ((st.length() > 1) && ((st[1] == ' ') || (st[1] == '\t'))) {
						st = st[0];
					}
				}
				if ((st.length() == 1) && ((isdigit(st[0])) || (st[0] == '0'))) {
					grid[i][j] = st[0] - '0';
					st = "";
				}
				else {
					cout << "Wrong format of element in the row " << i << " and the collumn " << j << "." << endl;
					return;
				}
			}
		}
	}
	else {
		cout << "File is not open." << endl;
	}
	inFile.close();
};

bool Solver::solveSudoku(int grid[N][N])				//Recursively solves sudoku - grid
{
	int row;
	int collumn;
	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	if (!findNextEmpty(grid, row, collumn, 0, -1)) {
		return true;
	}

	for (int number = 1; number <= N; number++) {
		if (isOk(grid, row, collumn, number)) {
			grid[row][collumn] = number;
			if (solveSudoku(grid)) {
				return true;
			}
			else {
				grid[row][collumn] = 0;
			}
		}
	}
	return false;
};

void Solver::printGrid(int grid[N][N])			//Prints the grid
{
	for (int r = 0; r < N; r++) {
		for (int c = 0; c < N; c++) {
			cout << grid[r][c] << " ";
		}
		cout << endl;
	}
	cout << endl;
};
												//Computes the possibilities for empty position (in the sequence in 'position'
void Solver::computeNumberPossibilities(int grid[N][N], int position, int fromRow, int fromCollumn, int& rowPos, int& collumnPos, int possibilities[N], int& numberPossibilities)
{
	this->findNextEmpty(grid, rowPos, collumnPos, fromRow, fromCollumn);
	for (int number = 1; number <= N; number++) {
		if (this->isOk(grid, rowPos, collumnPos, number)) {
			for (int i = 0; i < N; i++) {
				if (possibilities[i] == 0) {
					possibilities[i] = number;
					break;
				}
			}
		}
	}
	for (int i = 0; (i < N) && (possibilities[i] != 0); i++) {
		numberPossibilities++;
	}
	cout << "Number of possibilities for " << position << ". empty is " << numberPossibilities << endl;
};
								//Finds next empty position from R and C
bool Solver::findNextEmpty(int grid[N][N], int& row, int& collumn, int R, int C) //Finds an empty place in the sudoku grid (EMPTY = 0)
{
	for (row = R; row < N; row++) {
		if (row == R) {
			for (collumn = C + 1; collumn < N; collumn++) {
				if (grid[row][collumn] == EMPTY)
					return true;
			}
		}
		else {
			for (collumn = 0; collumn < N; collumn++) {
				if (grid[row][collumn] == EMPTY)
					return true;
			}
		}
	}
	return false;
};
						
bool Solver::checkContradictions(int grid[N][N])		//Checks the grid for contradictions against rules of sudoku 
{
	int n = 0;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			if (grid[i][j] != 0) {
				n = grid[i][j];
				grid[i][j] = 0;
				if (!isOk(grid, i, j, n)) {
					grid[i][j] = n;
					cout << "Wrong input in row " << i << " and collumn " << j << " number " << n << "." << endl;
					return false;
				}
				else {
					grid[i][j] = n;
				}
			}
		}

	}
	return true;
}

bool Solver::isUsedInRow(int grid[N][N], int row, int number)		//Checks if number is used in row
{
	for (int c = 0; c < N; c++) {
		if (grid[row][c] == number)
			return true;
	}
	return false;
};

bool Solver::isUsedInCollumn(int grid[N][N], int collumn, int number)		//Checks if number is used in collumn
{
	for (int r = 0; r < N; r++) {
		if (grid[r][collumn] == number)
			return true;
	}
	return false;
};

bool Solver::isUsedInBox(int grid[N][N], int boxRow, int boxCollumn, int number) //Checks if number is used in box 
{
	for (int row = boxRow; row < boxRow + 3; row++) {
		for (int col = boxCollumn; col < boxCollumn + 3; col++) {
			if (grid[row][col] == number)
				return true;
		}
	}
	return false;
};

bool Solver::isOk(int grid[N][N], int row, int collumn, int number) //Checks if number can be places on the position row, column
{
	return !isUsedInRow(grid, row, number) &&
		!isUsedInCollumn(grid, collumn, number) &&
		!isUsedInBox(grid, row - row % 3, collumn - collumn % 3, number);
};
