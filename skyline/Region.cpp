#include "stdafx.h"
#include "Region.h"


Region::Region()
{
	id = 0;
}

Region::Region(int id, double longtitudeLeft, double longtitudeRight, double latitudeBottom, double latitudeTop)
{
	this->id = id;
	this->longtitudeLeft = longtitudeLeft;
	this->longtitudeRight = longtitudeRight;
	this->latitudeBottom = latitudeBottom;
	this->latitudeTop = latitudeTop;
}


Region::~Region()
{
}

void Region::setLongtitudeLeft(double longtitudeLeft)
{
	this->longtitudeLeft = longtitudeLeft;
}

void Region::setLongtitudeRight(double longtitudeRight)
{
	this->longtitudeRight = longtitudeRight;
}

void Region::setLatitudeBottom(double latitudeBottom)
{
	this->latitudeBottom = latitudeBottom;
}

void Region::setLatitudeTop(double latitudeTop)
{
	this->latitudeTop = latitudeTop;
}

void Region::setId(int id)
{
	this->id = id;
}

double Region::getLongtitudeLeft()
{
	return longtitudeLeft;
}

double Region::getLongtitudeRight()
{
	return longtitudeRight;
}

double Region::getLatitudeBottom()
{
	return latitudeBottom;
}

double Region::getLatitudeTop()
{
	return latitudeTop;
}

int Region::getId()
{
	return id;
}

void Region::setBorderNodeList(BorderNode* regionBorderNode)
{
	borderNodeList.push_back(regionBorderNode);
}

vector<BorderNode*> Region::getBorderNodeList()
{
	return borderNodeList;
}

void Region::setNodeList(Node* regionNode)
{
	nodeList.push_back(regionNode);
}

void Region::setMinLambda(double minLambda)
{
	this->minLambda = minLambda;
}

void Region::setMaxLambda(double maxLambda)
{
	this->maxLambda = maxLambda;
}

vector<Node*> Region::getNodeList()
{
	return nodeList;
}

double Region::getMinLambda()
{
	return minLambda;
}

double Region::getMaxLambda()
{
	return maxLambda;
}

void Region::setBNNTableList(Destination* pushInNode)
{
	bNNTableList.push_back(pushInNode);
}

vector<Destination*> Region::getBNNTableList()
{
	return bNNTableList;
}

void Region::setNodeAmount(int nodeAmount)
{
	this->nodeAmount = nodeAmount;
}

int Region::getNodeAmount()
{
	return nodeAmount;
}

void Region::setMinRRDist(int minDist)
{
	minRRDistList.push_back(minDist);
}

vector<int> Region::getMinRRDistList()
{
	return minRRDistList;
}

void Region::setMinDeltaList(Destination* inputMinLambda)
{
	minLambdaList.push_back(inputMinLambda);
}

vector<Destination*> Region::getMinDeltaList()
{
	return minLambdaList;
}

void Region::setToQueryDestinationList(Destination* toQueryDestination)
{
	toQueryDestinationList.push_back(toQueryDestination);
}

vector<Destination*> Region::getToQueryDestinationList()
{
	return toQueryDestinationList;
}

void Region::setRegionNodePathList(Destination* inputPath)
{
	regionNodePathList.push_back(inputPath);
}

vector<Destination*> Region::getRegionNodePathList()
{
	return regionNodePathList;
}

void Region::setMaxRRDist(int inputMaxRRDist)
{
	maxRRDistList.push_back(inputMaxRRDist);
}

vector<int> Region::getMaxRRDistList()
{
	return maxRRDistList;
}
