#pragma once
class PointComponent
{
public:
	PointComponent();
	~PointComponent();
	int getPointX();
	void setPointX(int);
	int getPointY();
	void setPointY(int);
	int getPointA();
	void setPointA(int);
	int getPointB();
	void setPointB(int);
	int getTotal();
	int getNonSpatialNum();

	bool getIsSkyline();
	void setIsSkyline(bool);
private:
	int pointX, pointY, pointA, pointB, total, nonSpatialNum;
	bool isSkyline = true;

};

