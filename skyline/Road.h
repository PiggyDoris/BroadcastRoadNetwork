#pragma once
#include "Node.h"
class Road
{
public:
	Road();
	Road(Node*, Node*);
	~Road();

	void setStartNode(double, double);
	void setEndNode(double, double);
	void setDistance(double);
	Node* getStartNode();
	Node* getEndNode();
	double getDistance();

private:
	Node* startNode;
	Node* endNode;
	double dist = 0;
};

