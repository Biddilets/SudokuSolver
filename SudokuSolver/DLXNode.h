#pragma once


// Node structure for use in the matrix manipulated by DLX.
// All fields are public because the algorithm will make
// thousands of modifications to them--the getter/setter 
// overhead is substantial.
class DLXNode
{
public:
	DLXNode(void);
    ~DLXNode(void) { };

	DLXNode* column;
	DLXNode* left;
	DLXNode* right;
	DLXNode* up;
	DLXNode* down;

	int index;        // only filled for
	int columnLength; // column headers
};