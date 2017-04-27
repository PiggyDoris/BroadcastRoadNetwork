#include "stdafx.h"
#include "PointComponent.h"


PointComponent::PointComponent()
{
	pointX = 0;
	pointY = 0;
	pointA = 0;
	pointB = 0;
	total = 0;
	nonSpatialNum=0;
}


PointComponent::~PointComponent()
{
}

int PointComponent::getPointX()
{
	return pointX;
}

int PointComponent::getPointY()
{
	return pointY;
}


void PointComponent::setPointX(int x)
{
	pointX = x;
}


void PointComponent::setPointY(int y)
{
	pointY = y;
}


int PointComponent::getPointA()
{
	return pointA;
}

void PointComponent::setPointA(int a)
{
	pointA = a;
}

int PointComponent::getPointB()
{
	return pointB;
}

void PointComponent::setPointB(int b)
{
	pointB = b;
}

int PointComponent::getTotal()
{
	total = pointA + pointB + pointX + pointY;
	return total;
}

int PointComponent::getNonSpatialNum()
{
	nonSpatialNum = pointA + pointB;
	return nonSpatialNum;
}


bool PointComponent::getIsSkyline()
{
	return isSkyline;
}

void PointComponent::setIsSkyline(bool b)
{
	isSkyline = b;
}