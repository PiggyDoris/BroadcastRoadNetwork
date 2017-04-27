#pragma once
#include "Node.h"
#include <stdio.h>
#include <iostream>
#include <vector>
#include <string>
class BorderNode :
	public Node
{
public:
	BorderNode();
	~BorderNode();
	BorderNode(double,double);
	BorderNode(double, double,int);
	void setRegionIdList(int);
	void setWholeRegionIdList(vector<int>);
	vector<int> getRegionIdList();

private:
	vector<int> regionIdList;
};

