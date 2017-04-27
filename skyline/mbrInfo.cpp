#include "stdafx.h"
#include "mbrInfo.h"


mbrInfo::mbrInfo()
{
}


mbrInfo::~mbrInfo()
{
}


int mbrInfo::getMbrX_dec()
{
	return mbrX_dec;
}

void mbrInfo::setMbrX_dec(int x)
{
	mbrX_dec = x;
}

int mbrInfo::getMbrX_int()
{
	return mbrX_int;
}

void mbrInfo::setMbrX_int(int x)
{
	mbrX_int = x;
}

int mbrInfo::getMbrY_dec()
{
	return mbrY_dec;
}

void mbrInfo::setMbrY_dec(int y)
{
	mbrY_dec = y;
}

int mbrInfo::getMbrY_int()
{
	return mbrY_int;
}

void mbrInfo::setMbrY_int(int y)
{
	mbrY_int = y;
}
