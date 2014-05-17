#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include "Test.h"


void RunTestCases()
{
    Test t;

    t.AddAllSudokusInFile("..\\SudokuSolver.Test\\Test Cases\\top1465.txt");         // 1465 ordinary sudokus
    t.AddAllSudokusInFile("..\\SudokuSolver.Test\\Test Cases\\HardestDatabase.txt"); // 375 particularly hard sudokus
    t.AddAllSudokusInFile("..\\SudokuSolver.Test\\Test Cases\\unsolvable.txt");      // 3 unsolvable sudokus
    
    t.SolveAllSudokus();
}

int main(int argc, char **argv) 
{
    RunTestCases();
    _CrtDumpMemoryLeaks(); // logs memory leaks to VS output window
	
    return 0;
}