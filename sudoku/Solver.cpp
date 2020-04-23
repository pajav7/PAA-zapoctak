#include "Solver.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;


Solver::Solver()		//Constructor
{
};


Solver::~Solver()		//Destructor
{
};

void Solver::setGridFromFile(int grid[N][N], string filename) //Sets the grid from "filename"
{
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
				if ((st.length()==1) && ((isdigit(st[0])) || (st[0]=='0'))) {
					grid[i][j] = st[0]-'0';
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
	inputOK = true;
	inFile.close();
};

bool Solver::solveSudoku(int grid[N][N]) //Recursively solves sudoku in member grid, returns true if sudoku can be solved
{
	int row;
	int collumn;

	if (!findEmpty(grid, row, collumn)) {
		return true;
	}

	for (int number = 1; number <= N; number++) {
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
};

void Solver::printGrid(int grid[N][N])			//Prints sudoku in the grid
{
	for (int r = 0; r < N; r++) {
		for (int c = 0; c < N; c++) {
			cout << grid[r][c] << " ";
		}
		cout << endl;
	}
	cout << endl;
};

void Solver::writeGridToFile(int grid[N][N])		//Writes the sudoku grid into the file "reseniSudoku.txt"
{
	ofstream outFile;
	outFile.open("reseniSudoku.txt");
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			outFile << grid[i][j] << " ";
		}
		outFile << endl;
	}
	outFile.close();
}

void Solver::setInputOK(bool ok)		//Sets member inputOK
{
	this->inputOK = ok;
	return;
};

bool Solver::getInputOK()			//Returns member inputOK
{
	return this->inputOK;
};

bool Solver::findEmpty(int grid[N][N], int& row, int& collumn)		//Finds an empty position
{
	for (row = 0; row < N; row++) {
		for (collumn = 0; collumn < N; collumn++) {
			if (grid[row][collumn] == EMPTY)
				return true;
		}
	}
	return false;
};

bool Solver::checkContradictions(int grid[N][N])		//Checks if the input in grid has any contradictions against the rules of sudoku
{
	if (inputOK == false) {
		return false;
	}
	int n = 0;
	for (int i = 0; i < N; i++) {					
		for (int j = 0; j < N; j++) {
			if (grid[i][j] != 0) {
				n = grid[i][j];
				grid[i][j] = 0;
				if (!isOk(grid, i, j, n)) {
					grid[i][j] = n;
					cout << "Wrong input in row " << i << " and collumn " << j << " number " << n << "." << endl;
					inputOK = false;
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

bool Solver::isUsedInRow(int grid[N][N], int row, int number)		//Checks if number is in the row
{
	for (int c = 0; c < N; c++) {
		if (grid[row][c] == number)
			return true;
	}
	return false;
};

bool Solver::isUsedInCollumn(int grid[N][N], int collumn, int number)		//Checks if number is in the collumn
{
	for (int r = 0; r < N; r++) {
		if (grid[r][collumn] == number)
			return true;
	}
	return false;
};

bool Solver::isUsedInBox(int grid[N][N], int boxRow, int boxCollumn, int number) //Checks if number is in the box
{
	for (int row = boxRow; row < boxRow + 3; row++) {
		for (int col = boxCollumn; col < boxCollumn + 3; col++) {
			if (grid[row][col] == number)
				return true;
		}
	}
	return false;
};

bool Solver::isOk(int grid[N][N], int row, int collumn, int number) //Checks if number can be filled in the position row, collumn
{
	return !isUsedInRow(grid, row, number) &&
		!isUsedInCollumn(grid, collumn, number) &&
		!isUsedInBox(grid, row - row % 3, collumn - collumn % 3, number);
};
