
#include <iostream>
#include <chrono>

#include "Solver.h"

using namespace std::chrono;

int main()
{	
	auto start = high_resolution_clock::now();
	Solver s;
	
	int sudoku[N][N] = {{0, 3, 4, 0, 8, 2, 0, 0, 0},
						{5, 0, 0, 0, 4, 0, 9, 0, 0},
						{0, 9, 0, 0, 7, 0, 0, 1, 0},
						{0, 0, 3, 4, 0, 0, 0, 5, 0},
						{7, 0, 0, 0, 0, 0, 0, 0, 8},
						{0, 6, 0, 0, 0, 7, 3, 0, 0},
						{0, 2, 0, 0, 1, 0, 0, 9, 0},
						{0, 0, 7, 0, 9, 0, 0, 0, 2},
						{0, 0, 0, 7, 2, 0, 4, 8, 0} };
	s.printGrid(sudoku);
	s.solveSudoku(sudoku);
	s.printGrid(sudoku);

	auto stop = high_resolution_clock::now(); //Duration count

	auto duration = duration_cast<microseconds>(stop - start);
	cout << "Total time: " << duration.count() << " microseconds." << endl;
}

// Spuštění programu: Ctrl+F5 nebo nabídka Ladit > Spustit bez ladění
// Ladění programu: F5 nebo nabídka Ladit > Spustit ladění

// Tipy pro zahájení práce:
//   1. K přidání nebo správě souborů použijte okno Průzkumník řešení.
//   2. Pro připojení ke správě zdrojového kódu použijte okno Team Explorer.
//   3. K zobrazení výstupu sestavení a dalších zpráv použijte okno Výstup.
//   4. K zobrazení chyb použijte okno Seznam chyb.
//   5. Pokud chcete vytvořit nové soubory kódu, přejděte na Projekt > Přidat novou položku. Pokud chcete přidat do projektu existující soubory kódu, přejděte na Projekt > Přidat existující položku.
//   6. Pokud budete chtít v budoucnu znovu otevřít tento projekt, přejděte na Soubor > Otevřít > Projekt a vyberte příslušný soubor .sln.
