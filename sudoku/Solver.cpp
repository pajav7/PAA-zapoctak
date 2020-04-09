#include "Solver.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;


Solver::Solver() //constructor
{
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			this->grid[i][j] = -1;
		}
	}
};


Solver::~Solver() //Destructor
{
};

void Solver::setGridFromFile() //Sets a sudoku grid from the file "sudoku.txt", checks if the input is ok (not if it satisfies conditions for sudoku)
{
	string st;
	ifstream inFile("sudoku.txt");
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

void Solver::setGrid(int sud[N][N]) //Sets a sudoku grid from 2D array sud
{
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			grid[i][j] = sud[i][j];
		}
	}	
	inputOK = true;
};

bool Solver::solveSudoku() //Recursively solves sudoku in member grid
{
	int row;
	int collumn;

	if (!findEmpty(row, collumn)) {
		return true;
	}

	for (int number = 1; number <= N; number++) {
		if (isOk(row, collumn, number)) {
			this->grid[row][collumn] = number;

			if (solveSudoku()) {
				return true;
			}
			else {
				this->grid[row][collumn] = EMPTY;
			}
		}
	}
	return false;
};

void Solver::printGrid() //Prints the grid
{
	for (int r = 0; r < N; r++) {
		for (int c = 0; c < N; c++) {
			cout << this->grid[r][c] << " ";
		}
		cout << endl;
	}
	cout << endl;
};

void Solver::writeGridToFile() //Writes the sudoku grid into the file "reseniSudoku.txt"
{
	ofstream outFile;
	outFile.open("reseniSudoku.txt");
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			outFile << this->grid[i][j] << " ";
		}
		outFile << endl;
	}
	outFile.close();
}

bool Solver::findEmpty(int& row, int& collumn) //Finds an empty place in the sudoku grid (EMPTY = 0)
{
	for (row = 0; row < N; row++) {
		for (collumn = 0; collumn < N; collumn++) {
			if (this->grid[row][collumn] == EMPTY)
				return true;
		}
	}
	return false;
};

bool Solver::checkContradictions() // Controls if there is no number twice in the row, collumn of in the box
{
	if (inputOK == false) {
		return false;
	}
	int n = 0;
	for (int i = 0; i < N; i++) {					
		for (int j = 0; j < N; j++) {
			if (this->grid[i][j] != 0) {
				n = grid[i][j];
				grid[i][j] = 0;
				if (!isOk(i, j, n)) {
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

bool Solver::isUsedInRow(int row, int number) //Controls if 'number' is in the 'row'
{
	for (int c = 0; c < N; c++) {
		if (this->grid[row][c] == number)
			return true;
	}
	return false;
};

bool Solver::isUsedInCollumn(int collumn, int number) //Controls if 'number' is in the 'collumn'
{
	for (int r = 0; r < N; r++) {
		if (this->grid[r][collumn] == number)
			return true;
	}
	return false;
};

bool Solver::isUsedInBox(int boxRow, int boxCollumn, int number) //Controls if 'number' is in the box with initial element in the row 'boxRow' and collumn 'boxCollumn
{
	for (int row = boxRow; row < boxRow + 3; row++) {
		for (int col = boxCollumn; col < boxCollumn + 3; col++) {
			if (this->grid[row][col] == number)
				return true;
		}
	}
	return false;
};

bool Solver::isOk(int row, int collumn, int number) //Controls if 'number' has no contradictions in the 'row', 'collumn' and respective box
{
	return !isUsedInRow(row, number) &&
		!isUsedInCollumn(collumn, number) &&
		!isUsedInBox(row - row % 3, collumn - collumn % 3, number);
};
