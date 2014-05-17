#pragma once
#pragma comment(lib, "SudokuSolver.lib")
#include <vector>


class Test
{
public:
	Test() { };
	~Test();

    // adds each 81-character line in fileName to _sudokus.
    // non-digit characters and zeros in a line are considered blank cells
    void AddAllSudokusInFile(const char* fileName);

    bool IsSolved(int const * sudoku) const;

	void PrintSudokuToStdout(int const * sudoku) const;

	// if verbose, unsolved puzzles are printed to stdout
	void SolveAllSudokus(bool verbose = false);


private:
	std::vector<int*> _sudokus;
};