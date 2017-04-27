#include "stdafx.h"
#include "NPIClient.h"

#define _CRTDBG_MAP_ALLOC 
#define SIZE 100000
#define pi 3.1415926535897932384626433832795
#define EARTH_RADIUS 6378.137 //地球半徑 KM
#define _CRT_SECURE_NO_WARNINGS

NPIClient::NPIClient()
{
	sizePerPacket = 1024; //file size = 1KB
	recievedPacketNum = 0;
	startRegionId = 0;
	xPart = 0;
	yPart = 0;
	bbMaxDistSize = 0;
	firstComputeCandidateFlag = true;
	processFlag = 0;
	tuningDataSize = 0;
	fixedIndexSize = 0;
}


NPIClient::~NPIClient()
{
}

void NPIClient::getRandQueryPoint(string inputDir)
{
	int lineNumber = 0;
	int attributeFlag = 0;
	vector<int> nodeLineNumberList;
	char line[SIZE];
	char *pch;
	string str1;
	char *delim = ",";

	Node* regQueryPoint;
	fstream fin;
	fin.open(inputDir, ios::in);

	while (fin.getline(line, sizeof(line), '\n'))	//以行讀取
	{
		lineNumber++;
		pch = line;
		str1 = "";
		str1.append(pch);
		if (str1.find("n_", 0) != string::npos)
		{
			nodeLineNumberList.push_back(lineNumber);
		}
	}
	fin.close();
	lineNumber = 0;
	fin.open(inputDir, ios::in);

	vector<int> randIdList = nonRepeatRandAndSort(nodeLineNumberList.size(), 1);
	while (fin.getline(line, sizeof(line), '\n')){	//以行讀取
		lineNumber++;
		if (lineNumber > 3)
		{
			pch = strtok(line, delim);

			if (nodeLineNumberList.at(randIdList.back()) == lineNumber)
			{
				//cout << randIdList.at(index) << endl;
				while (pch != NULL)
				{
					attributeFlag++;
					if (attributeFlag == 1)
					{
						regQueryPoint = new Node();
						regQueryPoint->setNodeName(pch);
					}
					else if (attributeFlag == 2)
					{
						regQueryPoint->setLongtitude(strtod(pch, NULL));
					}
					else if (attributeFlag == 3)
					{
						regQueryPoint->setLatitude(strtod(pch, NULL));
						queryPoint = regQueryPoint;
						cout << setprecision(9) << "Query Point : " << queryPoint->getNodeName() << "," << queryPoint->getLongtitude() << "," << queryPoint->getLatitude() << endl;
					}
					pch = strtok(NULL, delim);
				}
			}
		}
	}
}

