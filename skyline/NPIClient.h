#pragma once
#include <iostream>
#include <fstream>
#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>  
#include <string>
#include <vector>
#include <algorithm>
#include <iomanip> //setprecision會用到的函式庫
#include <math.h>
#include "Destination.h"
#include "Node.h"
#include "BorderNode.h"
#include "Region.h"
#include "Bound.h"
#include "ListUDG.h"
using namespace std;
class NPIClient
{
public:
	NPIClient();
	~NPIClient();
	void setStartRegionId(int);
	void setPacketSize(int);
	void setQueryAmountK(int);
	void setQueryPoint(Node*);
	int getStartIndexId();
	int getPacketSize();
	int getQueryAmountK();
	Node* getQueryPoint();
	int getTuningDataSize();
	vector<Node*> getQueryKNNList();
	int getLatencyDataSize();
	int getBroadcastCycleSize();

	void getRandQueryPoint(string);
	void listenBroadcastIndex(string, string, string, string);
	void listenBroadcastData(string);
	void locateQueryPointRegion();
	void computeCandidateRegion();
	void computeKNNAnswer(string, string, string, string, string, string, int, int);
	void computeUpperBound();
	void computeLowerBound();
	void computeRRMinDist();

	int checkBorderNodeExistByName(vector<BorderNode*>, string);
	vector<int> nonRepeatRandAndSort(int, int);
	void addRecievedBytes(int);
	void computePacketSize();
	vector<int> findBorderNodeRegion(string);
	void computeToQueryDijkstra();
	void getAllRegionBorderNodeLine();
	void computePerIndexSize(string, string, string, string);
	void computePerDataSize(string);
	int computeDataLatencySize(string, int, int);
	int computeIndexLatencySize(int, int);


private:
	int queryAmountK;
	int processFlag;
	int startRegionId;
	int sizePerPacket;		//bytes
	int recievedPacketNum;
	int recievedBytes;
	int xPart, yPart;
	int bbMaxDistSize;
	int tuningTime;
	int latency;
	int nowRecievedNodeAmount;
	int fixedIndexSize;
	int latencyDataSize;
	int tuningDataSize;
	int broadcastCycleSize;
	bool firstComputeCandidateFlag;
	bool queryFinishedFlag;
	double upperBound;
	vector<Bound*> upperBoundList;
	vector<Bound*> lowerBoundList;
	string partitionType;
	vector<double> xPartList;
	vector<double> yPartList;
	vector<int> broadcastRegionIdSequence;
	vector<int> allRegionMaxdistList;
	vector<int> candidateRegionIdList;
	vector<int> allRegionNodeAmount;
	vector<int> recievedRegionList;
	vector<int> recievedRList;
	vector<int> recievedCandidateRegionIdList;
	vector<int> candidateRegionToListenList;
	vector<BorderNode*> borderGraphList;
	Node* queryPoint;
	vector<Node*> queryKNNList;
	vector<Region*> allRegionList;
	vector<Region*> candidateRegionList;
};

