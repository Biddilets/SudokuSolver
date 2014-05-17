#pragma once
#include "DLXMatrix.h"
#include "DLXNode.h"
#include <stack>
#include <vector>


// DLX solves an instance of EXACT COVER via a recursive, depth-first brute-force search.
// This search is surprisingly fast in practice because of the unique structure it uses
// to encode the EXACT COVER matrix.
class DLX
{
public:
    DLX(void) { _done = false; }
    ~DLX(void) { };

	inline std::stack<DLXNode*>* Solution() { return &_solution; }
    void Solve(DLXMatrix* matrix, std::vector<int> const * givenRows = nullptr);

private:
	bool _done;
	DLXMatrix* _matrix;
	std::stack<DLXNode*> _solution;
	
	void AddRowToSolution(int rowIndex);

	// marks a constraint satisfied by unlinking the column that represents it
    // (the unlinked column retains its links, making the operation easily reversible)
	void Cover(DLXNode* columnHead);

	void Recurse(); // search function, covers one column per call

	// the inverse of Cover, links a column back into the matrix
	void Uncover(DLXNode* columnHead);
};