void NPIClient::listenBroadcastIndex(string inputDir, string inputDir1, string inputDir2, string inputDir3)
{
	int nowRegionId;
	Region* nowRegion;
	int lineNumber = 0;
	vector<int> bNNTableList;
	char line[SIZE];
	char *pch;
	char *delim = ",";

	Node* regQueryPoint;
	fstream fin;

	//get .index. Read region information(coordinate, lambda, bnntableList)
	fin.open(inputDir + to_string(startRegionId), ios::in);
	while (fin.getline(line, sizeof(line), '\n'))	//以行讀取
	{
		int attributeFlag = 0;
		lineNumber++;
		pch = strtok(line, delim);
		if (lineNumber == 1)
		{
			nowRegionId = atoi(pch);
			startRegionId = nowRegionId;
			addRecievedBytes(sizeof(nowRegionId));
		}
		else if (lineNumber == 2)
		{
			if (xPart*yPart == 0)
			{
				while (pch != NULL)
				{
					attributeFlag++;
					if (attributeFlag == 1)
						partitionType = pch;
					else if (attributeFlag == 2)
						xPart = atoi(pch);
					else if (attributeFlag == 3)
						yPart = atoi(pch);
					pch = strtok(NULL, delim);
				}
			}
			addRecievedBytes(sizeof(char)* 4);
			addRecievedBytes(sizeof(xPart));
			addRecievedBytes(sizeof(yPart));
		}
		else if (lineNumber == 3)
		{
			int xPartCounter = 0;
			if (xPartList.size() == 0)
			{
				while (pch != NULL)
				{
					xPartList.push_back(strtod(pch, NULL));
					pch = strtok(NULL, delim);
					xPartCounter++;
				}
			}
			addRecievedBytes(sizeof(xPartList.back()) * xPartCounter);
		}
		else if (lineNumber == 4)
		{
			int yPartCounter = 0;
			if (yPartList.size() == 0)
			{
				while (pch != NULL)
				{
					yPartList.push_back(strtod(pch, NULL));
					pch = strtok(NULL, delim);
				}
				int regionCounter = 0;
				for (int i = 0; i < xPartList.size() - 1; i++)
				{
					for (int j = 0; j < yPartList.size() - 1; j++)
					{
						Region* regRegion = new Region(regionCounter, xPartList.at(i), xPartList.at(i + 1), yPartList.at(j), yPartList.at(j + 1));
						allRegionList.push_back(regRegion);
						regionCounter++;
					}
				}
			}
			addRecievedBytes(sizeof(yPartList.front()) * yPartCounter);
		}
		else if (lineNumber == 5)
		{
			while (pch != NULL)
			{
				//attributeFlag++;
				//if (startRegionId == attributeFlag - 1)
				//	nowRegion->setNodeAmount(atoi(pch));
				allRegionNodeAmount.push_back(atoi(pch));
				addRecievedBytes(sizeof(atoi(pch)));
				pch = strtok(NULL, delim);
			}
			for (int i = 0; i < allRegionList.size(); i++)
				allRegionList.at(i)->setNodeAmount(allRegionNodeAmount.at(i));
		}
	}
	fin.close();
	
	//get .bb
	if (bbMaxDistSize == 0)
	{
		//minRRDist
		lineNumber = 0;
		fin.open(inputDir1, ios::in);
		while (fin.getline(line, sizeof(line), '\n'))	//以行讀取
		{
			lineNumber++;
			pch = strtok(line, delim);
			while (pch != NULL)
			{
				allRegionList.at(lineNumber - 1)->setMinRRDist(atoi(pch));
				pch = strtok(NULL, delim);
			}
		}
		fin.close();

		//maxRRDist
		lineNumber = 0;
		fin.open(inputDir2, ios::in);
		while (fin.getline(line, sizeof(line), '\n'))	//以行讀取
		{
			lineNumber++;
			pch = strtok(line, delim);
			while (pch != NULL)
			{
				allRegionList.at(lineNumber - 1)->setMaxRRDist(atoi(pch));
				pch = strtok(NULL, delim);
			}
		}
		fin.close();

		//.maxdist
		lineNumber = 0;
		fin.open(inputDir3, ios::in);
		if (allRegionMaxdistList.size() == 0)
		{
			while (fin.getline(line, sizeof(line), '\n'))	//以行讀取
			{
				lineNumber++;
				int attributeFlag = 0;
				pch = strtok(line, delim);
				while (pch != NULL)
				{
					attributeFlag++;
					if (attributeFlag == 1)
						allRegionMaxdistList.push_back(atoi(line));
					break;

					pch = strtok(NULL, delim);
				}
			}
		}
		bbMaxDistSize += sizeof(allRegionList.back()->getMaxLambda()) * 2;
		bbMaxDistSize += sizeof(allRegionMaxdistList.back()) * allRegionMaxdistList.size();
		addRecievedBytes(sizeof(allRegionMaxdistList.back()) * allRegionMaxdistList.size());
		fin.close();
	}
	else
	{
		addRecievedBytes(bbMaxDistSize);
	}

	//compute packet
	computePerIndexSize(inputDir + to_string(startRegionId), inputDir1, inputDir2, inputDir3);
	computePacketSize();
	recievedRegionList.push_back(startRegionId);
}

void NPIClient::locateQueryPointRegion()
{
	for (int i = 0; i < allRegionList.size(); i++)
	{
		if (allRegionList.at(i)->getLatitudeBottom() <= queryPoint->getLatitude() && allRegionList.at(i)->getLatitudeTop() >= queryPoint->getLatitude() && allRegionList.at(i)->getLongtitudeLeft() <= queryPoint->getLongtitude() && allRegionList.at(i)->getLongtitudeRight() >= queryPoint->getLongtitude())
			queryPoint->setNodeRegionId(allRegionList.at(i)->getId());
	}
	cout << "Query Point region : " << queryPoint->getNodeRegionId() << endl;
}

