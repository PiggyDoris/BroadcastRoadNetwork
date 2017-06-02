#include "stdafx.h"
#include "ClientRNN.h"

#define _CRTDBG_MAP_ALLOC 
#define SIZE 100000
#define pi 3.1415926535897932384626433832795
#define EARTH_RADIUS 6378.137 //地球半徑 KM
#define _CRT_SECURE_NO_WARNINGS


ClientRNN::ClientRNN()
{
	sizePerPacket = 1024; //file size = 1KB
	recievedPacketNum = 0;
	startRegionId = 0;
	xPart = 0;
	yPart = 0;
	bbMaxDistSize = 0;
	maxDelta = -1;
	minDelta = -1;
	firstComputeCandidateFlag = true;
	processFlag = 0;
	tuningDataSize = 0;
	latencyDataSize = 0;
	fixedIndexSize = 0;
}

ClientRNN::~ClientRNN()
{
}

double ClientRNN::rad(double d)
{
	return d * pi / 180.0;
}

vector<int> ClientRNN::nonRepeatRandAndSort(int total, int needNum)
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

void ClientRNN::setQueryPoint(Node* queryPoint)
{
	this->queryPoint = queryPoint;
}

Node* ClientRNN::getQueryPoint()
{
	return queryPoint;
}

int ClientRNN::checkBorderNodeExistByName(vector<BorderNode*> borderNodeList, string nodeName)
{
	for (int i = 0; i < borderNodeList.size(); i++)
	{
		if (borderNodeList.at(i)->getNodeName() == nodeName)
			return i;
	}
	return -1;
}

int ClientRNN::getTuningDataSize()
{
	return tuningDataSize;
}

int ClientRNN::getLatencyDataSize()
{
	return latencyDataSize;
}

int ClientRNN::getBroadcastCycleSize()
{
	return broadcastCycleSize;
}

void ClientRNN::setPacketSize(int packetSize)
{
	this->sizePerPacket = packetSize;
}

int ClientRNN::getPacketSize()
{
	return sizePerPacket;
}

void ClientRNN::setStartRegionId(int startRegionId)
{
	this->startRegionId = startRegionId;
}

int ClientRNN::getStartIndexId()
{
	return startRegionId;
}

void ClientRNN::addRecievedBytes(int inputSize)
{
	recievedBytes = recievedBytes + inputSize;
	//cout << "Now Recieved Bytes : " << recievedBytes << endl;
}

void ClientRNN::computePacketSize()
{
	recievedPacketNum = recievedPacketNum + ceil((double)recievedBytes / (double)sizePerPacket);
}

void ClientRNN::computeRRMinDist()
{
}

vector<Node*> ClientRNN::getQueryKNNList()
{
	return queryKNNList;
}

//vector<Node*> ClientRNN::getQueryRNNList()
//{
//	return queryRNNList;
//}

void ClientRNN::computePerIndexSize(string inputDir, string inputDir1, string inputDir2, string inputDir3, string inputDir4, vector<int> bNNIndList)
{
	char line[SIZE];
	fstream fin;
	int lineNumber = 0;
	char *pch;
	char *delim = ",";

	if (fixedIndexSize == 0)
	{
		fin.open(inputDir, ios::in);
		//compute .index size
		while (fin.getline(line, sizeof(line), '\n'))	//?H??????
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
			else if (lineNumber != 5)
				fixedIndexSize += 3;
		}
		fin.close();


		//compute .bb (Border Graph) size
		for (int i = 0; i < allRegionList.size(); i++)
		{
			lineNumber = 0;
			fin.open(inputDir4 + to_string(i), ios::in);
			while (fin.getline(line, sizeof(line), '\n'))	//?H??????
				lineNumber++;
			fixedIndexSize += lineNumber * (lineNumber + 1) * 4;

			fin.close();
		}


		//compute .blist size
		lineNumber = 0;
		fin.open(inputDir2, ios::in);
		while (fin.getline(line, sizeof(line), '\n'))	//?H??????
		{
			int attributeFlag = 0;
			pch = strtok(line, delim);
			while (pch != NULL)
			{
				attributeFlag++;
				if (attributeFlag % 3 != 1)
				{
					fixedIndexSize += 4;
				}
				pch = strtok(NULL, delim);
			}
		}
		fin.close();


		//compute .maxdist
		lineNumber = 0;
		fin.open(inputDir3, ios::in);
		while (fin.getline(line, sizeof(line), '\n'))	//?H??????
		{
			int attributeFlag = 0;
			pch = strtok(line, delim);
			while (pch != NULL)
			{
				fixedIndexSize += 4;
				pch = strtok(NULL, delim);
			}
		}
		fin.close();
	}

	tuningDataSize += fixedIndexSize;

	//compute .nbnntable
	for (int i = 0; i < bNNIndList.size(); i++)
	{
		lineNumber = 0;
		fin.open(inputDir4 + to_string(i), ios::in);
		while (fin.getline(line, sizeof(line), '\n'))	//?H??????
		{
			int attributeFlag = 0;
			pch = strtok(line, delim);
			while (pch != NULL)
			{
				attributeFlag++;
				if (attributeFlag != 2)
				{
					tuningDataSize += 4;
					pch = strtok(NULL, delim);
				}
			}
		}
		fin.close();
	}
	cout << "Now tuning time : " << tuningDataSize << " bytes" << endl;
}


void ClientRNN::computePerDataSize(string inputDir)
{
	char line[SIZE];
	fstream fin;
	char *pch;
	char *delim = ",";
	fin.open(inputDir, ios::in);

	//compute .index size
	while (fin.getline(line, sizeof(line), '\n'))	//?H??????
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
	fin.close();
}

