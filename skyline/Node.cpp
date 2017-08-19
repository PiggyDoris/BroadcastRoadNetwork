#include "stdafx.h"
#include "Node.h"
#include <iostream>
using namespace std;


Node::Node()
{
	longtitude = -1;
	latitude = -1;
	nodeType = "Node";
	nodeName = "";
	counter = 0;
	isSpot = false;
}

Node::Node(double longtitude, double latitude)
{
	this->longtitude = longtitude;
	this->latitude = latitude;
}

Node::Node(double longtitude, double latitude,int nodeId)
{
	this->longtitude = longtitude;
	this->latitude = latitude;
	this->nodeName = "n_" + to_string(nodeId);
}

Node::Node(string nodeName)
{
	this->nodeName = nodeName;
}


Node::~Node()
{
}

void Node::setLongtitude(double longtitude)
{
	this->longtitude = longtitude;
}

void Node::setLatitude(double latitude)
{
	this->latitude = latitude;
}

double Node::getLongtitude() const
{
	return longtitude;
}

double Node::getLatitude() const
{
	return latitude;
}

int Node::getCounter()
{
	return counter;
}

string Node::getNodeType()
{
	return nodeType;
}

void Node::setRoadIdList(int roadId)
{
	roadIdList.push_back(roadId);
}

void Node::setCounter(int counter)
{
	this->counter = counter;
}

void Node::addCounter()
{
	counter++;
}

vector<int> Node::getRoadIdList()
{
	return roadIdList;
}

string Node::getNodeName()
{
	return nodeName;
}

void Node::setNodeName(string nodeName)
{
	this->nodeName = nodeName;
}

void Node::setNodeRegionId(int nodeRegionId)
{
	this->nodeRegionId = nodeRegionId;
}

void Node::setNodeId(int nodeId)
{
	this->nodeId = nodeId;
}

void Node::setDestinationList(Destination* destinationNode)
{
	if (destinationList.size() == 0)
	{
		destinationList.push_back(destinationNode);
	}
	else
	{
		bool biggerDistanceFlag = true;
		for (int i = 0; i < destinationList.size(); i++)
		{
			if (destinationNode->getPathDistance() < destinationList.at(i)->getPathDistance())
			{
				destinationList.insert(destinationList.begin() + i, destinationNode);
				biggerDistanceFlag = false;
				break;
			}
		}
		if (biggerDistanceFlag == true)
		{
			destinationList.push_back(destinationNode);
		}
	}
}

int Node::getNodeRegionId()
{
	return nodeRegionId;
}

int Node::getNodeId()
{
	return nodeId;
}

std::vector<Destination*> Node::getDestinationList()
{
	return destinationList;
}

bool Node::operator()(const Node* t1, const Node* t2)
{
	return t1->nodeId < t2->nodeId;
}

bool Node::operator<(const Node& ti) const
{
	return nodeId < ti.nodeId;
}

//friend bool Node::sortLatitude(const Node* latitude1, const Node* latitude2)
//{
//	return latitude1->getlatitude() < latitude2->getlatitude();
//}
//
//friend bool Node::sortLatitude()(const Node* latitude1, const Node* latitude2)
//{
//	return latitude1->latitude < latitude2->latitude;
//}


void Node::setNodeLineNumber(int nodeLineNumber)
{
	this->nodeLineNumber = nodeLineNumber;
}

int Node::getNodeLineNumber()
{
	return nodeLineNumber;
}

void Node::setToQueryDistance(int toQueryDistance)
{
	this->toQueryDistance = toQueryDistance;
}

int Node::getToQueryDistance()
{
	return toQueryDistance;
}

void Node::setIsSpot(bool isSpot)
{
	this->isSpot = isSpot;
}

bool Node::getIsSpot()
{
	return isSpot;
}

