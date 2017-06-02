#pragma once
#include <iostream>
#include "Node.h"
#include "Region.h"
using namespace std;
class Bound
{
public:
	Bound();
	~Bound();
	Bound(string);
	void setBoundType(string);
	string getBoundType();
	void setUpperBound(double);
	double getUpperBound();
	void setLowerBound(double);
	double geLowerBound();
	void setQueryRegionId(int);
	int getQueryRegionId();
	void setStartPointRegionId(int);
	void setDestinationRegionId(int);
	int getDestinationRegionId();
	void setQueryPoint(Node*);
	Node* getQueryPoint();
	void setQueryRegion(Region*);
	Region* getQueryRegion();
	void setStartPointRegion(Region*);
	void setDestinationRegion(Region*);
	Region* getDestinationRegion();
	bool operator()(const Bound* t1, const Bound* t2);
	bool operator < (const Bound& ti) const;

private:
	string boundType;
	double upperBound;
	double lowerBound;
	int queryRegionId;
	int startPointRegionId;
	int destinationRegionId;
	Node* queryPoint;
	Region* queryRegion;
	Region* startPointRegion;
	Region* destinationRegion;
};