int ClientRNN::computeIndexLatencySize(string inputDir, string inputDir1, int startRId, int endRId)
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

			//get BNN to listen
			vector<int> bNNIndList;
			lineNumber = 0;
			fin.open(inputDir1 + to_string(j), ios::in);
			while (fin.getline(line, sizeof(line), '\n'))	//?H??????
			{
				lineNumber++;
				pch = strtok(line, delim);
				if (lineNumber == 6)
				{
					while (pch != NULL)
					{
						bNNIndList.push_back(atoi(pch));
						pch = strtok(NULL, delim);
					}
				}
			}
			fin.close();

			//compute all BNN table size
			for (int i = 0; i < bNNIndList.size(); i++)
			{
				lineNumber = 0;
				fin.open(inputDir + to_string(i), ios::in);
				while (fin.getline(line, sizeof(line), '\n'))	//?H??????
				{
					int attributeFlag = 0;
					pch = strtok(line, delim);
					while (pch != NULL)
					{
						attributeFlag++;
						if (attributeFlag != 2)
						{
							nowLatencySize += 4;
							pch = strtok(NULL, delim);
						}
					}
				}
				fin.close();
			}
			//cout << "Now latency time : " << nowLatencySize << " bytes" << endl;

			if (broadcastRepeatFlag == true && j == allRegionList.size())
			{
				j = -1;
				endRId = regEndRid;
			}
		}
	}
	return nowLatencySize;
}

int ClientRNN::computeDataLatencySize(string inputDir, int startRId, int endRId)
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
		while (fin.getline(line, sizeof(line), '\n'))	//?H??????
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

vector<int> ClientRNN::findBorderNodeRegion(string nodeName)
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

//ask
double ClientRNN::computeSectionDistance(double x_lat, double x_lng, double y_lat, double y_lng)
{
	double x_radLat = 0;
	double y_radLat = 0;
	double a = 0, b = 0;

	x_radLat = rad(x_lat);
	y_radLat = rad(y_lat);
	a = x_radLat - y_radLat;
	b = rad(x_lng) - rad(y_lng);

	double dist = 2 * asin(sqrt(pow(sin(a / 2), 2) + cos(x_radLat)*cos(y_radLat)*pow(sin(b / 2), 2)));
	dist = dist * EARTH_RADIUS;
	dist = dist * 1000;

	return dist;
}

