#include "stdafx.h"
#include "Road.h"


Road::Road()
{
	startNode = new Node();
	endNode = new Node();
}

Road::Road(Node* startNode, Node* endNode)
{
	this->startNode = startNode;
	this->endNode = endNode;
}


Road::~Road()
{
	delete startNode;
	delete endNode;
}

void Road::setStartNode(double longtitude, double latitude)
{
	startNode->setLongtitude(longtitude);
	startNode->setLatitude(latitude);
}

void Road::setEndNode(double longtitude, double latitude)
{
	endNode->setLongtitude(longtitude);
	endNode->setLatitude(latitude);
}

void Road::setDistance(double dist)
{
	this->dist = dist;
}

Node* Road::getStartNode()
{
	return startNode;
}

Node* Road::getEndNode()
{
	return endNode;
}

double Road::getDistance()
{
	return dist;
}