void NPIClient::listenBroadcastData(string inputDir)
{
	//get .bnnnX
	char line[SIZE];
	fstream fin;
	int lineNumber = 0;
	char *pch;
	char *delim = ",";
	fin.open(inputDir, ios::in);
	vector<BorderNode*> regBorderNodeList;
	vector<Node*> spotList;
	//get all node
	while (fin.getline(line, sizeof(line), '\n'))	//以行讀取
	{
		int attributeFlag = 0;
		lineNumber++;
		pch = strtok(line, delim);
		if (lineNumber == 1)
		{
			while (pch != NULL)
			{
				Node* regNode = new Node();
				regNode->setNodeName(pch);
				allRegionList.at(startRegionId)->setNodeList(regNode);
				pch = strtok(NULL, delim);
			}
		}
		else
		{
			while (pch != NULL)
			{
				attributeFlag++;
				if (attributeFlag == 1)
				{
					string str1 = "";
					str1.append(pch);
					if (str1.find("n_", 0) != string::npos)
					{
						vector<Node*> regRegionNodeList = allRegionList.at(startRegionId)->getNodeList();
						for (int i = 0; i < regRegionNodeList.size(); i++)
						{
							if (regRegionNodeList.at(i)->getNodeName() == pch)
							{
								regRegionNodeList.at(i)->setIsSpot(true);
								spotList.push_back(regRegionNodeList.at(i));
								break;
							}
						}
					}
					else
					{
						BorderNode* regBorderNode = new BorderNode();
						regBorderNode->setNodeName(pch);
						allRegionList.at(startRegionId)->setBorderNodeList(regBorderNode);
						regBorderNodeList.push_back(regBorderNode);
					}
					addRecievedBytes(sizeof(pch));
					break;
				}
				pch = strtok(NULL, delim);
			}
		}

	}
	fin.close();

	lineNumber = 0;
	fin.open(inputDir, ios::in);
	//get all distance
	int borderListInd = 0;
	int regionNodeListInd = 0;
	while (fin.getline(line, sizeof(line), '\n'))	//以行讀取
	{
		lineNumber++;
		int attributeFlag = 0;
		pch = strtok(line, delim);
		Node* nowNode;
		string str1 = "";
		str1.append(pch);
		if (lineNumber == 1)
			continue;
		if (str1.find("b_", 0) != string::npos)
		{
			nowNode = regBorderNodeList.at(borderListInd);
			borderListInd++;
		}
		else
		{
			nowNode = spotList.at(regionNodeListInd);
			regionNodeListInd++;
		}
		while (pch != NULL)
		{
			attributeFlag++;
			if (attributeFlag != 1)
			{
				Node* regNode = allRegionList.at(startRegionId)->getNodeList().at(attributeFlag - 2);
				Destination* regDestination = new Destination();
				regDestination->setDestinationNode(regNode);
				regDestination->setStartPointNode(nowNode);
				regDestination->setPathDistance(strtod(pch, NULL));
				nowNode->setDestinationList(regDestination);

				allRegionList.at(startRegionId)->setRegionNodePathList(regDestination);
				addRecievedBytes(sizeof(regDestination->getPathDistance()));
			}
			pch = strtok(NULL, delim);
		}
	}
	computePerDataSize(inputDir);
	startRegionId++;
}

void NPIClient::computeCandidateRegion()
{
	computeUpperBound();
	computeLowerBound();

	if (firstComputeCandidateFlag == true)
	{
		for (int i = 0; i < lowerBoundList.size(); i++)
		{
			if (lowerBoundList.at(i)->getUpperBound() != -1 && lowerBoundList.at(i)->getUpperBound() < upperBound)
				candidateRegionIdList.push_back(lowerBoundList.at(i)->getDestinationRegionId());
		}
	}

	//get toListenCandidateList
	candidateRegionToListenList = candidateRegionIdList;
	for (int i = 0; i < recievedRegionList.size(); i++)
	{
		for (int j = 0; j < candidateRegionToListenList.size(); j++)
		{
			if (recievedRegionList.at(i) == candidateRegionToListenList.at(j))
			{
				candidateRegionToListenList.erase(candidateRegionToListenList.begin() + j);
				break;
			}
		}
	}

	//get now recieved node amount
	nowRecievedNodeAmount = 0;
	recievedCandidateRegionIdList.clear();
	for (int i = 0; i < recievedRegionList.size(); i++)
	{
		for (int j = 0; j < candidateRegionIdList.size(); j++)
		{
			if (recievedRegionList.at(i) == candidateRegionIdList.at(j))
			{
				nowRecievedNodeAmount += allRegionNodeAmount.at(candidateRegionIdList.at(j));
				recievedCandidateRegionIdList.push_back(candidateRegionIdList.at(j));
			}
		}
	}

	firstComputeCandidateFlag = false;

	cout << endl;
	cout << "Query Region R" << queryPoint->getNodeRegionId() << endl;
	cout << "Now Broadcast R" << startRegionId << endl;
	cout << "Candidate List : " << endl;
	for (int i = 0; i < candidateRegionIdList.size(); i++)
		cout << "R" << candidateRegionIdList.at(i) << endl;
}