//get Query Point 
void ClientRNN::getRandQueryPoint(string inputDir){
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

//listen BA Broadcast Index
void ClientRNN::listenBroadcastIndex(string inputDir, string inputDir1, string inputDir2, string inputDir3, string inputDir4, string inputDir5){
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
			addRecievedBytes(sizeof(char) * 4);
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
			nowRegion = allRegionList.at(nowRegionId);
			while (pch != NULL)
			{
				attributeFlag++;
				if (attributeFlag == 1)
					nowRegion->setMinLambda(strtod(pch, NULL));
				else if (attributeFlag == 2)
					nowRegion->setMaxLambda(strtod(pch, NULL));

				addRecievedBytes(sizeof(strtod(pch, NULL)));
				pch = strtok(NULL, delim);
			}
		}
		////////////////////////////
		else if (lineNumber == 6)
		{
			while (pch != NULL)
			{
				bNNTableList.push_back(atoi(pch));
				addRecievedBytes(sizeof(bNNTableList.back()));
				pch = strtok(NULL, delim);
			}
		}
		else if (lineNumber == 7)
		{
			while (pch != NULL)
			{
				//attributeFlag++;
				//if (startRegionId == attributeFlag - 1)
				//	nowRegion->setNodeAmount(atoi(pch));
				allRegionSpotAmount.push_back(atoi(pch));
				addRecievedBytes(sizeof(atoi(pch)));
				pch = strtok(NULL, delim);
			}
			for (int i = 0; i < allRegionList.size(); i++)
				allRegionList.at(i)->setNodeAmount(allRegionSpotAmount.at(i));
		}
	}
	fin.close();

	if (lastRecievedBNNSequenceBoolList.size() == 0)
	{
		for (int i = 0; i < allRegionList.size(); i++)
			lastRecievedBNNSequenceBoolList.push_back(false);
	}

	//get .nbnntable in bnntableList
	for (int i = 0; i < bNNTableList.size(); i++)
	{
		lineNumber = 0;
		//if recieved, add recieved bytes and continue 
		int bNNExistFlag = -1;
		for (int j = 0; j < recievedBNNRegionList.size(); j++)
		{
			if (bNNTableList.at(i) == recievedBNNRegionList.at(j))
			{
				bNNExistFlag = j;
				lastRecievedBNNSequenceBoolList.at(bNNTableList.at(i)) = true;
			}
		}
		if (bNNExistFlag != -1)
		{
			addRecievedBytes(recievedBNNSizeList.at(bNNExistFlag));
			continue;
		}

		fin.open(inputDir1 + to_string(bNNTableList.at(i)), ios::in);
		int recievedBNNBytes = 0;
		while (fin.getline(line, sizeof(line), '\n'))	//以行讀取
		{
			int attributeFlag = 0;
			lineNumber++;
			BorderNode* nowBorderNode;
			Node* nowNode;
			Destination* nowDestination;

			pch = strtok(line, delim);
			while (pch != NULL)
			{
				attributeFlag++;
				if (attributeFlag == 1)
				{
					nowBorderNode = new BorderNode();
					nowBorderNode->setNodeName(pch);
					recievedBNNBytes += sizeof(nowBorderNode->getNodeName());
					addRecievedBytes(sizeof(nowBorderNode->getNodeName()));
					allRegionList.at(bNNTableList.at(i))->setBorderNodeList(nowBorderNode);
				}
				else if (attributeFlag == 2)
				{
					nowNode = new Node();
					nowNode->setNodeName(pch);
					recievedBNNBytes += sizeof(nowNode->getNodeName());
					addRecievedBytes(sizeof(nowNode->getNodeName()));
				}
				else if (attributeFlag == 3)
				{
					nowDestination = new Destination();
					nowDestination->setStartPointNode(nowBorderNode);
					nowDestination->setDestinationNode(nowNode);
					nowDestination->setPathDistance(strtod(pch, NULL));
					Region* regBNNRegion = allRegionList.at(bNNTableList.at(i));
					regBNNRegion->setBNNTableList(nowDestination);
					recievedBNNBytes += sizeof(sizeof(strtod(pch, NULL)));
					addRecievedBytes(sizeof(strtod(pch, NULL)));
				}
				pch = strtok(NULL, delim);
			}
		}
		recievedBNNSizeList.push_back(recievedBNNBytes);
		fin.close();
		recievedBNNRegionList.push_back(bNNTableList.at(i));
	}

	//get .bb
	if (bbMaxDistSize == 0)
	{
		lineNumber = 0;
		fin.open(inputDir2, ios::in);
		//get all border node
		while (fin.getline(line, sizeof(line), '\n'))	//以行讀取
		{
			lineNumber++;
			int attributeFlag = 0;
			pch = strtok(line, delim);
			while (pch != NULL)
			{
				attributeFlag++;
				if (attributeFlag == 1)
				{
					BorderNode* regBorderNode = new BorderNode();
					regBorderNode->setNodeName(pch);
					borderGraphList.push_back(regBorderNode);

					bbMaxDistSize += sizeof(pch);
					addRecievedBytes(sizeof(pch));
					break;
				}
				pch = strtok(NULL, delim);
			}
		}
		fin.close();

		lineNumber = 0;
		fin.open(inputDir2, ios::in);
		//get all border to border distance
		while (fin.getline(line, sizeof(line), '\n'))	//以行讀取
		{
			lineNumber++;
			int attributeFlag = 0;
			pch = strtok(line, delim);
			BorderNode* nowBorderNode;
			nowBorderNode = borderGraphList.at(lineNumber - 1);
			while (pch != NULL)
			{
				attributeFlag++;
				if (attributeFlag != 1)
				{
					BorderNode* regBorderNode = borderGraphList.at(attributeFlag - 2);
					Destination* regDestination = new Destination();
					regDestination->setDestinationNode(regBorderNode);
					regDestination->setStartPointNode(nowBorderNode);
					regDestination->setPathDistance(strtod(pch, NULL));
					nowBorderNode->setDestinationList(regDestination);

					bbMaxDistSize += sizeof(regDestination->getPathDistance());
					addRecievedBytes(sizeof(regDestination->getPathDistance()));
				}
				pch = strtok(NULL, delim);
			}
		}
		fin.close();


		//.blist
		lineNumber = 0;
		fin.open(inputDir3, ios::in);
		while (fin.getline(line, sizeof(line), '\n'))
		{
			int attributeFlag = 0;
			lineNumber++;
			pch = strtok(line, delim);
			string regNodeName;
			int checkNodeExistInd;
			while (pch != NULL)
			{
				attributeFlag++;
				if (attributeFlag % 3 == 1)
				{
					//check node already added when listening .bnntable
					checkNodeExistInd = checkBorderNodeExistByName(allRegionList.at(lineNumber - 1)->getBorderNodeList(), pch);
					if (allRegionList.at(lineNumber - 1)->getBorderNodeList().size() == 0 || checkNodeExistInd == -1)
					{
						BorderNode* regBorderNode = new BorderNode();
						regBorderNode->setNodeName(pch);
						regNodeName = pch;
						allRegionList.at(lineNumber - 1)->setBorderNodeList(regBorderNode);
						checkNodeExistInd = allRegionList.at(lineNumber - 1)->getBorderNodeList().size() - 1;
					}
				}
				else if (attributeFlag % 3 == 2)
				{
					allRegionList.at(lineNumber - 1)->getBorderNodeList().at(checkNodeExistInd)->setLongtitude(strtod(pch, NULL));
				}
				else if (attributeFlag % 3 == 0)
				{
					allRegionList.at(lineNumber - 1)->getBorderNodeList().at(checkNodeExistInd)->setLatitude(strtod(pch, NULL));
				}

				bbMaxDistSize += sizeof(pch);
				addRecievedBytes(sizeof(pch));

				pch = strtok(NULL, delim);
			}
		}
		fin.close();

		//get long & lat from allRegionList to borderGraphList 
		int regNum = xPart * yPart;
		for (int j = 0; j < regNum; j++)
		{
			vector<BorderNode*> regRegionBorderList = allRegionList.at(j)->getBorderNodeList();
			for (int k = 0; k < regRegionBorderList.size(); k++)
			{
				for (int l = 0; l < borderGraphList.size(); l++)
				{
					if (regRegionBorderList.at(k)->getNodeName() == borderGraphList.at(l)->getNodeName())
					{
						borderGraphList.at(l)->setNodeRegionId(j);
						borderGraphList.at(l)->setLongtitude(regRegionBorderList.at(k)->getLongtitude());
						borderGraphList.at(l)->setLatitude(regRegionBorderList.at(k)->getLatitude());
						break;
					}
				}
			}
		}

		//set region of border node in borderGraphList
		for (int j = 0; j < borderGraphList.size(); j++)
		{
			BorderNode* regBorderNode = borderGraphList.at(j);
			vector<int> borderRegionIdList = findBorderNodeRegion(regBorderNode->getNodeName());
			regBorderNode->setWholeRegionIdList(borderRegionIdList);
		}

		vector<int> allRegionMinDist;
		for (int j = 0; j < regNum; j++)
		{
			for (int k = 0; k < regNum; k++)
			{
				allRegionMinDist.push_back(-1);
			}
		}

		////////////////////////////////////////////////////////////////////////////////
		//minRRDist (use .npimin to speedup)
		lineNumber = 0;
		fin.open(inputDir5, ios::in);
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

		//get Rj -> Rk minDist
		//for (int j = 0; j < borderGraphList.size(); j++)
		//{
		//	BorderNode* regBorderNode = borderGraphList.at(j);
		//	vector<Destination*> regBorderDestList = regBorderNode->getDestinationList();
		//	for (int k = 0; k < regBorderDestList.size(); k++)
		//	{
		//		vector<int> borderRegionIdList = findBorderNodeRegion(regBorderDestList.at(k)->getDestinationNode()->getNodeName());
		//		for (int l = 0; l < regBorderNode->getRegionIdList().size(); l++)
		//		{
		//			for (int m = 0; m < borderRegionIdList.size(); m++)
		//			{
		//				int regMinDist = allRegionMinDist.at(regBorderNode->getRegionIdList().at(l) * regNum + borderRegionIdList.at(m));
		//				if (regMinDist < 0 || regMinDist > regBorderDestList.at(k)->getPathDistance())
		//					allRegionMinDist.at(regBorderNode->getRegionIdList().at(l) * regNum + borderRegionIdList.at(m)) = regBorderDestList.at(k)->getPathDistance();
		//			}
		//		}
		//	}
		//}
		////compare R(0,1) vs R(1,0)
		//for (int j = 0; j < allRegionMinDist.size(); j++)
		//{
		//	for (int k = 0; k < allRegionMinDist.size(); k++)
		//	{
		//		if (remainder(j, regNum) == k/regNum && j/regNum == remainder(k, regNum))
		//		{
		//			allRegionMinDist.at(j) = min(allRegionMinDist.at(j), allRegionMinDist.at(k));
		//			allRegionMinDist.at(k) = allRegionMinDist.at(j);
		//			break;
		//		}
		//	}
		//}
		////set minimum to allregionList
		//for (int j = 0; j < allRegionMinDist.size(); j++)
		//{
		//	Region* regRegion = allRegionList.at(j / regNum);
		//	regRegion->setMinRRDist(allRegionMinDist.at(j));
		//}
		////cout region to region minimum
		//for (int j = 0; j < allRegionList.size(); j++)
		//{
		//	for (int k = 0; k < allRegionList.at(j)->getMinRRDistList().size(); k++)
		//		cout << allRegionList.at(j)->getMinRRDistList().at(k) << " ";
		//	cout << endl;
		//}
		///////////////////////////////////////////////////////////////////////////////////////


		//.maxdist
		lineNumber = 0;
		fin.open(inputDir4, ios::in);
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
					else if (attributeFlag == 2)
						allRegionList.at(lineNumber - 1)->setMinLambda(strtod(pch, NULL));
					else if (attributeFlag == 3)
						allRegionList.at(lineNumber - 1)->setMaxLambda(strtod(pch, NULL));

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
	computePerIndexSize(inputDir + to_string(startRegionId), inputDir2, inputDir3, inputDir4, inputDir1, bNNTableList);
	computePacketSize();
	recievedRegionList.push_back(startRegionId);
}

