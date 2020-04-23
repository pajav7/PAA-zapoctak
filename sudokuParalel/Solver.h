#ifndef SOLVER_H
#define SOLVER_H
#include <string>

using namespace std;

const int N = 9;							//Dimension 

class Solver
{
public:
	Solver();								//Constructor
	~Solver();								//Destructor
	const int EMPTY = 0;					//Constant for empty possition
	void setGridFromFile(int grid[N][N], string filename);		//Sets grid from "filename"

	bool solveSudoku(int grid[N][N]);							//Returns true if sudoku can be solved
	void printGrid(int grid[N][N]);								//Prints the grid
	void computeNumberPossibilities(int grid[N][N], int position, int fromRow, int fromCollumn, int &rowPos, int &collumnPos, int possibilies[N], int &numberPossibilities);
																//Computes the possibilities for empty position (in the sequence in 'position'
	bool findNextEmpty(int grid[N][N],int& row, int& collumn, int R, int C); //Finds the next empty starting from the R and C
	bool checkContradictions(int grid[N][N]);								//Checks the grid for contradictions against rules of sudoku
	
	bool isUsedInRow(int grid[N][N], int row, int number);					//Checks if number is used in row 
	bool isUsedInCollumn(int grid[N][N], int collumn, int number);				//Checks if number is used in collumn
	bool isUsedInBox(int grid[N][N], int boxRow, int boxCollumn, int number);		//Checks if number is used in box 
	bool isOk(int grid[N][N], int row, int collumn, int number);					//Checks if number can be places on the position row, column

};
#endif
