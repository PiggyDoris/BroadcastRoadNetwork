#include "stdafx.h"
#include "Bound.h"

Bound::Bound()
{
}

Bound::Bound(string boundType)
{
	this->boundType = boundType;
}

Bound::~Bound()
{
}

void Bound::setUpperBound(double upperBound)
{
	this->upperBound = upperBound;
}

double Bound::getUpperBound()
{
	return upperBound;
}

void Bound::setLowerBound(double lowerBound)
{
	this->lowerBound = lowerBound;
}

double Bound::geLowerBound()
{
	return lowerBound;
}

void Bound::setQueryRegionId(int queryRegionId)
{
	this->queryRegionId = queryRegionId;
}

int Bound::getQueryRegionId()
{
	return queryRegionId;
}

void Bound::setStartPointRegionId(int startPointRegionId)
{
	this->startPointRegionId = startPointRegionId;
}

void Bound::setDestinationRegionId(int destinationRegionId)
{
	this->destinationRegionId = destinationRegionId;
}

int Bound::getDestinationRegionId()
{
	return destinationRegionId;
}

void Bound::setQueryPoint(Node* queryPoint)
{
	this->queryPoint = queryPoint;
}

Node* Bound::getQueryPoint()
{
	return queryPoint;
}

void Bound::setQueryRegion(Region* queryRegion)
{
	this->queryRegion = queryRegion;
}

Region* Bound::getQueryRegion()
{
	return queryRegion;
}

void Bound::setStartPointRegion(Region* startPointRegion)
{
	this->startPointRegion = startPointRegion;
}

void Bound::setDestinationRegion(Region* destinationRegion)
{
	this->destinationRegion = destinationRegion;
}

Region* Bound::getDestinationRegion()
{
	return destinationRegion;
}

void Bound::setBoundType(string boundType)
{
	this->boundType = boundType;
}

std::string Bound::getBoundType()
{
	return boundType;
}

bool Bound::operator()(const Bound* t1, const Bound* t2)
{
	return t1->upperBound < t2->upperBound;
}

bool Bound::operator<(const Bound& ti) const
{
	return upperBound < ti.upperBound;
}