void NPIClient::computeUpperBound()
{
	//get query to border max EudDist
	//int queryPointRegionId = queryPoint->getNodeRegionId();
	vector<int> queryRRMinDistList = allRegionList.at(queryPoint->getNodeRegionId())->getMinRRDistList();
	vector<int> queryRRMaxDistList = allRegionList.at(queryPoint->getNodeRegionId())->getMaxRRDistList();
	int queryRegionMaxDist = allRegionMaxdistList.at(queryPoint->getNodeRegionId());

	if (upperBoundList.size() == 0)
	{
		for (int i = 0; i < queryRRMinDistList.size(); i++)
		{
			Bound* regUpperBound = new Bound("UpperBound");
			regUpperBound->setQueryPoint(queryPoint);
			regUpperBound->setQueryRegion(allRegionList.at(queryPoint->getNodeRegionId()));
			regUpperBound->setQueryRegionId(queryPoint->getNodeRegionId());
			regUpperBound->setDestinationRegion(allRegionList.at(i));
			regUpperBound->setDestinationRegionId(i);
			if (queryRRMinDistList.at(i) == -1 || allRegionList.at(i)->getNodeAmount() == 0)
			{
				regUpperBound->setUpperBound(-1);
				upperBoundList.push_back(regUpperBound);
				continue;
			}
			if (i == queryPoint->getNodeRegionId())
				regUpperBound->setUpperBound(allRegionMaxdistList.at(i));
			else
				regUpperBound->setUpperBound(queryRegionMaxDist + queryRRMaxDistList.at(i) + allRegionMaxdistList.at(i));
			upperBoundList.push_back(regUpperBound);
		}
		sort(upperBoundList.begin(), upperBoundList.end(), Bound());

		int nowNodeAmount = 0;
		double regUpperBound = 0;
		int ubListInd = 0;
		//need all region node amount
		while (nowNodeAmount < queryAmountK)
		{
			int regRegionId = upperBoundList.at(ubListInd)->getDestinationRegionId();
			nowNodeAmount += allRegionNodeAmount.at(regRegionId);
			regUpperBound = upperBoundList.at(ubListInd)->getUpperBound();
			ubListInd++;
		}
		upperBound = regUpperBound;
	}

	cout << "upper bound : " << upperBound << endl;
	for (int i = 0; i < upperBoundList.size(); i++)
	{
		cout << "Region" << upperBoundList.at(i)->getDestinationRegionId() << " UB :" << upperBoundList.at(i)->getUpperBound() << endl;
	}

	for (int i = 0; i < lowerBoundList.size(); i++)
	{
		cout << "Region" << lowerBoundList.at(i)->getDestinationRegionId() << " LB :" << lowerBoundList.at(i)->getUpperBound() << endl;
	}

	//UPDATE
}

void NPIClient::computeLowerBound()
{
	//REMEMBER TO UPDATE
	vector<int> queryRRMinDistList = allRegionList.at(queryPoint->getNodeRegionId())->getMinRRDistList();

	//compute lower bound
	if (lowerBoundList.size() == 0)
	{
		for (int i = 0; i < allRegionList.size(); i++)
		{
			double regLowerBound = allRegionList.at(queryPoint->getNodeRegionId())->getMinRRDistList().at(i);
			if (queryPoint->getNodeRegionId() == i)
				regLowerBound = 0;
			else if (allRegionList.at(queryPoint->getNodeRegionId())->getMinRRDistList().at(i) == -1 || allRegionList.at(i)->getNodeAmount() == 0)
				regLowerBound = -1;
			Bound* regBound = new Bound();
			regBound->setBoundType("LowerBound");
			regBound->setUpperBound(regLowerBound);
			regBound->setQueryPoint(queryPoint);
			regBound->setQueryRegionId(queryPoint->getNodeRegionId());
			regBound->setDestinationRegion(allRegionList.at(i));
			regBound->setDestinationRegionId(i);
			lowerBoundList.push_back(regBound);
			cout << "R" << regBound->getQueryRegionId() << " -> R" << regBound->getDestinationRegionId() << " : " << regBound->getUpperBound() << endl;
		}
	}
}

