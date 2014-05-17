#include "DLX.h"


void DLX::AddRowToSolution(int rowIndex)
{
    DLXNode* node = _matrix->Row(rowIndex);
    do
    {
        Cover(node->column);
        node = node->right;
    } while (node != _matrix->Row(rowIndex));

    _solution.push(node);
}

void DLX::Cover(DLXNode* columnHead)
{
    // unlink every node in the column
    for (DLXNode* row = columnHead->down; row != columnHead; row = row->down)
        for (DLXNode* col = row->right; col != row; col = col->right)
        {
            col->up->down = col->down;
            col->down->up = col->up;
            col->column->columnLength--;
        }

    columnHead->right->left = columnHead->left;  // unlink the
    columnHead->left->right = columnHead->right; // column header
}

// each call covers one column
void DLX::Recurse()
{
    if (_matrix->Root()->right == _matrix->Root())
    {
        _done = true; // all columns covered,
        return;       // nothing left to do
    }

    DLXNode* column = _matrix->ShortestColumn();
    Cover(column);

    DLXNode* row = column->down;
    while (row != column)
    {
        _solution.push(row);
        for (DLXNode* right = row->right; right != row; right = right->right)
            Cover(right->column);

        if (!_done)
            Recurse();
        if (_done)
            return;

        // if we're here, we need to backtrack
        _solution.pop();
        column = row->column;
        for (DLXNode* left = row->left; left != row; left = left->left)
            Uncover(left->column);

        row = row->down;
    }

    Uncover(column);
}

void DLX::Solve(DLXMatrix* matrix, std::vector<int> const * givenRows)
{
    _matrix = matrix;

    if (givenRows)
        for (size_t i = 0; i < givenRows->size(); ++i)
            AddRowToSolution(givenRows->at(i));

    Recurse();
}

void DLX::Uncover(DLXNode* columnHead)
{
    // link every node in the column to its former neighbors
	for (DLXNode* row = columnHead->up; row != columnHead; row = row->up)
		for (DLXNode* col = row->left; col != row; col = col->left)
		{
			col->up->down = col;
			col->down->up = col;
			col->column->columnLength++;
		}

	columnHead->right->left = columnHead; // relink the
	columnHead->left->right = columnHead; // column header
}