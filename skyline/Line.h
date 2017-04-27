#pragma once
#include "Node.h"
using namespace std;

class Line
{
public:
	Line();
	~Line();

private:
	Node* startNode;
	Node* endNode;
};