void NPIClient::computeKNNAnswer(string inputDir, string inputDir1, string inputDir2, string inputDir3, string inputDir4, string inputDir5, int queryAmountK, int startRegionId)
{
	bool queryRegionRecievedFlag = false;
	if (processFlag == 0)
	{
		if (queryPoint == NULL)
			getRandQueryPoint(inputDir);
		this->queryAmountK = queryAmountK;
		this->startRegionId = startRegionId;
		processFlag = 1;
	}
	while (processFlag == 1 || candidateRegionToListenList.size() != 0)
	{
		bool isRegionCandidate = false;
		for (int i = 0; i < candidateRegionToListenList.size(); i++)
		{
			if (candidateRegionToListenList.at(i) == this->startRegionId)
			{
				isRegionCandidate = true;
				break;
			}
		}
		//continue if not candidate Region & not first time process
		if (isRegionCandidate == false && processFlag != 1)
		{
			this->startRegionId++;
			continue;
		}
		else
			recievedRList.push_back(this->startRegionId);

		listenBroadcastIndex(inputDir1, inputDir2, inputDir3, inputDir4);
		if (processFlag == 1)
		{
			locateQueryPointRegion();
			processFlag = 2;
		}
		computeCandidateRegion();
		listenBroadcastData(inputDir5 + to_string(this->startRegionId));

		//if finished listening cycle tail, start next listen from head
		if (this->startRegionId == allRegionList.size())
			this->startRegionId = 0;
	}

	//do dijkstra
	computeToQueryDijkstra();
	queryFinishedFlag = true;

	if (this->startRegionId == 0)
	{
		latencyDataSize = computeIndexLatencySize(startRegionId, allRegionList.size() - 1);
		latencyDataSize += computeDataLatencySize(inputDir5, startRegionId, allRegionList.size() - 1);
	}
	else
	{
		latencyDataSize = computeIndexLatencySize(startRegionId, this->startRegionId - 1);
		latencyDataSize += computeDataLatencySize(inputDir5, startRegionId, this->startRegionId - 1);
	}
	broadcastCycleSize = computeIndexLatencySize(0, allRegionList.size() - 1);
	broadcastCycleSize += computeDataLatencySize(inputDir5, 0, allRegionList.size() - 1);

	cout << endl << "///////////////////////////" << endl;
	cout << "NPI Recieved Region : " << endl;
	for (int i = 0; i < recievedRList.size(); i++)
		cout << "R" << recievedRList.at(i) << endl;

	cout << endl;
	cout << "Query Point : " << queryPoint->getNodeName() << ", R" << queryPoint->getNodeRegionId() << endl;
	cout << "Query KNN Answer : " << endl;
	for (int i = 0; i < queryKNNList.size(); i++)
	{
		cout << queryKNNList.at(i)->getNodeName() << ", " << queryKNNList.at(i)->getToQueryDistance() << endl;
	}
}

