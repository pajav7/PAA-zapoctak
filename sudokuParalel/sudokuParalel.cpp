
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
	int solutionRank = world_size;
	int solutionFound = 0;

	if (world_rank == 0) {
		s.setGridFromFile(sudoku);
	}

	MPI_Bcast(*(sudoku), N * N, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Barrier(MPI_COMM_WORLD);

	if ((world_size > 1) && (world_rank == 1)) {
		if (s.checkContradictions(sudoku) == false) {
			cout << "The program can not run due to some contradictions on the input" << endl;
		}
		s.printGrid(sudoku);
	}
	else if (world_size == 1) {
		if (s.checkContradictions(sudoku) == false) {
			cout << "The program can not run due to some contradictions on the input" << endl;
		}

		s.printGrid(sudoku);
	}

	if (world_rank == 0) {
		s.computeNumberPossibilities(sudoku, 1, 0, -1, rowFirst, collumnFirst, possibleFirst, numberPossibleFirst);
	}

	MPI_Bcast(&rowFirst, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&collumnFirst, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&numberPossibleFirst, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(possibleFirst, N, MPI_INT, 0, MPI_COMM_WORLD);

	MPI_Request request;
	MPI_Barrier(MPI_COMM_WORLD);

	if (world_size >= 2 * numberPossibleFirst) {
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

		//MPI_Barrier(MPI_COMM_WORLD);

		int numForOne = world_size / numberPossibleFirst;

		int flag;

		for (int posFirst = 0; posFirst < numberPossibleFirst; posFirst++) {
			for (int posSecond = 0; posSecond < numberPossibleSecond; posSecond++) {
				if (world_rank == posFirst * numForOne + (posSecond % numForOne)) {
					sudoku[rowFirst][collumnFirst] = possibleFirst[posFirst];
					sudoku[rowSecond][collumnSecond] = possibleSecond[posSecond];
					if (s.solveSudoku(sudoku)) {
						cout << world_rank << " found solution:" << endl;
						s.printGrid(sudoku);
						solutionRank = world_rank;
						solutionFound = 1;
						for (int i = 0; i < world_size; i++) {
							if (i != solutionRank) {
								MPI_Isend(&solutionRank, 1, MPI_INT, i, 0, MPI_COMM_WORLD, &request);
							}
						}
						goto theEnd;
					}
					else {
						cout << world_rank << " ended without solution" << endl;
					}
				}
				MPI_Iprobe(MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &flag, MPI_STATUS_IGNORE);
				if (flag != 0) {
					cout << world_rank << " konci kvuli flag == true" << endl;
					goto theEnd;
				}
			}
		}
	}
	else {
		int flag;
		for (int possibility = 0; possibility < numberPossibleFirst; possibility++) {
			if (world_rank == possibility % world_size) {
				sudoku[rowFirst][collumnFirst] = possibleFirst[possibility];
				if (s.solveSudoku(sudoku)) {
					cout << endl;
					cout << world_rank << " found solution:" << endl;
					solutionFound = 1;
					s.printGrid(sudoku);
					solutionRank = world_rank;
					for (int i = 0; i < world_size; i++) {
						if (i != solutionRank) {
							MPI_Isend(&solutionRank, 1, MPI_INT, i, 0, MPI_COMM_WORLD, &request);
						}
					}
					break;
				}
				else {
					cout << world_rank << " ended without solution" << endl;
				}
			}
			MPI_Iprobe(MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &flag, MPI_STATUS_IGNORE);
			if (flag != 0) {
				cout << world_rank << " konci kvuli flag == true" << endl;
				break;
			}
		}
	}
	theEnd:

	/*MPI_Request request2;
	if (world_rank == solutionRank) {
		MPI_Isend(&solutionRank, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &request2);
	}
	
	int flag2 = 0;
	if (world_rank == 0) {
		MPI_Iprobe(MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &flag2, MPI_STATUS_IGNORE);
		if (flag2 != 0) {
			MPI_Recv(&solutionRank, 1, MPI_INT, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		}
	}*/
	
	auto stop = chrono::high_resolution_clock::now(); //Duration count

	auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
	MPI_Barrier(MPI_COMM_WORLD);

	if (world_rank == 0) {
		cout << "Total time for thread " << world_rank << " is: " << duration.count() << " microseconds." << endl;
	}
	MPI_Finalize();

}

