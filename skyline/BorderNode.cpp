#include "stdafx.h"
#include "BorderNode.h"


BorderNode::BorderNode()
{
	nodeType = "BorderNode";
}

BorderNode::BorderNode(double longtitude, double latitude)
{
	this->longtitude = longtitude;
	this->latitude = latitude;
}

BorderNode::BorderNode(double longtitude, double latitude, int nodeId)
{
	this->longtitude = longtitude;
	this->latitude = latitude;
	this->nodeName = "b_" + to_string(nodeId);
}

BorderNode::~BorderNode()
{
}

void BorderNode::setRegionIdList(int regionId)
{
	regionIdList.push_back(regionId);
}

vector<int> BorderNode::getRegionIdList()
{
	return regionIdList;
}

void BorderNode::setWholeRegionIdList(vector<int> regionIdList)
{
	this->regionIdList = regionIdList;
}