void NPIClient::computeToQueryDijkstra()
{
	//query region destination list
	Region* regQueryRegion = allRegionList.at(queryPoint->getNodeRegionId());
	vector<Destination*> regQueryRegionPathList = regQueryRegion->getRegionNodePathList();

	//border to border destination list
	vector<Destination*> regToQueryDestinationList;
	for (int i = 0; i < recievedCandidateRegionIdList.size(); i++)
	{
		vector<Destination*> regToQueryList = allRegionList.at(recievedCandidateRegionIdList.at(i))->getToQueryDestinationList();
		for (int j = 0; j < regToQueryList.size(); j++)
			regToQueryDestinationList.push_back(regToQueryList.at(j));
	}

	//destination region destination list
	vector<Destination*> regCandidateDestinationList;
	for (int i = 0; i < recievedCandidateRegionIdList.size(); i++)
	{
		vector<Destination*> regPathList = allRegionList.at(recievedCandidateRegionIdList.at(i))->getRegionNodePathList();
		for (int j = 0; j < regPathList.size(); j++)
			regCandidateDestinationList.push_back(regPathList.at(j));
	}


	//--------用第一個點的 regionId 查詢 bnnn 中的 borderNode節點距離 

	vector<string> dijkstraNodeList;
	vector<EData*> edgesList;

	//push query node name
	dijkstraNodeList.push_back(queryPoint->getNodeName());

	//push query region border node name
	for (int j = 0; j < regQueryRegion->getBorderNodeList().size(); j++)
		dijkstraNodeList.push_back(regQueryRegion->getBorderNodeList().at(j)->getNodeName());

	//push destination region border name
	for (int i = 0; i < recievedCandidateRegionIdList.size(); i++)
	{
		if (recievedCandidateRegionIdList.at(i) == queryPoint->getNodeRegionId())
			continue;
		Region* regDestinationRegion = allRegionList.at(recievedCandidateRegionIdList.at(i));
		for (int j = 0; j < regDestinationRegion->getBorderNodeList().size(); j++)
		{
			bool borderNodeExistFlag = false;
			for (int k = 0; k < dijkstraNodeList.size(); k++)
			{
				if (dijkstraNodeList.at(k) == regDestinationRegion->getBorderNodeList().at(j)->getNodeName())
				{
					borderNodeExistFlag = true;
					break;
				}
			}
			if (borderNodeExistFlag == false)
				dijkstraNodeList.push_back(regDestinationRegion->getBorderNodeList().at(j)->getNodeName());
		}
	}

	int destinationNodeAmount = 0;
	vector<bool> isSpot;
	//push query region node (exclude queryPoint)
	for (int j = 0; j < regQueryRegion->getNodeList().size(); j++)
	{
		if (regQueryRegion->getNodeList().at(j)->getNodeName() != queryPoint->getNodeName())
		{
			dijkstraNodeList.push_back(regQueryRegion->getNodeList().at(j)->getNodeName());
			isSpot.push_back(regQueryRegion->getNodeList().at(j)->getIsSpot());
			destinationNodeAmount++;
		}
	}

	//push destination region node name
	for (int i = 0; i < recievedCandidateRegionIdList.size(); i++)
	{
		if (recievedCandidateRegionIdList.at(i) == queryPoint->getNodeRegionId())
			continue;
		Region* regDestinationRegion = allRegionList.at(recievedCandidateRegionIdList.at(i));
		for (int j = 0; j < regDestinationRegion->getNodeList().size(); j++)
		{
			dijkstraNodeList.push_back(regDestinationRegion->getNodeList().at(j)->getNodeName());
			isSpot.push_back(regDestinationRegion->getNodeList().at(j)->getIsSpot());
			destinationNodeAmount++;
		}
	}

	//push query minDelta list to EData
	for (int j = 0; j < regQueryRegionPathList.size(); j++)
		edgesList.push_back(new EData(regQueryRegionPathList.at(j)->getStartPointNode()->getNodeName(), regQueryRegionPathList.at(j)->getDestinationNode()->getNodeName(), regQueryRegionPathList.at(j)->getPathDistance()));

	//push border border list to EData
	for (int j = 0; j < regToQueryDestinationList.size(); j++)
		edgesList.push_back(new EData(regToQueryDestinationList.at(j)->getStartPointNode()->getNodeName(), regToQueryDestinationList.at(j)->getDestinationNode()->getNodeName(), regToQueryDestinationList.at(j)->getPathDistance()));

	//push border node list to EData
	for (int j = 0; j < regCandidateDestinationList.size(); j++)
		edgesList.push_back(new EData(regCandidateDestinationList.at(j)->getStartPointNode()->getNodeName(), regCandidateDestinationList.at(j)->getDestinationNode()->getNodeName(), regCandidateDestinationList.at(j)->getPathDistance()));


	//		cout << "done" << endl;
	int prev[MAX] = { 0 };
	int dist[MAX] = { 0 };

	int vlen = dijkstraNodeList.size();
	int elen = edgesList.size();

	ListUDG* pG;

	pG = new ListUDG(dijkstraNodeList, vlen, edgesList, elen);
	vector<int> candidateNodeDistList = pG->bottomDijkstra(0, prev, dist, destinationNodeAmount);
	vector<Node*> kNNList;

	int regMinInd;
	int regMinDist = -1;
	for (int i = 0; i < queryAmountK; i++)
	{
		for (int j = 0; j < candidateNodeDistList.size(); j++)
		{
			//if is spot & lower then now minimum, push back
			if ((regMinDist == -1 || regMinDist > candidateNodeDistList.at(j)) && candidateNodeDistList.at(j) != -1 && candidateNodeDistList.at(j) < upperBound && isSpot.at(j) == true)
			{
				regMinDist = candidateNodeDistList.at(j);
				regMinInd = j;
			}
		}
		if (regMinDist != -1)
		{
			Node* regMinNode = new Node();
			regMinNode->setNodeName(dijkstraNodeList.at(dijkstraNodeList.size() - destinationNodeAmount + regMinInd));
			regMinNode->setToQueryDistance(regMinDist);
			cout << "Candidate node : " << regMinNode->getNodeName() << ", " << regMinNode->getToQueryDistance() << endl;
			kNNList.push_back(regMinNode);
			candidateNodeDistList.at(regMinInd) = -1;
			regMinDist = -1;
		}
	}

	queryKNNList = kNNList;

	//Update candidate region
	candidateRegionIdList.clear();
	for (int i = 0; i < lowerBoundList.size(); i++)
	{
		if (lowerBoundList.at(i)->getUpperBound() != -1 && lowerBoundList.at(i)->getUpperBound() < kNNList.back()->getToQueryDistance())
			candidateRegionIdList.push_back(lowerBoundList.at(i)->getDestinationRegionId());
	}

	//Update candidate to listen region
	for (int i = 0; i < candidateRegionToListenList.size(); i++)
	{
		bool toListenRegionExistFlag = false;
		for (int j = 0; j < candidateRegionIdList.size(); j++)
		{
			if (candidateRegionIdList.at(j) == candidateRegionToListenList.at(i))
			{
				toListenRegionExistFlag = true;
				break;
			}
		}
		if (toListenRegionExistFlag == false)
			candidateRegionToListenList.erase(candidateRegionToListenList.begin() + i);
	}
}


