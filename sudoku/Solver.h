#ifndef SOLVER_H
#define SOLVER_H

using namespace std;

const int N = 9;

class Solver
{
	int grid[N][N] = { 0 };
	bool inputOK = false;
public:
	Solver(); //Constructor
	~Solver();
	const int EMPTY = 0;
	
	void setGridFromFile();
	void setGrid(int sud[N][N]);
	bool solveSudoku();
	void printGrid();
	void writeGridToFile();

	bool findEmpty(int& row, int& collumn);
	bool checkContradictions();

	bool isUsedInRow(int row, int number);
	bool isUsedInCollumn(int collumn, int number);
	bool isUsedInBox(int boxRow, int boxCollumn, int number);
	bool isOk(int row, int collumn, int number);

};

#endif