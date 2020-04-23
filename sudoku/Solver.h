#ifndef SOLVER_H
#define SOLVER_H
#include <string>

using namespace std;

const int N = 9;

class Solver
{
	bool inputOK = false;	
public:
	Solver();				//Constructor
	~Solver();				//Destructor
	const int EMPTY = 0;		//Constant for empty positions 
	
	void setGridFromFile(int grid[N][N], string filename);		//Sets the grid from "filename"
	bool solveSudoku(int grid[N][N]);					//Returns true if sudoku can be solved
	void printGrid(int grid[N][N]);						//Prints sudoku in the grid
	void writeGridToFile(int grid[N][N]);					//Writes grid into the file "reseniSudoku.txt"
	void setInputOK(bool ok);							//Sets member inputOK
	bool getInputOK();							//Returns inputOK

	bool findEmpty(int grid[N][N], int& row, int& collumn);		//Finds an empty position
	bool checkContradictions(int grid[N][N]);			//Checks if the input in grid has any contradictions against the rules of sudoku

	bool isUsedInRow(int grid[N][N], int row, int number);		//Checks if number is in the row
	bool isUsedInCollumn(int grid[N][N], int collumn, int number);		//Checks if number is in the collumn
	bool isUsedInBox(int grid[N][N], int boxRow, int boxCollumn, int number);		//Checks if number is in the box
	bool isOk(int grid[N][N], int row, int collumn, int number);		//Checks if number can be filled in the position row, collumn

};

#endif