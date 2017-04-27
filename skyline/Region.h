#pragma once
#include "BorderNode.h"
#include <vector>
using namespace std;

class Region
{
public:
	Region();
	Region(int, double, double, double, double);
	~Region();
	void setId(int);
	void setLongtitudeLeft(double);
	void setLongtitudeRight(double);
	void setLatitudeBottom(double);
	void setLatitudeTop(double);
	void setBorderNodeList(BorderNode*);
	void setNodeList(Node*);
	void setMinLambda(double);
	void setMaxLambda(double);
	void setBNNTableList(Destination*);
	void setNodeAmount(int);
	void setMinRRDist(int);
	void setMaxRRDist(int);
	void setMinDeltaList(Destination*);
	void setToQueryDestinationList(Destination*);
	void setRegionNodePathList(Destination*);

	int getId();
	double getLongtitudeLeft();
	double getLongtitudeRight();
	double getLatitudeBottom();
	double getLatitudeTop();
	vector<BorderNode*> getBorderNodeList();
	vector<Node*> getNodeList();
	double getMinLambda();
	double getMaxLambda();
	vector<Destination*> getBNNTableList();
	int getNodeAmount();
	vector<int> getMinRRDistList();
	vector<int> getMaxRRDistList();
	vector<Destination*> getMinDeltaList();
	vector<Destination*> getToQueryDestinationList();
	vector<Destination*> getRegionNodePathList();

private:
	int id;
	int nodeAmount;
	double longtitudeLeft, longtitudeRight, latitudeBottom, latitudeTop, minLambda, maxLambda;
	vector<BorderNode*> borderNodeList;
	vector<Node*> nodeList;
	vector<Destination*> bNNTableList;
	vector<Destination*> minLambdaList;
	vector<Destination*> toQueryDestinationList;
	vector<Destination*> regionNodePathList;
	vector<int> minRRDistList;
	vector<int> maxRRDistList;
};