void ClientRNN::listenBroadcastData(string inputDir)
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
	while (fin.getline(line, sizeof(line), '\n'))	//?H??????
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
	while (fin.getline(line, sizeof(line), '\n'))	//?H??????
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
	startRegionId++;
	computePerDataSize(inputDir);
}

//After recieve Index to know Query located Region
void ClientRNN::locateQueryPointRegion(){
	for (int i = 0; i < allRegionList.size(); i++)
	{
		if (allRegionList.at(i)->getLatitudeBottom() <= queryPoint->getLatitude() && allRegionList.at(i)->getLatitudeTop() >= queryPoint->getLatitude() && allRegionList.at(i)->getLongtitudeLeft() <= queryPoint->getLongtitude() && allRegionList.at(i)->getLongtitudeRight() >= queryPoint->getLongtitude())
			queryPoint->setNodeRegionId(allRegionList.at(i)->getId());
	}
	cout << "Query Point region : " << queryPoint->getNodeRegionId() << endl;
}
void ClientRNN::locateNodeRegion(Node* targetNode){
	for (int i = 0; i < allRegionList.size(); i++){
		if (allRegionList.at(i)->getLatitudeBottom() <= targetNode->getLatitude() && allRegionList.at(i)->getLatitudeTop() >= targetNode->getLatitude() && allRegionList.at(i)->getLongtitudeLeft() <= targetNode->getLongtitude() && allRegionList.at(i)->getLongtitudeRight() >= targetNode->getLongtitude())
			targetNode->setNodeRegionId(allRegionList.at(i)->getId());
	}
}