vector<int> NPIClient::nonRepeatRandAndSort(int total, int needNum)
{
	vector<int> card;
	vector<int> randomList;
	srand(time(NULL));
	for (int i = 0; i < total; i++)
	{
		card.push_back(i);
	}
	for (int index = 0; index < total; index++)
	{
		int n = rand() % total;
		int m = rand() % total;
		swap(card.at(m), card.at(n));
	}
	for (int index = 0; index < needNum; index++)
	{
		randomList.push_back(card.at(index));
	}
	sort(randomList.begin(), randomList.end());
	return randomList;
}

void NPIClient::setPacketSize(int packetSize)
{
	this->sizePerPacket = packetSize;
}

int NPIClient::getPacketSize()
{
	return sizePerPacket;
}

void NPIClient::setStartRegionId(int startRegionId)
{
	this->startRegionId = startRegionId;
}

int NPIClient::getStartIndexId()
{
	return startRegionId;
}

void NPIClient::addRecievedBytes(int inputSize)
{
	recievedBytes = recievedBytes + inputSize;
	//cout << "Now Recieved Bytes : " << recievedBytes << endl;
}

void NPIClient::computePacketSize()
{
	recievedPacketNum = recievedPacketNum + ceil((double)recievedBytes / (double)sizePerPacket);
}

void NPIClient::computeRRMinDist()
{
}

vector<int> NPIClient::findBorderNodeRegion(string nodeName)
{
	vector<int> nodeIdList;
	for (int i = 0; i < allRegionList.size(); i++)
	{
		vector<BorderNode*> regBorderNodeList = allRegionList.at(i)->getBorderNodeList();
		for (int j = 0; j < regBorderNodeList.size(); j++)
		{
			if (nodeName == regBorderNodeList.at(j)->getNodeName())
				nodeIdList.push_back(i);
		}
	}
	return nodeIdList;
}

void NPIClient::setQueryAmountK(int queryAmountK)
{
	this->queryAmountK = queryAmountK;
}

int NPIClient::getQueryAmountK()
{
	return queryAmountK;
}

int NPIClient::checkBorderNodeExistByName(vector<BorderNode*> borderNodeList, string nodeName)
{
	for (int i = 0; i < borderNodeList.size(); i++)
	{
		if (borderNodeList.at(i)->getNodeName() == nodeName)
			return i;
	}
	return -1;
}

void NPIClient::getAllRegionBorderNodeLine()
{
	Region* regQueryRegion = allRegionList.at(queryPoint->getNodeRegionId());
	int queryRegionBorderCounter = 0;
	for (int i = 0; i < borderGraphList.size(); i++)
	{
		for (int j = 0; j < allRegionList.size(); j++)
		{
			vector<BorderNode*> regBorderList = allRegionList.at(j)->getBorderNodeList();
			for (int k = 0; k < regBorderList.size(); k++)
			{
				if (borderGraphList.at(i)->getNodeName() == regBorderList.at(k)->getNodeName())
					regBorderList.at(k)->setNodeLineNumber(i);
			}
		}
	}
}

