#include <iostream>
#include "SudokuSolver.h"


SudokuSolver::SudokuSolver(int dimension)
{
	_dimension = dimension;
	_boxDimension = static_cast<int>(sqrt(dimension));
	_cells = dimension * dimension;
	_dlx = nullptr;
	_matrix = nullptr;
	_sudoku = nullptr;
}

SudokuSolver::~SudokuSolver(void)
{
    DeletePointers();
}

/* 
* Reducing standard Sudoku to EXACT COVER:
*
* Each column of the DLX matrix represents one of the four
* constraints on digit placement:
* (1) Each cell holds exactly one digit. (row-column constraint)
* (2) Each digit appears exactly once in each row. (row-value constraint)
* (3) Each digit appears exactly once in each column. (column-value constraint)
* (4) Each digit appears exactly once in each 3x3 block. (box-value constraint)
*
* There are 81 cells in standard sudoku and each assignment of a digit to one
* satisfies a unique set of constraints.  Thus 81 columns are required to
* encode each constraint over the entire puzzle and the DLX matrix has 
* 81 * 4 = 324 columns.
*
* Each row of the matrix represents the assignment of one digit to a cell.
* There are 9 possible assignments.  Hence there are 9 * 81 = 729 rows.
* Because each assignment satisfies all four constraints, each row  has exactly
* four entries. The overall structure has 236,196 cells 98.8% of which contain zero.
*/
void SudokuSolver::BuildMatrix()
{
	_matrix = new DLXMatrix(_cells * 4);
    DLXNode* rowHeader;
	int row, column, rowIndex;

	for (int i = 0; i < _cells; ++i)
	{
		row = i / _dimension;
		column = i % _dimension;
		if (_sudoku[i] > 0 && _sudoku[i] <= _dimension) // this cell's value is given; need only one row
		{
            rowHeader = BuildRow(row, column, _sudoku[i] - 1);
			rowIndex = _matrix->AddRow(rowHeader);

			_givenRows.push_back(rowIndex);
		}
		else 
            for (int j = 0; j < _dimension; ++j)
            {
                rowHeader = BuildRow(row, column, j);
                _matrix->AddRow(rowHeader);
            }
	}
}

// builds the row representing placement of value at (row, column),
// returns a pointer to one of its entries
DLXNode* SudokuSolver::BuildRow(int row, int column, int value)
{
    int columnIndices[4] = { ConstraintColumnIndex(row, column),
        ConstraintColumnIndex(row, value, 1),
        ConstraintColumnIndex(column, value, 2),
        ConstraintColumnIndex(BoxIndex(row, column), value, 3) };

    // each row has one entry for each constraint
    DLXNode* const nodes[4] = { new DLXNode, new DLXNode, new DLXNode, new DLXNode };
    for (int i = 0; i < 4; ++i)
    {
        _matrix->AppendNodeToColumn(nodes[i], columnIndices[i]);
        if (i < 3)
            nodes[i]->right = nodes[i + 1];
        if (i > 0)
            nodes[i]->left = nodes[i - 1];
    }
    nodes[0]->left = nodes[3];
    nodes[3]->right = nodes[0];

    return nodes[0];
}

void SudokuSolver::DeletePointers()
{
    if (_matrix)
    {
        delete _matrix;
        _matrix = nullptr;
    }
    if (_dlx)
    {
        delete _dlx;
        _dlx = nullptr;
    }
}

bool SudokuSolver::OutputSolution()
{
    // the solution stack has one node from each of the rows whose set union
    // constitutes the sudoku's solution; we need to reverse the encoding
    // to build the solved puzzle
	DLXNode* node;
	int row, column, value, _, columnIndex;
	while (_solution->size() > 0)
	{
		value = row = -1;
		node = _solution->top();
		while (value < 0 || row < 0)
		{
			columnIndex = node->column->index;
			if (columnIndex < _cells) // this column encodes the row-column constraint
				ConstraintRowColumn(columnIndex, row, column);
			else if (columnIndex >= _cells * 3) // this one the box-value constraint
				ConstraintRowColumn(columnIndex, _, value);
			node = node->right;
		}
		int cell = FlatArrayIndex(row, column);
		_sudoku[cell] = value + 1;
		_solution->pop();
	}

	return true;
}

void SudokuSolver::Reset()
{
	_givenRows.clear();
    DeletePointers();
	_sudoku = nullptr;
}

bool SudokuSolver::Solve(int sudoku[])
{
	if (_sudoku)
		Reset();

	_sudoku = sudoku;
	BuildMatrix();
	
    _dlx = new DLX;
	_dlx->Solve(_matrix, &_givenRows);
	
    _solution = _dlx->Solution();
	if (_solution->size() != _cells)
		return false; // puzzle has no solution

	OutputSolution();
	return true;
}