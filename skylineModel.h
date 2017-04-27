#include <string>
#include <vector>
#include <iostream>
#include "PointComponent.h"
#include "mbrInfo.h"
#include "Node.h"
#include "Road.h"
#include "Region.h"
#include "BorderNode.h"
#include "Client.h"
#include "NPIClient.h"
#include <direct.h>
#include <fstream>
#include <cstring>
#include <windows.h>


#pragma once

using namespace std;

class skylineModel
{
public:
	skylineModel();
	~skylineModel();
	vector<PointComponent*>* getPoints();
	vector<PointComponent*>* findSkyline(vector<PointComponent*>* points);
	void create(int);
	
	void display();
	void loadFile();
	void writeToMBRFile();
	void writeLandmarkToMBRFile();
	void writeRoadToMBRFile();
	void writeLocation(mbrInfo*,mbrInfo*);
	void findNonSpatialSkyline();
	void displaySkyline(vector<PointComponent*>);
	void findIntersection();
	double rad(double);

	void precomputationIntegrate();
	void copyOriginalData(string, string);
	void writeGridPartition(string, string, int, int);
	void writeAllBorderRoadDistance(string, string);
	void writeRegionAllNodeBorderData(string, string);
	void writeBorderRoadDistanceAndRegion(string, string);
	void dijkstra(string, string);
	void writeAllOriginRoadDistance();
	void writeRegionNodeData(string, string);
	void writeOrderedIdRegionNodeData(string, string);
	void writeRegionBorderData(string, string);
	void writeBBDist(string, string);
	void writeRandomSpots(string, string, int);
	void writeKNNAnswer(string, string);
	void writeBNAndNNDist(string, string, string, int);
	void writeRegionMaxLamdaDist(string, string, string, string);
	void writeBNNTable(string, string, string, string);
	void writeNewBNNTable(string, string, string, string);
	void writeNewSpotBNNTable(string, string, string);
	void writeAllMaxDistance(string, string, string);
	void writeIndex(string, string, string, string, string, int);
	void writeBorderList(string, string);
	void writeBNAndNSDist(string, string, string, string, int);

	void writeNPIIndex(string, string, string);
	void writeMinMaxRR(string, string, string);
	vector<int> findBorderNodeRegion(vector<Region*>, string nodeName);
	
	vector<int> getBNNTableInNLayers(string, int, int);
	int getRegionNumber(string);
	Node* randQueryPoint(string);
	vector<int> nonRepeatRandAndSort(int, int);
	vector<int> getNodeLineNumberListObjreg(string);
	vector<Region*> getRegionListByBound(vector<double>, vector<double>);
	int checkNodeRegion(vector<Region*>, Node*);
	vector<BorderNode*> uniteBorderListWithAllRegion(vector<Region*>);
	vector<BorderNode*> uniteTwoBorderList(vector<BorderNode*>, vector<BorderNode*>);
	bool checkNodeRoadIdExist(Node*, int);
	int checkNodeExistAndReturnNodeInd(vector<Node*>, Node*);
	int checkBorderNodeExistAndReturnNodeInd(vector<BorderNode*>, BorderNode*);
	double computeRoadDistance(vector<Node*>);
	double computeSectionDistance(double, double, double, double);
	bool isBorderNodeInside(Road*, vector<double>, vector<double>);
	void addBorderNodeToList(Road*, vector<double>, vector<double>);
	BorderNode* findBorderNode(Road*, double, double);
	void deleteNodeList(vector<Node*>);
	void deleteRoadList(vector<Road*>);
	void deleteFileLastLine(string, string);

	int align(char*);

private:
	int clientProcessedFlag;
	vector<PointComponent*> points;
	vector<PointComponent*> skylinePoints;
	vector<BorderNode*> borderNodeList;
};

