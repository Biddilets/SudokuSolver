#pragma once
#include "DLXNode.h"
#include <vector>


class DLXMatrix
{
public:
	DLXMatrix(int columns);
	DLXMatrix(const DLXMatrix&);
	~DLXMatrix(void);

    inline int AddRow(DLXNode* header) { _rows.push_back(header); return _rows.size() - 1; }
	void AppendNodeToColumn(DLXNode* DLXNode, int columnIndex);
	inline const DLXNode* Root() { return _root; }
	inline DLXNode* Row(int rowIndex) { return _rows.at(rowIndex); }
    DLXNode* ShortestColumn() const;

private:
	std::vector<DLXNode*> _columns;
	DLXNode* _root;
	std::vector<DLXNode*> _rows;
	
	void BuildColumns();
};