
#include "mpi.h"
#include <iostream>
#include <chrono>
#include "Solver.h"

using namespace std;


int main(const int argc, char*** argv)
{
	MPI_Init(&argc, argv);

	auto start = chrono::high_resolution_clock::now(); //Duration count

	int world_rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);			//cislo procesu

	int world_size;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);			//pocet procesu 
	cout << "world_rank = " << world_rank << endl;


	Solver s;
	int possibleFirst[N] = { 0 };
	int numberPossibleFirst = 0;
	int rowFirst = 0;
	int collumnFirst = 0;
	int sudoku[N][N] = { 0 };
	int solutionRank = 0;
	bool solutionFound = false;
	
	/*int sudoku[N][N] = { {0, 3, 4, 0, 8, 2, 0, 0, 0},
						{5, 0, 0, 0, 4, 0, 9, 0, 0},
						{0, 9, 0, 0, 7, 0, 0, 1, 0},
						{0, 0, 3, 4, 0, 0, 0, 5, 0},
						{7, 0, 0, 0, 0, 0, 0, 0, 8},
						{0, 6, 0, 0, 0, 7, 3, 0, 0},
						{0, 2, 0, 0, 1, 0, 0, 9, 0},
						{0, 0, 7, 0, 9, 0, 0, 0, 2},
						{0, 0, 0, 7, 2, 0, 4, 8, 0} };*/

	if (world_rank == 0) {
		/*int sudoku[N][N] = { {0,0,0,0,1,0,3,8,7},
					{0,0,4,0,0,5,0,0,1},
					{0,1,0,0,0,2,4,0,0},
					{0,5,6,0,0,0,0,4,0},
					{2,0,0,0,0,0,0,0,3},
					{0,7,0,0,0,0,2,6,0},
					{0,0,1,6,0,0,0,9,0},
					{6,0,0,8,0,0,7,0,0},
					{7,9,8,0,3,0,0,0,0} };*/
		s.setGridFromFile(sudoku);
	}
	MPI_Bcast(*(sudoku), N * N, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Barrier(MPI_COMM_WORLD);
	
	if(world_rank==1) s.printGrid(sudoku);

	if ((world_size > 1) && (world_rank == 1)) {
		s.checkContradictions(sudoku);
	}
	else if(world_size == 1){
		s.checkContradictions(sudoku);
	}

	if(world_rank ==0){
		s.computeNumberPossibilities(sudoku, 1, 0, -1, rowFirst, collumnFirst, possibleFirst, numberPossibleFirst);
	}

	MPI_Bcast(&rowFirst, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&collumnFirst, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&numberPossibleFirst, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(possibleFirst, N, MPI_INT, 0, MPI_COMM_WORLD);
	//if (s.checkContradictions()) {

	MPI_Barrier(MPI_COMM_WORLD);

	if (world_size > numberPossibleFirst) {
		int possibleSecond[N] = { 0 };
		int numberPossibleSecond = 0;
		int rowSecond = 0;
		int collumnSecond = 0;

		if (world_rank == 0) {
			s.computeNumberPossibilities(sudoku, 2, rowFirst, collumnFirst, rowSecond, collumnSecond, possibleSecond, numberPossibleSecond);
		}
		MPI_Bcast(&rowSecond, 1, MPI_INT, 0, MPI_COMM_WORLD);
		MPI_Bcast(&collumnSecond, 1, MPI_INT, 0, MPI_COMM_WORLD);
		MPI_Bcast(&numberPossibleSecond, 1, MPI_INT, 0, MPI_COMM_WORLD);
		MPI_Bcast(possibleSecond, N, MPI_INT, 0, MPI_COMM_WORLD);

		//cout << world_rank << " ma possibilities for second " << numberPossibleSecond << endl;
		//cout << world_rank << " ma hodnotu rowSecond a collumnSecond " << rowSecond << collumnSecond << endl;

		MPI_Barrier(MPI_COMM_WORLD);

		int numForOne = world_size / numberPossibleFirst;
		
		for (int posFirst = 0; posFirst < numberPossibleFirst; posFirst++) {
			for (int posSecond = 0; posSecond < numberPossibleSecond; posSecond++) {
				if (world_rank == posFirst * numForOne + (posSecond % numForOne)) {
					sudoku[rowFirst][collumnFirst] = possibleFirst[posFirst];
					sudoku[rowSecond][collumnSecond] = possibleSecond[posSecond];
					if (s.solveSudoku(sudoku, 0, 0)) {
						cout << world_rank << " found solution:" << endl;
						s.printGrid(sudoku);
						solutionRank = world_rank;
						solutionFound = true;
						break;
					}
					else {
						cout << world_rank << " ended without solution" << endl;
					}
				}
			}
		}
	}
	else if (world_size == numberPossibleFirst) {
		cout << "Pocet vlaken je presny pocet moznosti pro prvni volne policko" << endl;
		for (int possibility = 0; possibility < numberPossibleFirst; possibility++) {
			if (world_rank == possibility) {
				sudoku[rowFirst][collumnFirst] = possibleFirst[possibility];
				if (s.solveSudoku(sudoku, 0, 0)) {
					cout << world_rank << " found solution:" << endl;
					solutionFound = true;
					s.printGrid(sudoku);
					solutionRank = world_rank;
					break;
				}
				else {
					cout << world_rank << " ended without solution" << endl;
				}
			}
		}
	}
	else {
		cout << "Pocet vlaken je mensi" << endl;
		for (int possibility = 0; possibility < numberPossibleFirst; possibility++) {
			if (world_rank == possibility % world_size) {
				sudoku[rowFirst][collumnFirst] = possibleFirst[possibility];
				if (s.solveSudoku(sudoku, 0, 0)) {
					cout << world_rank << " found solution:" << endl;
					solutionFound = true;
					s.printGrid(sudoku);
					solutionRank = world_rank;
					break;
				}
				else {
					cout << world_rank << " ended without solution" << endl;
				}
			}
		}
	}
	
	auto stop = chrono::high_resolution_clock::now(); //Duration count
	
	auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
	if ((world_rank == solutionRank) && (solutionFound == true)) {
		cout << "Total time for thread "<< world_rank << " is: " << duration.count() << " microseconds." << endl;
		}
	
	MPI_Finalize();
}