void ClientRNN::computeUpperBound(){

	//upperBound(R_all) = ∞;
	for (int i = 0; i < allRegionList.size(); i++){
		Bound* regUpperBound = new Bound("UpperBound");
		regUpperBound->setUpperBound(INT_MAX);
		upperBoundList.push_back(regUpperBound);
	}
	//noneZeroSpotRegions = getNoneZeroSpotRegions();
	for (int i = 0; i < allRegionSpotAmount.size(); i++){
		if (allRegionSpotAmount.at(i) != 0){
			noneZeroSpotRegionList.push_back(i);
			//cout <<" noneZeroSpotRegionList : "<<noneZeroSpotRegionList.at(i)<<endl;
		}	
	}
	//for  R_i  in noneZeroSpotRegions do
		//for  R_j  in noneZeroSpotRegions do
	for (int i = 0; i < allRegionMaxdistList.size(); i++){
		for (int j = 0; j < allRegionMaxdistList.size(); j++){
			vector<int>::iterator it = find(noneZeroSpotRegionList.begin(), noneZeroSpotRegionList.end(), i);
			if (it != noneZeroSpotRegionList.end()) {
				//if R_i =  R_j  and | R_i | ?2 then maxdist(R_i, R_j) = maxdist(R_i);
				if (i == j&& allRegionSpotAmount.at(i) >= 2){
					Bound* regUpperBound = new Bound("UpperBound");
					upperBoundList[i]->setDestinationRegionId(j);
					regUpperBound->setUpperBound(allRegionMaxdistList.at(i));
					upperBoundList[i] = regUpperBound;
				}
				//else
				else {
					//compute mindist(R_i, R_j) by borderGraph;
					//compute maxdist(R_i, R_j) = maxdist(R_i) + mindist(R_i, R_j) + maxdist(R_j);
					Bound* regUpperBound = new Bound("UpperBound");					
					regUpperBound->setUpperBound(allRegionMaxdistList.at(i) + allRegionList.at(i)->getMinRRDistList().at(j) + allRegionMaxdistList.at(j));
					int k = regUpperBound->getUpperBound();
					if (upperBoundList[i]->getUpperBound() > k){
						upperBoundList[i]->setUpperBound(k);
						upperBoundList[i]->setDestinationRegionId(j);
						cout << "Region : " << i << " UpperBound : " << upperBoundList[i]->getUpperBound();
					}
					else break;
					//upperBoundList[i] = regUpperBound;
				}
				cout << "找到搜尋值！" << endl;
			}
			else {
				cout << "找不到搜尋值！" << endl;
			}		
	//upperBound(R_i) = min(upperBound(R_i), maxdist(R_i, R_j))
		}
	}
	//sort(upperBoundList.begin(), upperBoundList.end(), Bound());

	/*int nowNodeAmount = 0;
	double regUpperBound = 0;
	int ubListInd = 0;*/
	//need all region node amount
	//while (nowNodeAmount < queryAmountK)//not input queryAmountK
	//{
	//	int regRegionId = upperBoundList.at(ubListInd)->getDestinationRegionId();
	//	nowNodeAmount += allRegionSpotAmount.at(regRegionId);
	//	regUpperBound = upperBoundList.at(ubListInd)->getUpperBound();
	//	ubListInd++;
	//}
	/*upperBound = regUpperBound;*/

	cout << "upper bound : " << upperBound << endl;
	for (int i = 0; i < upperBoundList.size(); i++){
		cout << "Region" << upperBoundList.at(i)->getDestinationRegionId() << " UB :" << upperBoundList.at(i)->getUpperBound() << endl;
	}

	for (int i = 0; i < lowerBoundList.size(); i++)
	{
		cout << "Region" << lowerBoundList.at(i)->getDestinationRegionId() << " LB :" << lowerBoundList.at(i)->getUpperBound() << endl;
	}

	//UPDATE
}

void ClientRNN::computeCandidateRegion(){
	//ComputeEveryRegionUpperBound();
	computeUpperBound();
	//ComputeLowerBound();
	computeLowerBound();
	//for (int i = 0; i < allRegionList.size(); i++){
	//	candidateRegionList.push_back(allRegionList.at(i));
	//}
	//for each region? R?_1, …, ? R?_i, …, R_n do
	for (int i = 0; i < allRegionList.size(); i++){
		//if mindist(R_i, q)>upperBound(R_i) then
		if (allRegionSpotAmount.at(i)!=0){
			int lower = lowerBoundList.at(i)->geLowerBound();
			int upper = upperBoundList.at(i)->getUpperBound();
			if (lower < upper){
				//candidateList.deletequeue(R_i);
				candidateRegionList.push_back(allRegionList.at(i));
			}
		}
	}
	for (int i = 0; i < candidateRegionList.size(); i++){
		candidateRegionIdList.push_back(candidateRegionList.at(i)->getId());
	}
	//if (firstComputeCandidateFlag == true)
	//{
	//	for (int i = 0; i < lowerBoundList.size(); i++)
	//	{
	//		int regDestinationRegionId = lowerBoundList.at(i)->getDestinationRegion()->getId();
	//		if (lowerBoundList.at(i)->getUpperBound() != -1 && lowerBoundList.at(i)->getUpperBound() < upperBound/* && allRegionSpotAmount.at(regDestinationRegionId) != 0*/)
	//			candidateRegionIdList.push_back(lowerBoundList.at(i)->getDestinationRegionId());
	//	}
	//}
	//else
	//{
	//	//Update, erase
	//	//if region in candidateList, it's lower bound > upper bound, erase this region from candidate
	//	for (int i = 0; i < candidateRegionIdList.size(); i++)
	//	{
	//		for (int j = 0; j < lowerBoundList.size(); j++)
	//		{
	//			if (lowerBoundList.at(j)->getUpperBound() > upperBound && candidateRegionIdList.at(i) == lowerBoundList.at(j)->getDestinationRegionId())
	//			{
	//				if (lowerBoundList.at(j)->getUpperBound() != -1)
	//				{
	//					candidateRegionIdList.erase(candidateRegionIdList.begin() + i);
	//					break;
	//				}
	//			}
	//		}
	//	}
	//}

	////get toListenCandidateList
	//candidateRegionToListenList = candidateRegionIdList;
	//for (int i = 0; i < recievedRegionList.size(); i++)
	//{
	//	for (int j = 0; j < candidateRegionToListenList.size(); j++)
	//	{
	//		if (recievedRegionList.at(i) == candidateRegionToListenList.at(j))
	//		{
	//			candidateRegionToListenList.erase(candidateRegionToListenList.begin() + j);
	//			break;
	//		}
	//	}
	//}

	////get now recieved node amount
	//nowRecievedNodeAmount = 0;
	//recievedCandidateRegionIdList.clear();
	//for (int i = 0; i < recievedRegionList.size(); i++)
	//{
	//	for (int j = 0; j < candidateRegionIdList.size(); j++)
	//	{
	//		if (recievedRegionList.at(i) == candidateRegionIdList.at(j))
	//		{
	//			nowRecievedNodeAmount += allRegionSpotAmount.at(candidateRegionIdList.at(j));
	//			recievedCandidateRegionIdList.push_back(candidateRegionIdList.at(j));
	//		}
	//	}
	//}

	firstComputeCandidateFlag = false;

	cout << endl;
	cout << "Query Region R" << queryPoint->getNodeRegionId() << endl;
	//cout << "Now Broadcast R" << startRegionId << endl;
	cout << "Candidate List : " << endl;
	for (int i = 0; i < candidateRegionIdList.size(); i++)
		cout << "R" << candidateRegionIdList.at(i) << endl;
}

