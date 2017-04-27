#include "stdafx.h"
#include "Destination.h"
#include "Node.h"
#include <string>
#include <iostream>
using namespace std;

Destination::Destination()
{
}


Destination::Destination(Node* destinationNode, int pathDistance)
{
	this->pathDistance = pathDistance;
	this->destinationNode = destinationNode;
}

Destination::~Destination()
{
}

void Destination::setStartPointNode(Node* startPointNode)
{
	this->startPointNode = startPointNode;
}

void Destination::setStartPointId(int startPointId)
{
	this->startPointId = startPointId;
}

void Destination::setPathDistance(int pathDistance)
{
	this->pathDistance = pathDistance;
}

void Destination::setDestinationNode(Node* destinationNode)
{
	this->destinationNode = destinationNode;
}

void Destination::setDestinationId(int destinationId)
{
	this->destinationId = destinationId;
}

void Destination::setDestinationNodeName(string destinationNodeName)
{
	this->destinationNodeName = destinationNodeName;
}

void Destination::setRegionId(int regionId)
{
	this->regionId = regionId;
}

Node* Destination::getStartPointNode()
{
	return startPointNode;
}

int Destination::getStartPointId()
{
	return startPointId;
}

int Destination::getPathDistance()
{
	return pathDistance;
}

Node* Destination::getDestinationNode()
{
	return destinationNode;
}

int Destination::getDestinationId()
{
	return destinationId;
}

string Destination::getDestinationNodeName()
{
	return destinationNodeName;
}

int Destination::getRegionId()
{
	return regionId;
}

bool Destination::operator()(const Destination* t1, const Destination* t2)
{
	return t1->pathDistance < t2->pathDistance;
}

bool Destination::operator<(const Destination& ti) const
{
	return pathDistance < ti.pathDistance;
}