void NPIClient::computePerIndexSize(string inputDir, string inputDir1, string inputDir2, string inputDir3)
{
	char line[SIZE];
	fstream fin;
	int lineNumber = 0;
	char *pch;
	char *delim = ",";

	if (fixedIndexSize == 0)
	{
		fin.open(inputDir, ios::in);
		//compute .npiind size
		while (fin.getline(line, sizeof(line), '\n'))	//以行讀取
		{
			int attributeFlag = 0;
			lineNumber++;
			pch = strtok(line, delim);
			if (lineNumber > 4)
			{
				while (pch != NULL)
				{
					fixedIndexSize += 4;
					pch = strtok(NULL, delim);
				}
			}
			else
				fixedIndexSize += 3;
		}
		fin.close();

		fin.open(inputDir1, ios::in);
		//compute .npirrmax
		while (fin.getline(line, sizeof(line), '\n'))	//以行讀取
		{
			pch = strtok(line, delim);
			while (pch != NULL)
			{
				fixedIndexSize += 4;
				pch = strtok(NULL, delim);
			}
		}
		fin.close();

		fin.open(inputDir2, ios::in);
		//compute .npirrmin
		while (fin.getline(line, sizeof(line), '\n'))	//以行讀取
		{
			pch = strtok(line, delim);
			while (pch != NULL)
			{
				fixedIndexSize += 4;
				pch = strtok(NULL, delim);
			}
		}
		fin.close();

		//compute .maxdist
		lineNumber = 0;
		fin.open(inputDir3, ios::in);
		while (fin.getline(line, sizeof(line), '\n'))	//以行讀取
		{
			int attributeFlag = 0;
			pch = strtok(line, delim);
			while (pch != NULL)
			{
				attributeFlag++;
				if (attributeFlag == 1)
					fixedIndexSize += 4;
				pch = strtok(NULL, delim);
			}
		}
		fin.close();
	}

	tuningDataSize += fixedIndexSize;
	cout << "Now tuning time : " << tuningDataSize << " bytes" << endl;
}

void NPIClient::computePerDataSize(string inputDir)
{
	char line[SIZE];
	fstream fin;
	char *pch;
	char *delim = ",";
	fin.open(inputDir, ios::in);

	//compute .index size
	while (fin.getline(line, sizeof(line), '\n'))	//以行讀取
	{
		int attributeFlag = 0;
		pch = strtok(line, delim);
		while (pch != NULL)
		{
			tuningDataSize += 4;
			pch = strtok(NULL, delim);
		}
	}
	cout << "Now tuning time : " << tuningDataSize << " bytes" << endl;
}

int NPIClient::computeIndexLatencySize(int startRId, int endRId)
{
	int nowLatencySize = 0;
	if (fixedIndexSize != 0)
	{
		char line[SIZE];
		fstream fin;
		int lineNumber = 0;
		char *pch;
		char *delim = ",";
		bool broadcastRepeatFlag = false;
		int regEndRid;
		//if start > end, listen to the last and start from begining
		if (startRId > endRId)
		{
			broadcastRepeatFlag = true;
			regEndRid = endRId;
			endRId = allRegionList.size();
		}

		for (int j = startRId; j <= endRId; j++)
		{
			//add fixed part of the index
			nowLatencySize += fixedIndexSize;

			if (broadcastRepeatFlag == true && j == allRegionList.size())
			{
				j = -1;
				endRId = regEndRid;
			}
		}
	}
	return nowLatencySize;
}

int NPIClient::computeDataLatencySize(string inputDir, int startRId, int endRId)
{
	int nowLatencySize = 0;
	bool broadcastRepeatFlag = false;
	int regEndRid;
	//if start > end, listen to the last and start from begining
	if (startRId > endRId)
	{
		broadcastRepeatFlag = true;
		regEndRid = endRId;
		endRId = allRegionList.size();
	}
	for (int i = startRId; i <= endRId; i++)
	{
		char line[SIZE];
		fstream fin;
		char *pch;
		char *delim = ",";
		fin.open(inputDir + to_string(i), ios::in);

		//compute .index size
		while (fin.getline(line, sizeof(line), '\n'))	//以行讀取
		{
			int attributeFlag = 0;
			pch = strtok(line, delim);
			while (pch != NULL)
			{
				nowLatencySize += 4;
				pch = strtok(NULL, delim);
			}
		}
		//cout << "Now latency time : " << nowLatencySize << " bytes" << endl;

		if (broadcastRepeatFlag == true && i == allRegionList.size())
		{
			i = -1;
			endRId = regEndRid;
		}
	}
	return nowLatencySize;
}

void NPIClient::setQueryPoint(Node* queryPoint)
{
	this->queryPoint = queryPoint;
}

Node* NPIClient::getQueryPoint()
{
	return queryPoint;
}

int NPIClient::getTuningDataSize()
{
	return tuningDataSize;
}

vector<Node*> NPIClient::getQueryKNNList()
{
	return queryKNNList;
}

int NPIClient::getLatencyDataSize()
{
	return latencyDataSize;
}

int NPIClient::getBroadcastCycleSize()
{
	return broadcastCycleSize;
}