void ClientRNN::computeLowerBound(){
	//REMEMBER TO UPDATE
	vector<int> queryRRMinDistList = allRegionList.at(queryPoint->getNodeRegionId())->getMinRRDistList();
	if (maxDelta < 0 || minDelta < 0)
		computeDelta();

	//initial recievedBNN bool list as all false when first use
	if (recievedBNNSequenceBoolList.size() == 0)
	{
		for (int i = 0; i < allRegionList.size(); i++)
			recievedBNNSequenceBoolList.push_back(false);
	}

	//set recievedBNN as true
	for (int i = 0; i < recievedBNNRegionList.size(); i++)
		recievedBNNSequenceBoolList.at(recievedBNNRegionList.at(i)) = true;

	//compute lower bound of recieved region
	for (int i = 0; i < recievedBNNRegionList.size(); i++)
	{
		if (lastRecievedBNNSequenceBoolList.at(recievedBNNRegionList.at(i)) == true)
			continue;

		bool isCandidateFlag = false;
		for (int j = 0; j < candidateRegionToListenList.size(); j++)
		{
			if (candidateRegionToListenList.at(j) == recievedBNNRegionList.at(i))
				isCandidateFlag = true;
		}
		if (isCandidateFlag == false && firstComputeCandidateFlag == false)
			continue;

		double regLowerBound;
		if (recievedBNNRegionList.at(i) == queryPoint->getNodeRegionId())
			regLowerBound = 0;
		//else if (allRegionList.at(recievedBNNRegionList.at(i))->getNodeAmount() == 0)
		//	regLowerBound = -1;
		else
		{
			Region* regQueryRegion = allRegionList.at(queryPoint->getNodeRegionId());
			//get borderNode in Rq  &  borderNode in Ri  &  dist from Rq to Ri
			vector<BorderNode*> regQueryBorderNodeList;

			for (int j = 0; j < borderGraphList.size(); j++)
			{
				for (int k = 0; k < regQueryRegion->getBorderNodeList().size(); k++)
				{
					if (borderGraphList.at(j)->getNodeName() == regQueryRegion->getBorderNodeList().at(k)->getNodeName())
						regQueryBorderNodeList.push_back(borderGraphList.at(j));
				}
			}
			//check query region border list, if borderNode.region is now region, add to toQuery DestinationList
			for (int j = 0; j < regQueryBorderNodeList.size(); j++)
			{
				vector<Destination*> regQueryDestinationList = regQueryBorderNodeList.at(j)->getDestinationList();
				for (int k = 0; k < regQueryDestinationList.size(); k++)
				{
					vector<BorderNode*> borderCheckList = allRegionList.at(recievedBNNRegionList.at(i))->getBorderNodeList();
					for (int l = 0; l < borderCheckList.size(); l++)
					{
						if (borderCheckList.at(l)->getNodeName() == regQueryDestinationList.at(k)->getDestinationNode()->getNodeName())
						{
							allRegionList.at(recievedBNNRegionList.at(i))->setToQueryDestinationList(regQueryDestinationList.at(k));
						}
					}
					//if (regQueryDestinationList.at(k)->getDestinationNode()->getNodeRegionId() == recievedBNNRegionList.at(i))
					//{
					//	allRegionList.at(recievedBNNRegionList.at(i))->setToQueryDestinationList(regQueryDestinationList.at(k));
					//}
				}
			}
			//minDelta
			queryPoint;
			vector<Destination*> regMinDeltaList = regQueryRegion->getMinDeltaList();
			vector<Destination*> regToQueryDestinationList = allRegionList.at(recievedBNNRegionList.at(i))->getToQueryDestinationList();
			vector<Destination*> regBNNTableList = allRegionList.at(recievedBNNRegionList.at(i))->getBNNTableList();


			//--------?????@???I?? regionId ?d?? bnnn ???? borderNode?`?I?Z?÷ 

			vector<string> dijkstraNodeList;
			vector<EData*> edgesList;

			//push query node name
			dijkstraNodeList.push_back(queryPoint->getNodeName());

			//push query region border node name
			for (int j = 0; j < regQueryRegion->getBorderNodeList().size(); j++)
				dijkstraNodeList.push_back(regQueryRegion->getBorderNodeList().at(j)->getNodeName());

			//push destination region border name
			Region* regDestinationRegion = allRegionList.at(recievedBNNRegionList.at(i));
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

			//push BNNTable node name
			vector<string> regBNNTableSingleNameList;
			for (int j = 0; j < regBNNTableList.size(); j++)
			{
				if (j == 0)
				{
					regBNNTableSingleNameList.push_back(regBNNTableList.at(j)->getDestinationNode()->getNodeName());
					dijkstraNodeList.push_back(regBNNTableList.at(j)->getDestinationNode()->getNodeName());
				}
				else
				{
					bool bnnExistFlag = false;
					for (int k = 0; k < regBNNTableSingleNameList.size(); k++)
					{
						if (regBNNTableSingleNameList.at(k) == regBNNTableList.at(j)->getDestinationNode()->getNodeName())
						{
							bnnExistFlag = true;
							break;
						}
					}
					if (bnnExistFlag == false)
					{
						regBNNTableSingleNameList.push_back(regBNNTableList.at(j)->getDestinationNode()->getNodeName());
						dijkstraNodeList.push_back(regBNNTableList.at(j)->getDestinationNode()->getNodeName());
					}
				}
			}

			//push query minDelta list to EData
			for (int j = 0; j < regMinDeltaList.size(); j++)
				edgesList.push_back(new EData(regMinDeltaList.at(j)->getStartPointNode()->getNodeName(), regMinDeltaList.at(j)->getDestinationNode()->getNodeName(), regMinDeltaList.at(j)->getPathDistance()));

			//push border border list to EData
			for (int j = 0; j < regToQueryDestinationList.size(); j++)
				edgesList.push_back(new EData(regToQueryDestinationList.at(j)->getStartPointNode()->getNodeName(), regToQueryDestinationList.at(j)->getDestinationNode()->getNodeName(), regToQueryDestinationList.at(j)->getPathDistance()));

			//push border node list to EData
			for (int j = 0; j < regBNNTableList.size(); j++)
				edgesList.push_back(new EData(regBNNTableList.at(j)->getStartPointNode()->getNodeName(), regBNNTableList.at(j)->getDestinationNode()->getNodeName(), regBNNTableList.at(j)->getPathDistance()));


			//		cout << "done" << endl;
			int prev[MAX] = { 0 };
			int dist[MAX] = { 0 };

			int vlen = dijkstraNodeList.size();
			int elen = edgesList.size();

			ListUDG* pG;

			pG = new ListUDG(dijkstraNodeList, vlen, edgesList, elen);
			regLowerBound = pG->dijkstra(0, prev, dist, regBNNTableSingleNameList.size());
			cout << "Update LB R" << queryPoint->getNodeRegionId() << "-> R" << recievedBNNRegionList.at(i) << "  :  " << regLowerBound << endl;

			//save all minDelta & bb & BNN
			//regLowerBound =  minDelta + min(bb + BNN);
		}
		//add Lower Bound if not exist, else update it
		bool lowerBoundExistFlag = false;
		for (int j = 0; j < lowerBoundList.size(); j++)
		{
			if (lowerBoundList.at(j)->getQueryRegionId() == queryPoint->getNodeRegionId() && lowerBoundList.at(j)->getDestinationRegionId() == recievedBNNRegionList.at(i))
			{
				lowerBoundExistFlag = true;
				lowerBoundList.at(j)->setLowerBound(regLowerBound);
				break;
			}
		}

		if (lowerBoundExistFlag == false)
		{
			Bound* regBound = new Bound();
			regBound->setBoundType("LowerBound");
			regBound->setLowerBound(regLowerBound);
			regBound->setQueryPoint(queryPoint);
			regBound->setQueryRegionId(queryPoint->getNodeRegionId());
			regBound->setDestinationRegion(allRegionList.at(recievedBNNRegionList.at(i)));
			regBound->setDestinationRegionId(recievedBNNRegionList.at(i));
			lowerBoundList.push_back(regBound);
			cout << "Update LB R" << queryPoint->getNodeRegionId() << "-> R" << recievedBNNRegionList.at(i) << "  :  " << regLowerBound << endl;
		}
	}

	//compute lower bound of not recieved region
	for (int i = 0; i < recievedBNNSequenceBoolList.size(); i++)
	{
		bool lowerBoundExistFlag = false;
		for (int j = 0; j < lowerBoundList.size(); j++)
		{
			if (lowerBoundList.at(j)->getDestinationRegionId() == i)
				lowerBoundExistFlag = true;
		}
		if (recievedBNNSequenceBoolList.at(i) == false && lowerBoundExistFlag == false)
		{
			double regLowerBound = minDelta + allRegionList.at(queryPoint->getNodeRegionId())->getMinRRDistList().at(i);
			if (queryPoint->getNodeRegionId() == i)
				regLowerBound = 0;
			else if (allRegionList.at(queryPoint->getNodeRegionId())->getMinRRDistList().at(i) == -1 || allRegionList.at(i)->getNodeAmount() == 0)
				regLowerBound = -1;
			Bound* regBound = new Bound();
			regBound->setBoundType("LowerBound");
			regBound->setLowerBound(regLowerBound);
			regBound->setQueryPoint(queryPoint);
			regBound->setQueryRegionId(queryPoint->getNodeRegionId());
			regBound->setDestinationRegion(allRegionList.at(i));
			regBound->setDestinationRegionId(i);
			lowerBoundList.push_back(regBound);
			cout << "R" << regBound->getQueryRegionId() << " -> R" << regBound->getDestinationRegionId() << " : " << regBound->getUpperBound() << endl;
		}
	}
}

