#pragma once
#include <stdio.h>
#include <iostream>
#include "Intersection.h"
class NormalIntersection :
	public Intersection
{
public:
	NormalIntersection();
	~NormalIntersection();

private:
	string nodeType;
};

