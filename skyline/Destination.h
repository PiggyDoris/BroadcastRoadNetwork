#ifndef DESTINATION_H
#define DESTINATION_H
#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
using namespace std;

class Node;

class Destination
{
public:
	Destination();	
	Destination(Node*, int);
	~Destination();

	void setStartPointNode(Node*);
	void setStartPointId(int);
	void setPathDistance(int);
	void setDestinationNode(Node*);
	void setDestinationId(int);
	void setDestinationNodeName(string);
	void setRegionId(int);

	Node* getStartPointNode();
	int getStartPointId();
	int getPathDistance();
	Node* getDestinationNode();
	int getDestinationId();
	string getDestinationNodeName();
	int getRegionId();
	bool operator()(const Destination* t1, const Destination* t2);
	bool operator < (const Destination& ti) const;

private:
	int startPointId;
	Node* startPointNode;
	int pathDistance;
	int destinationId;
	Node* destinationNode;
	string destinationNodeName;
	int regionId;
};

#endif