//Delta==fy
void ClientRNN::computeDelta()
{
	int queryPointRegionId = queryPoint->getNodeRegionId();
	Region* queryPointRegion = allRegionList.at(queryPointRegionId);
	vector<BorderNode*> qRegionBorderList = queryPointRegion->getBorderNodeList();

	for (int i = 0; i < qRegionBorderList.size(); i++)
	{
		double eDist;
		eDist = abs(computeSectionDistance(queryPoint->getLatitude(), queryPoint->getLongtitude(), qRegionBorderList.at(i)->getLatitude(), qRegionBorderList.at(i)->getLongtitude()));

		//set minDeltaList in Rq
		Destination* regMinDeltaDestination = new Destination();
		regMinDeltaDestination->setStartPointNode(queryPoint);
		regMinDeltaDestination->setDestinationNode(qRegionBorderList.at(i));
		regMinDeltaDestination->setDestinationNodeName(qRegionBorderList.at(i)->getNodeName());
		regMinDeltaDestination->setPathDistance(queryPointRegion->getMinLambda() * eDist);
		queryPointRegion->setMinDeltaList(regMinDeltaDestination);

		maxDelta = max(maxDelta, queryPointRegion->getMaxLambda() * eDist);
		if (minDelta < 0)
			minDelta = queryPointRegion->getMinLambda() * eDist;
		minDelta = min(minDelta, queryPointRegion->getMinLambda() * eDist);
	}
	//get minimum between maxDelta & maxDist
	maxDelta = min(maxDelta, (double)allRegionMaxdistList.at(queryPointRegionId));
}

