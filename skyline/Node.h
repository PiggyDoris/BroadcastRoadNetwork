#ifndef NODE_H
#define NODE_H
#pragma once

#include <stdio.h>
#include <iostream>
#include <vector>
#include <string>
#include "Destination.h"
using namespace std;

class Node
{
public:
	Node();
	~Node();
	Node(string);
	Node(double, double);
	Node(double, double,int);

	void setLongtitude(double);
	void setLatitude(double);
	void setRoadIdList(int);
	void setCounter(int);
	void addCounter();
	void setNodeName(string);
	void setNodeRegionId(int);
	void setNodeId(int);
	void setDestinationList(Destination*);
	void setNodeLineNumber(int);
	void setToQueryDistance(int);
	void setIsSpot(bool);
	double getLongtitude();
	double getLatitude();
	int getCounter();
	string getNodeType();
	vector<int> getRoadIdList();
	string getNodeName();
	int getNodeRegionId();
	int getNodeId();
	vector<Destination*> getDestinationList();
	int getNodeLineNumber();
	int getToQueryDistance();
	bool getIsSpot();
	bool operator()(const Node* t1, const Node* t2);
	bool operator < (const Node& ti) const;

protected:
	int nodeId;
	int nodeRegionId;
	int counter;
	int toQueryDistance;
	int nodeLineNumber;
	bool isSpot;
	double longtitude;
	double latitude;
	string nodeType;
	vector<int> roadIdList;
	string nodeName;
	vector<Destination*> destinationList;
};

#endif