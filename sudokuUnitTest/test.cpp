#include "pch.h"
#include <CppUnitTest.h>
#include "../sudoku/Solver.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace {
	TEST(TestSetGrid, PositiveCase) {
		int s[N][N];
		int expected[N][N] = { {0, 3, 4, 0, 8, 2, 0, 0, 0},
						{5, 0, 0, 0, 4, 0, 9, 0, 0},
						{0, 9, 0, 0, 7, 0, 0, 1, 0},
						{0, 0, 3, 4, 0, 0, 0, 5, 0},
						{7, 0, 0, 0, 0, 0, 0, 0, 8},
						{0, 6, 0, 0, 0, 7, 3, 0, 0},
						{0, 2, 0, 0, 1, 0, 0, 9, 0},
						{0, 0, 7, 0, 9, 0, 0, 0, 2},
						{0, 0, 0, 7, 2, 0, 4, 8, 0} };
		Solver solver;
		solver.setGridFromFile(s, "sudoku3.txt");
		for (int i = 0; i < N; i++) {
			EXPECT_TRUE(0 == std::memcmp(s[i], expected[i], N));
		}
	}

	TEST(TestContradiction, NegativeCase) {
		Solver solver;
		solver.setInputOK(false);
		int checkTry[N][N] = { 0 };
		EXPECT_FALSE(solver.checkContradictions(checkTry));

		solver.setInputOK(true);
		int s[N][N];
		/*int s[N][N] = { {0, 3, 4, 0, 8, 2, 0, 0, 0},
						{5, 0, 0, 0, 4, 0, 9, 0, 0},
						{0, 9, 0, 0, 7, 0, 0, 1, 0},
						{0, 0, 3, 4, 0, 0, 0, 5, 0},
						{7, 0, 0, 0, 0, 0, 5, 0, 8},
						{0, 6, 0, 0, 0, 7, 3, 0, 0},
						{0, 2, 0, 0, 1, 0, 0, 9, 0},
						{0, 0, 7, 0, 9, 0, 0, 0, 2},
						{0, 0, 0, 7, 2, 0, 4, 8, 0} };*/
		solver.setGridFromFile(s, "sudoku3.txt");
		s[4][6] = 5;
		EXPECT_FALSE(solver.checkContradictions(s));

		s[4][6] = 7;
		EXPECT_FALSE(solver.checkContradictions(s));

		s[4][6] = 9;
		solver.printGrid(s);
		EXPECT_FALSE(solver.checkContradictions(s));
	}

	TEST(TestContradiction, PositiveCase) {
		Solver solver;
		solver.setInputOK(true);
		int s[N][N]; /*= { {0, 3, 4, 0, 8, 2, 0, 0, 0},
						{5, 0, 0, 0, 4, 0, 9, 0, 0},
						{0, 9, 0, 0, 7, 0, 0, 1, 0},
						{0, 0, 3, 4, 0, 0, 0, 5, 0},
						{7, 0, 0, 0, 0, 0, 0, 0, 8},
						{0, 6, 0, 0, 0, 7, 3, 0, 0},
						{0, 2, 0, 0, 1, 0, 0, 9, 0},
						{0, 0, 7, 0, 9, 0, 0, 0, 2},
						{0, 0, 0, 7, 2, 0, 4, 8, 0} };*/
		solver.setGridFromFile(s, "sudoku3.txt");
		EXPECT_TRUE(solver.checkContradictions(s));
	}

	TEST(TestSolve, PositiveCase) {
		Solver solver;
		int s[N][N];/* = { {0, 3, 4, 0, 8, 2, 0, 0, 0},
						{5, 0, 0, 0, 4, 0, 9, 0, 0},
						{0, 9, 0, 0, 7, 0, 0, 1, 0},
						{0, 0, 3, 4, 0, 0, 0, 5, 0},
						{7, 0, 0, 0, 0, 0, 0, 0, 8},
						{0, 6, 0, 0, 0, 7, 3, 0, 0},
						{0, 2, 0, 0, 1, 0, 0, 9, 0},
						{0, 0, 7, 0, 9, 0, 0, 0, 2},
						{0, 0, 0, 7, 2, 0, 4, 8, 0} };*/
		solver.setGridFromFile(s, "sudoku3.txt");
		EXPECT_TRUE(solver.solveSudoku(s));

		int reseni[N][N] = { {1, 3, 4, 9, 8, 2, 6, 7, 5},
							{5, 7, 8, 6, 4, 1, 9, 2, 3},
							{6, 9, 2, 3, 7, 5, 8, 1, 4},
							{2, 1, 3, 4, 6, 8, 7, 5, 9},
							{7, 4, 5, 1, 3, 9, 2, 6, 8},
							{8, 6, 9, 2, 5, 7, 3, 4, 1},
							{3, 2, 6, 8, 1, 4, 5, 9, 7},
							{4, 8, 7, 5, 9, 6, 1, 3, 2},
							{9, 5, 1, 7, 2, 3, 4, 8, 6} };
		for (int i = 0; i < N; i++) {
			EXPECT_TRUE(0 == std::memcmp(reseni[i], s[i], N));
		}
	}

	TEST(TestSolve, NegativeCase) {
		Solver solver;
		int s[N][N];/* = { {0, 3, 4, 0, 8, 2, 0, 0, 0},
						{5, 0, 0, 0, 4, 0, 9, 0, 0},
						{0, 9, 0, 0, 7, 0, 0, 1, 0},
						{0, 0, 3, 4, 0, 0, 0, 5, 0},
						{7, 0, 0, 0, 0, 0, 6, 0, 8}, //pridana 6 na miste 6... nema reseni
						{0, 6, 0, 0, 0, 7, 3, 0, 0},
						{0, 2, 0, 0, 1, 0, 0, 9, 0},
						{0, 0, 7, 0, 9, 0, 0, 0, 2},
						{0, 0, 0, 7, 2, 0, 4, 8, 0} };*/
		solver.setGridFromFile(s, "sudoku3.txt");
		s[4][6] = 6;
		EXPECT_FALSE(solver.solveSudoku(s));
	}

}

/*TEST(TestCaseName, TestName) {
  EXPECT_EQ(1, 1);
  EXPECT_TRUE(true);
}*/