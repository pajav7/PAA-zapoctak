
#include "mpi.h"
#include <iostream>
#include <chrono>
#include "Solver.h"
#include <string>

using namespace std;


int main(const int argc, char* argv[])
{
	MPI_Init(&argc, &argv);								//Inicialization for multiple thread application

	auto start = chrono::high_resolution_clock::now();	//Duration count

	int world_rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);			//Number of the thread

	int world_size;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);			//Total number of all the threads
	//cout << "world_rank = " << world_rank << endl;


	Solver s;
	int possibleFirst[N] = { 0 };
	int numberPossibleFirst = 0;
	int rowFirst = 0;
	int collumnFirst = 0;
	int sudoku[N][N] = { 0 };
	int solutionRank = world_size;
	int solutionFound = 0;
	

	if (world_rank == 0) {								//Thread with number 0 sets the Grid from file
		if (argc > 1) {
			string textFile = argv[1];
			cout << "Sudoku from text file: " <<  textFile << endl;
			s.setGridFromFile(sudoku, textFile);
		}
		else {
			cout << "Sudoku from text file: sudoku1.txt - default setting" << endl;
			s.setGridFromFile(sudoku, "sudoku1.txt");
		}
	}

	MPI_Bcast(*(sudoku), N * N, MPI_INT, 0, MPI_COMM_WORLD);		//Thread 0 broadcast the Grid
	MPI_Barrier(MPI_COMM_WORLD);

	if ((world_size > 1) && (world_rank == 1)) {					//Thread 1 checks if there are any contradictions of the input
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

	if (world_rank == 0) {											//Thread 0 finds all possibilities for the first empty possition
		s.computeNumberPossibilities(sudoku, 1, 0, -1, rowFirst, collumnFirst, possibleFirst, numberPossibleFirst);
	}
	
	MPI_Bcast(&rowFirst, 1, MPI_INT, 0, MPI_COMM_WORLD);			//Thread 0 broadcast position of the first empty position and possibilities
	MPI_Bcast(&collumnFirst, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&numberPossibleFirst, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(possibleFirst, N, MPI_INT, 0, MPI_COMM_WORLD);

	MPI_Request request;											//Request as a flag if the solution is found already
	MPI_Barrier(MPI_COMM_WORLD);

	if (world_size > numberPossibleFirst && world_size > 1) {		//For number of threads greater than number of possibilities for first empty possition
		int possibleSecond[N] = { 0 };
		int numberPossibleSecond = 0;
		int rowSecond = 0;
		int collumnSecond = 0;

		if (world_rank == 0) {										//Thread 0 computes number of possibilities for the second empty possition
			s.computeNumberPossibilities(sudoku, 2, rowFirst, collumnFirst, rowSecond, collumnSecond, possibleSecond, numberPossibleSecond);
		}

		int remainder = 0;										
		int numForOne[N] = { 0 };

		if (world_rank == 1) {									//Thread 1 computes number of threads for one possibility for the first empty possition
			remainder = world_size % numberPossibleFirst;			//Remainder is the number of threads that are surplus
			for (int i = 0; i < numberPossibleFirst; i++) {
				if (remainder > i) {
					numForOne[i] = world_size / numberPossibleFirst + 1;
				}
				else {
					numForOne[i] = world_size / numberPossibleFirst;
				}
			}
		}

		MPI_Bcast(&rowSecond, 1, MPI_INT, 0, MPI_COMM_WORLD);				//Thread 0 broadcast possition for the second empty possition 
		MPI_Bcast(&collumnSecond, 1, MPI_INT, 0, MPI_COMM_WORLD);			//and number of possibilities for the second empty 
		MPI_Bcast(&numberPossibleSecond, 1, MPI_INT, 0, MPI_COMM_WORLD);
		MPI_Bcast(possibleSecond, N, MPI_INT, 0, MPI_COMM_WORLD);
		MPI_Bcast(&remainder, 1, MPI_INT, 1, MPI_COMM_WORLD);				//Thread 1 broadcast the remainder and number of threads for every
		MPI_Bcast(numForOne, N, MPI_INT, 1, MPI_COMM_WORLD);				// possibility for the first empty possition

		//MPI_Barrier(MPI_COMM_WORLD);

		int flag;
		int numForPrevious = 0;

		for (int posFirst = 0; posFirst < numberPossibleFirst; posFirst++) {	//For loop to consider every possibility for first possition
			for (int posSecond = 0; posSecond < numberPossibleSecond; posSecond++) {	//For loop to consider every possibility for second possition
				if (world_rank == numForPrevious + (posSecond % numForOne[posFirst])) {	//Assignment of thread to particular possibility
					sudoku[rowFirst][collumnFirst] = possibleFirst[posFirst];			//Setting of the first and second empty possition
					sudoku[rowSecond][collumnSecond] = possibleSecond[posSecond];
					if (s.solveSudoku(sudoku)) {										//Thread searching for the solution with particular setting
						cout << world_rank << " found solution:" << endl;					//Thread found solution
						s.printGrid(sudoku);
						solutionRank = world_rank;
						solutionFound = 1;
						for (int i = 0; i < world_size; i++) {								//Thread sends message to other threads as a flag
							if (i != solutionRank) {
								MPI_Isend(&solutionRank, 1, MPI_INT, i, 0, MPI_COMM_WORLD, &request);
							}
						}
						goto theEnd;
					}
					else {																	//Thread didn't found the solution
						//cout << world_rank << " ended without solution" << endl;
					}
				}
				MPI_Iprobe(MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &flag, MPI_STATUS_IGNORE);	//Thread checks if any thread sends message
				if (flag != 0) {
					//cout << world_rank << " konci kvuli flag == true" << endl;
					goto theEnd;
				}
			}
			numForPrevious = numForPrevious + numForOne[posFirst];
		}
	}
	else {											//For number of threads lower than number of possibilities for first empty possition
		int flag;
		for (int possibility = 0; possibility < numberPossibleFirst; possibility++) {	//For loop to consider every possibility for first possition
			if (world_rank == possibility % world_size) {								//Assignment of thread to possibility
				sudoku[rowFirst][collumnFirst] = possibleFirst[possibility];
				if (s.solveSudoku(sudoku)) {											//Thread searching for the solution with particular setting
					cout << endl;															//Thread found solution
					cout << world_rank << " found solution:" << endl;
					solutionFound = 1;
					s.printGrid(sudoku);
					solutionRank = world_rank;
					for (int i = 0; i < world_size; i++) {									//Thread sends message to other threads as a flag
						if (i != solutionRank) {
							MPI_Isend(&solutionRank, 1, MPI_INT, i, 0, MPI_COMM_WORLD, &request);
						}
					}
					break;
				}
			}
			MPI_Iprobe(MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &flag, MPI_STATUS_IGNORE);		//Thread checks if any thread sends message
			if (flag != 0) {
				//cout << world_rank << " konci kvuli flag == true" << endl;
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
	
	auto stop = chrono::high_resolution_clock::now();										//Duration count

	auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
	MPI_Barrier(MPI_COMM_WORLD);

	if (world_rank == 0) {																	//Thread 0 writes the total time
		cout << "Total time for thread " << world_rank << " is: " << duration.count() << " microseconds." << endl;
	}

	MPI_Finalize();

}

