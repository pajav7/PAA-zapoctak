#ifndef SOLVER_H
#define SOLVER_H

using namespace std;

const int N = 9;

class Solver
{
public:
	Solver();
	~Solver();
	const int EMPTY = 0;
	bool solveSudoku(int grid[][N]);
	void printGrid(int grid[][N]);

	bool findEmpty(int grid[][N], int& row, int& collumn);

	bool isUsedInRow(int grid[][N], int row, int number);
	bool isUsedInCollumn(int grid[][N], int collumn, int number);
	bool isUsedInBox(int grid[][N], int boxRow, int boxCollumn, int number);
	bool isOk(int grid[][N], int row, int collumn, int number);

};

#endif