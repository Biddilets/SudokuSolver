#include <algorithm>
#include <ctype.h>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include "..\SudokuSolver\SudokuSolver.h"
#include "Test.h"
#include <time.h>
using namespace std;


Test::~Test()
{
	for (size_t i = 0; i < _sudokus.size(); ++i)
		delete[] _sudokus[i];
}

void Test::AddAllSudokusInFile(const char* fileName)
{
    ifstream file(fileName);
    if (!file)
        return;

    size_t sudokusBefore = _sudokus.size();
    string line;
    while (getline(file, line))
    {
        if (line.length() != 81)
            continue;

        int* sudoku = new int[81];
        for (int i = 0; i < 81; ++i)
            sudoku[i] = isdigit(line[i]) ? line[i] - '0' : 0;

        _sudokus.push_back(sudoku);
    }
    file.close();

    cout << "Added " << _sudokus.size() - sudokusBefore << " sudokus from " << fileName << endl;
}

bool Test::IsSolved(int const * sudoku) const
{
	int row[9], column[9], box[9];
	int i, j, k;

	for (i = 0; i < 9; ++i)
	{
		k = 0;
		for (j = i * 9; j < (i * 9) + 9; ++j)
			row[k++] = sudoku[j];

		sort(begin(row), end(row));

		for (j = 0; j < 9; ++j)
			if (row[j] != j + 1)
				return false;

		k = 0;
		for (j = i; k < 9; j += 9)
			column[k++] = sudoku[j];

		sort(begin(column), end(column));

		for (j = 0; j < 9; ++j)
			if (column[j] != j + 1)
				return false;

		k = 0;
		for (j = (i * 3) % 9; k < 9; j += 9)
			for (int l = 0; l < 3; ++l)
				box[k++] = sudoku[j + l];

		sort(begin(box), end(box));

		for (j = 0; j < 9; ++j)
			if (box[j] != j + 1)
				return false;
	}

	return true;
}

void Test::PrintSudokuToStdout(int const * sudoku) const
{
	for (int i = 0; i < 81;)
	{
		cout << sudoku[i] << " ";
		if (++i % 9 == 0)
			cout << endl;
	}

	cout << endl;
}

void Test::SolveAllSudokus(bool verbose)
{
	int solved = 0;
	vector<double>* const solveTimes = new vector<double>(_sudokus.size());

    cout << "Solving..." << endl;

	SudokuSolver s;
	for (size_t i = 0; i < _sudokus.size(); ++i)
	{
		clock_t start = clock();
		s.Solve(_sudokus[i]);
        int elapsedMs = clock() - start;
        solveTimes->push_back(elapsedMs);

        if (IsSolved(_sudokus[i]))
			++solved;
		else if (verbose)
		{
			cout << "Invalid solution: " << endl;
            PrintSudokuToStdout(_sudokus[i]);
		}
	}

	double totalTime = 0;
	double maxTime = 0;
	for (size_t i = 0; i < solveTimes->size(); ++i)
	{
		if ((*solveTimes)[i] > maxTime)
			maxTime = (*solveTimes)[i];
		totalTime += (*solveTimes)[i];
	}
	double avgTime = totalTime / solveTimes->size();

	cout << "Solved " << solved << " / " << _sudokus.size() << " sudokus averaging "
		 << setprecision(4) << avgTime << "ms per solution." << endl;
    cout << "Slowest: " << maxTime << "ms." << endl;

	delete solveTimes;
}