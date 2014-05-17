#include "DLXMatrix.h"


DLXMatrix::DLXMatrix(int columns)
{
	_root = new DLXNode;
	_root->right = _root->left = _root;
	_columns = std::vector<DLXNode*>(columns);
	BuildColumns();
	_rows = std::vector<DLXNode*>();
}

DLXMatrix::~DLXMatrix(void)
{
    for (size_t i = 0; i < _columns.size(); ++i)
		delete _columns[i];

	for (size_t i = 0; i < _rows.size(); ++i)
	{
		DLXNode* node = _rows[i]->right;
		while (node != _rows[i])
		{
			DLXNode* temp = node->right;
			delete node;
			node = temp;
		}
		delete _rows[i];
	}

	delete _root;
}

void DLXMatrix::AppendNodeToColumn(DLXNode* node, int columnIndex)
{
    DLXNode* columnHeader = _columns.at(columnIndex);
    node->column = node->down = columnHeader;
    node->up = columnHeader->up;
    columnHeader->up->down = node;
    columnHeader->up = node;

    columnHeader->columnLength++;
}

void DLXMatrix::BuildColumns()
{
	for (size_t i = 0; i < _columns.size(); ++i)
	{
		DLXNode* node = new DLXNode;
		node->index = i;
		node->left = _root->left;
		node->right = _root;
		node->up = node->down = node->column = node;

		_root->left->right = node;
		_root->left = node;

		_columns[i] = node;
	}
}

DLXNode* DLXMatrix::ShortestColumn() const
{
    DLXNode* cursor = _root->right;
    DLXNode* shortestColumn = cursor;
    int minLength = cursor->columnLength;

    while (cursor != _root)
    {
        if (cursor->columnLength < minLength)
        {
            shortestColumn = cursor;
            minLength = cursor->columnLength;
        }

        cursor = cursor->right;
    }

    return shortestColumn;
}