//compute whoes 1NN Answer include Query  
void ClientRNN::computeKNNAnswer(){
	//if candidateList.spot(R_i) 1NN = queryPoint q
	//RNN_AnswerList.enqueue(R_i)
	bool queryRegionRecievedFlag = false;
	//processFlag to know which process step right now & judge 
	if (processFlag == 0)
	{
		if (queryPoint == NULL)
			//getRandQueryPoint(inputDir);//get query point from obj
		//this->queryAmountK = queryAmountK; //KNN client use
		this->startRegionId = startRegionId;
		processFlag = 1;
	}
	

	//if dijkstra not compute before, do dijkstra
	if (queryFinishedFlag == false)
	{
		computeToQueryDijkstra();
		queryFinishedFlag = true;
	}

	if (this->startRegionId == 0)
	{
		//latencyDataSize = computeIndexLatencySize(inputDir2, inputDir1, startRegionId, allRegionList.size() - 1);
		//latencyDataSize += computeDataLatencySize(inputDir6, startRegionId, allRegionList.size() - 1);
	}
	else
	{
		//latencyDataSize = computeIndexLatencySize(inputDir2, inputDir1, startRegionId, this->startRegionId - 1);
		//latencyDataSize += computeDataLatencySize(inputDir6, startRegionId, this->startRegionId - 1);
	}
	//broadcastCycleSize = computeIndexLatencySize(inputDir2, inputDir1, 0, allRegionList.size() - 1);
	//broadcastCycleSize += computeDataLatencySize(inputDir6, 0, allRegionList.size() - 1);

	cout << endl << "///////////////////////////" << endl;
	cout << "BNN Recieved Region : " << endl;
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
void ClientRNN::computeRNNAnswer(string inputDir, string inputDir1, string inputDir2, string inputDir3, string inputDir4, string inputDir5, string inputDir6, string inputDir7, int startRegionId){
	bool queryRegionRecievedFlag = false;
	/*candidateList = R_all;
	receiveIndex();
	locate Region R_q containing q;*/
	if (processFlag == 0)
	{
		if (queryPoint == NULL)
			getRandQueryPoint(inputDir);
		//this->queryAmountK = queryAmountK; not compute KNN
		this->startRegionId = startRegionId;
		processFlag = 1;
	}
	/*
	for each border node in R_q , b_1, …,  b_i, …, b_n do
		compute ??[φ?_bi?^-, ?φ_bi?^+] = [λ^-, λ^+]×?dist?_E(q, ? b?_i);
		1if ?φ_bi?^+>maxdist(R_q) then ?φ_bi?^ += maxdist(R_q);
	compute Candidate Region
	computeCandidateRegion();
		ComputeEveryRegionUpperBound();
		ComputeLowerBound();
		for each region? R?_1, …, ? R?_i, …, R_n do
			if mindist(R_i, q)>upperBound(R_i) then
				candidateList.deletequeue(R_i);
	while candidateList is not empty do
		if now broadcast region ? candidateList then
			receiveDataSegment();*/
	while (processFlag == 1 || candidateRegionToListenList.size() != 0){
		bool isRegionCandidate = false;
		//this circle to know which Region is Candidate Region
		for (int i = 0; i < candidateRegionToListenList.size(); i++){
			if (candidateRegionToListenList.at(i) == this->startRegionId){
				isRegionCandidate = true;
				break;
			}
		}
		//continue if not candidate Region & not first time process
		if (isRegionCandidate == false && processFlag != 1){
			this->startRegionId++;
			continue;
		}
		else
			recievedRList.push_back(this->startRegionId);
		listenBroadcastIndex(inputDir1, inputDir2, inputDir3, inputDir4, inputDir5, inputDir7);
		if (processFlag == 1){
			locateQueryPointRegion();//located Query Point with Region
			processFlag = 2;
		}
		computeCandidateRegion();
		listenBroadcastData(inputDir6 + to_string(this->startRegionId));
		if (this->startRegionId - 1 == queryPoint->getNodeRegionId()){
			//queryRegionRecievedFlag = true;
			if (nowRecievedNodeAmount >= 1){
				//computeToQueryDijkstra();
				if (candidateRegionToListenList.size() == 0)
					queryFinishedFlag = true;
			}
		}
		/*else if (queryRegionRecievedFlag == true && nowRecievedNodeAmount >= 1){
			computeToQueryDijkstra();
			if (candidateRegionToListenList.size() == 0)
				queryFinishedFlag = true;
		}*/

		//if finished listening cycle tail, start next listen from head
		if (this->startRegionId == allRegionList.size())
			this->startRegionId = 0;
	}
	//for each candidateList? R?_1, …, ? R?_i, …, R_n do
		//compute candidateList.spot(R_i) 1NN by Dijkstra;
	//computeKNNAnswer();
	for (int i = 0; i < candidateRegionIdList.size(); i++){
		cout << "candidate Region" << candidateRegionIdList.at(i) << endl;
	}

	/*if (queryFinishedFlag == false){
		computeToQueryDijkstra();
		queryFinishedFlag = true;
	}*/
	if (this->startRegionId == 0){
		latencyDataSize = computeIndexLatencySize(inputDir2, inputDir1, startRegionId, allRegionList.size() - 1);
		latencyDataSize += computeDataLatencySize(inputDir6, startRegionId, allRegionList.size() - 1);
	}
	else{
		latencyDataSize = computeIndexLatencySize(inputDir2, inputDir1, startRegionId, this->startRegionId - 1);
		latencyDataSize += computeDataLatencySize(inputDir6, startRegionId, this->startRegionId - 1);
	}
	broadcastCycleSize = computeIndexLatencySize(inputDir2, inputDir1, 0, allRegionList.size() - 1);
	broadcastCycleSize += computeDataLatencySize(inputDir6, 0, allRegionList.size() - 1);

	cout << endl << "///////////////////////////" << endl;

}

//compute final shortest path
void ClientRNN::computeToQueryDijkstra(){
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


	//--------?????@???I?? regionId ?d?? bnnn ???? borderNode?`?I?Z?÷ 

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
	for (int i = 0; i < 1; i++)
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