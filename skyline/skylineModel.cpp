#include "stdafx.h"
#include "skylineModel.h"
#include "math.h"
#include <algorithm>
#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>  
#include <vector>
#include "PointComponent.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include "string.h"
#include "stdlib.h"
#include "stdio.h"
#include <typeinfo>
#include <crtdbg.h>
#include <direct.h>
#include "EData.h"
#include "ListUDG.h"
#include "Destination.h"

#define _CRTDBG_MAP_ALLOC 
#define SIZE 100000
#define pi 3.1415926535897932384626433832795
#define EARTH_RADIUS 6378.137 //地球半徑 KM
#define _CRT_SECURE_NO_WARNINGS
using namespace std;

skylineModel::skylineModel()
{
	clientProcessedFlag = 0;
}


skylineModel::~skylineModel()
{
}

void skylineModel::create(int pointNum)
{
	for (int index = 0; index < pointNum;index++)
	{
		PointComponent* point = new PointComponent();
//		point->setPointX(rand() % 6+1);
//		point->setPointY(rand() % 6+1);
		point->setPointA(rand() % 8+1);
		point->setPointB(rand() % 8+1);
		points.push_back(point);
	}

	//cout << "number:" << getPoints()->size() << endl;
	//for (int index = 0; index < getPoints()->size(); index++)
	//{
	//	cout << index << ": [(" << getPoints()->at(index)->getPointX() << "," << getPoints()->at(index)->getPointY() << ")";
	//	cout << "  (" << getPoints()->at(index)->getPointA() << "," << getPoints()->at(index)->getPointB() << ")]" << endl;
	//}
}


vector<PointComponent*>* skylineModel::getPoints()				//取得全部的節點
{
	return &points;
}

vector<PointComponent*>* skylineModel::findSkyline(vector<PointComponent*>* points)	
{


	for (int index = 0; index< points->size(); index++)
	{
		for (int i = 0; i < points->size(); i++)
		{
			if (points->at(index)->getPointX() <= points->at(i)->getPointX() && points->at(index)->getPointY() <= points->at(i)->getPointY() && points->at(index)->getPointA() <= points->at(i)->getPointA() && points->at(index)->getPointB() <= points->at(i)->getPointB() && points->at(index)->getTotal()<points->at(i)->getTotal())
			{
				points->at(i)->setIsSkyline(false);
			}
		}
	}

	for (int index = 0; index < points->size(); index++)
	{
		if (points->at(index)->getIsSkyline())
		{
			skylinePoints.push_back(points->at(index));
		}
	}
	displaySkyline(skylinePoints);
	return &skylinePoints;
}

void skylineModel::display()
{
	cout << "How many points:";
	int num;
	cin >> num;
	create(num);
	cout << "Where is the query:";
	string queryPosition;
	cin >> queryPosition;

	size_t posOfFirstBracket = queryPosition.find("("),
		posOfComma = queryPosition.find(","),
		posOfSecondBracket = queryPosition.find(")");

	string positionX = queryPosition.substr(posOfFirstBracket + 1, posOfComma - posOfFirstBracket - 1);
	string positionY = queryPosition.substr(posOfComma + 1, posOfSecondBracket - posOfComma - 1);
	cout << "posX:" << positionX << endl;
	cout << "posY:" << positionY << endl;

	findSkyline(getPoints());
}

void skylineModel::findNonSpatialSkyline()
{
	cout << "How many points:";
	int num;
	cin >> num;
	create(num);
	vector<PointComponent*>* points;
	points = getPoints();
	for (int index = 0; index < points->size(); index++)
	{
		for (int i = 0; i < points->size(); i++)
		{
			if (points->at(index)->getPointA() <= points->at(i)->getPointA() && points->at(index)->getPointB() <= points->at(i)->getPointB() && points->at(index)->getNonSpatialNum() < points->at(i)->getNonSpatialNum())
			{
				points->at(i)->setIsSkyline(false);
			}
		}
	}

	for (int index = 0; index < points->size(); index++)
	{
		if (points->at(index)->getIsSkyline())
		{
			skylinePoints.push_back(points->at(index));
		}
	}
	displaySkyline(skylinePoints);
}


void skylineModel::displaySkyline(vector<PointComponent*> skylinePoints)
{
	cout << "number:" << skylinePoints.size() << endl << endl;
	for (int index = 0; index < skylinePoints.size(); index++)
	{
		if (skylinePoints.at(index)->getIsSkyline())
		{
			cout << index << ": [(" << skylinePoints.at(index)->getPointX() << "," << skylinePoints.at(index)->getPointY() << ")";
			cout << "  (" << skylinePoints.at(index)->getPointA() << "," << skylinePoints.at(index)->getPointB() << ")]" << endl;
		}
	}
}



void skylineModel::loadFile()
{
	cout << "Please enter your file direction!"<<endl;
	string fileDir;
	cin >> fileDir;//輸入檔案

	char line[SIZE];

	fstream fin;
	fstream file;      //宣告fstream物件
	fin.open(fileDir, ios::in);
	file.open("Reader.txt", ios::out);		//開啟檔案為輸出狀態，若檔案已存在則清除檔案內容重新寫入
	char input[SIZE];
	cout << "Please enter your siteNum:";
	cin >> input;
	int attributeFlag=0;

	char *delim = ",";
	char *pch;
	char result[SIZE];
	char *firstField;
	char *secondField;

	clock_t nStart = clock(); // 開始時間
	while (fin.getline(line, sizeof(line), '\n')){	//以行讀取
		strcpy(result, line);
		//cout << line << endl;
		pch = strtok(line, delim);
		attributeFlag = 0;

		while (pch != NULL)
		{
			attributeFlag++;
			if (attributeFlag == 1)
			{
				//		cout << result << endl;
				firstField = pch;
				break;
			}
			if (attributeFlag == 2)
			{
				//		cout << result << endl;
				secondField = pch;
				break;
			}
			if (strcmp(pch, input) == 0 && attributeFlag==3)
			{
		//		cout << result << endl;
				file << result << "\n";
				break;
			}
		//	cout << "pch:" << pch << endl;
		//	cout << "line:" << line << endl;
			pch = strtok(NULL, delim);
		}
	}
	file.close();       //關閉檔案
	clock_t nEnd = clock(); // 開始時間
	cout << (float)(nStart) << endl;
	cout << (float)(nEnd) << endl;
	cout << (float)(nEnd - nStart)/CLK_TCK << endl;
	cout << endl;
}


void skylineModel::writeLandmarkToMBRFile()
{
	cout << "Please enter TWN_LANDMARK.geo" << endl;
	string fileDir;
	cin >> fileDir;//輸入檔案

	char line[SIZE];

	fstream fin;
	ofstream file;      //宣告fstream物件
	fin.open(fileDir, ios::in);
	file.open("TWN_LANDMARK.txt", ios::out | ios::binary);		//開啟檔案為輸出狀態，若檔案已存在則清除檔案內容重新寫入

	int attributeFlag = 0;

	char *delim = ",";
	char *pch;
	char result[SIZE];
	char *firstField;
	char *secondField;

	clock_t nStart = clock(); // 開始時間
	while (fin.getline(line, sizeof(line), '\n')){	//以行讀取
		strcpy(result, line);
		//cout << line << endl;
		pch = strtok(line, delim);
		attributeFlag = 0;

		while (pch != NULL)
		{
			attributeFlag++;
			if (attributeFlag == 2)
			{
				file << pch << " ";
			//	break;
			}
			if (attributeFlag == 3)
			{
				file << pch << " 0 0";
				file << "\n";
				break;
			}
			pch = strtok(NULL, delim);
		}
	}
	file.close();       //關閉檔案
	clock_t nEnd = clock(); // 開始時間
	cout << (float)(nStart) << endl;
	cout << (float)(nEnd) << endl;
	cout << (float)(nEnd - nStart) / CLK_TCK << endl;
	cout << endl;
}

void skylineModel::writeRoadToMBRFile()
{
	cout << "Please enter TPEC_ROAD.geo" << endl;
	string fileDir;
	cin >> fileDir;//輸入檔案

	char line[SIZE];

	fstream fin;
	ofstream file;      //宣告fstream物件
	fin.open(fileDir, ios::in);
	string fileName;
	cout << "Enter file name.\n";
	cin >> fileName;
	file.open(fileName, ios::out);		//開啟檔案為輸出狀態，若檔案已存在則清除檔案內容重新寫入

	int attributeFlag = 0;

	char *delim = ",";
	char *point = ".";
	char *pch;
	char *splitFloat;
	char result[SIZE];
	char *firstField;
	char *secondField;
	int MBR_x = 0, MBR_y = 0, MBR_Length = 0, MBR_Height = 0;
	mbrInfo *mbrA = new mbrInfo();	//mbrA is new point
	mbrInfo *mbrB = new mbrInfo();	//mbrB is old point

	clock_t nStart = clock(); // 開始時間
	while (fin.getline(line, sizeof(line), '\n')){	//以行讀取

		//strcpy(result, line);
		//cout << line << endl;
	//	pch = strtok(line, delim);
		attributeFlag = 0;		//第幾個位置

		std::vector<char*> v;
		char* chars_array = strtok(line, delim);
		while (chars_array!=NULL)
		{
			v.push_back(chars_array);
			chars_array = strtok(NULL, delim);
		}

		for (size_t attributeFlag = 0; attributeFlag < v.size(); attributeFlag++)
		{
			if (attributeFlag % 2 == 0 && attributeFlag / 2 >= 1)
			{
				if (attributeFlag == 2)
				{
					splitFloat = strtok(v[attributeFlag], point);
				//	align(splitFloat);
					mbrA->setMbrX_int(atoi(splitFloat));
					splitFloat = strtok(NULL, point);
					mbrA->setMbrX_dec(align(splitFloat));
				}
				if (attributeFlag > 3)
				{
					mbrB->setMbrX_dec(mbrA->getMbrX_dec());
					mbrB->setMbrX_int(mbrA->getMbrX_int());

					splitFloat = strtok(v[attributeFlag], point);
					mbrA->setMbrX_int(atoi(splitFloat));
					splitFloat = strtok(NULL, point);
					mbrA->setMbrX_dec(align(splitFloat));
				}

			}
			if (attributeFlag % 2 == 1 && attributeFlag / 2 >= 1)
			{
				if (attributeFlag == 3)
				{
					splitFloat = strtok(v[attributeFlag], point);
					mbrA->setMbrY_int(atoi(splitFloat));
					splitFloat = strtok(NULL, point);
					mbrA->setMbrY_dec(align(splitFloat));
				}
				if (attributeFlag > 4)
				{
					mbrB->setMbrY_dec(mbrA->getMbrY_dec());
					mbrB->setMbrY_int(mbrA->getMbrY_int());

					splitFloat = strtok(v[attributeFlag], point);
					mbrA->setMbrY_int(atoi(splitFloat));
					splitFloat = strtok(NULL, point);
					mbrA->setMbrY_dec(align(splitFloat));
					int  l = 0, h = 0;
					if (mbrA->getMbrX_int() > mbrB->getMbrX_int())
					{
						l = (mbrA->getMbrX_dec() - mbrB->getMbrX_dec() + 1000000) / 2;
						int carry = mbrA->getMbrX_dec() + mbrB->getMbrX_dec();
						if (carry > 1000000)
						{

							int dec = abs(carry - 1000000) / 2;
							int order = (int)(log(dec) / log(10) + 1);

							file << mbrA->getMbrX_int() << ".";
							for (int i = 0; i < 6 - order; i++)
							{
								file << "0";
							}
							file << dec << " ";
						}
						else
						{
							int dec = abs(carry +1000000) / 2;
							int order = (int)(log(dec) / log(10) + 1);
							file << mbrB->getMbrX_int() << ".";
							for (int i = 0; i < 6 - order; i++)
							{
								file << "0";
							}
							file << dec << " ";
						}
					}
					else if (mbrA->getMbrX_int()<mbrB->getMbrX_int())
					{
						l = (mbrA->getMbrX_dec() - mbrB->getMbrX_dec() - 1000000) / 2;
						int carry = mbrA->getMbrX_dec() + mbrB->getMbrX_dec();
						if (carry > 1000000)
						{
							int dec = abs(carry - 1000000) / 2;
							int order = (int)(log(dec) / log(10) + 1);
							file << mbrB->getMbrX_int() << ".";
							for (int i = 0; i < 6 - order; i++)
							{
								file << "0";
							}
							file << dec << " ";
						}
						else
						{
							int dec = abs(carry + 1000000) / 2;
							int order = (int)(log(dec) / log(10) + 1);
							file << mbrA->getMbrX_int() << ".";
							for (int i = 0; i < 6 - order; i++)
							{
								file << "0";
							}
							file << dec << " ";
						}
					}
					else
					{
						l = (mbrA->getMbrX_dec() - mbrB->getMbrX_dec()) / 2;
						int carry = mbrA->getMbrX_dec() + mbrB->getMbrX_dec();
						int dec = abs(carry) / 2;
						int order = (int)(log(dec) / log(10) + 1);
						file << mbrA->getMbrX_int() << ".";
						for (int i = 0; i < 6 - order; i++)
						{
							file << "0";
						}
						file << dec << " ";

					}

					if (mbrA->getMbrY_int() > mbrB->getMbrY_int())
					{
						int carry = mbrA->getMbrY_dec() + mbrB->getMbrY_dec();
	
						h=(mbrB->getMbrY_dec() - mbrA->getMbrY_dec()-1000000)/2;
						if (carry > 1000000)
						{
							int dec = abs(carry - 1000000) / 2;
							int order = (int)(log(dec) / log(10) + 1);
							file << mbrA->getMbrY_int() << ".";
							for (int i = 0; i < 6 - order; i++)
							{
								file << "0";
							}
							file << dec << " ";
							if (dec == 499903)
							{
								cout << "111\n";
							}
						}
						else
						{
							int dec = abs(carry+1000000) / 2;
							int order = (int)(log(dec) / log(10) + 1);
							file << mbrB->getMbrY_int() << ".";
							for (int i = 0; i < 6 - order; i++)
							{
								file << "0";
							}
							file << dec << " ";
							if (dec == 499903)
							{
								cout << "111\n";
							}
						}

					}
					else if (mbrA->getMbrY_int() < mbrB->getMbrY_int())
					{
						h = (mbrA->getMbrY_dec() - mbrB->getMbrY_dec() - 1000000) / 2;
						int carry = mbrA->getMbrY_dec() + mbrB->getMbrY_dec();

						if (carry > 1000000)
						{
							int dec = abs(carry - 1000000) / 2;
							int order = (int)(log(dec) / log(10) + 1);
							file << mbrB->getMbrY_int() << ".";
							for (int i = 0; i < 6 - order; i++)
							{
								file << "0";
							}
							file << dec << " ";
						}
						else
						{
							int dec = abs(carry+1000000) / 2;
							int order = (int)(log(dec) / log(10) + 1);
							file << mbrA->getMbrY_int() << ".";
							for (int i = 0; i < 6 - order; i++)
							{
								file << "0";
							}
							file << dec << " ";
							if (dec == 499903)
							{
								cout << "111\n";
							}
						}

					}
					else
					{
						h = (mbrA->getMbrY_dec() - mbrB->getMbrY_dec() ) / 2;
						int carry = mbrA->getMbrY_dec() + mbrB->getMbrY_dec();
						int dec = abs(carry ) / 2;
						int order = (int)(log(dec) / log(10) + 1);
						file << mbrA->getMbrY_int() << ".";
						for (int i = 0; i < 6 - order; i++)
						{
							file << "0";
						}
						file << dec << " ";

					}

					//file << l*0.000001 << " " << h*0.000001 << "\n";
					if (l >= 0)
					{
						file << "0.";
					}
					else
						file << "-0.";

					for (int i = 0; i < 6 - (int)(log(abs(l)) / log(10) + 1); i++)
					{
						file << "0";
					}
					file << abs(l) << " ";
					if (h >= 0)
					{
						file << "0.";
					}
					else
						file << "-0.";

					for (int i = 0; i < 6 - (int)(log(abs(h)) / log(10) + 1); i++)
					{
						file << "0";
					}
					file << abs(h) << "\n";

					}
			}

		}

	}
	file.close();
}

void skylineModel::writeLocation(mbrInfo* mbrA,mbrInfo* mbrB)
{
	ofstream file;
	file.open("TPEC_ROAD.txt", ios::out);		//開啟檔案為輸出狀態，若檔案已存在則清除檔案內容重新寫入
	if (mbrA->getMbrX_int()>mbrB->getMbrX_int())
	{
		int carry = mbrA->getMbrX_dec() + mbrB->getMbrX_dec();
		int dec = abs(carry - 1000000) / 2;
		int order = (int)(log(dec) / log(10) + 1);
		if (carry>1000000)
		{
			file << mbrA->getMbrX_int() << ".";
			for (int i = 0; i < 6 - order; i++)
			{
				file << "0";
			}
			file << dec << " ";
		}
		else
		{
			file << mbrB->getMbrX_int() << ".";
			for (int i = 0; i < 6 - order; i++)
			{
				file << "0";
			}
			file << dec << " ";
		}
	}
	else if (mbrA->getMbrX_int()<mbrB->getMbrX_int())
	{
		int carry = mbrA->getMbrX_dec() + mbrB->getMbrX_dec();
		int dec = abs(carry - 1000000) / 2;
		int order = (int)(log(dec) / log(10) + 1);
		if (carry > 1000000)
		{
			file << mbrB->getMbrX_int() << ".";
			for (int i = 0; i < 6 - order; i++)
			{
				file << "0";
			}
			file << dec << " ";
		}
		else
		{
			file << mbrA->getMbrX_int() << ".";
			for (int i = 0; i < 6 - order; i++)
			{
				file << "0";
			}
			file << dec << " ";
		}
	}
	else
	{
		int carry = mbrA->getMbrX_dec() + mbrB->getMbrX_dec();
		file << mbrA->getMbrX_int() << "." << carry / 2 << " ";
	}

	if (mbrA->getMbrY_int() > mbrB->getMbrY_int())
	{
		int carry = mbrA->getMbrY_dec() + mbrB->getMbrY_dec();
		int dec = abs(carry - 1000000) / 2;
		int order = (int)(log(dec) / log(10) + 1);
		if (carry > 1000000)
		{
			file << mbrA->getMbrY_int() << ".";
			for (int i = 0; i < 6 - order; i++)
			{
				file << "0";
			}
			file << dec << " ";
		}
		else
		{
			file << mbrB->getMbrY_int() << ".";
			for (int i = 0; i < 6 - order; i++)
			{
				file << "0";
			}
			file << dec << " ";
		}
	}
	else if (mbrA->getMbrY_int() > mbrB->getMbrY_int())
	{
		int carry = mbrA->getMbrY_dec() + mbrB->getMbrY_dec();
		int dec = abs(carry - 1000000) / 2;
		int order = (int)(log(dec) / log(10) + 1);
		if (carry > 1000000)
		{
			file << mbrB->getMbrY_int() << ".";
			for (int i = 0; i < 6 - order; i++)
			{
				file << "0";
			}
			file << dec << " ";
		}
		else
		{
			file << mbrA->getMbrY_int() << ".";
			for (int i = 0; i < 6 - order; i++)
			{
				file << "0";
			}
			file << dec << " ";
		}
			
	}
	else
	{
		int carry = mbrA->getMbrY_dec() + mbrB->getMbrY_dec();
		file << mbrA->getMbrY_int() << "." << carry / 2 << " ";
	}
	cout << "0\n";
	file << "\n";
}

int skylineModel::align(char* a)
{
	int num;
	num = atoi(a);
	//cout << strlen(a);
	if (strlen(a)!=6)
	{
		//cout << strlen(a);
		num = atoi(a);
		for (int i = 0; i < 6 - strlen(a); i++)
		{
			num *= 10;
		}
	}
	return num;
}

void skylineModel::findIntersection()
{
	cout << "Please enter LIC_NODE.geo" << endl;
	string fileDir;
	cin >> fileDir;//輸入檔案

	cout << "Please enter output file" << endl;
	string outputDir;
	cin >> outputDir;//輸入檔案

	char line[SIZE];

	char *pch;
	char *delim = ",";
	fstream fin;
	ofstream file;      //宣告fstream物件
	fin.open(fileDir, ios::in);
	file.open(outputDir, ios::out | ios::binary);		//開啟檔案為輸出狀態，若檔案已存在則清除檔案內容重新寫入

	clock_t nStart = clock(); // 開始時間
	while (fin.getline(line, sizeof(line), '\n')){	//以行讀取
//		strcpy(result, line);
		//cout << line << endl;
		pch = strtok(line, delim);
		int attributeFlag = 0;
		double id = 0, longitude = 0, latitude = 0;

		while (pch != NULL)
		{
			attributeFlag++;
			if (attributeFlag == 1)
			{
				id = strtod(pch,NULL);
				//file << pch << ",";
				//	break;
			}
			if (attributeFlag==2)
			{
				longitude = strtod(pch, NULL);
			}
			if (attributeFlag == 3)
			{
				latitude = strtod(pch, NULL);
		//		file << pch << " 0 0";
		//		file << "\n";
		//		break;
			}
			if (latitude>26.13 && latitude<26.18)
			{

				cout << setprecision(9) << "id:" << id << " longitude:" << longitude << " latitude:" << latitude << "\n";
				file << setprecision(9) << id << "," << longitude << "," << latitude << endl;
			}
			pch = strtok(NULL, delim);
		}
	}
}

double  skylineModel::rad(double d)
{
	return d * pi / 180.0;
}

double skylineModel::computeSectionDistance(double x_lat, double x_lng, double y_lat, double y_lng)
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

//產生節點距離txt
void skylineModel::writeAllOriginRoadDistance()
{
	cout << "Please enter LIC_Road.geo" << endl;
	string fileDir;
	cin >> fileDir;//輸入檔案

	cout << "Please enter output file" << endl;
	string outputDir;
	cin >> outputDir;//輸入檔案

	char line[SIZE];

	char *pch;
	char *delim = ",";
	fstream fin;
	ofstream file;      //宣告fstream物件
	fin.open(fileDir, ios::in);
	file.open(outputDir, ios::out | ios::binary);		//開啟檔案為輸出狀態，若檔案已存在則清除檔案內容重新寫入
	clock_t nStart = clock(); // 開始時間
	
	//以行讀取
	while (fin.getline(line, sizeof(line), '\n'))
	{	
		pch = strtok(line, delim);
		int attributeFlag = 0;
		double id = 0, turningAmount = 0, roadDist = 0;
		vector<Node*> turningPointList;

		while (pch != NULL)
		{
			Node* turningPoint;
			attributeFlag++;
			//記錄ID, 轉折點, 起點
			if (attributeFlag == 1)
			{
				id = strtod(pch, NULL);
			}
			else if (attributeFlag == 2)
			{
				turningAmount = strtod(pch, NULL);
			}

			//將起點&終點交替放入register
			//_CrtDumpMemoryLeaks();
			if (attributeFlag % 2 == 1 && attributeFlag != 1)
			{
				//要使用時再分配，否則會有記憶體洩漏的狀況發生 -- 重要!!
				turningPoint = new Node();
				//_CrtDumpMemoryLeaks();
				turningPoint->setLongtitude(strtod(pch, NULL));
				turningPointList.push_back(turningPoint);
			}
			else if (attributeFlag % 2 == 0 && attributeFlag != 2)
			{
				turningPoint = turningPointList.back();
				turningPoint->setLatitude(strtod(pch, NULL));
				if (turningPointList.size() > 2)
				{
					roadDist = computeRoadDistance(turningPointList);
				}
			}

			//記錄終點並計算 dist(起點, 終點)
			if (turningAmount > 0 && attributeFlag == turningAmount * 2 + 2)
			{
				file << setprecision(9) << id << "," << roadDist << "," << turningPointList.front()->getLongtitude() << "," << turningPointList.front()->getLatitude() << "," << turningPointList.back()->getLongtitude() << "," << turningPointList.back()->getLatitude() << endl;
				for (int i = 0; i < turningPointList.size(); i++)
				{
					turningPoint = turningPointList.at(i);
					delete turningPoint;
				}
				vector<Node*>().swap(turningPointList);
			}
			pch = strtok(NULL, delim);
		}
	}
	//_CrtDumpMemoryLeaks();
}


void skylineModel::writeAllBorderRoadDistance(string inputDir, string outputDir)
{
	outputDir = outputDir + ".bn";

	int lineNumber = 0;
	char line[SIZE];
	char *pch;
	char *delim = ",";
	fstream fin;
	ofstream file;      //宣告fstream物件
	fin.open(inputDir, ios::in);
	file.open(outputDir, ios::out | ios::binary);		//開啟檔案為輸出狀態，若檔案已存在則清除檔案內容重新寫入
	clock_t nStart = clock(); // 開始時間

	//以行讀取
	while (fin.getline(line, sizeof(line), '\n'))
	{
		pch = strtok(line, delim);
		int attributeFlag = 0;
		int part_x, part_y;
		string partitionType;
		string roadType;
		double id = 0, turningAmount = 0, roadDist = 0;
		vector<double> partitionLongtitudeList;
		vector<double> partitionLatitudeList;
		vector<Node*> turningPointList;
		lineNumber++;

		//read Grid partition information
		if (lineNumber < 4)
		{
			if (lineNumber == 1)
			{
				while (pch != NULL)
				{
					attributeFlag++;
					if (attributeFlag == 1)
						partitionType = pch;
					else if (attributeFlag == 2)
						part_x = strtod(pch, NULL);
					else if (attributeFlag == 3)
						part_y = strtod(pch, NULL);
					pch = strtok(NULL, delim);
				}
				file  << setprecision(9) << partitionType << "," << part_x << "," << part_y << endl;
			}
			else if (lineNumber == 2)
			{
				while (pch != NULL)
				{
					partitionLongtitudeList.push_back(strtod(pch, NULL));
					pch = strtok(NULL, delim);
				}
				for (int i = 0; i < partitionLongtitudeList.size(); i++)
				{
					file << setprecision(9) << partitionLongtitudeList.at(i);
					if (i != partitionLongtitudeList.size() - 1)
					{
						file << ",";
					}
				}
				file << endl;
			}
			else if (lineNumber == 3)
			{
				while (pch != NULL)
				{
					partitionLatitudeList.push_back(strtod(pch, NULL));
					pch = strtok(NULL, delim);
				}
				for (int i = 0; i < partitionLatitudeList.size(); i++)
				{
					file << setprecision(9) << partitionLatitudeList.at(i);
					if (i != partitionLatitudeList.size() - 1)
					{
						file << ",";
					}
				}
				file << endl;
			}
		}
		else
		{
			while (pch != NULL)
			{
				Node* turningPoint;
				attributeFlag++;
				//記錄ID, 轉折點, 起點
				if (attributeFlag == 1)
				{
					id = strtod(pch, NULL);
				}
				else if (attributeFlag == 2)
				{
					turningAmount = strtod(pch, NULL);
				}
				else if (attributeFlag == 3)
				{
					roadType = pch;
				}

				//將起點&終點交替放入register
				//_CrtDumpMemoryLeaks();
				if (attributeFlag % 2 == 1 && attributeFlag != 1 && attributeFlag != 3)
				{
					turningPoint = turningPointList.back();
					turningPoint->setLatitude(strtod(pch, NULL));
					if (turningPointList.size() > 1)
					{
						roadDist = computeRoadDistance(turningPointList);
					}
				}
				else if (attributeFlag % 2 == 0 && attributeFlag != 2)
				{
					//要使用時再分配，否則會有記憶體洩漏的狀況發生 -- 重要!!
					turningPoint = new Node();
					turningPoint->setLongtitude(strtod(pch, NULL));
					turningPointList.push_back(turningPoint);
				}

				//記錄終點並計算 dist(起點, 終點)
				if (turningAmount > 0 && attributeFlag == turningAmount * 2 + 3)
				{
					file << setprecision(9) << id << "," << turningAmount << "," << roadType << "," << roadDist << "," << turningPointList.front()->getLongtitude() << "," << turningPointList.front()->getLatitude() << "," << turningPointList.back()->getLongtitude() << "," << turningPointList.back()->getLatitude() << endl;
					for (int i = 0; i < turningPointList.size(); i++)
					{
						turningPoint = turningPointList.at(i);
						delete turningPoint;
					}
					vector<Node*>().swap(turningPointList);
				}
				pch = strtok(NULL, delim);
			}
		}
	}
	//_CrtDumpMemoryLeaks();
}


double skylineModel::computeRoadDistance(vector<Node*> roadTurningPointList)
{
	double roadDist = 0;
	for (int i = 1; i < roadTurningPointList.size(); i++)
	{
		roadDist = roadDist + abs(computeSectionDistance(roadTurningPointList.at(i-1)->getLatitude(), roadTurningPointList.at(i-1)->getLongtitude(), roadTurningPointList.at(i)->getLatitude(), roadTurningPointList.at(i)->getLongtitude()));
	}

	return roadDist;
}

void skylineModel::writeGridPartition(string inputDir, string outputDir, int part_x, int part_y)
{
	char line[SIZE];
	char *pch;
	char *delim = ",";
	int roadId;
	double maxLongtitude = -1, maxLatitude = -1;
	double minLongtitude = -1, minLatitude = -1;
	vector<string> roadTypeList;
	vector<double> partitionLongtitudeList;
	vector<double> partitionLatitudeList;
	fstream fin;
	fin.open(inputDir, ios::in);


	clock_t nStart = clock(); // 開始時間
	while (fin.getline(line, sizeof(line), '\n')){	//以行讀取
		pch = strtok(line, delim);
		if (pch == NULL)
			break;
		int attributeFlag = 0;
		double id = 0, longitude = 0, latitude = 0;

		while (pch != NULL)
		{
			attributeFlag++;
			if (attributeFlag != 1 && attributeFlag != 2)
			{
				if (attributeFlag % 2 == 1)
				{
					longitude = strtod(pch, NULL);
					if (maxLongtitude == -1)
					{
						maxLongtitude = longitude;
						minLongtitude = longitude;
					}
					else
					{
						//maxNode->setLongtitude(max(longitude, maxNode->getLongtitude()));
						if (longitude > maxLongtitude)
							maxLongtitude = longitude;
						else if (longitude < minLongtitude)
							minLongtitude = longitude;
					}
				}
				else
				{
					latitude = strtod(pch, NULL);
					if (maxLatitude == -1)
					{
						maxLatitude = latitude;
						minLatitude = latitude;
					}
					else
					{
						if (latitude > maxLatitude)
							maxLatitude = latitude;
						else if (latitude < minLatitude)
							minLatitude = latitude;
					}
				}
			}
			pch = strtok(NULL, delim);
		}
	}

	maxLongtitude += 0.0005;
	minLongtitude -= 0.0005;
	maxLatitude += 0.0005;
	minLatitude -= 0.0005;

	//找出切割的經緯度並存入VECTOR
	double x_Interval = (maxLongtitude - minLongtitude) / part_x;
	//cout << "Longtitude : " << endl;
	for (int i = 0; i <= part_x; i++)
	{
		partitionLongtitudeList.push_back(minLongtitude + (x_Interval * i));
		//cout << setprecision(9) << minLongtitude + (x_Interval * i) << endl;
	}
	double y_Interval = (maxLatitude - minLatitude) / part_y;
	//cout << "Latitude : " << endl;
	for (int i = 0; i <= part_y; i++)
	{
		partitionLatitudeList.push_back(minLatitude + (y_Interval * i));
		//cout << setprecision(9) << minLatitude + (y_Interval * i) << endl;
	}


	//重讀檔案以找出Border Node
	ofstream file;      //宣告fstream物件
	outputDir = outputDir + ".bgeo";
	fstream fin1;
	fin1.open(inputDir, ios::in);
	file.open(outputDir, ios::out | ios::binary);

	//print Grid partition information
	file << "Grid," << part_x << "," << part_y << endl;
	for (int i = 0; i < partitionLongtitudeList.size(); i++)
	{
		file << setprecision(9) << partitionLongtitudeList.at(i);
		if (i != partitionLongtitudeList.size() - 1)
			file << ",";
	}
	file << endl;
	for (int i = 0; i < partitionLatitudeList.size(); i++)
	{
		file << setprecision(9) << partitionLatitudeList.at(i);
		if (i != partitionLatitudeList.size() - 1)
			file << ",";
	}
	file << endl;

	while (fin1.getline(line, sizeof(line), '\n')){	//以行讀取
		vector<Node*> nodeList;
		vector<Road*> roadList;
		pch = strtok(line, delim);
		if (pch == NULL)
			break;
		int attributeFlag = 0;
		double id = 0, longitude = 0, latitude = 0;

		while (pch != NULL)
		{
			attributeFlag++;

			//依序檢查每個Road Section檢查並找出Border Node
			if (attributeFlag != 1 && attributeFlag != 2)
			{
				if (attributeFlag % 2 == 1)
				{
					longitude = strtod(pch, NULL);

					Node* roadIntersection = new Node();
					roadIntersection->setLongtitude(longitude);
					nodeList.push_back(roadIntersection);
				}
				else
				{
					latitude = strtod(pch, NULL);

					Node* roadIntersection = nodeList.back();
					roadIntersection->setLatitude(latitude);
					
					if (nodeList.size() > 1)
					{
						Road* roadSection = new Road(nodeList.at(nodeList.size() - 2), roadIntersection);
						//如果有找到Border Node
						if (isBorderNodeInside(roadSection, partitionLongtitudeList, partitionLatitudeList) == true)
						{
							addBorderNodeToList(roadSection, partitionLongtitudeList, partitionLatitudeList);
							nodeList.insert(nodeList.end() - 1, borderNodeList.back());
						}
					}
				}
			}
			else if(attributeFlag == 1)
			{
				roadId = strtod(pch, NULL);
			}
			pch = strtok(NULL, delim);
		}

		int turningPointCounter = 0;
		vector<int> turningPointCounterList;
		//記錄此條路中是由幾個Border Node組成
		for (int i = 0; i < nodeList.size(); i++)
		{
			turningPointCounter++;
			if (i == 0 || i == nodeList.size()-1)
			{
				if (nodeList.at(i)->getNodeType() != "BorderNode")
					roadTypeList.push_back("n");
				if (i == nodeList.size() - 1)
					turningPointCounterList.push_back(turningPointCounter);
			}
			if (nodeList.at(i)->getNodeType() == "BorderNode")
			{
				roadTypeList.push_back("b");
				turningPointCounterList.push_back(turningPointCounter);
				turningPointCounter = 1;
			}
		}

		//印出該條路的資訊
		int roadTypeListCounter = 0;
		//要分成兩行寫，否則會有超出範圍的錯誤
		file << roadId << "," << turningPointCounterList.at(roadTypeListCounter) << ",";
		file << roadTypeList.at(roadTypeListCounter++) << roadTypeList.at(roadTypeListCounter) << ",";
		for (int i = 0; i < nodeList.size(); i++)
		{
			if (nodeList.at(i)->getNodeType() != "BorderNode")
			{
				//檢查有沒有到該行的最後一個
				if (i != nodeList.size() - 1)
					file << setprecision(9) << nodeList.at(i)->getLongtitude() << "," << nodeList.at(i)->getLatitude() << ",";
				else
					file << setprecision(9) << nodeList.at(i)->getLongtitude() << "," << nodeList.at(i)->getLatitude();
			}
			else
			{
				file << setprecision(9) << nodeList.at(i)->getLongtitude() << "," << nodeList.at(i)->getLatitude() << endl;
				if (i != nodeList.size() - 1)
				{
					file << roadId << "," << turningPointCounterList.at(roadTypeListCounter) << ",";
					file << roadTypeList.at(roadTypeListCounter++) << roadTypeList.at(roadTypeListCounter) << "," << nodeList.at(i)->getLongtitude() << "," << nodeList.at(i)->getLatitude() << ",";
				}
			}
		}
		roadTypeList.resize(0);
		deleteNodeList(nodeList);
		deleteRoadList(roadList);
		file << endl;
	}

	//cout << "maxNode : " << setprecision(10) << "long : " << maxLongtitude << "   ||  lat : " << maxLatitude << endl;
	//cout << "minNode : " << setprecision(10) << "long : " << minLongtitude << "   ||  lat : " << minLatitude << endl;
	//cout << "maxLong - minLong : " << maxLongtitude - minLongtitude << endl;
	//cout << "maxLat - minLat : " << maxLatitude - minLatitude << endl;
}

bool skylineModel::isBorderNodeInside(Road* roadSection, vector<double> x_partitionLineList, vector<double> y_partitionLineList)
{
	Node* startNode = roadSection->getStartNode();
	Node* endNode = roadSection->getEndNode();
	for (int i = 0; i < x_partitionLineList.size(); i++)
	{
		if ((startNode->getLongtitude() - x_partitionLineList.at(i)) * (endNode->getLongtitude() - x_partitionLineList.at(i)) < 0)
		{
			return true;
		}
	}
	for (int i = 0; i < y_partitionLineList.size(); i++)
	{
		if ((startNode->getLatitude() - y_partitionLineList.at(i)) * (endNode->getLatitude() - y_partitionLineList.at(i)) < 0)
		{
			return true;
		}
	}

	return false;
}

void skylineModel::addBorderNodeToList(Road* roadSection, vector<double> x_partitionLineList, vector<double> y_partitionLineList)
{
	Node* startNode = roadSection->getStartNode();
	Node* endNode = roadSection->getEndNode();
	for (int i = 0; i < x_partitionLineList.size(); i++)
	{
		if ((startNode->getLongtitude() - x_partitionLineList.at(i)) * (endNode->getLongtitude() - x_partitionLineList.at(i)) < 0)
		{
			//分點公式取出點cordinate
			BorderNode* borderNode = findBorderNode(roadSection, x_partitionLineList.at(i), -1);
			borderNodeList.push_back(borderNode);
		}
	}
	for (int i = 0; i < y_partitionLineList.size(); i++)
	{
		if ((startNode->getLatitude() - y_partitionLineList.at(i)) * (endNode->getLatitude() - y_partitionLineList.at(i)) < 0)
		{
			//分點公式取出點cordinate
			BorderNode* borderNode = findBorderNode(roadSection, -1, y_partitionLineList.at(i));
			borderNodeList.push_back(borderNode);
		}
	}
}

BorderNode* skylineModel::findBorderNode(Road* roadSection, double x_partitionLine, double y_partitionLine)
{
	Node* startNode = roadSection->getStartNode();
	Node* endNode = roadSection->getEndNode();
	BorderNode* borderNode = new BorderNode();
	double m = 0, n = 0;
	if (x_partitionLine > 0)
	{
		m = x_partitionLine - startNode->getLongtitude();
		n = x_partitionLine - endNode->getLongtitude();
		m = abs(m);
		n = abs(n);
		borderNode->setLongtitude(x_partitionLine);
		borderNode->setLatitude(n / (m + n)*startNode->getLatitude() + m / (m + n)*endNode->getLatitude());
	}
	if (y_partitionLine > 0)
	{
		m = y_partitionLine - startNode->getLatitude();
		n = y_partitionLine - endNode->getLatitude();
		m = abs(m);
		n = abs(n);
		borderNode->setLongtitude(n / (m + n)*startNode->getLongtitude() + m / (m + n)*endNode->getLongtitude());
		borderNode->setLatitude(y_partitionLine);

	}

	return borderNode;
}

void skylineModel::writeRandomSpots(string inputDir, string outputDir, int viewPointAmount)
{
	int lineNumber = 0;
	char line[SIZE];
	vector<int> nodeLineNumberList;
	char *pch;
	string str1;
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
	fin.open(inputDir, ios::in);

	ofstream fwriter;
	fwriter.open(outputDir + ".spot", ios::out);
	vector<int> randIdList = nonRepeatRandAndSort(nodeLineNumberList.size(), viewPointAmount);
	lineNumber = 0;
	int attributeFlag = 0;
	int index = 0;
	while (fin.getline(line, sizeof(line), '\n')){	//以行讀取
		lineNumber++;
		if (lineNumber > 3)
		{
			if (nodeLineNumberList.at(randIdList.at(index)) == lineNumber)
			{
				fwriter << line << endl;
				if (randIdList.size() == index + 1)
					break;
				else
					index++;
			}
		}
	}
}

vector<int> skylineModel::nonRepeatRandAndSort(int total, int needNum)
{
	vector<int> card;
	vector<int> randomList;
	srand(time(NULL));
	for (int i = 0; i < total;i++)
	{
		card.push_back(i);
	}
	for (int index = 0; index < total;index++)
	{
		int n = rand() % total;
		int m = rand() % total;
		swap(card.at(m), card.at(n));
	}
	for (int index = 0; index < needNum;index++)
	{
		randomList.push_back(card.at(index));
	}
	sort(randomList.begin(),randomList.end());
	return randomList;
}

void skylineModel::deleteNodeList(vector<Node*> nodeList)
{
	for (int i = 0; i < nodeList.size(); i++)
	{
		Node* nodeReg = nodeList.at(i);
		delete nodeReg;
	}
	vector<Node*>().swap(nodeList);
}

void skylineModel::deleteRoadList(vector<Road*> roadList)
{
	for (int i = 0; i < roadList.size(); i++)
	{
		Road* roadReg = roadList.at(i);
		roadReg->~Road();
	}
	vector<Road*>().swap(roadList);
}

void skylineModel::writeRegionNodeData(string inputDir, string outputDir)
{
	char line[SIZE];
	int lineNumber = 0;
	char *pch;
	char *delim = ",";
	fstream fin;
	ofstream file;      //宣告fstream物件
	fin.open(inputDir, ios::in);
	file.open(outputDir + ".nreg", ios::out | ios::binary);		//開啟檔案為輸出狀態，若檔案已存在則清除檔案內容重新寫入
	clock_t nStart = clock(); // 開始時間

	vector<double> partitionLongtitudeList;
	vector<double> partitionLatitudeList;
	vector<Region*> regionList;
	vector<Node*> nodeList;

	//以行讀取
	while (fin.getline(line, sizeof(line), '\n'))
	{
		Node* regionNode;
		pch = strtok(line, delim);
		int attributeFlag = 0;
		string partitionType;
		string roadType;
		int part_x, part_y;
		double roadId = 0, turningAmount = 0, roadDist = 0;
		lineNumber++;

		//get Grid Partition information
		if (lineNumber < 4)
		{
			if (lineNumber == 1)
			{
				while (pch != NULL)
				{
					attributeFlag++;
					if (attributeFlag == 1)
						partitionType = pch;
					else if (attributeFlag == 2)
						part_x = strtod(pch, NULL);
					else if (attributeFlag == 3)
						part_y = strtod(pch, NULL);
					pch = strtok(NULL, delim);
				}
				file << partitionType << "," << part_x << "," << part_y << endl;
			}
			else if (lineNumber == 2)
			{
				while (pch != NULL)
				{
					partitionLongtitudeList.push_back(strtod(pch, NULL));
					pch = strtok(NULL, delim);
				}
				//print grid partition line
				for (int i = 0; i < partitionLongtitudeList.size(); i++)
				{
					file << setprecision(9) << partitionLongtitudeList.at(i);
					if (i != partitionLongtitudeList.size() - 1)
					{
						file << ",";
					}
				}
				file << endl;
			}
			else if (lineNumber == 3)
			{
				while (pch != NULL)
				{
					partitionLatitudeList.push_back(strtod(pch, NULL));
					pch = strtok(NULL, delim);
				}
				//print grid partition line
				for (int i = 0; i < partitionLatitudeList.size(); i++)
				{
					file << setprecision(9) << partitionLatitudeList.at(i);
					if (i != partitionLatitudeList.size() - 1)
					{
						file << ",";
					}
				}
				file << endl;

				//get Regions bounds
				regionList = getRegionListByBound(partitionLongtitudeList, partitionLatitudeList);
			}
		}
		else    //get Border Node
		{
			while (pch != NULL)
			{
				attributeFlag++;
				//記錄ID, 轉折點, 起點
				if (attributeFlag == 1)
				{
					roadId = strtod(pch, NULL);
				}
				else if (attributeFlag == 2)
				{
					turningAmount = strtod(pch, NULL);
				}
				else if (attributeFlag == 3)
				{
					roadType = pch;
				}
				else if (attributeFlag == 4)
				{
					roadDist = strtod(pch, NULL);
				}

				//將起點&終點交替放入register
				//get Node
				if (roadType == "nn")
				{
					if (attributeFlag == 5)
					{
						regionNode = new Node();
						regionNode->setLongtitude(strtod(pch, NULL));
						regionNode->setRoadIdList(roadId);
						//nodeList.push_back(regionNode);
					}
					else if (attributeFlag == 6)
					{
						//regionNode = nodeList.back();
						regionNode->setLatitude(strtod(pch, NULL));
						int duplicateNodeInd = checkNodeExistAndReturnNodeInd(nodeList, regionNode);
						if (duplicateNodeInd != -1)
						{
							if (checkNodeRoadIdExist(nodeList.at(duplicateNodeInd), regionNode->getRoadIdList().back()) == false)
								nodeList.at(duplicateNodeInd)->setRoadIdList(regionNode->getRoadIdList().back());
						}
						else
							nodeList.push_back(regionNode);
					}
					if (attributeFlag == 7)
					{
						regionNode = new Node();
						regionNode->setLongtitude(strtod(pch, NULL));
						regionNode->setRoadIdList(roadId);
						//nodeList.push_back(regionNode);
					}
					else if (attributeFlag == 8)
					{
						//regionNode = nodeList.back();
						regionNode->setLatitude(strtod(pch, NULL));
						int duplicateNodeInd = checkNodeExistAndReturnNodeInd(nodeList, regionNode);
						if (duplicateNodeInd != -1)
						{
							if (checkNodeRoadIdExist(nodeList.at(duplicateNodeInd), regionNode->getRoadIdList().back()) == false)
								nodeList.at(duplicateNodeInd)->setRoadIdList(regionNode->getRoadIdList().back());
						}
						else
							nodeList.push_back(regionNode);
					}
				}
				else if (roadType == "nb")
				{
					if (attributeFlag == 5)
					{
						regionNode = new Node();
						regionNode->setLongtitude(strtod(pch, NULL));
						regionNode->setRoadIdList(roadId);
						//nodeList.push_back(regionNode);
					}
					else if (attributeFlag == 6)
					{
						//regionNode = nodeList.back();
						regionNode->setLatitude(strtod(pch, NULL));
						int duplicateNodeInd = checkNodeExistAndReturnNodeInd(nodeList, regionNode);
						if (duplicateNodeInd != -1)
						{
							if (checkNodeRoadIdExist(nodeList.at(duplicateNodeInd), regionNode->getRoadIdList().back()) == false)
								nodeList.at(duplicateNodeInd)->setRoadIdList(regionNode->getRoadIdList().back());
						}
						else
							nodeList.push_back(regionNode);
					}
				}
				else if (roadType == "bn")
				{
					if (attributeFlag == 7)
					{
						regionNode = new Node();
						regionNode->setLongtitude(strtod(pch, NULL));
						regionNode->setRoadIdList(roadId);
						//nodeList.push_back(regionNode);
					}
					else if (attributeFlag == 8)
					{
						//regionNode = nodeList.back();
						regionNode->setLatitude(strtod(pch, NULL));
						int duplicateNodeInd = checkNodeExistAndReturnNodeInd(nodeList, regionNode);
						if (duplicateNodeInd != -1)
						{
							if (checkNodeRoadIdExist(nodeList.at(duplicateNodeInd), regionNode->getRoadIdList().back()) == false)
								nodeList.at(duplicateNodeInd)->setRoadIdList(regionNode->getRoadIdList().back());
						}
						else
							nodeList.push_back(regionNode);
					}
				}
				pch = strtok(NULL, delim);
			}
		}
	}

	//get Node Regions
	for (int i = 0; i < nodeList.size(); i++)
	{
		for (int j = 0; j < regionList.size(); j++)
		{
			if (nodeList.at(i)->getLongtitude() >= regionList.at(j)->getLongtitudeLeft() && nodeList.at(i)->getLongtitude() <= regionList.at(j)->getLongtitudeRight() && nodeList.at(i)->getLatitude() >= regionList.at(j)->getLatitudeBottom() && nodeList.at(i)->getLatitude() <= regionList.at(j)->getLatitudeTop())
			{
				regionList.at(j)->setNodeList(nodeList.at(i));
			}
		}
	}

	//print node list
	for (int i = 0; i < regionList.size(); i++)
	{
		vector<Node*> regionNList = regionList.at(i)->getNodeList();
		for (int j = 0; j < regionNList.size(); j++)
		{
			file << setprecision(9) << regionNList.at(j)->getLongtitude() << "," << regionNList.at(j)->getLatitude() << "," << regionList.at(i)->getId() << ";";
			for (int i = 0; i < regionNList.at(j)->getRoadIdList().size(); i++)
			{
				file << setprecision(9) << regionNList.at(j)->getRoadIdList().at(i);
				if (i != regionNList.at(j)->getRoadIdList().size() - 1)
					file << ",";
			}
			file << endl;
		}
	}
	//_CrtDumpMemoryLeaks();
}

void skylineModel::writeOrderedIdRegionNodeData(string inputDir, string outputDir)
{
	char line[SIZE];

	fstream fin1;
	ofstream file;
	//	file.close();
	outputDir = outputDir + ".oobjreg";
	file.open(outputDir, ios::out);
	fin1.open(inputDir, ios::in);
	//cout << "inputDir:" << inputDir << endl;
	//cout << "outputDir:" << outputDir << endl;

	int lineNumber = 0;
	char *pch;
	char *delim = ",";
	int nodeId = 0, borderId = 0;
	vector<Node*> nodeList;
	int totalNode = 0, totalBorderNode = 0;
	vector<int> pos;
	//cout << "read file " << inputDir << endl;

	while (fin1.getline(line, sizeof(line), '\n'))
	{
		pch = strtok(line, delim);
		int attributeFlag = 0;
		lineNumber++;

		if (lineNumber > 3)
		{
			Node* regNode;
			while (pch != NULL)
			{
				attributeFlag++;
				if (attributeFlag == 1)
				{
					regNode = new Node(); 
					regNode->setNodeName(pch);
					nodeList.push_back(regNode);
				}
				else if (attributeFlag == 2)
				{
					regNode->setLongtitude(strtod(pch, NULL));
				}
				else if (attributeFlag == 3)
				{
					regNode->setLatitude(strtod(pch, NULL));
				}
				else if (attributeFlag == 4)
				{
					regNode->setNodeRegionId(atoi(pch));
				}

				pch = strtok(NULL, delim);
			}
		}
	}

	//get node id & sort by id
	delim = "_";
	for (int i = 0; i < nodeList.size(); i++)
	{
		strcpy(line, nodeList.at(i)->getNodeName().c_str());
		pch = strtok(line, delim);
		int attributeFlag = 0;
		while (pch != NULL)
		{
			attributeFlag++;
			if (attributeFlag == 2)
				nodeList.at(i)->setNodeId(atoi(pch));

			pch = strtok(NULL, delim);
		}
	}
	sort(nodeList.begin(), nodeList.end(), Node());

	//write .oobjreg file
	for (int i = 0; i < nodeList.size(); i++)
	{
		if (i > 0 && nodeList.at(i)->getNodeName() != nodeList.at(i - 1)->getNodeName())
		{
			file << setprecision(9) << nodeList.at(i)->getNodeName() << ",";
			file << setprecision(9) << nodeList.at(i)->getLongtitude() << ",";
			file << setprecision(9) << nodeList.at(i)->getLatitude() << ",";
			file << setprecision(9) << nodeList.at(i)->getNodeRegionId() << endl;
		}
		else if (i == 0)
		{
			file << setprecision(9) << nodeList.at(i)->getNodeName() << ",";
			file << setprecision(9) << nodeList.at(i)->getLongtitude() << ",";
			file << setprecision(9) << nodeList.at(i)->getLatitude() << ",";
			file << setprecision(9) << nodeList.at(i)->getNodeRegionId() << endl;
		}
	}
}

void skylineModel::writeRegionBorderData(string inputDir, string outputDir)
{
	BorderNode* regionBorderNode;
	char line[SIZE];
	int lineNumber = 0;
	char *pch;
	char *delim = ",";
	fstream fin;
	ofstream file;      //宣告fstream物件
	fin.open(inputDir, ios::in);
	file.open(outputDir + ".breg", ios::out | ios::binary);		//開啟檔案為輸出狀態，若檔案已存在則清除檔案內容重新寫入
	clock_t nStart = clock(); // 開始時間

	int regionId = 0;
	vector<double> partitionLongtitudeList;
	vector<double> partitionLatitudeList;
	vector<Region*> regionList;
	vector<BorderNode*> borderNodeList;

	//以行讀取
	while (fin.getline(line, sizeof(line), '\n'))
	{
		pch = strtok(line, delim);
		int attributeFlag = 0;
		string partitionType;
		string roadType;
		int part_x, part_y;
		double roadId = 0, turningAmount = 0, roadDist = 0;
		lineNumber++;

		//get Grid Partition information
		if (lineNumber < 4)
		{
			if (lineNumber == 1)
			{
				while (pch != NULL)
				{
					attributeFlag++;
					if (attributeFlag == 1)
						partitionType = pch;
					else if (attributeFlag == 2)
						part_x = strtod(pch, NULL);
					else if (attributeFlag == 3)
						part_y = strtod(pch, NULL);
					pch = strtok(NULL, delim);
				}
				file << partitionType << "," << part_x << "," << part_y << endl;
			}
			else if (lineNumber == 2)
			{
				while (pch != NULL)
				{
					partitionLongtitudeList.push_back(strtod(pch, NULL));
					pch = strtok(NULL, delim);
				}
				//print grid partition line
				for (int i = 0; i < partitionLongtitudeList.size(); i++)
				{
					file << setprecision(9) << partitionLongtitudeList.at(i);
					if (i != partitionLongtitudeList.size() - 1)
					{
						file << ",";
					}
				}
				file << endl;
			}
			else if (lineNumber == 3)
			{
				while (pch != NULL)
				{
					partitionLatitudeList.push_back(strtod(pch, NULL));
					pch = strtok(NULL, delim);
				}
				//print grid partition line
				for (int i = 0; i < partitionLatitudeList.size(); i++)
				{
					file << setprecision(9) << partitionLatitudeList.at(i);
					if (i != partitionLatitudeList.size() - 1)
					{
						file << ",";
					}
				}
				file << endl;
				//get Regions bounds
				for (int i = 1; i < partitionLongtitudeList.size(); i++)
				{
					for (int j = 1; j < partitionLatitudeList.size(); j++)
					{
						Region* gridRegion = new Region(regionId, partitionLongtitudeList.at(i - 1), partitionLongtitudeList.at(i), partitionLatitudeList.at(j - 1), partitionLatitudeList.at(j));
						regionList.push_back(gridRegion);
						regionId++;
					}
				}
			}
		}
		else    //get Border Node
		{
			while (pch != NULL)
			{
				attributeFlag++;
				//記錄ID, 轉折點, 起點
				if (attributeFlag == 1)
				{
					roadId = strtod(pch, NULL);
				}
				else if (attributeFlag == 2)
				{
					turningAmount = strtod(pch, NULL);
				}
				else if (attributeFlag == 3)
				{
					roadType = pch;
					if (roadType == "nn")
						break;
				}
				else if (attributeFlag == 4)
				{
					roadDist = strtod(pch, NULL);
				}

				//將起點&終點交替放入register
				//find Border Node
				if (roadType == "nb")
				{
					if (attributeFlag == 7)
					{
						regionBorderNode = new BorderNode();
						regionBorderNode->setLongtitude(strtod(pch, NULL));
						regionBorderNode->setRoadIdList(roadId);
						//borderNodeList.push_back(regionBorderNode);
					}
					else if (attributeFlag == 8)
					{
						//regionBorderNode = borderNodeList.back();
						regionBorderNode->setLatitude(strtod(pch, NULL));
						int duplicateNodeInd = checkBorderNodeExistAndReturnNodeInd(borderNodeList, regionBorderNode);
						if (duplicateNodeInd != -1)
						{
							if (checkNodeRoadIdExist(borderNodeList.at(duplicateNodeInd), regionBorderNode->getRoadIdList().back()) == false)
								borderNodeList.at(duplicateNodeInd)->setRoadIdList(regionBorderNode->getRoadIdList().back());
						}
						else
							borderNodeList.push_back(regionBorderNode);
					}
				}
				pch = strtok(NULL, delim);
			}
		}
	}
	//get Border Node Regions
	for (int i = 0; i < borderNodeList.size(); i++)
	{
		for (int j = 0; j < regionList.size(); j++)
		{
			if (borderNodeList.at(i)->getLongtitude() >= regionList.at(j)->getLongtitudeLeft() && borderNodeList.at(i)->getLongtitude() <= regionList.at(j)->getLongtitudeRight() && borderNodeList.at(i)->getLatitude() >= regionList.at(j)->getLatitudeBottom() && borderNodeList.at(i)->getLatitude() <= regionList.at(j)->getLatitudeTop())
			{
				regionList.at(j)->setBorderNodeList(borderNodeList.at(i));
			}
		}
	}
	
	vector<BorderNode*> regionBList = uniteBorderListWithAllRegion(regionList);

	for (int i = 0; i < regionBList.size(); i++)
	{
		file << regionBList.at(i)->getLongtitude() << "," << regionBList.at(i)->getLatitude() << ",";
		for (int j = 0; j < regionBList.at(i)->getRegionIdList().size(); j++)
		{
			file << regionBList.at(i)->getRegionIdList().at(j);
			if (j != regionBList.at(i)->getRegionIdList().size() - 1)
				file << ",";
			else
				file << ";";
		}
		for (int j = 0; j < regionBList.at(i)->getRoadIdList().size(); j++)
		{
			file << regionBList.at(i)->getRoadIdList().at(j);
			if (j != regionBList.at(i)->getRoadIdList().size() - 1)
				file << ",";
		}
		file << endl;
	}

	//_CrtDumpMemoryLeaks();
}

void skylineModel::writeBBDist(string inputDir, string outputDir)
{
	char line[SIZE];

	fstream fin1, fin2;
	ofstream file;
	//	file.close();
	outputDir = outputDir + ".bb";
	file.open(outputDir, ios::out);
	fin1.open(inputDir, ios::in);
	fin2.open(inputDir, ios::in);
	//cout << "inputDir:" << inputDir << endl;
	//cout << "outputDir:" << outputDir << endl;

	int lineNumber = 0;
	char *pch;
	char *delim = ",";
	int nodeId = 0, borderId = 0;
	vector<string> nodeList;
	vector<EData*> edgesList;
	vector<double> partitionLongtitudeList;
	vector<double> partitionLatitudeList;
	int totalNode = 0, totalBorderNode = 0;
	vector<int> pos;
	//cout << "read file " << inputDir << endl;

	while (fin1.getline(line, sizeof(line), '\n'))
	{
		string line2 = line;
		lineNumber++;

		if (line2.find("b", 0) == 0)
			pos.push_back(lineNumber);
	}

	while (fin2.getline(line, sizeof(line), '\n'))
	{
		string line2 = line;
		pch = strtok(line, delim);
		int attributeFlag = 0;
		lineNumber++;

		if (line2.find("b", 0) == 0)
		{
			file << pch << ",";
			while (pch != NULL)
			{
				attributeFlag++;
				for (int index = 0; index < pos.size(); index++)
				{
					if (attributeFlag - 1 == pos.at(index))
					{
						file << pch;
						if (index != pos.size() - 1)
							file << ",";
					}
				}
				pch = strtok(NULL, delim);
			}
			file << endl;
		}
	}
}

void skylineModel::writeKNNAnswer(string inputDir, string outputDir)
{

}

void skylineModel::writeBNAndNNDist(string inputDir1, string inputDir2, string inputDir3, string outputDir, int processCounter)
{
	fstream fin1, fin2, fin3, file2;
	fin1.open(inputDir1, ios::in);
	fin3.open(inputDir1, ios::in);
	fin2.open(inputDir2, ios::in);

	int regionNum;
	int lineNumber = 0;
	char *pch;
	char *delim = ",";
	int nodeId = 0, borderId = 0;

	vector<string> detectRegion;
	vector<Node*>nodeList;
	char line[SIZE];

	int processStep;
	file2.open(inputDir3, ios::in);
	while (file2.getline(line, sizeof(line), '\n'))
	{
		pch = strtok(line, delim);
		processStep = atoi(pch);
		break;
	}
	file2.close();

	while (fin2.getline(line, sizeof(line), '\n'))
	{
		lineNumber++;
		pch = strtok(line, delim);
		int attributeFlag = 0;

		if (lineNumber == 1)
		{
			while (pch != NULL)
			{
				attributeFlag++;
				if (attributeFlag == 2)
					regionNum = atoi(pch);
				if (attributeFlag == 3)
					regionNum = regionNum * atoi(pch);

				pch = strtok(NULL, delim);
			}
			break;
		}
	}
	fin2.close();
	lineNumber = 0;
	fin2.open(inputDir2, ios::in);

	//cout << "read file " << inputDir << endl;
	for (int i = 0; i < regionNum; i++)
	{
		char s[64];
		_itoa(i, s, 10);
		string str = ",";
		str.append(s);
		str.append(",");
		detectRegion.push_back(str);
		string str1;
	}

	while (fin2.getline(line, sizeof(line), '\n'))
	{
		string line2 = line;
		string str = "";
		lineNumber++;
		for (int i = 0; i < detectRegion.size(); i++)
		{
			if (line2.find(detectRegion.at(i), 0) < 40)
			{
				pch = strtok(line, delim);
				str.append(pch);
				int regionId = 0;
				Node* node = new Node();
				node->setNodeName(str);
				node->setNodeRegionId(i);
				nodeList.push_back(node);
				//				pos.push_back(lineNumber);
			}
		}
	}
	for (int i = processCounter; i < detectRegion.size(); i++)
	{
		vector<int> pos;
		int lineNumber = 0;
		char s[64];
		_itoa(i, s, 10);
		string str = "";
		str.append(s);
		fstream finput1, finput2, file;
		finput1.open(inputDir1, ios::in);
		finput2.open(inputDir1, ios::in);
		file.open(outputDir + ".bnnn" + s, ios::out);
		while (finput1.getline(line, sizeof(line), '\n'))
		{
			string line2 = line;

			lineNumber++;

			for (int index = 0; index < nodeList.size(); index++)
			{
				if (i == nodeList.at(index)->getNodeRegionId() && line2.find(nodeList.at(index)->getNodeName() + ",", 0) < 10)
				{
					if (line2.find("b", 0) < 10)
					{
						break;
					}
					//cout <<"node:"<< nodeList.at(index)->getNodeName()<<endl;
					pos.push_back(lineNumber);
					break;
				}
			}
		}
		cout << "region:" << i << " pos size:" << pos.size() << endl;
		if (pos.size() == 0)
		{
			file2.open(inputDir3, ios::out);
			file2 << processStep << endl;
			if (i != detectRegion.size() - 1)
				file2 << "bnnn," << i + 1 << endl;
			file2.close();
			continue;
		}

		while (finput2.getline(line, sizeof(line), '\n'))
		{
			string line2 = line;
			pch = strtok(line, delim);
			int attributeFlag = 0;
			lineNumber++;
			for (int j = 0; j < nodeList.size(); j++)
			{
				if (line2.find(nodeList.at(j)->getNodeName() + ",", 0) < 10 && nodeList.at(j)->getNodeRegionId() == i)
				{
					file << pch << ",";
					while (pch != NULL)
					{
						attributeFlag++;
						for (int index = 0; index < pos.size(); index++)
						{
							if (attributeFlag - 1 == pos.at(index))
							{
								file << pch;
								if (index != pos.size() - 1)
									file << ",";
							}
						}
						pch = strtok(NULL, delim);
					}
					file << endl;
				}
			}
		}
		file2.open(inputDir3, ios::out);
		file2 << processStep << endl;
		if (i != detectRegion.size() - 1)
			file2 << "bnnn," << i + 1 << endl;
		file2.close();
	}
}

void skylineModel::writeRegionMaxLamdaDist(string inputDir, string inputDir2, string inputDir3, string outputDir)
{
	int lineNumber = 0;
	int regionNum;
	int regDist;
	vector<int> nodeLineNumberList;
	char line[SIZE];
	char *pch;
	string str1;
	char *delim = ",";

	fstream fin;
	ofstream foutDir;


	fin.open(inputDir2, ios::in);
	while (fin.getline(line, sizeof(line), '\n'))
	{
		lineNumber++;
		pch = strtok(line, delim);
		int attributeFlag = 0;

		if (lineNumber == 1)
		{
			while (pch != NULL)
			{
				attributeFlag++;
				if (attributeFlag == 2)
					regionNum = atoi(pch);
				if (attributeFlag == 3)
					regionNum = regionNum * atoi(pch);

				pch = strtok(NULL, delim);
			}
			break;
		}
	}
	fin.close();
	lineNumber = 0;

	for (int i = 0; i < regionNum; i++)
	{
		vector<Node*> nodeList;
		int maxDist = 0;
		fin.open(inputDir + to_string(i), ios::in);
		foutDir.open(outputDir + ".maxld" + to_string(i), ios::out);

		//get maxDist & get node, border node name from .bnns
		while (fin.getline(line, sizeof(line), '\n')){	//以行讀取
			lineNumber++;
			int attributeFlag = 0;

			pch = strtok(line, delim);
			if (lineNumber > 1)
			{
				while (pch != NULL)
				{
					attributeFlag++;
					if (attributeFlag != 1)
					{
						regDist = strtod(pch, NULL);
						if (maxDist < regDist)
							maxDist = regDist;
					}
					//else
					//{
					//	str1 = "";
					//	str1.append(pch);
					//	Node *newNode;
					//	if (str1.find("n_", 0) != string::npos)
					//		newNode = new Node();
					//	else
					//		newNode = new BorderNode();

					//	newNode->setNodeName(pch);
					//	nodeList.push_back(newNode);
					//}
					pch = strtok(NULL, delim);
				}
			}
			
		}
		foutDir << maxDist << endl;
		fin.close();
		foutDir.close();
		lineNumber = 0;

		//get all node from .bnnn
		fin.open(inputDir3 + to_string(i), ios::in);
		while (fin.getline(line, sizeof(line), '\n'))
		{	//以行讀取
			Node* newNode;
			lineNumber++;
			int attributeFlag = 0;
			pch = strtok(line, delim);
			while (pch != NULL)
			{
				attributeFlag++;
				if (attributeFlag == 1)
				{
					str1 = "";
					str1.append(pch);
					if (str1.find("n_", 0) != string::npos)
						newNode = new Node();
					else
						newNode = new BorderNode();
					newNode->setNodeName(pch);
					newNode->setNodeRegionId(i);
					nodeList.push_back(newNode);
				}
				pch = strtok(NULL, delim);
			}
		}
		fin.close();

		//get node, border node coordinate from .objreg
		lineNumber = 0;
		int nodeListIndex = 0;
		fin.open(inputDir2, ios::in);
		while (fin.getline(line, sizeof(line), '\n')){	//以行讀取
			lineNumber++;
			str1 = "";
			str1.append(line);
			if (lineNumber > 3 && nodeListIndex < nodeList.size() && str1.find(nodeList.at(nodeListIndex)->getNodeName(), 0) != string::npos)
			{
				int attributeFlag = 0;
				pch = strtok(line, delim);
				while (pch != NULL)
				{
					attributeFlag++;
					if (attributeFlag == 2)
					{
						nodeList.at(nodeListIndex)->setLongtitude(strtod(pch, NULL));
					}
					else if (attributeFlag == 3)
					{
						nodeList.at(nodeListIndex)->setLatitude(strtod(pch, NULL));
					}
					pch = strtok(NULL, delim);
				}
				nodeListIndex++;
				if (nodeListIndex >= nodeList.size())
					break;
			}
		}
		fin.close();
		lineNumber = 0;
		nodeListIndex = 0;

		//open .bnnn and compute lambda
		double minLambda = 0;
		double maxLambda = 0;
		string maxNodeName, nowNodeName;
		int maxNodeAttributeDist;
		double maxLong, maxLat, nowLong, nowLat, eDistance;
		fin.open(inputDir3 + to_string(i), ios::in);
		while (fin.getline(line, sizeof(line), '\n')){	//以行讀取
			nodeListIndex = 0;
			lineNumber++;
			str1 = "";
			str1.append(line);
			
			int attributeFlag = 0;
			pch = strtok(line, delim);
			if (nodeList.at(lineNumber - 1)->getNodeType() == "BorderNode")
			{
				while (pch != NULL)
				{
					attributeFlag++;
					//compute Lambda
					if (attributeFlag != 1)
					{
						while (nodeList.at(nodeListIndex)->getNodeType() != "Node")
							nodeListIndex++;

						double eDist = abs(computeSectionDistance(nodeList.at(lineNumber - 1)->getLatitude(), nodeList.at(lineNumber - 1)->getLongtitude(), nodeList.at(nodeListIndex)->getLatitude(), nodeList.at(nodeListIndex)->getLongtitude()));
						double realDist = atoi(pch);

						if (eDist != 0 || realDist != 0)
						{
							double regLambda = realDist / eDist;
							if (regLambda > maxLambda)
							{
								maxLambda = regLambda;
								maxNodeName = nodeList.at(lineNumber - 1)->getNodeName();
								maxLong = nodeList.at(lineNumber - 1)->getLongtitude();
								maxLat = nodeList.at(lineNumber - 1)->getLatitude();
								maxNodeAttributeDist = realDist;
								eDistance = eDist;
								nowNodeName = nodeList.at(nodeListIndex)->getNodeName();
								nowLong = nodeList.at(nodeListIndex)->getLongtitude();
								nowLat = nodeList.at(nodeListIndex)->getLatitude();
							}
							if (regLambda < minLambda || minLambda == 0)
								minLambda = regLambda;
							if (minLambda < 1)
								minLambda = 1;
						}
						nodeListIndex++;

						//if (nodeListIndex >= nodeList.size())
						//	break;
					}
					pch = strtok(NULL, delim);
				}
			}
		}
		cout << setprecision(9) << "R" << i << " : " << endl;
		cout << setprecision(9) << maxNodeName << "," << maxLong << "," << maxLat << endl;
		cout << setprecision(9) << "maxDist : " << maxNodeAttributeDist << endl;
		cout << setprecision(9) << "eDist : " << eDistance << endl;
		cout << setprecision(9) << "maxLambda : " << maxLambda << endl;
		cout << setprecision(9) << nowNodeName << "," << nowLong << "," << nowLat << endl;
		cout << "-----------------------------------" << endl;
		fin.close();
		foutDir.open(outputDir + ".maxld" + to_string(i), ios::out | ios::app);
		foutDir << setprecision(9) << minLambda << "," << maxLambda << endl;
		foutDir.close();
	}
}

void skylineModel::writeBNNTable(string inputDir, string inputDir2, string inputDir3, string outputDir)
{
	outputDir = outputDir + ".bnntable";

	int lineNumber = 0;
	char line[SIZE];
	//char newLine[SIZE];
	char *pch;
	char *delim = ",";
	int part_x, part_y;
	vector<int> sequencedAllNodeRegionId;
	vector<Node*> nodeList;
	vector<Node*> allNodeIndList;
	vector<Node*> borderNodeList;
	vector<Node*> finishedBorderNodeList;
	vector<string> sequencedAllNodeName;
	fstream fin;
	ofstream file;      //宣告fstream物件
	fin.open(inputDir2, ios::in);

	//以行讀取 .objreg
	while (fin.getline(line, sizeof(line), '\n'))
	{
		pch = strtok(line, delim);
		int attributeFlag = 0;
		string partitionType;
		double roadDist = 0;
		lineNumber++; 
		Node* regNode;

		//read Grid partition information
		if (lineNumber < 4)
		{
			if (lineNumber == 1)
			{
				while (pch != NULL)
				{
					attributeFlag++;
					if (attributeFlag == 1)
						partitionType = pch;
					else if (attributeFlag == 2)
						part_x = strtod(pch, NULL);
					else if (attributeFlag == 3)
						part_y = strtod(pch, NULL);
					pch = strtok(NULL, delim);
				}
			}
		}
		else
		{
			//取 border Node List & Node List
			while (pch != NULL)
			{
				attributeFlag++;
				//記錄ID, 轉折點, 起點
				if (attributeFlag == 1)
				{
					pch = line;
					string str1 = "";
					str1.append(pch);

					if (str1.find("b_", 0) != string::npos)
					{
						regNode = new BorderNode();
						borderNodeList.push_back(regNode);
					}
					else
					{
						regNode = new Node();
						nodeList.push_back(regNode);
					}
					regNode->setNodeName(pch);
					allNodeIndList.push_back(regNode);

					//get node position of all region
					//regNode->setNodeId(lineNumber - 4);
				}
				else if (attributeFlag == 2)
				{
					regNode->setLongtitude(strtod(pch, NULL));
				}
				else if (attributeFlag == 3)
				{
					regNode->setLatitude(strtod(pch, NULL));
				}
				else if (attributeFlag == 4)
				{
					regNode->setNodeRegionId(strtod(pch, NULL));
				}

				pch = strtok(NULL, delim);
			}
		}
	}
	fin.close(); 

	//read .objreg
	lineNumber = 0;
	fin.open(inputDir3, ios::in);
	while (fin.getline(line, sizeof(line), '\n'))
	{
		pch = strtok(line, delim);
		int attributeFlag = 0;
		double roadDist = 0;
		lineNumber++;
		Node* regNode;

		//取 border Node List & Node List
		while (pch != NULL)
		{
			attributeFlag++;
			//記錄ID, 轉折點, 起點
			if (attributeFlag == 4)
				sequencedAllNodeRegionId.push_back(atoi(pch));
			pch = strtok(NULL, delim);
		}
	}
	fin.close();



	//get all node name by .bntable
	lineNumber = 0;
	fin.open(inputDir, ios::in);
	while (fin.getline(line, sizeof(line), '\n'))
	{
		pch = strtok(line, delim);
		int attributeFlag = 0;
		while (pch != NULL)
		{
			attributeFlag++;
			if (attributeFlag == 1)
				sequencedAllNodeName.push_back(pch);
			pch = strtok(NULL, delim);
		}
	}
	fin.close();

	//open .bntable, create .bnntable
	for (int i = 0; i < part_x * part_y; i++)
	{
		lineNumber = 0;
		int borderListInd = 0;
		fin.open(inputDir, ios::in);
		while (fin.getline(line, sizeof(line), '\n'))
		{
			pch = strtok(line, delim);
			int attributeFlag = 0;
			int allNodeListInd = 0;
			double roadDist = 0;
			lineNumber++;
			Node* regNode;

			pch = line;
			string str1 = "";
			str1.append(pch);

			//If border node is in the next region
			//get non repeat BNNtable of this Region and go to next region loop
			if (borderNodeList.size() == 0 || borderNodeList.front()->getNodeRegionId() != i)
			{
				vector<Destination*> sortFirstDestinationList;
				//-------------------------write file----------------------------
				file.open(outputDir + to_string(i), ios::out);		//開啟檔案為輸出狀態，若檔案已存在則清除檔案內容重新寫入
				for (int j = 0; j < finishedBorderNodeList.size(); j++)
				{
					//get all first, DestinationList overflow
					//finishedBorderNodeList.at(j)->getDestinationList().front()->setStartPointNode(finishedBorderNodeList.at(j));

					//add node only if destination region != now region (i)
					Node* tmpNode = finishedBorderNodeList.at(j);
					while (tmpNode->getDestinationList().at(tmpNode->getCounter())->getRegionId() == i)
						tmpNode->addCounter();

					int listCounter = tmpNode->getCounter();
					sortFirstDestinationList.push_back(finishedBorderNodeList.at(j)->getDestinationList().at(listCounter));

					//file << finishedBorderNodeList.at(j)->getNodeName() << ";";
				}
				//sort all first
				sort(sortFirstDestinationList.begin(), sortFirstDestinationList.end(), Destination());
				//check repeat node
				for (int j = 0; j < sortFirstDestinationList.size(); j++)
				{
					for (int k = 0; k < sortFirstDestinationList.size(); k++)
					{
						if (j != k && sortFirstDestinationList.at(j)->getDestinationId() == sortFirstDestinationList.at(k)->getDestinationId())
						{
							//get next
							//if j<k, get k's startPoint's next shortest destination node
							if (j < k)
							{
								Node* tmpNode = sortFirstDestinationList.at(k)->getStartPointNode();
								tmpNode->addCounter();
								int listCounter = sortFirstDestinationList.at(k)->getStartPointNode()->getCounter();
								while (tmpNode->getDestinationList().at(listCounter)->getRegionId() == i)
								{
									tmpNode->addCounter();
									listCounter = sortFirstDestinationList.at(k)->getStartPointNode()->getCounter();
								}
								sortFirstDestinationList.at(k) = sortFirstDestinationList.at(k)->getStartPointNode()->getDestinationList().at(listCounter);
							}
							//else, get j's startPoint's next shortest destination node
							else
							{
								Node* tmpNode = sortFirstDestinationList.at(j)->getStartPointNode();
								tmpNode->addCounter();
								int listCounter = sortFirstDestinationList.at(j)->getStartPointNode()->getCounter();
								while (tmpNode->getDestinationList().at(listCounter)->getRegionId() == i)
								{
									tmpNode->addCounter();
									listCounter = sortFirstDestinationList.at(j)->getStartPointNode()->getCounter();
								}
								sortFirstDestinationList.at(j) = sortFirstDestinationList.at(j)->getStartPointNode()->getDestinationList().at(listCounter);
							}

							//sort
							sort(sortFirstDestinationList.begin(), sortFirstDestinationList.end(), Destination());
							k = -1;
							j = 0;
						}
					}
				}

				//find node region in sortFirstDestinationList
				//for (int j = 0; j < nodeList.size(); j++)
				//{
				//	for (int k = 0; k < sortFirstDestinationList.size(); k++)
				//	{
				//		if (nodeList.at(j)->getNodeName() == sortFirstDestinationList.at(k)->getDestinationNodeName())
				//		{
				//			sortFirstDestinationList.at(k)->setRegionId(nodeList.at(j)->getNodeRegionId());
				//			nodeList.erase(nodeList.begin() + j);
				//			j--;
				//			break;
				//		}
				//	}
				//}

				//file out BNN table
				for (int j = 0; j < sortFirstDestinationList.size(); j++)
				{
					Destination* regDest = sortFirstDestinationList.at(j);
					file << regDest->getStartPointNode()->getNodeName() << ",";
					file << regDest->getDestinationNodeName() << ",";
					file << regDest->getPathDistance() << ",";
					file << regDest->getRegionId() << ",";
					file << setprecision(9) << regDest->getStartPointNode()->getLongtitude() << ",";
					file << setprecision(9) << regDest->getStartPointNode()->getLatitude() << endl;
				}

				//initialize finishedBorderNodeList
				finishedBorderNodeList.resize(0);

				file.close();
				break;
			}

			//If line is first border node in list
			if (borderNodeList.size() > 0)
			{
				if (str1.find(borderNodeList.front()->getNodeName(), 0) != string::npos)
				{
					//borderListInd++;

					//set node destination list
					while (pch != NULL)
					{
						attributeFlag++;

						if (attributeFlag != 1)
						{
							Destination* regDestination = new Destination();
							regDestination->setStartPointNode(borderNodeList.front());
							//regDestination->setStartPointId(borderNodeList.front()->getNodeId());
							regDestination->setDestinationId(attributeFlag - 2);
							//find node name
							regDestination->setDestinationNodeName(sequencedAllNodeName.at(attributeFlag - 2));
							regDestination->setPathDistance(strtod(pch, NULL));
							regDestination->setRegionId(sequencedAllNodeRegionId.at(attributeFlag - 2));

							//set destination list and sort by distance
							//if destination is node, push to destinationList
							string str2 = regDestination->getDestinationNodeName();
							if (str2.find("n_", 0) != string::npos)
								borderNodeList.front()->setDestinationList(regDestination);
						}

						pch = strtok(NULL, delim);
					}

					finishedBorderNodeList.push_back(borderNodeList.front());
					borderNodeList.erase(borderNodeList.begin());
				}
			}
			
		}
		fin.close();
	}
}

void skylineModel::writeNewBNNTable(string inputDir, string inputDir2, string inputDir3, string outputDir)
{
	outputDir = outputDir + ".nbnntable";

	int lineNumber = 0;
	char line[SIZE];
	//char newLine[SIZE];
	char *pch;
	char *delim = ",";
	int part_x, part_y;
	vector<int> sequencedAllNodeRegionId;
	vector<Node*> nodeList;
	vector<Node*> allNodeIndList;
	vector<Node*> borderNodeList;
	vector<Node*> finishedBorderNodeList;
	vector<string> sequencedAllNodeName;
	fstream fin;
	ofstream file;      //宣告fstream物件
	fin.open(inputDir2, ios::in);

	//以行讀取 .objreg 取 border Node List & Node List
	while (fin.getline(line, sizeof(line), '\n'))
	{
		pch = strtok(line, delim);
		int attributeFlag = 0;
		string partitionType;
		double roadDist = 0;
		lineNumber++;
		Node* regNode;

		//read Grid partition information
		if (lineNumber < 4)
		{
			if (lineNumber == 1)
			{
				while (pch != NULL)
				{
					attributeFlag++;
					if (attributeFlag == 1)
						partitionType = pch;
					else if (attributeFlag == 2)
						part_x = strtod(pch, NULL);
					else if (attributeFlag == 3)
						part_y = strtod(pch, NULL);
					pch = strtok(NULL, delim);
				}
			}
		}
		else
		{
			//取 border Node List & Node List
			while (pch != NULL)
			{
				attributeFlag++;
				//記錄ID, 轉折點, 起點
				if (attributeFlag == 1)
				{
					pch = line;
					string str1 = "";
					str1.append(pch);

					if (str1.find("b_", 0) != string::npos)
					{
						regNode = new BorderNode();
						borderNodeList.push_back(regNode);
					}
					else
					{
						regNode = new Node();
						nodeList.push_back(regNode);
					}
					regNode->setNodeName(pch);
					allNodeIndList.push_back(regNode);

					//get node position of all region
					//regNode->setNodeId(lineNumber - 4);
				}
				else if (attributeFlag == 2)
				{
					regNode->setLongtitude(strtod(pch, NULL));
				}
				else if (attributeFlag == 3)
				{
					regNode->setLatitude(strtod(pch, NULL));
				}
				else if (attributeFlag == 4)
				{
					regNode->setNodeRegionId(strtod(pch, NULL));
				}

				pch = strtok(NULL, delim);
			}
		}
	}
	fin.close();

	//read .oobjreg
	int allNodeAmount;
	lineNumber = 0;
	fin.open(inputDir3, ios::in);
	while (fin.getline(line, sizeof(line), '\n'))
	{
		pch = strtok(line, delim);
		int attributeFlag = 0;
		double roadDist = 0;
		lineNumber++;
		Node* regNode;

		//取 border Node List & Node List
		while (pch != NULL)
		{
			attributeFlag++;
			//記錄ID, 轉折點, 起點
			if (attributeFlag == 4)
				sequencedAllNodeRegionId.push_back(atoi(pch));
			pch = strtok(NULL, delim);
		}
	}
	fin.close();
	allNodeAmount = lineNumber;


	//get all node name by .bntable
	lineNumber = 0;
	fin.open(inputDir, ios::in);
	while (fin.getline(line, sizeof(line), '\n'))
	{
		pch = strtok(line, delim);
		int attributeFlag = 0;
		while (pch != NULL)
		{
			attributeFlag++;
			if (attributeFlag == 1)
				sequencedAllNodeName.push_back(pch);
			break;
			pch = strtok(NULL, delim);
		}
	}
	fin.close();

	//open .bntable, create .bnntable
	for (int i = 0; i < part_x * part_y; i++)
	{
		lineNumber = 0;
		int borderListInd = 0;
		fin.open(inputDir, ios::in);
		while (fin.getline(line, sizeof(line), '\n'))
		{
			pch = strtok(line, delim);
			int attributeFlag = 0;
			int allNodeListInd = 0;
			double roadDist = 0;
			lineNumber++;
			Node* regNode;

			pch = line;
			string str1 = "";
			str1.append(pch);

			//If border node is in the next region
			//get non repeat BNNtable of this Region and go to next region loop
			if (borderNodeList.size() == 0 || borderNodeList.front()->getNodeRegionId() != i)
			{
				vector<Destination*> sortFirstDestinationList;
				//-------------------------write file----------------------------
				file.open(outputDir + to_string(i), ios::out);		//開啟檔案為輸出狀態，若檔案已存在則清除檔案內容重新寫入
				for (int j = 0; j < finishedBorderNodeList.size(); j++)
				{
					//get all first, DestinationList overflow
					//finishedBorderNodeList.at(j)->getDestinationList().front()->setStartPointNode(finishedBorderNodeList.at(j));

					//get counter
					//add node counter if destination region != now region (i)
					Node* tmpNode = finishedBorderNodeList.at(j);
					while (tmpNode->getCounter() < tmpNode->getDestinationList().size() - 1 && tmpNode->getDestinationList().at(tmpNode->getCounter())->getRegionId() != i)
						tmpNode->addCounter();
					
					if (tmpNode->getDestinationList().at(tmpNode->getCounter())->getRegionId() == i)
					{
						int listCounter = tmpNode->getCounter();
						sortFirstDestinationList.push_back(finishedBorderNodeList.at(j)->getDestinationList().at(listCounter));
					}
					tmpNode->setCounter(0);
					//file << finishedBorderNodeList.at(j)->getNodeName() << ";";
				}
				//sort all first
				sort(sortFirstDestinationList.begin(), sortFirstDestinationList.end(), Destination());
				//check repeat node


				//file out BNN table
				for (int j = 0; j < sortFirstDestinationList.size(); j++)
				{
					Destination* regDest = sortFirstDestinationList.at(j);
					file << regDest->getStartPointNode()->getNodeName() << ",";
					file << regDest->getDestinationNodeName() << ",";
					file << regDest->getPathDistance() << endl;
					//file << regDest->getRegionId() << ",";
					//file << setprecision(9) << regDest->getStartPoinhtNode()->getLongtitude() << ",";
					//file << setprecision(9) << regDest->getStartPoinhtNode()->getLatitude() << endl;
				}

				//initialize finishedBorderNodeList
				finishedBorderNodeList.resize(0);

				file.close();
				break;
			}

			//If line is first border node in list
			if (borderNodeList.size() > 0)
			{
				if (str1.find(borderNodeList.front()->getNodeName(), 0) != string::npos)
				{
					//borderListInd++;

					//set node destination list
					while (pch != NULL)
					{
						attributeFlag++;

						if (attributeFlag != 1)
						{
							Destination* regDestination = new Destination();
							regDestination->setStartPointNode(borderNodeList.front());
							//regDestination->setStartPointId(borderNodeList.front()->getNodeId());
							regDestination->setDestinationId(attributeFlag - 2);
							//find node name
							regDestination->setDestinationNodeName(sequencedAllNodeName.at(attributeFlag - 2));
							regDestination->setPathDistance(strtod(pch, NULL));
							regDestination->setRegionId(sequencedAllNodeRegionId.at(attributeFlag - 2));

							//set destination list and sort by distance
							//if destination is node, push to destinationList
							string str2 = regDestination->getDestinationNodeName();
							if (str2.find("n_", 0) != string::npos)
								borderNodeList.front()->setDestinationList(regDestination);
						}

						pch = strtok(NULL, delim);
					}

					finishedBorderNodeList.push_back(borderNodeList.front());
					borderNodeList.erase(borderNodeList.begin());

					//fix last line number bug
					if (lineNumber == allNodeAmount && finishedBorderNodeList.size() != 0 && borderNodeList.size() != 0)
						i--;
				}
			}

		}
		fin.close();
	}
}

void skylineModel::writeNewSpotBNNTable(string inputDir, string inputDir2, string outputDir)
{
	outputDir = outputDir + ".nbnntable";

	int lineNumber = 0;
	char line[SIZE];
	//char newLine[SIZE];
	char *pch;
	char *delim = ",";
	int part_x, part_y;
	vector<int> sequencedAllNodeRegionId;
	vector<Node*> allNodeIndList;
	vector<Node*> borderNodeList;
	vector<Node*> finishedBorderNodeList;
	vector<string> sequencedAllNodeName;
	fstream fin;
	ofstream file;      //宣告fstream物件
	
	fin.open(inputDir2, ios::in);
	while (fin.getline(line, sizeof(line), '\n'))
	{
		pch = strtok(line, delim);
		int attributeFlag = 0;
		string partitionType;
		double roadDist = 0;
		lineNumber++;
		Node* regNode;

		//read Grid partition information from objreg
		if (lineNumber < 4)
		{
			if (lineNumber == 1)
			{
				while (pch != NULL)
				{
					attributeFlag++;
					if (attributeFlag == 1)
						partitionType = pch;
					else if (attributeFlag == 2)
						part_x = strtod(pch, NULL);
					else if (attributeFlag == 3)
						part_y = strtod(pch, NULL);
					pch = strtok(NULL, delim);
				}
			}
		}
	}
	fin.close();

	for (int i = 0; i < part_x * part_y; i++)
	{
		lineNumber = 0;
		fin.open(inputDir + to_string(i), ios::in);
		vector<Destination*> bNNSpotDestinationList;
		vector<Node*> spotList;
		vector<Node*> nodeList;
		vector<int> spotLineNumber;

		//get all spot list from .bnns
		while (fin.getline(line, sizeof(line), '\n'))
		{
			pch = strtok(line, delim);
			int attributeFlag = 0;
			lineNumber++;
			Node* regNode;
			if (lineNumber == 1)
			{
				while (pch != NULL)
				{
					//get all node
					regNode = new Node();
					regNode->setNodeName(pch);
					nodeList.push_back(regNode);
					pch = strtok(NULL, delim);
				}
			}
			else if (lineNumber > 1)
			{
				string str1 = "";
				str1.append(pch);
				while (pch != NULL)
				{
					//get all spotFlag to true
					attributeFlag++;
					if (attributeFlag == 1 && str1.find("n_", 0) != string::npos)
					{
						for (int j = 0; j < nodeList.size(); j++)
						{
							if (nodeList.at(j)->getNodeName() == pch)
							{
								nodeList.at(j)->setIsSpot(true);
								break;
							}
						}
					}
					pch = strtok(NULL, delim);
				}
			}
		}
		fin.close();


		//find all borderNode BNN from .bnns
		lineNumber = 0;
		fin.open(inputDir + to_string(i), ios::in);
		while (fin.getline(line, sizeof(line), '\n'))
		{
			pch = strtok(line, delim);
			int attributeFlag = 0;
			int minDistCount = -1;
			int minDist = -1;
			bool isBorderFlag = false;
			double roadDist = 0;
			lineNumber++;
			Node* regNode;
			BorderNode* regBorderNode;
			if (lineNumber > 1)
			{
				string str1 = "";
				str1.append(pch);
				while (pch != NULL)
				{
					attributeFlag++;
					int spotListInd = 0;
					if (attributeFlag == 1 && str1.find("b_", 0) != string::npos)
					{
						isBorderFlag = true;
						regBorderNode = new BorderNode();
						regBorderNode->setNodeName(pch);
					}
					else if (attributeFlag > 1)
					{
						//compare spot miniDist
						if ((minDist == -1 || minDist > atoi(pch)) && nodeList.at(attributeFlag - 2)->getIsSpot() == true)
						{
							minDist = atoi(pch);
							minDistCount = attributeFlag - 2;
						}
					}
					pch = strtok(NULL, delim);
				}
				if (isBorderFlag == true && minDistCount != -1)
				{
					regNode = nodeList.at(minDistCount);
					Destination* regDestination = new Destination();
					regDestination->setStartPointNode(regBorderNode);
					regDestination->setDestinationNode(regNode);
					regDestination->setDestinationNodeName(regNode->getNodeName());
					regDestination->setPathDistance(minDist);
					bNNSpotDestinationList.push_back(regDestination);
				}
			}
		}
		fin.close();

		//write .nbnntable
		file.open(outputDir + to_string(i), ios::trunc | ios::out);
		for (int j = 0; j < bNNSpotDestinationList.size(); j++)
		{
			file << bNNSpotDestinationList.at(j)->getStartPointNode()->getNodeName() << ",";
			file << bNNSpotDestinationList.at(j)->getDestinationNodeName() << ",";
			file << bNNSpotDestinationList.at(j)->getPathDistance() << endl;
		}
		file.close();
	}
}

void skylineModel::writeAllMaxDistance(string inputDir, string inputDir2, string outputDir)
{
	outputDir = outputDir + ".maxdist";
	int lineNumber = 0;
	int regionNum;
	char line[SIZE];
	char *pch;
	char *delim = ",";

	fstream fin;
	//clear prev .maxdist data
	fin.open(outputDir, ios::trunc | ios::out);
	fin.close();

	//get region number
	fin.open(inputDir2, ios::in);
	while (fin.getline(line, sizeof(line), '\n'))
	{
		lineNumber++;
		pch = strtok(line, delim);
		int attributeFlag = 0;

		if (lineNumber == 1)
		{
			while (pch != NULL)
			{
				attributeFlag++;
				if (attributeFlag == 2)
					regionNum = atoi(pch);
				if (attributeFlag == 3)
					regionNum = regionNum * atoi(pch);

				pch = strtok(NULL, delim);
			}
			break;
		}
	}
	fin.close();

	//get all distance and write file
	for (int i = 0; i < regionNum; i++)
	{
		int regionMaxDist;
		double minLambda, maxLambda;
		lineNumber = 0;
		fin.open(inputDir + to_string(i), ios::in);
		while (fin.getline(line, sizeof(line), '\n'))
		{
			lineNumber++;
			pch = strtok(line, delim);
			int attributeFlag = 0;

			if (lineNumber == 1)
			{
				while (pch != NULL)
				{
					attributeFlag++;
					if (attributeFlag == 1)
						regionMaxDist = atoi(pch);

					pch = strtok(NULL, delim);
				}
			}
			else if (lineNumber == 2)
			{
				while (pch != NULL)
				{
					attributeFlag++;
					if (attributeFlag == 1)
						minLambda = strtod(pch, NULL);
					else if (attributeFlag == 2)
						maxLambda = strtod(pch, NULL);

					pch = strtok(NULL, delim);
				}
			}
		}
		fin.close();

		fin.open(outputDir, ios::out | ios::app);
		fin << regionMaxDist << "," << minLambda << "," << maxLambda << endl;
		fin.close();
	}
}

void skylineModel::writeIndex(string inputDir, string inputDir1, string inputDir2, string inputDir3, string outputDir, int layerNum)
{
	outputDir = outputDir + ".index";

	//get region amount	&& get region bound		(1) .bn
	int lineNumber = 0;
	int regionNum;
	int xPart;
	int yPart;
	char line[SIZE];
	char *pch;
	char *delim = ",";
	vector<double> partXList;
	vector<double> partYList;
	vector<Region*> regionList;
	string roadType;

	fstream fin;
	ofstream file;

	//get region number
	fin.open(inputDir, ios::in);
	while (fin.getline(line, sizeof(line), '\n'))
	{
		lineNumber++;
		pch = strtok(line, delim);
		int attributeFlag = 0;

		if (lineNumber == 1)
		{
			while (pch != NULL)
			{
				attributeFlag++;
				if (attributeFlag == 1)
					roadType = pch;
				if (attributeFlag == 2)
					xPart = atoi(pch);
				if (attributeFlag == 3)
				{
					yPart = atoi(pch);
					regionNum = xPart * yPart;
				}

				pch = strtok(NULL, delim);
			}
		}
		else if (lineNumber == 2)
		{
			while (pch != NULL)
			{
				partXList.push_back(strtod(pch, NULL));
				pch = strtok(NULL, delim);
			}
		}
		else if (lineNumber == 3)
		{
			while (pch != NULL)
			{
				partYList.push_back(strtod(pch, NULL));
				pch = strtok(NULL, delim);
			}
			break;
		}
	}
	fin.close();

	int regionId = 0;
	//get region bound
	for (int i = 0; i < partXList.size() - 1; i++)
	{
		for (int j = 0; j < partYList.size() - 1; j++)
		{
			Region* regRegion = new Region(regionId, partXList.at(i), partXList.at(i + 1), partYList.at(j), partYList.at(j + 1));
			regionId++;
			regionList.push_back(regRegion);
		}
	}

	//write id & bound
	for (int i = 0; i < regionNum; i++)
	{
		//clear prev .index data
		file.open(outputDir + to_string(i), ios::trunc | ios::out);
		file << i << endl;
		file << roadType << "," << xPart << "," << yPart << endl;
		for (int j = 0; j < partXList.size(); j++)
		{
			file << setprecision(9) << partXList.at(j);
			if (j != partXList.size() - 1)
				file << ",";
			else
				file << endl;
		}
		for (int j = 0; j < partYList.size(); j++)
		{
			file << setprecision(9) << partYList.at(j);
			if (j != partYList.size() - 1)
				file << ",";
			else
				file << endl;
		}
		file.close();
	}

	//get region lambda by .maxld			(3) .maxld * n
	for (int i = 0; i < regionNum; i++)
	{
		lineNumber = 0;
		fin.open(inputDir1 + to_string(i), ios::in);
		file.open(outputDir + to_string(i), ios::out | ios::app);
		
		while (fin.getline(line, sizeof(line), '\n'))
		{
			lineNumber++;
			pch = strtok(line, delim);
			int attributeFlag = 0;

			if (lineNumber == 2)
			{
				while (pch != NULL)
				{
					attributeFlag++;
					if (attributeFlag == 1)
						regionList.at(i)->setMinLambda(strtod(pch, NULL));
					else if (attributeFlag == 2)
						regionList.at(i)->setMaxLambda(strtod(pch, NULL));

					pch = strtok(NULL, delim);
				}
			}
		}
		file << regionList.at(i)->getMinLambda() << "," << regionList.at(i)->getMaxLambda() << endl;

		vector<int> bNNTableToListenList;
		bNNTableToListenList = getBNNTableInNLayers(inputDir2, i, layerNum);
		for (int j = 0; j < bNNTableToListenList.size(); j++)
		{
			file << bNNTableToListenList.at(j);
			if (j != bNNTableToListenList.size() - 1)
				file << ",";
			else
				file << endl;
		}

		file.close();
		fin.close();
	}

	vector<int> regionNodeAmountList;
	for (int i = 0; i < regionNum; i++)
	{
		lineNumber = 0;
		fin.open(inputDir3 + to_string(i), ios::in);
		int regionNodeAmount = 0;

		while (fin.getline(line, sizeof(line), '\n'))
		{
			lineNumber++;
			int attributeFlag = 0;
			pch = strtok(line, delim);
			if (lineNumber == 1)
				continue;
			while (pch != NULL)
			{
				attributeFlag++;
				if (attributeFlag == 1)
				{
					string str1 = "";
					str1.append(pch);
					if (str1.find("n_", 0) != string::npos)
						regionNodeAmount++;
					break;
				}
				pch = strtok(NULL, delim);
			}
		}
		regionNodeAmountList.push_back(regionNodeAmount);

		fin.close();
	}
	for (int i = 0; i < regionNum; i++)
	{
		file.open(outputDir + to_string(i), ios::out | ios::app);
		for (int j = 0; j < regionNodeAmountList.size(); j++)
		{
			file << regionNodeAmountList.at(j);
			if (j != regionNodeAmountList.size() - 1)
				file << ",";
		}
		file << endl;
		file.close();
	}
}

void skylineModel::writeNPIIndex(string inputDir, string inputDir1, string outputDir)
{
	outputDir = outputDir + ".npiind";

	//get region amount	&& get region bound		(1) .bn
	int lineNumber = 0;
	int regionNum;
	int xPart;
	int yPart;
	char line[SIZE];
	char *pch;
	char *delim = ",";
	vector<double> partXList;
	vector<double> partYList;
	vector<Region*> regionList;
	string roadType;

	fstream fin;
	ofstream file;

	//get region number
	fin.open(inputDir, ios::in);
	while (fin.getline(line, sizeof(line), '\n'))
	{
		lineNumber++;
		pch = strtok(line, delim);
		int attributeFlag = 0;

		if (lineNumber == 1)
		{
			while (pch != NULL)
			{
				attributeFlag++;
				if (attributeFlag == 1)
					roadType = pch;
				if (attributeFlag == 2)
					xPart = atoi(pch);
				if (attributeFlag == 3)
				{
					yPart = atoi(pch);
					regionNum = xPart * yPart;
				}

				pch = strtok(NULL, delim);
			}
		}
		else if (lineNumber == 2)
		{
			while (pch != NULL)
			{
				partXList.push_back(strtod(pch, NULL));
				pch = strtok(NULL, delim);
			}
		}
		else if (lineNumber == 3)
		{
			while (pch != NULL)
			{
				partYList.push_back(strtod(pch, NULL));
				pch = strtok(NULL, delim);
			}
			break;
		}
	}
	fin.close();

	int regionId = 0;
	//get region bound
	for (int i = 0; i < partXList.size() - 1; i++)
	{
		for (int j = 0; j < partYList.size() - 1; j++)
		{
			Region* regRegion = new Region(regionId, partXList.at(i), partXList.at(i + 1), partYList.at(j), partYList.at(j + 1));
			regionId++;
			regionList.push_back(regRegion);
		}
	}

	//write id & bound
	for (int i = 0; i < regionNum; i++)
	{
		//clear prev .index data
		file.open(outputDir + to_string(i), ios::trunc | ios::out);
		file << i << endl;
		file << roadType << "," << xPart << "," << yPart << endl;
		for (int j = 0; j < partXList.size(); j++)
		{
			file << setprecision(9) << partXList.at(j);
			if (j != partXList.size() - 1)
				file << ",";
			else
				file << endl;
		}
		for (int j = 0; j < partYList.size(); j++)
		{
			file << setprecision(9) << partYList.at(j);
			if (j != partYList.size() - 1)
				file << ",";
			else
				file << endl;
		}
		file.close();
	}

	vector<int> regionNodeAmountList;
	for (int i = 0; i < regionNum; i++)
	{
		lineNumber = 0;
		fin.open(inputDir1 + to_string(i), ios::in);
		int regionNodeAmount = 0;

		while (fin.getline(line, sizeof(line), '\n'))
		{
			int attributeFlag = 0;
			pch = strtok(line, delim);
			while (pch != NULL)
			{
				attributeFlag++;
				if (attributeFlag == 1)
				{
					string str1 = "";
					str1.append(pch);
					if (str1.find("n_", 0) != string::npos)
						regionNodeAmount++;
					break;
				}
				pch = strtok(NULL, delim);
			}
		}
		regionNodeAmountList.push_back(regionNodeAmount);

		fin.close();
	}
	for (int i = 0; i < regionNum; i++)
	{
		file.open(outputDir + to_string(i), ios::out | ios::app);
		for (int j = 0; j < regionNodeAmountList.size(); j++)
		{
			file << regionNodeAmountList.at(j);
			if (j != regionNodeAmountList.size() - 1)
				file << ",";
		}
		file << endl;
		file.close();
	}
}

void skylineModel::writeMinMaxRR(string inputDir, string inputDir1, string outputDir)
{
	//get all region 
	string outputDir1 = outputDir + ".npirrmax";
	outputDir = outputDir + ".npirrmin";

	//get region amount	&& get region bound		(1) .objreg
	int lineNumber = 0;
	int regionNum;
	int xPart;
	int yPart;
	char line[SIZE];
	char *pch;
	char *delim = ",";
	vector<double> partXList;
	vector<double> partYList;
	vector<Region*> regionList;

	fstream fin;
	ofstream file;

	//get region number
	fin.open(inputDir, ios::in);
	while (fin.getline(line, sizeof(line), '\n'))
	{
		lineNumber++;
		pch = strtok(line, delim);
		int attributeFlag = 0;

		if (lineNumber == 1)
		{
			while (pch != NULL)
			{
				attributeFlag++;
				if (attributeFlag == 2)
					xPart = atoi(pch);
				if (attributeFlag == 3)
				{
					yPart = atoi(pch);
					regionNum = xPart * yPart;
				}

				pch = strtok(NULL, delim);
			}
		}
		else if (lineNumber == 2)
		{
			while (pch != NULL)
			{
				partXList.push_back(strtod(pch, NULL));
				pch = strtok(NULL, delim);
			}
		}
		else if (lineNumber == 3)
		{
			while (pch != NULL)
			{
				partYList.push_back(strtod(pch, NULL));
				pch = strtok(NULL, delim);
			}
			break;
		}
	}
	fin.close();

	int regionId = 0;
	//get region bound
	for (int i = 0; i < partXList.size() - 1; i++)
	{
		for (int j = 0; j < partYList.size() - 1; j++)
		{
			Region* regRegion = new Region(regionId, partXList.at(i), partXList.at(i + 1), partYList.at(j), partYList.at(j + 1));
			regionId++;
			regionList.push_back(regRegion);
		}
	}

	//set all region border node list
	lineNumber = 0;
	fin.open(inputDir, ios::in);
	while (fin.getline(line, sizeof(line), '\n'))
	{
		lineNumber++;
		pch = strtok(line, delim);
		int attributeFlag = 0;
		BorderNode* regBorderNode;

		if (lineNumber > 3)
		{
			while (pch != NULL)
			{
				attributeFlag++;
				if (attributeFlag == 1)
				{
					string str1 = "";
					str1.append(pch);
					if (str1.find("b_", 0) != string::npos)
					{
						regBorderNode = new BorderNode();
						regBorderNode->setNodeName(pch);
					}
					else
						break;
				}
				if (attributeFlag == 4)
				{
					regionList.at(atoi(pch))->setBorderNodeList(regBorderNode);
				}

				pch = strtok(NULL, delim);
			}
		}
	}
	fin.close();

	//set borderGraphList by .bb
	//get .bb
	vector<BorderNode*> borderGraphList;
	lineNumber = 0;
	fin.open(inputDir1, ios::in);
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
				break;
			}
			pch = strtok(NULL, delim);
		}
	}
	fin.close();

	lineNumber = 0;
	fin.open(inputDir1, ios::in);
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
			}
			pch = strtok(NULL, delim);
		}
	}
	fin.close();
		


	//set region of border node in borderGraphList
	for (int j = 0; j < borderGraphList.size(); j++)
	{
		BorderNode* regBorderNode = borderGraphList.at(j);
		vector<int> borderRegionIdList = findBorderNodeRegion(regionList, regBorderNode->getNodeName());
		regBorderNode->setWholeRegionIdList(borderRegionIdList);
	}

	vector<int> allRegionMinDist;
	vector<int> allRegionMaxDist;
	for (int j = 0; j < regionNum; j++)
	{
		for (int k = 0; k < regionNum; k++)
		{
			allRegionMinDist.push_back(-1);
			allRegionMaxDist.push_back(-1);
		}
	}

	//minRRDist
	//get Rj -> Rk minDist
	for (int j = 0; j < borderGraphList.size(); j++)
	{
		BorderNode* regBorderNode = borderGraphList.at(j);
		vector<Destination*> regBorderDestList = regBorderNode->getDestinationList();
		for (int k = 0; k < regBorderDestList.size(); k++)
		{
			vector<int> borderRegionIdList = findBorderNodeRegion(regionList, regBorderDestList.at(k)->getDestinationNode()->getNodeName());
			for (int l = 0; l < regBorderNode->getRegionIdList().size(); l++)
			{
				for (int m = 0; m < borderRegionIdList.size(); m++)
				{
					int regMinDist = allRegionMinDist.at(regBorderNode->getRegionIdList().at(l) * regionNum + borderRegionIdList.at(m));
					if (regMinDist < 0 || regMinDist > regBorderDestList.at(k)->getPathDistance())
						allRegionMinDist.at(regBorderNode->getRegionIdList().at(l) * regionNum + borderRegionIdList.at(m)) = regBorderDestList.at(k)->getPathDistance();

					int regMaxDist = allRegionMaxDist.at(regBorderNode->getRegionIdList().at(l) * regionNum + borderRegionIdList.at(m));
					if (regMaxDist < regBorderDestList.at(k)->getPathDistance())
						allRegionMaxDist.at(regBorderNode->getRegionIdList().at(l) * regionNum + borderRegionIdList.at(m)) = regBorderDestList.at(k)->getPathDistance();
				}
			}
		}
	}
	//compare R(0,1) vs R(1,0)
	for (int j = 0; j < allRegionMinDist.size(); j++)
	{
		for (int k = 0; k < allRegionMinDist.size(); k++)
		{
			if (remainder(j, regionNum) == k / regionNum && j / regionNum == remainder(k, regionNum))
			{
				allRegionMinDist.at(j) = min(allRegionMinDist.at(j), allRegionMinDist.at(k));
				allRegionMinDist.at(k) = allRegionMinDist.at(j);
				allRegionMaxDist.at(j) = max(allRegionMaxDist.at(j), allRegionMaxDist.at(k));
				allRegionMaxDist.at(k) = allRegionMaxDist.at(j);
				break;
			}
		}
	}
	//set minimum to allregionList
	for (int j = 0; j < allRegionMinDist.size(); j++)
	{
		Region* regRegion = regionList.at(j / regionNum);
		regRegion->setMinRRDist(allRegionMinDist.at(j));
		Region* regMaxRegion = regionList.at(j / regionNum);
		regMaxRegion->setMaxRRDist(allRegionMaxDist.at(j));
	}

	file.open(outputDir, ios::out);
	//cout region to region minimum
	for (int j = 0; j < regionList.size(); j++)
	{
		for (int k = 0; k < regionList.at(j)->getMinRRDistList().size(); k++)
		{
			file << regionList.at(j)->getMinRRDistList().at(k);
			if (k != regionList.size() - 1)
				file  << ",";
		}
		file << endl;
	}
	file.close();

	file.open(outputDir1, ios::out);
	//cout region to region minimum
	for (int j = 0; j < regionList.size(); j++)
	{
		for (int k = 0; k < regionList.at(j)->getMaxRRDistList().size(); k++)
		{
			file << regionList.at(j)->getMaxRRDistList().at(k);
			if (k != regionList.size() - 1)
				file << ",";
		}
		file << endl;
	}
	file.close();
}

vector<int> skylineModel::findBorderNodeRegion(vector<Region*> allRegionList, string nodeName)
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

std::vector<int> skylineModel::getBNNTableInNLayers(string inputDir, int regionId, int layerNum)
{
	vector<int> candidateRegionIdList;
	vector<int> layerCandidateList;
	vector<int> nowToDoCandidateList;
	vector<Region*> allRegionList;

	int lineNumber = 0;
	char line[SIZE];
	char *pch;
	char *delim = ",";

	fstream fin;

	//set border node to allRegionList by .blist
	fin.open(inputDir, ios::in);
	while (fin.getline(line, sizeof(line), '\n'))
	{
		lineNumber++;
		Region* regRegion = new Region();
		regRegion->setId(lineNumber - 1);
		pch = strtok(line, delim);
		int attributeFlag = 0;

		allRegionList.push_back(regRegion);
		while (pch != NULL)
		{
			attributeFlag++;
			if (attributeFlag % 3 == 1)
			{
				BorderNode* regBorderNode = new BorderNode();
				regBorderNode->setNodeName(pch);
				regRegion->setBorderNodeList(regBorderNode);
			}
			pch = strtok(NULL, delim);
		}
	}
	fin.close();

	//get adjacent Region
	//continue if already candidate
	candidateRegionIdList.push_back(regionId);
	layerCandidateList.push_back(regionId);
	vector<bool> isRegionCandidateList;
	for (int i = 0; i < allRegionList.size(); i++)
	{
		if (i != regionId)
			isRegionCandidateList.push_back(false);
		else
			isRegionCandidateList.push_back(true);
	}


	for (int i = 0; i < layerNum; i++)
	{
		nowToDoCandidateList = layerCandidateList;
		for (int j = 0; j < nowToDoCandidateList.size(); j++)
		{
			if (nowToDoCandidateList.at(j) > allRegionList.back()->getId())
				continue;
			vector<BorderNode*> startRegionBorderList = allRegionList.at(nowToDoCandidateList.at(j))->getBorderNodeList();
			for (int k = 0; k < allRegionList.size(); k++)
			{
				//if i = center region, pass
				if (isRegionCandidateList.at(k) == true)
					continue;
				vector<BorderNode*> regRegionBorderList = allRegionList.at(k)->getBorderNodeList();
				for (int l = 0; l < startRegionBorderList.size(); l++)
				{
					for (int m = 0; m < regRegionBorderList.size(); m++)
					{
						if (startRegionBorderList.at(l)->getNodeName() == regRegionBorderList.at(m)->getNodeName())
						{
							candidateRegionIdList.push_back(allRegionList.at(k)->getId());
							layerCandidateList.push_back(allRegionList.at(k)->getId());
							isRegionCandidateList.at(k) = true;
							break;
						}
					}
					if (isRegionCandidateList.at(k) == true)
						break;
				}
			}
			layerCandidateList.erase(layerCandidateList.begin());
		}
	}
	

	return candidateRegionIdList;
}

int skylineModel::getRegionNumber(string inputfile)
{
	int lineNumber = 0;
	int regionNum;
	char line[SIZE];
	char *pch;
	char *delim = ",";


	fstream fin;
	fin.open(inputfile, ios::in);
	while (fin.getline(line, sizeof(line), '\n'))
	{
		lineNumber++;
		pch = strtok(line, delim);
		int attributeFlag = 0;

		if (lineNumber == 1)
		{
			while (pch != NULL)
			{
				attributeFlag++;
				if (attributeFlag == 2)
					regionNum = atoi(pch);
				if (attributeFlag == 3)
					regionNum = regionNum * atoi(pch);

				pch = strtok(NULL, delim);
			}
			break;
		}
	}
	fin.close();
	return regionNum;
}

Node* skylineModel::randQueryPoint(string inputDir)
{
	int lineNumber = 0;
	int attributeFlag = 0;
	vector<int> nodeLineNumberList;
	char line[SIZE];
	char *pch;
	string str1;
	char *delim = ",";

	Node* queryPoint;
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
						queryPoint = new Node();
						queryPoint->setNodeName(pch);
					}
					else if (attributeFlag == 2)
					{
						queryPoint->setLongtitude(strtod(pch, NULL));
					}
					else if (attributeFlag == 3)
					{
						queryPoint->setLatitude(strtod(pch, NULL));
						cout << "Query Point : " << queryPoint->getNodeName() << "," << queryPoint->getLongtitude() << "," << queryPoint->getLatitude() << endl;
						return queryPoint;
					}
					pch = strtok(NULL, delim);
				}
			}
		}
	}
}

vector<int> skylineModel::getNodeLineNumberListObjreg(string inputDir)
{
	int lineNumber = 0;
	vector<int> nodeLineNumberList;
	char line[SIZE];
	char *pch;
	string str1;
	char *delim = ",";
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
	return nodeLineNumberList;
}

void skylineModel::writeRegionAllNodeBorderData(string inputDir, string outputDir)
{
	char line[SIZE];
	int lineNumber = 0;
	char *pch;
	char *delim = ",";
	fstream fin;
	ofstream file;      //宣告fstream物件
	fin.open(inputDir, ios::in);
	file.open(outputDir + ".objreg", ios::out);		//開啟檔案為輸出狀態，若檔案已存在則清除檔案內容重新寫入
	clock_t nStart = clock(); // 開始時間

	vector<double> partitionLongtitudeList;
	vector<double> partitionLatitudeList;
	vector<Region*> regionList;
	vector<Node*> nodeList;

	//以行讀取
	while (fin.getline(line, sizeof(line), '\n'))
	{
		Node* regionNode;
		pch = strtok(line, delim);
		int attributeFlag = 0;
		string partitionType;
		string roadType;
		int part_x, part_y;
		double roadId = 0, turningAmount = 0, roadDist = 0;
		lineNumber++;

		//get Grid Partition information
		if (lineNumber < 4)
		{
			if (lineNumber == 1)
			{
				while (pch != NULL)
				{
					attributeFlag++;
					if (attributeFlag == 1)
						partitionType = pch;
					else if (attributeFlag == 2)
						part_x = strtod(pch, NULL);
					else if (attributeFlag == 3)
						part_y = strtod(pch, NULL);
					pch = strtok(NULL, delim);
				}
				file << partitionType << "," << part_x << "," << part_y << endl;
			}
			else if (lineNumber == 2)
			{
				while (pch != NULL)
				{
					partitionLongtitudeList.push_back(strtod(pch, NULL));
					pch = strtok(NULL, delim);
				}
				//print grid partition line
				for (int i = 0; i < partitionLongtitudeList.size(); i++)
				{
					file << setprecision(9) << partitionLongtitudeList.at(i);
					if (i != partitionLongtitudeList.size() - 1)
					{
						file << ",";
					}
				}
				file << endl;
			}
			else if (lineNumber == 3)
			{
				while (pch != NULL)
				{
					partitionLatitudeList.push_back(strtod(pch, NULL));
					pch = strtok(NULL, delim);
				}
				//print grid partition line
				for (int i = 0; i < partitionLatitudeList.size(); i++)
				{
					file << setprecision(9) << partitionLatitudeList.at(i);
					if (i != partitionLatitudeList.size() - 1)
					{
						file << ",";
					}
				}
				file << endl;

				//get Regions bounds
				regionList = getRegionListByBound(partitionLongtitudeList, partitionLatitudeList);
			}
		}
		else    //get Border Node
		{
			while (pch != NULL)
			{
				attributeFlag++;
				//記錄ID, 轉折點, 起點
				if (attributeFlag == 1)
				{
					roadId = strtod(pch, NULL);
				}
				else if (attributeFlag == 2)
				{
					turningAmount = strtod(pch, NULL);
				}
				else if (attributeFlag == 3)
				{
					roadType = pch;
				}
				else if (attributeFlag == 4)
				{
					roadDist = strtod(pch, NULL);
				}

				//將起點&終點交替放入register
				//get Node
				if (attributeFlag == 5 || attributeFlag == 8)
				{
					regionNode = new Node();
					regionNode->setLongtitude(strtod(pch, NULL));
					regionNode->setRoadIdList(roadId);
				}
				else if (attributeFlag == 6 || attributeFlag == 9)
				{
					regionNode->setLatitude(strtod(pch, NULL));
				}
				else if (attributeFlag == 7 || attributeFlag == 10)
				{
					regionNode->setNodeName(pch);
					bool nodeExistFlag = false;
					for (int i = 0; i < nodeList.size(); i++)
					{
						if (regionNode->getNodeName() == nodeList.at(i)->getNodeName())
						{
							nodeExistFlag = true;
							if (checkNodeRoadIdExist(nodeList.at(i), roadId) == false)
								nodeList.at(i)->setRoadIdList(roadId);
						}
					}
					if (nodeExistFlag == false)
						nodeList.push_back(regionNode);
				}
				pch = strtok(NULL, delim);
			}
		}
	}

	//get Node Regions
	for (int i = 0; i < nodeList.size(); i++)
	{
		for (int j = 0; j < regionList.size(); j++)
		{
			if (nodeList.at(i)->getLongtitude() >= regionList.at(j)->getLongtitudeLeft() && nodeList.at(i)->getLongtitude() <= regionList.at(j)->getLongtitudeRight() && nodeList.at(i)->getLatitude() >= regionList.at(j)->getLatitudeBottom() && nodeList.at(i)->getLatitude() <= regionList.at(j)->getLatitudeTop())
			{
				regionList.at(j)->setNodeList(nodeList.at(i));
			}
		}
	}

	//print node list
	for (int i = 0; i < regionList.size(); i++)
	{
		vector<Node*> regionNList = regionList.at(i)->getNodeList();
		for (int j = 0; j < regionNList.size(); j++)
		{
			file << setprecision(9) << regionNList.at(j)->getNodeName() << "," << regionNList.at(j)->getLongtitude() << "," << regionNList.at(j)->getLatitude() << "," << regionList.at(i)->getId() << ",";
			for (int i = 0; i < regionNList.at(j)->getRoadIdList().size(); i++)
			{
				file << setprecision(9) << regionNList.at(j)->getRoadIdList().at(i);
				if (i != regionNList.at(j)->getRoadIdList().size() - 1)
					file << ",";
			}
			file << endl;
		}
	}
	//_CrtDumpMemoryLeaks();
}

void skylineModel::precomputationIntegrate_RNN()
{
	char line[SIZE];
	int progressStep;
	int spotNumber;
	cout << endl << "Please enter outputfile name" << endl;
	string outputDir;
	cin >> outputDir;//輸出檔案
	string outputFileDir;
	string experimentalFolder = "experimental_RNN\\";
	outputFileDir = experimentalFolder + outputDir + "\\" + outputDir;

	//建立資料夾
	char a[50];		
	char b[] = ".\\";
	strcpy(a, b);
	strcat(a, experimentalFolder.c_str());
	_mkdir(a);
	strcat(a, outputDir.c_str());
	_mkdir(a);

	//讀取目前進度
	fstream fgetprocess;
	fgetprocess.open(outputFileDir + ".progress", ios::in);
	while (fgetprocess.getline(line, sizeof(line), '\n'))
	{
		progressStep = atoi(line);
		if (progressStep != 0)
			cout << "-----------Step " << progressStep << " loaded, continue steps after.-----------"<< endl;
		break;
	}
	fgetprocess.close();


	string fileDir;
	string partitionSizeOption;
	int part_x, part_y;
	ofstream file;      //宣告fstream物件
	clock_t nStart = clock(); // 開始時間

	//若執行完Step 0, 執行Step 1
	if (progressStep < 1)
	{
		cout << endl << "Please enter CITY_ROAD.geo" << endl;
		cin >> fileDir;//輸入檔案
		copyOriginalData("experimentData\\"+fileDir, outputFileDir + ".geo");

		cout << endl << "Please enter Grid Partition size" << endl;
		//cout << "0. 1*1 " << endl;
		cout << "1. 2*2 " << endl;
		cout << "2. 4*4 " << endl;
		cout << "3. 8*8 " << endl;
		cout << "4. 16*16 " << endl;
		cin >> partitionSizeOption;//選擇格狀切歌大小

		clock_t START, END;
		START = clock();

		//if (partitionSizeOption == "0")
		//{
		//	part_x = 1;
		//	part_y = 1;
		//	writeGridPartition(fileDir, outputFileDir, part_x, part_y);
		//}
		if (partitionSizeOption == "1")
		{
			part_x = 2;
			part_y = 2;
			writeGridPartition(outputFileDir + ".geo", outputFileDir, part_x, part_y);
		}
		else if (partitionSizeOption == "2")
		{
			part_x = 4;
			part_y = 4;
			writeGridPartition(outputFileDir + ".geo", outputFileDir, part_x, part_y);
		}
		else if (partitionSizeOption == "3")
		{
			part_x = 8;
			part_y = 8;
			writeGridPartition(outputFileDir + ".geo", outputFileDir, part_x, part_y);
		}
		else if (partitionSizeOption == "4")
		{
			part_x = 16;
			part_y = 16;
			writeGridPartition(outputFileDir + ".geo", outputFileDir, part_x, part_y);
		}
		file.open(outputFileDir + ".progress", ios::trunc | ios::out);
		file << "1" << endl;	//print progress
		file.close();
		cout << endl << "Process 1 Finished" << endl;

		END = clock();
		cout << (float)(END - START) / CLOCKS_PER_SEC << endl;
	}
 	
	//若執行完Step 1, 執行Step 2
	if (progressStep < 2)
	{
		clock_t START, END;
		START = clock();

		writeAllBorderRoadDistance(outputFileDir + ".bgeo", outputFileDir);
		file.open(outputFileDir + ".progress", ios::trunc | ios::out);
		file << "2" << endl;
		file.close();
		cout << "Process 2 Finished" << endl;

		END = clock();
		cout << (float)(END - START) / CLOCKS_PER_SEC << endl;
	}

	//若執行完Step 2, 執行Step 3
	if (progressStep < 3)
	{
		clock_t START, END;
		START = clock();

		writeBorderRoadDistanceAndRegion(outputFileDir + ".bn", outputFileDir);
		file.open(outputFileDir + ".progress", ios::trunc | ios::out);
		file << "3" << endl;
		file.close();
		cout << "Process 3 Finished" << endl;

		END = clock();
		cout << (float)(END - START) / CLOCKS_PER_SEC << endl;
	}
	if (progressStep < 4)
	{
		clock_t START, END;
		START = clock();

		//writeRegionBorderData(outputFileDir + ".bn", outputFileDir);
		writeRegionAllNodeBorderData(outputFileDir + ".bnreg", outputFileDir);
		writeOrderedIdRegionNodeData(outputFileDir + ".objreg", outputFileDir);
		//writeRegionNodeData(outputFileDir + ".bn", outputFileDir);
		file.open(outputFileDir + ".progress", ios::trunc | ios::out);
		file << "4" << endl;
		file.close();
		cout << "Process 4 Finished" << endl;

		END = clock();
		cout << (float)(END - START) / CLOCKS_PER_SEC << endl;
	}
	if (progressStep < 5)
	{
		clock_t START, END;
		START = clock();

		dijkstra(outputFileDir + ".bnreg", outputFileDir);
		file.open(outputFileDir + ".progress", ios::trunc | ios::out);
		file << "5" << endl;
		file.close();
		cout << "Process 5 Finished" << endl;

		END = clock();
		cout << (float)(END - START) / CLOCKS_PER_SEC << endl;
	}
	if (progressStep < 6)
	{
		clock_t START, END;
		START = clock();

		writeBBDist(outputFileDir + ".bntable", outputFileDir);
		file.open(outputFileDir + ".progress", ios::trunc | ios::out);
		file << "6" << endl;
		file.close();
		cout << "Process 6 Finished" << endl;

		END = clock();
		cout << (float)(END - START) / CLOCKS_PER_SEC << endl;
	}
	if (progressStep < 7)
	{
		clock_t START, END;
		START = clock();

		vector<int> nodeLineNumberList;
		nodeLineNumberList = getNodeLineNumberListObjreg(outputFileDir + ".objreg");
		cout << endl << "Totally " << nodeLineNumberList.size() << " spots" << endl;
		cout << "Please enter spot number" << endl;
		cin >> spotNumber;
		writeRandomSpots(outputFileDir + ".objreg", outputFileDir, spotNumber);
		Node* queryPoint = randQueryPoint(outputFileDir + ".objreg");
		file.open(outputFileDir + ".progress", ios::trunc | ios::out);
		file << "7" << endl;
		file.close();
		cout << "Process 7 Finished" << endl;

		END = clock();
		cout << (float)(END - START) / CLOCKS_PER_SEC << endl;
	}
	if (progressStep < 8)
	{
		clock_t START, END;
		START = clock();

		fstream fin;
		fin.open(outputFileDir + ".progress", ios::in);
		char *pch;
		char *delim = ",";
		string nsProcessFlag = "";
		int nsProcessCounter = 0;
		int lineNumber = 0;
		while (fin.getline(line, sizeof(line), '\n'))
		{
			pch = strtok(line, delim);
			int attributeFlag = 0;
			lineNumber++;
			if (lineNumber == 2)
			{
				while (pch != NULL)
				{
					attributeFlag++;
					if (attributeFlag == 1)
						nsProcessFlag = pch;
					else if (attributeFlag == 2)
						nsProcessCounter = atoi(pch);
					pch = strtok(NULL, delim);
				}
			}
		}

		if (nsProcessFlag == "")
		{
			nsProcessCounter = 0;
			writeBNAndNNDist(outputFileDir + ".bntable", outputFileDir + ".objreg", outputFileDir + ".progress", outputFileDir, nsProcessCounter);
			writeBNAndNSDist(outputFileDir + ".bntable", outputFileDir + ".objreg", outputFileDir + ".spot", outputFileDir + ".progress", outputFileDir, nsProcessCounter);
		}
		else if (nsProcessFlag == "bnnn")
		{
			writeBNAndNNDist(outputFileDir + ".bntable", outputFileDir + ".objreg", outputFileDir + ".progress", outputFileDir, nsProcessCounter);
			nsProcessCounter = 0;
			writeBNAndNSDist(outputFileDir + ".bntable", outputFileDir + ".objreg", outputFileDir + ".spot", outputFileDir + ".progress", outputFileDir, nsProcessCounter);
		}
		else if (nsProcessFlag == "bnns")
		{
			writeBNAndNSDist(outputFileDir + ".bntable", outputFileDir + ".objreg", outputFileDir + ".spot", outputFileDir + ".progress", outputFileDir, nsProcessCounter);
		}
		
		file.open(outputFileDir + ".progress", ios::trunc | ios::out);
		file << "8" << endl;
		file.close();
		cout << "Process 8 Finished" << endl;

		END = clock();
		cout << (float)(END - START) / CLOCKS_PER_SEC << endl;
	}
	if (progressStep < 9)
	{
		clock_t START, END;
		START = clock();

		writeRegionMaxLamdaDist(outputFileDir + ".bnns", outputFileDir + ".objreg", outputFileDir + ".bnnn", outputFileDir);
		file.open(outputFileDir + ".progress", ios::trunc | ios::out);
		file << "9" << endl;
		file.close();
		cout << "Process 9 Finished" << endl;

		END = clock();
		cout << (float)(END - START) / CLOCKS_PER_SEC << endl;
	}
	if (progressStep < 10)
	{
		clock_t START, END;
		START = clock();

		writeAllMaxDistance(outputFileDir + ".maxld", outputFileDir + ".bn", outputFileDir);
		file.open(outputFileDir + ".progress", ios::trunc | ios::out);
		file << "10" << endl;
		file.close();
		cout << "Process 10 Finished" << endl;

		END = clock();
		cout << (float)(END - START) / CLOCKS_PER_SEC << endl;
	}
	if (progressStep < 11)
	{
		clock_t START, END;
		START = clock();

		//writeBNNTable(outputFileDir + ".bntable", outputFileDir + ".objreg", outputFileDir + ".oobjreg", outputFileDir);
		//writeNewBNNTable(outputFileDir + ".bntable", outputFileDir + ".objreg", outputFileDir + ".oobjreg", outputFileDir);
		writeNewSpotBNNTable(outputFileDir + ".bnns", outputFileDir + ".objreg", outputFileDir);

		file.open(outputFileDir + ".progress", ios::trunc | ios::out);
		file << "10" << endl;
		file.close();
		cout << "Process 11 Finished" << endl;

		END = clock();
		cout << (float)(END - START) / CLOCKS_PER_SEC << endl;
	}
	if (progressStep < 12)
	{
		clock_t START, END;
		START = clock();

		writeBorderList(outputFileDir + ".objreg", outputFileDir);
		writeIndex(outputFileDir + ".bn", outputFileDir + ".maxld", outputFileDir + ".blist", outputFileDir + ".bnns", outputFileDir, 1);
		file.open(outputFileDir + ".progress", ios::trunc | ios::out);
		file << "12" << endl;
		file.close();
		cout << "Process 12 Finished" << endl;

		END = clock();
		cout << (float)(END - START) / CLOCKS_PER_SEC << endl;
	}
	if (progressStep < 13)
	{
		clock_t START, END;
		START = clock();

		//writeNPIIndex(outputFileDir + ".bn", outputFileDir + ".bnns", outputFileDir);
		writeMinMaxRR(outputFileDir + ".objreg", outputFileDir + ".bb", outputFileDir);
		file.open(outputFileDir + ".progress", ios::trunc | ios::out);
		file << "13" << endl;
		file.close();
		cout << "Process 13 Finished" << endl;

		END = clock();
		cout << (float)(END - START) / CLOCKS_PER_SEC << endl;
	}

	vector<int> candidateRegionIdList;
	candidateRegionIdList = getBNNTableInNLayers(outputFileDir + ".blist", 0, 2);
	cout << endl;
	for (int i = 0; i < candidateRegionIdList.size(); i++)
		cout << candidateRegionIdList.at(i) << " ";
	cout << endl;
	candidateRegionIdList = getBNNTableInNLayers(outputFileDir + ".blist", 1, 1);
	for (int i = 0; i < candidateRegionIdList.size(); i++)
		cout << candidateRegionIdList.at(i) << " ";
	cout << endl << endl;

	//compute RNN

	//int queryAmountK;
	//cout << "Please enter the value of k" << endl;
	//cin >> queryAmountK;
	int startRegionId = 0;
	file.open(outputFileDir + ".ba", ios::app | ios::out);
	file << "BroadcastCycle, TuningTime, Latency" << endl;
	file.close();
	//file.open(outputFileDir + "_" + to_string(queryAmountK) + ".npi", ios::app | ios::out);
	//file << "BroadcastCycle, TuningTime, Latency" << endl;
	//file.close();
	for (int i = 0; i < 1; i++)
	{
		ClientRNN* clientRNN = new ClientRNN();
		clientRNN->computeRNNAnswer(outputFileDir + ".objreg", outputFileDir + ".index", outputFileDir + ".nbnntable", outputFileDir + ".bb", outputFileDir + ".blist", outputFileDir + ".maxdist", outputFileDir + ".bnns", outputFileDir + ".npirrmin", startRegionId);
		Node* queryPoint = clientRNN->getQueryPoint();
		//NPIClient* npiclient = new NPIClient();
		//npiclient->setQueryPoint(queryPoint);
		//npiclient->computeKNNAnswer(outputFileDir + ".objreg", outputFileDir + ".npiind", outputFileDir + ".npirrmin", outputFileDir + ".npirrmax", outputFileDir + ".maxdist", outputFileDir + ".bnns", queryAmountK, startRegionId);

		file.open(outputFileDir + ".ba", ios::app | ios::out);
		file << clientRNN->getBroadcastCycleSize() << "," << clientRNN->getTuningDataSize() << "," << clientRNN->getLatencyDataSize() << endl;
		file.close();

		//file.open(outputFileDir + "_" + to_string(queryAmountK) + ".npi", ios::app | ios::out);
		//file << npiclient->getBroadcastCycleSize() << "," << npiclient->getTuningDataSize() << "," << npiclient->getLatencyDataSize() << endl;
		//file.close();
	}

	cout << "All Finished" << endl;
	//cout << "BNN Result : " << endl;
	//for (int i = 0; i < client->getQueryKNNList().size(); i++)
	//{
	//	cout << client->getQueryKNNList().at(i)->getNodeName() << endl;
	//}
	//cout << "NPI Result : " << endl;
	//for (int i = 0; i < npiclient->getQueryKNNList().size(); i++)
	//{
	//	cout << npiclient->getQueryKNNList().at(i)->getNodeName() << endl;
	//}

	//cout << endl << "-------------------------------------------" << endl;
	//cout << "Time Result : " << endl << endl;
	//cout << "BNN broadcast cycle : " << client->getBroadcastCycleSize() << endl;
	//cout << "NPI broadcast cycle : " << npiclient->getBroadcastCycleSize() << endl;
	//cout << "----------------------------------" << endl;
	//cout << "BNN tuning time : " << client->getTuningDataSize() << endl;
	//cout << "NPI tuning time : " << npiclient->getTuningDataSize() << endl;
	//cout << "----------------------------------" << endl;
	//cout << "BNN latency time : " << client->getLatencyDataSize() << endl;
	//cout << "NPI latency time : " << npiclient->getLatencyDataSize() << endl;
}

void skylineModel::precomputationIntegrate()
{
	char line[SIZE];
	int progressStep;
	int spotNumber;
	cout << endl << "Please enter outputfile name" << endl;
	string outputDir;
	cin >> outputDir;//輸出檔案
	string outputFileDir;
	string experimentalFolder = "experimental_KNN\\";
	outputFileDir = experimentalFolder + outputDir + "\\" + outputDir;

	//建立資料夾
	char a[50];
	char b[] = ".\\";
	strcpy(a, b);
	strcat(a, experimentalFolder.c_str());
	_mkdir(a);
	strcat(a, outputDir.c_str());
	_mkdir(a);

	//讀取目前進度
	fstream fgetprocess;
	fgetprocess.open(outputFileDir + ".progress", ios::in);
	while (fgetprocess.getline(line, sizeof(line), '\n'))
	{
		progressStep = atoi(line);
		if (progressStep != 0)
			cout << "-----------Step " << progressStep << " loaded, continue steps after.-----------" << endl;
		break;
	}
	fgetprocess.close();


	string fileDir;
	string partitionSizeOption;
	int part_x, part_y;
	ofstream file;      //宣告fstream物件
	clock_t nStart = clock(); // 開始時間

	//若執行完Step 0, 執行Step 1
	if (progressStep < 1)
	{
		cout << endl << "Please enter CITY_ROAD.geo" << endl;
		cin >> fileDir;//輸入檔案
		copyOriginalData("experimentData\\" + fileDir, outputFileDir + ".geo");

		cout << endl << "Please enter Grid Partition size" << endl;
		//cout << "0. 1*1 " << endl;
		cout << "1. 2*2 " << endl;
		cout << "2. 4*4 " << endl;
		cout << "3. 8*8 " << endl;
		cout << "4. 16*16 " << endl;
		cin >> partitionSizeOption;//選擇格狀切歌大小

		clock_t START, END;
		START = clock();

		//if (partitionSizeOption == "0")
		//{
		//	part_x = 1;
		//	part_y = 1;
		//	writeGridPartition(fileDir, outputFileDir, part_x, part_y);
		//}
		if (partitionSizeOption == "1")
		{
			part_x = 2;
			part_y = 2;
			writeGridPartition(outputFileDir + ".geo", outputFileDir, part_x, part_y);
		}
		else if (partitionSizeOption == "2")
		{
			part_x = 4;
			part_y = 4;
			writeGridPartition(outputFileDir + ".geo", outputFileDir, part_x, part_y);
		}
		else if (partitionSizeOption == "3")
		{
			part_x = 8;
			part_y = 8;
			writeGridPartition(outputFileDir + ".geo", outputFileDir, part_x, part_y);
		}
		else if (partitionSizeOption == "4")
		{
			part_x = 16;
			part_y = 16;
			writeGridPartition(outputFileDir + ".geo", outputFileDir, part_x, part_y);
		}
		file.open(outputFileDir + ".progress", ios::trunc | ios::out);
		file << "1" << endl;	//print progress
		file.close();
		cout << endl << "Process 1 Finished" << endl;

		END = clock();
		cout << (float)(END - START) / CLOCKS_PER_SEC << endl;
	}

	//若執行完Step 1, 執行Step 2
	if (progressStep < 2)
	{
		clock_t START, END;
		START = clock();

		writeAllBorderRoadDistance(outputFileDir + ".bgeo", outputFileDir);
		file.open(outputFileDir + ".progress", ios::trunc | ios::out);
		file << "2" << endl;
		file.close();
		cout << "Process 2 Finished" << endl;

		END = clock();
		cout << (float)(END - START) / CLOCKS_PER_SEC << endl;
	}

	//若執行完Step 2, 執行Step 3
	if (progressStep < 3)
	{
		clock_t START, END;
		START = clock();

		writeBorderRoadDistanceAndRegion(outputFileDir + ".bn", outputFileDir);
		file.open(outputFileDir + ".progress", ios::trunc | ios::out);
		file << "3" << endl;
		file.close();
		cout << "Process 3 Finished" << endl;

		END = clock();
		cout << (float)(END - START) / CLOCKS_PER_SEC << endl;
	}
	if (progressStep < 4)
	{
		clock_t START, END;
		START = clock();

		//writeRegionBorderData(outputFileDir + ".bn", outputFileDir);
		writeRegionAllNodeBorderData(outputFileDir + ".bnreg", outputFileDir);
		writeOrderedIdRegionNodeData(outputFileDir + ".objreg", outputFileDir);
		//writeRegionNodeData(outputFileDir + ".bn", outputFileDir);
		file.open(outputFileDir + ".progress", ios::trunc | ios::out);
		file << "4" << endl;
		file.close();
		cout << "Process 4 Finished" << endl;

		END = clock();
		cout << (float)(END - START) / CLOCKS_PER_SEC << endl;
	}
	if (progressStep < 5)
	{
		clock_t START, END;
		START = clock();

		dijkstra(outputFileDir + ".bnreg", outputFileDir);
		file.open(outputFileDir + ".progress", ios::trunc | ios::out);
		file << "5" << endl;
		file.close();
		cout << "Process 5 Finished" << endl;

		END = clock();
		cout << (float)(END - START) / CLOCKS_PER_SEC << endl;
	}
	if (progressStep < 6)
	{
		clock_t START, END;
		START = clock();

		writeBBDist(outputFileDir + ".bntable", outputFileDir);
		file.open(outputFileDir + ".progress", ios::trunc | ios::out);
		file << "6" << endl;
		file.close();
		cout << "Process 6 Finished" << endl;

		END = clock();
		cout << (float)(END - START) / CLOCKS_PER_SEC << endl;
	}
	if (progressStep < 7)
	{
		clock_t START, END;
		START = clock();

		vector<int> nodeLineNumberList;
		nodeLineNumberList = getNodeLineNumberListObjreg(outputFileDir + ".objreg");
		cout << endl << "Totally " << nodeLineNumberList.size() << " spots" << endl;
		cout << "Please enter spot number" << endl;
		cin >> spotNumber;
		writeRandomSpots(outputFileDir + ".objreg", outputFileDir, spotNumber);
		Node* queryPoint = randQueryPoint(outputFileDir + ".objreg");
		file.open(outputFileDir + ".progress", ios::trunc | ios::out);
		file << "7" << endl;
		file.close();
		cout << "Process 7 Finished" << endl;

		END = clock();
		cout << (float)(END - START) / CLOCKS_PER_SEC << endl;
	}
	if (progressStep < 8)
	{
		clock_t START, END;
		START = clock();

		fstream fin;
		fin.open(outputFileDir + ".progress", ios::in);
		char *pch;
		char *delim = ",";
		string nsProcessFlag = "";
		int nsProcessCounter = 0;
		int lineNumber = 0;
		while (fin.getline(line, sizeof(line), '\n'))
		{
			pch = strtok(line, delim);
			int attributeFlag = 0;
			lineNumber++;
			if (lineNumber == 2)
			{
				while (pch != NULL)
				{
					attributeFlag++;
					if (attributeFlag == 1)
						nsProcessFlag = pch;
					else if (attributeFlag == 2)
						nsProcessCounter = atoi(pch);
					pch = strtok(NULL, delim);
				}
			}
		}

		if (nsProcessFlag == "")
		{
			nsProcessCounter = 0;
			writeBNAndNNDist(outputFileDir + ".bntable", outputFileDir + ".objreg", outputFileDir + ".progress", outputFileDir, nsProcessCounter);
			writeBNAndNSDist(outputFileDir + ".bntable", outputFileDir + ".objreg", outputFileDir + ".spot", outputFileDir + ".progress", outputFileDir, nsProcessCounter);
		}
		else if (nsProcessFlag == "bnnn")
		{
			writeBNAndNNDist(outputFileDir + ".bntable", outputFileDir + ".objreg", outputFileDir + ".progress", outputFileDir, nsProcessCounter);
			nsProcessCounter = 0;
			writeBNAndNSDist(outputFileDir + ".bntable", outputFileDir + ".objreg", outputFileDir + ".spot", outputFileDir + ".progress", outputFileDir, nsProcessCounter);
		}
		else if (nsProcessFlag == "bnns")
		{
			writeBNAndNSDist(outputFileDir + ".bntable", outputFileDir + ".objreg", outputFileDir + ".spot", outputFileDir + ".progress", outputFileDir, nsProcessCounter);
		}

		file.open(outputFileDir + ".progress", ios::trunc | ios::out);
		file << "8" << endl;
		file.close();
		cout << "Process 8 Finished" << endl;

		END = clock();
		cout << (float)(END - START) / CLOCKS_PER_SEC << endl;
	}
	if (progressStep < 9)
	{
		clock_t START, END;
		START = clock();

		writeRegionMaxLamdaDist(outputFileDir + ".bnns", outputFileDir + ".objreg", outputFileDir + ".bnnn", outputFileDir);
		file.open(outputFileDir + ".progress", ios::trunc | ios::out);
		file << "9" << endl;
		file.close();
		cout << "Process 9 Finished" << endl;

		END = clock();
		cout << (float)(END - START) / CLOCKS_PER_SEC << endl;
	}
	if (progressStep < 10)
	{
		clock_t START, END;
		START = clock();

		writeAllMaxDistance(outputFileDir + ".maxld", outputFileDir + ".bn", outputFileDir);
		file.open(outputFileDir + ".progress", ios::trunc | ios::out);
		file << "10" << endl;
		file.close();
		cout << "Process 10 Finished" << endl;

		END = clock();
		cout << (float)(END - START) / CLOCKS_PER_SEC << endl;
	}
	if (progressStep < 11)
	{
		clock_t START, END;
		START = clock();

		//writeBNNTable(outputFileDir + ".bntable", outputFileDir + ".objreg", outputFileDir + ".oobjreg", outputFileDir);
		//writeNewBNNTable(outputFileDir + ".bntable", outputFileDir + ".objreg", outputFileDir + ".oobjreg", outputFileDir);
		writeNewSpotBNNTable(outputFileDir + ".bnns", outputFileDir + ".objreg", outputFileDir);

		file.open(outputFileDir + ".progress", ios::trunc | ios::out);
		file << "10" << endl;
		file.close();
		cout << "Process 11 Finished" << endl;

		END = clock();
		cout << (float)(END - START) / CLOCKS_PER_SEC << endl;
	}
	if (progressStep < 12)
	{
		clock_t START, END;
		START = clock();

		writeBorderList(outputFileDir + ".objreg", outputFileDir);
		writeIndex(outputFileDir + ".bn", outputFileDir + ".maxld", outputFileDir + ".blist", outputFileDir + ".bnns", outputFileDir, 1);
		file.open(outputFileDir + ".progress", ios::trunc | ios::out);
		file << "12" << endl;
		file.close();
		cout << "Process 12 Finished" << endl;

		END = clock();
		cout << (float)(END - START) / CLOCKS_PER_SEC << endl;
	}
	if (progressStep < 13)
	{
		clock_t START, END;
		START = clock();

		writeNPIIndex(outputFileDir + ".bn", outputFileDir + ".bnns", outputFileDir);
		writeMinMaxRR(outputFileDir + ".objreg", outputFileDir + ".bb", outputFileDir);
		file.open(outputFileDir + ".progress", ios::trunc | ios::out);
		file << "13" << endl;
		file.close();
		cout << "Process 13 Finished" << endl;

		END = clock();
		cout << (float)(END - START) / CLOCKS_PER_SEC << endl;
	}

	vector<int> candidateRegionIdList;
	candidateRegionIdList = getBNNTableInNLayers(outputFileDir + ".blist", 0, 2);
	cout << endl;
	for (int i = 0; i < candidateRegionIdList.size(); i++)
		cout << candidateRegionIdList.at(i) << " ";
	cout << endl;
	candidateRegionIdList = getBNNTableInNLayers(outputFileDir + ".blist", 1, 1);
	for (int i = 0; i < candidateRegionIdList.size(); i++)
		cout << candidateRegionIdList.at(i) << " ";
	cout << endl << endl;

	//compute KNN
	int queryAmountK;
	cout << "Please enter the value of k" << endl;
	cin >> queryAmountK;
	int startRegionId = 0;
	file.open(outputFileDir + "_" + to_string(queryAmountK) + ".ba", ios::app | ios::out);
	file << "BroadcastCycle, TuningTime, Latency" << endl;
	file.close();
	file.open(outputFileDir + "_" + to_string(queryAmountK) + ".npi", ios::app | ios::out);
	file << "BroadcastCycle, TuningTime, Latency" << endl;
	file.close();
	for (int i = 0; i < 10; i++)
	{
		Client* client = new Client();
		client->computeKNNAnswer(outputFileDir + ".objreg", outputFileDir + ".index", outputFileDir + ".nbnntable", outputFileDir + ".bb", outputFileDir + ".blist", outputFileDir + ".maxdist", outputFileDir + ".bnns", outputFileDir + ".npirrmin", queryAmountK, startRegionId);
		Node* queryPoint = client->getQueryPoint();
		NPIClient* npiclient = new NPIClient();
		npiclient->setQueryPoint(queryPoint);
		npiclient->computeKNNAnswer(outputFileDir + ".objreg", outputFileDir + ".npiind", outputFileDir + ".npirrmin", outputFileDir + ".npirrmax", outputFileDir + ".maxdist", outputFileDir + ".bnns", queryAmountK, startRegionId);

		file.open(outputFileDir + "_" + to_string(queryAmountK) + ".ba", ios::app | ios::out);
		file << client->getBroadcastCycleSize() << "," << client->getTuningDataSize() << "," << client->getLatencyDataSize() << endl;
		file.close();

		file.open(outputFileDir + "_" + to_string(queryAmountK) + ".npi", ios::app | ios::out);
		file << npiclient->getBroadcastCycleSize() << "," << npiclient->getTuningDataSize() << "," << npiclient->getLatencyDataSize() << endl;
		file.close();
	}

	cout << "All Finished" << endl;
	//cout << "BNN Result : " << endl;
	//for (int i = 0; i < client->getQueryKNNList().size(); i++)
	//{
	//	cout << client->getQueryKNNList().at(i)->getNodeName() << endl;
	//}
	//cout << "NPI Result : " << endl;
	//for (int i = 0; i < npiclient->getQueryKNNList().size(); i++)
	//{
	//	cout << npiclient->getQueryKNNList().at(i)->getNodeName() << endl;
	//}

	//cout << endl << "-------------------------------------------" << endl;
	//cout << "Time Result : " << endl << endl;
	//cout << "BNN broadcast cycle : " << client->getBroadcastCycleSize() << endl;
	//cout << "NPI broadcast cycle : " << npiclient->getBroadcastCycleSize() << endl;
	//cout << "----------------------------------" << endl;
	//cout << "BNN tuning time : " << client->getTuningDataSize() << endl;
	//cout << "NPI tuning time : " << npiclient->getTuningDataSize() << endl;
	//cout << "----------------------------------" << endl;
	//cout << "BNN latency time : " << client->getLatencyDataSize() << endl;
	//cout << "NPI latency time : " << npiclient->getLatencyDataSize() << endl;
}


void skylineModel::copyOriginalData(string inputDir, string outputDir)
{
	char line[SIZE];
	char *pch;
	char *delim = ",";
	int lineNumber = 0;
	int lineNumberCounter = 0;
	fstream fin;
	ofstream file;      //宣告fstream物件
	fin.open(inputDir, ios::in);
	file.open(outputDir, ios::out | ios::binary);		//開啟檔案為輸出狀態，若檔案已存在則清除檔案內容重新寫入
	clock_t nStart = clock(); // 開始時間
	//cout << "copyOriginalData ...";
	//以行讀取
	while (fin.getline(line, sizeof(line), '\n'))
	{
		lineNumber++;
	//	cout << "reading...";
	}
	fin.close();

	fin.open(inputDir, ios::in);
	while (fin.getline(line, sizeof(line), '\n'))
	{
		lineNumberCounter++;
		file << line;
		if (lineNumberCounter != lineNumber)
			file << endl;
	}
}

int skylineModel::checkNodeExistAndReturnNodeInd(vector<Node*> nodeList, Node* checkNode)
{
	for (int i = 0; i < nodeList.size(); i++)
	{
		if (checkNode->getLongtitude() == nodeList.at(i)->getLongtitude() && checkNode->getLatitude() == nodeList.at(i)->getLatitude())
		{
			return i;
		}
	}
	return -1;
}

bool skylineModel::checkNodeRoadIdExist(Node* checkNode, int checkRoadId)
{
	for (int i = 0; i < checkNode->getRoadIdList().size(); i++)
	{
		if (checkNode->getRoadIdList().at(i) == checkRoadId)
			return true;
	}
	return false;
}

int skylineModel::checkBorderNodeExistAndReturnNodeInd(vector<BorderNode*> borderNodeList, BorderNode* checkNode)
{
	for (int i = 0; i < borderNodeList.size(); i++)
	{
		if (checkNode->getLongtitude() == borderNodeList.at(i)->getLongtitude() && checkNode->getLatitude() == borderNodeList.at(i)->getLatitude())
		{
			return i;
		}
	}
	return -1;
}

vector<BorderNode*> skylineModel::uniteBorderListWithAllRegion(vector<Region*> regionList)
{
	vector<BorderNode*> borderNodeList;

	for (int i = 0; i < regionList.size(); i++)
	{
		for (int j = 0; j < regionList.at(i)->getBorderNodeList().size(); j++)
			regionList.at(i)->getBorderNodeList().at(j)->setRegionIdList(i);
	}

	for (int i = 0; i < regionList.size(); i++)
	{
		if (borderNodeList.size() == 0)
			borderNodeList = regionList.at(i)->getBorderNodeList();
		else
			uniteTwoBorderList(borderNodeList, regionList.at(i)->getBorderNodeList());
	}

	return borderNodeList;
}

vector<BorderNode*> skylineModel::uniteTwoBorderList(vector<BorderNode*> borderNodeList, vector<BorderNode*> regionBorderNodeList)
{
	if (borderNodeList.size() == 0)
		borderNodeList = regionBorderNodeList;
	else
	{
		for (int i = 0; i < regionBorderNodeList.size(); i++)
		{
			bool existFile = false;
			for (int j = 0; j < borderNodeList.size(); j++)
			{
				if (borderNodeList.at(j)->getLongtitude() == regionBorderNodeList.at(i)->getLongtitude() && borderNodeList.at(j)->getLatitude() == regionBorderNodeList.at(i)->getLatitude())
				{
					borderNodeList.at(j)->setRegionIdList(regionBorderNodeList.at(i)->getRegionIdList().back());
					existFile = true;
				}
			}
			if (existFile == false)
				borderNodeList.push_back(regionBorderNodeList.at(i));
		}
	}

	return borderNodeList;
}

void skylineModel::writeBorderRoadDistanceAndRegion(string inputDir, string outputDir)
{
	int prev[MAX] = { 0 };
	int dist[MAX] = { 0 };
	//重讀檔案以找出Border Node
	char line[SIZE];

	ofstream file;
	fstream fin1;
	outputDir = outputDir + ".bnreg";
	fin1.open(inputDir, ios::in);
	file.open(outputDir, ios::out | ios::binary);

	int lineNumber = 0;
	char *pch;
	char *delim = ",";
	int nodeId = 0, borderId = 0;
	vector<Node*> nodeList;
	vector<Region*> regionList;
	vector<BorderNode*> borderNodeList;
	vector<double> partitionLongtitudeList;
	vector<double> partitionLatitudeList;
	int totalNode = 0, totalBorderNode=0;

	//cout << "read file " << inputDir<<endl;

	while (fin1.getline(line, sizeof(line), '\n'))
	{
		pch = strtok(line, delim);
		int attributeFlag = 0;
		string type;
		string partitionType;
		int part_x, part_y;
		int roadRegionId;
		double longtitude, latitude;
		double dist;
		lineNumber++;
		//cout << "lineNumber " << lineNumber << endl;
		if (lineNumber < 4)
		{
			if (lineNumber == 1)
			{
				while (pch != NULL)
				{
					attributeFlag++;
					if (attributeFlag == 1)
						partitionType = pch;
					else if (attributeFlag == 2)
						part_x = strtod(pch, NULL);
					else if (attributeFlag == 3)
						part_y = strtod(pch, NULL);
					pch = strtok(NULL, delim);
				}
				file << setprecision(9) << partitionType << "," << part_x << "," << part_y << endl;
			}
			else if (lineNumber == 2)
			{
				while (pch != NULL)
				{
					partitionLongtitudeList.push_back(strtod(pch, NULL));
					pch = strtok(NULL, delim);
				}
				for (int i = 0; i < partitionLongtitudeList.size(); i++)
				{
					file << setprecision(9) << partitionLongtitudeList.at(i);
					if (i != partitionLongtitudeList.size() - 1)
					{
						file << ",";
					}
				}
				file << endl;
			}
			else if (lineNumber == 3)
			{
				while (pch != NULL)
				{
					partitionLatitudeList.push_back(strtod(pch, NULL));
					pch = strtok(NULL, delim);
				}
				for (int i = 0; i < partitionLatitudeList.size(); i++)
				{
					file << setprecision(9) << partitionLatitudeList.at(i);
					if (i != partitionLatitudeList.size() - 1)
					{
						file << ",";
					}
				}
				file << endl;
				regionList = getRegionListByBound(partitionLongtitudeList, partitionLatitudeList);
			}
		}
		else
		{
			while (pch != NULL)
			{
				attributeFlag++;
				if (attributeFlag == 1)
				{
					//road id
					file << pch << ",";
				}
				if (attributeFlag == 2)
				{
					file << pch << ",";
					//turning point
				}
				if (attributeFlag == 3)
				{
					//start node type & end point type
					file << pch << ",";
					type = pch;
				}
				if (attributeFlag == 4)
				{
					//distance
					dist = strtod(pch, NULL);
					file << pch << ",";
				}
				if (attributeFlag == 5)
				{
					//start point x
					longtitude = strtod(pch, NULL);
					file << pch << ",";
				}
				if (attributeFlag == 6)
				{
					//start point y
					file << pch << ",";
					latitude = strtod(pch, NULL);
					if (type == "nn" || type == "nb")
					{
						bool nodeExistFlag = false;
						for (int index = 0; index < nodeList.size(); index++)
						{
							if (nodeList.at(index)->getLatitude() == latitude && nodeList.at(index)->getLongtitude() == longtitude)
							{
								nodeExistFlag = true;
								file << nodeList.at(index)->getNodeName() << ",";
								break;
							}
						}
						if (!nodeExistFlag)
						{
							//
							totalNode++;
							Node* node = new Node(longtitude, latitude, nodeId++);
							// check Node Region
							roadRegionId = checkNodeRegion(regionList, node);
							node->setNodeRegionId(roadRegionId);

							nodeList.push_back(node);
							file << node->getNodeName() << ",";
						}
					}
					else
					{
						bool borderNodeExistFlag = false;
						for (int index = 0; index < borderNodeList.size(); index++)
						{
							if (borderNodeList.at(index)->getLatitude() == latitude && borderNodeList.at(index)->getLongtitude() == longtitude)
							{
								borderNodeExistFlag = true;
								file << borderNodeList.at(index)->getNodeName() << ",";
								break;
							}
						}
						if (!borderNodeExistFlag)
						{
							totalBorderNode++;
							BorderNode* borderNode = new BorderNode(longtitude, latitude, nodeId++);
							borderNodeList.push_back(borderNode);
							file << borderNode->getNodeName() << ",";
						}
					}
				}
				if (attributeFlag == 7)
				{
					//end point x
					longtitude = strtod(pch, NULL);
					file << pch << ",";
				}
				if (attributeFlag == 8)
				{
					//end point y
					file << pch << ",";
					latitude = strtod(pch, NULL);
					if (type == "nn" || type == "bn")
					{
						bool nodeExistFlag = false;
						for (int index = 0; index < nodeList.size(); index++)
						{
							if (nodeList.at(index)->getLatitude() == latitude && nodeList.at(index)->getLongtitude() == longtitude)
							{
								nodeExistFlag = true;
								file << nodeList.at(index)->getNodeName() << "," << nodeList.at(index)->getNodeRegionId();
								break;
							}
						}
						if (!nodeExistFlag)
						{
							totalNode++;
							Node* node = new Node(longtitude, latitude, nodeId);
							nodeId++;
							// check Node Region
							node->setNodeRegionId(checkNodeRegion(regionList, node));
							// add to nodeList
							nodeList.push_back(node);
							file << node->getNodeName() << "," << node->getNodeRegionId();
						}
					}
					else
					{
						bool borderNodeExistFlag = false;
						for (int index = 0; index < borderNodeList.size(); index++)
						{
							if (borderNodeList.at(index)->getLatitude() == latitude && borderNodeList.at(index)->getLongtitude() == longtitude)
							{
								borderNodeExistFlag = true;
								file << borderNodeList.at(index)->getNodeName() << "," << roadRegionId;
								break;
							}
						}
						if (!borderNodeExistFlag)
						{
							totalBorderNode++;
							BorderNode* borderNode = new BorderNode(longtitude, latitude, nodeId++);
							borderNodeList.push_back(borderNode);
							file << borderNode->getNodeName() << "," << roadRegionId;
						}

					}
					file << endl;
				}
				pch = strtok(NULL, delim);
			}
		}
	}
}

void skylineModel::dijkstra(string inputDir, string outputDir)
{
	//重讀檔案以找出Border Node
	char line[SIZE];

	fstream fin1;
	outputDir = outputDir + ".bntable";
	fin1.open(inputDir, ios::in);


	int lineNumber = 0;
	char *pch;
	char *delim = ",";
	int nodeId = 0, borderId = 0;
	vector<string> nodeList;
	vector<EData*> edgesList;
	//vector<Region*> regionList;
	//vector<BorderNode*> borderNodeList;
	vector<double> partitionLongtitudeList;
	vector<double> partitionLatitudeList;
	int totalNode = 0, totalBorderNode = 0;

	//cout << "read file " << inputDir << endl;

	while (fin1.getline(line, sizeof(line), '\n'))
	{
		pch = strtok(line, delim);
		int attributeFlag = 0;
		string type;
		string partitionType;
		int part_x, part_y;
		int roadRegionId;
		string longtitude, latitude,nodeName1,nodeName2;
		double dist;
		lineNumber++;
		//cout << "lineNumber " << lineNumber << endl;

		if (lineNumber>=4)
		{
			while (pch != NULL)
			{
				attributeFlag++;
				if (attributeFlag == 1)
				{
					//road id
				}
				if (attributeFlag == 2)
				{
					//turning point
				}
				if (attributeFlag == 3)
				{
					//start node type & end point type
					type = pch;
				}
				if (attributeFlag == 4)
				{
					//distance
					dist = strtod(pch, NULL);
				}
				if (attributeFlag == 5)
				{
					//start point x
					longtitude = pch;
				}
				if (attributeFlag == 6)
				{
					//start point y
					latitude = pch;
				}
				if (attributeFlag == 7)
				{
					nodeName1 = pch;
					bool nodeExistFlag = false;
					for (int index = 0; index < nodeList.size(); index++)
					{
						if (nodeList.at(index) == nodeName1)
						{
							nodeExistFlag = true;
							break;
						}
					}
					if (!nodeExistFlag)
					{
						nodeList.push_back(nodeName1);
					}
				}
				if (attributeFlag == 8)
				{
					//end point y
					longtitude =pch;
				}
				if (attributeFlag == 9)
				{
					//end point y
					latitude = pch;
				}
				if (attributeFlag == 10)
				{
					nodeName2 = pch;
					bool nodeExistFlag = false;
					for (int index = 0; index < nodeList.size(); index++)
					{
						if (nodeList.at(index) == nodeName2)
						{
							nodeExistFlag = true;
							break;
						}
					}
					if (!nodeExistFlag)
					{
						nodeList.push_back(nodeName2);
					}
				}
				if (attributeFlag == 11)
				{
					//end point y
					roadRegionId = atoi(pch);
				}
				pch = strtok(NULL, delim);
			}
			if (nodeName1!="")
			{
				edgesList.push_back(new EData(nodeName1, nodeName2, dist));
			}
		}

	}

	int prev[MAX] = { 0 };
	int dist[MAX] = { 0 };

	int vlen = nodeList.size();
	
	int elen = edgesList.size();
	//cout << vlen << " " << elen;
	ListUDG* pG;

	// 自定义"图"(输入矩阵队列)
	//pG = new ListUDG();
	// 采用已有的"图"
	ofstream file;
	file.open(outputDir, ios::trunc | ios::out);
	file.close();
	pG = new ListUDG(nodeList, vlen, edgesList, elen);
	for (int i = 0; i < vlen; i++)
	{
		pG->dijkstra(i, prev, dist, outputDir);
	}
}


vector<Region*> skylineModel::getRegionListByBound(vector<double> partitionLongtitudeList, vector<double> partitionLatitudeList)
{
	vector<Region*> regionList;
	int regionId = 0;
	for (int i = 1; i < partitionLongtitudeList.size(); i++)
	{
		for (int j = 1; j < partitionLatitudeList.size(); j++)
		{
			Region* gridRegion = new Region(regionId, partitionLongtitudeList.at(i - 1), partitionLongtitudeList.at(i), partitionLatitudeList.at(j - 1), partitionLatitudeList.at(j));
			regionList.push_back(gridRegion);
			regionId++;
		}
	}

	return regionList;
}

int skylineModel::checkNodeRegion(vector<Region*> regionList, Node* checkNode)
{
	//get Node Regions
	for (int i = 0; i < regionList.size(); i++)
	{
		if (checkNode->getLongtitude() >= regionList.at(i)->getLongtitudeLeft() && checkNode->getLongtitude() <= regionList.at(i)->getLongtitudeRight() && checkNode->getLatitude() >= regionList.at(i)->getLatitudeBottom() && checkNode->getLatitude() <= regionList.at(i)->getLatitudeTop())
		{
			return i;
		}
	}

	return -1;
}

void skylineModel::writeBorderList(string inputDir, string outputDir)
{
	char line[SIZE];

	fstream fin;
	ofstream file;
	outputDir = outputDir + ".blist";

	int lineNumber = 0;
	char *pch;
	char *delim = ",";
	vector<BorderNode*> borderNodeList;
	int xPart, yPart, regionNum;

	//clean prev .blist
	file.open(outputDir, ios::trunc | ios::out);
	file.close();

	//get regionNum from .bn
	fin.open(inputDir, ios::in);
	while (fin.getline(line, sizeof(line), '\n'))
	{
		lineNumber++;
		int attributeFlag = 0;
		pch = strtok(line, delim);

		string str1 = "";
		str1.append(pch);
		if (str1.find("b_", 0) != string::npos)
		{
			BorderNode* regBorderNode = new BorderNode();
			while (pch != NULL)
			{
				attributeFlag++;
				if (attributeFlag == 1)
					regBorderNode->setNodeName(pch);
				else if (attributeFlag == 2)
					regBorderNode->setLongtitude(strtod(pch, NULL));
				else if (attributeFlag == 3)
					regBorderNode->setLatitude(strtod(pch, NULL));
				else if (attributeFlag == 4)
					regBorderNode->setNodeRegionId(atoi(pch));
				pch = strtok(NULL, delim);
			}
			borderNodeList.push_back(regBorderNode);
		}
	}
	fin.close();

	file.open(outputDir, ios::out);
	int regionIdCounter = 0;
	for (int i = 0; i < borderNodeList.size(); i++)
	{
		while (regionIdCounter < borderNodeList.at(i)->getNodeRegionId())
		{
			file << endl;
			regionIdCounter++;
		}
		file << setprecision(9) << borderNodeList.at(i)->getNodeName() << ",";
		file << setprecision(9) << borderNodeList.at(i)->getLongtitude() << ",";
		file << setprecision(9) << borderNodeList.at(i)->getLatitude();
		if (i != borderNodeList.size() - 1 && borderNodeList.at(i)->getNodeRegionId() == borderNodeList.at(i + 1)->getNodeRegionId())
			file << ",";
		else if (i == borderNodeList.size() - 1 && regionIdCounter != borderNodeList.size() - 1)
			file << endl;
	}
	file.close();

	//deleteFileLastLine(outputDir, outputDir);
}

void skylineModel::deleteFileLastLine(string inputDir, string outputDir)
{
	vector<string> tmp_files;

	fstream file;

	file.open(inputDir, ios::in);
	string lineContent;
	int lineNumber = 0;
	while (getline(file, lineContent, '\n'))
	{
		lineNumber++;
	}
	cout << "lineNum : " << lineNumber << endl;
	file.close();
	file.open(inputDir, ios::in);
	while (getline(file, lineContent, '\n'))
	{
		tmp_files.push_back(lineContent + "\n");
	}
	file.close();

	ofstream outfile(outputDir, ios::out);
	vector<string>::iterator siter = tmp_files.begin();

	copy(tmp_files.begin(), tmp_files.end(), ostream_iterator<string>(outfile));
	cout << "ok!" << endl;
	outfile.close();
}

void skylineModel::writeBNAndNSDist(string inputDir1, string inputDir2, string inputDir3, string inputDir4, string outputDir, int processCounter)
{
	fstream fin1, fin2, fin3, fin4, file2;
	fin1.open(inputDir1, ios::in);
	fin3.open(inputDir1, ios::in);
	fin2.open(inputDir2, ios::in);
	fin4.open(inputDir3, ios::in);


	int lineNumber3 = 0;
	char *pch;
	char *delim = ",";


	//vector<string> detectRegion;
	vector<Node*>nodeList;


	char line[SIZE];
	int regionNum;
	int lineNumber = 0;
	int nodeId = 0, borderId = 0;
	
	int processStep;
	file2.open(inputDir4, ios::in);
	while (file2.getline(line, sizeof(line), '\n'))
	{
		pch = strtok(line, delim);
		processStep = atoi(pch);
		break;
	}
	file2.close();

	//count all region number
	while (fin2.getline(line, sizeof(line), '\n'))
	{
		lineNumber++;
		pch = strtok(line, delim);
		int attributeFlag = 0;

		if (lineNumber == 1)
		{
			while (pch != NULL)
			{
				attributeFlag++;
				if (attributeFlag == 2)
					regionNum = atoi(pch);
				if (attributeFlag == 3)
					regionNum = regionNum * atoi(pch);

				pch = strtok(NULL, delim);
			}
			break;
		}
	}
	fin2.close();
	lineNumber = 0;
	fin2.open(inputDir2, ios::in);

	//get all spots
	vector<string> spotNameList;
	char line1[300];
	char* pch1;
	while (fin4.getline(line1, sizeof(line1), '\n'))
	{
		string line3 = line1;
		pch1 = strtok(line1, delim);
		spotNameList.push_back(pch1);
	}

	//.objreg get node region
	while (fin2.getline(line, sizeof(line), '\n'))
	{
		lineNumber++;
		pch = strtok(line, delim);
		int attributeFlag = 0;

		if (lineNumber > 3)
		{
			Node* regNode = new Node();
			while (pch != NULL)
			{
				attributeFlag++;
				if (attributeFlag == 1)
				{
					regNode->setNodeName(pch);
					nodeList.push_back(regNode);
					for (int j = 0; j < spotNameList.size(); j++)
					{
						if (regNode->getNodeName() == spotNameList.at(j))
						{
							regNode->setIsSpot(true);
						}
					}
				}
				else if (attributeFlag == 4)
				{
					regNode->setNodeRegionId(atoi(pch));
				}
				pch = strtok(NULL, delim);
			}
		}
	}
	for (int i = processCounter; i < regionNum; i++)
	{
		vector<int> pos;
		int lineNumber = 0;
		fstream finput1, finput2, file;
		finput1.open(inputDir1, ios::in);
		finput2.open(inputDir1, ios::in);
		file.open(outputDir + ".bnns" + to_string(i), ios::out);
		while (finput1.getline(line, sizeof(line), '\n'))
		{
			string line2 = line;

			lineNumber++;

			for (int index = 0; index < nodeList.size(); index++)
			{
				if (i == nodeList.at(index)->getNodeRegionId() && line2.find(nodeList.at(index)->getNodeName() + ",", 0) < 10)
				{
					if (line2.find("b", 0) < 10)
					{
						break;
					}
					pos.push_back(lineNumber);
					break;
				}

			}
		}
		cout << "region:" << i << " pos size:" << pos.size() << endl;
		if (pos.size() == 0)
		{
			file2.open(inputDir4, ios::out);
			file2 << processStep << endl;
			if (i != regionNum - 1)
				file2 << "bnns," << i + 1 << endl;
			file2.close();
			continue;
		}

		vector<string> nodeNameList;

		for (int j = 0; j < nodeList.size(); j++)
		{
			if (nodeList.at(j)->getNodeRegionId() == i && nodeList.at(j)->getNodeName().find("n_", 0) < 10)
			{
				nodeNameList.push_back(nodeList.at(j)->getNodeName());
			}
		}


		for (int j = 0; j < nodeNameList.size(); j++)
		{
			file << nodeNameList.at(j);
			if (nodeNameList.size() - 1 != j)
			{
				file << ",";
			}
		}

		file << endl;

		while (finput2.getline(line, sizeof(line), '\n'))
		{
			int test = 0;
			string line2 = line;
			pch = strtok(line, delim);
			int attributeFlag = 0;
			lineNumber++;


			for (int j = 0; j < nodeList.size(); j++)
			{

				if (nodeList.at(j)->getNodeName().find("n_", 0) < 5 && nodeList.at(j)->getIsSpot() == false)
				{
					continue;
				}
				if (line2.find(nodeList.at(j)->getNodeName() + ",", 0) < 10 && nodeList.at(j)->getNodeRegionId() == i)
				{
					file << pch << ",";
					while (pch != NULL)
					{
						attributeFlag++;
						for (int index = 0; index < pos.size(); index++)
						{
							if (attributeFlag - 1 == pos.at(index))
							{
								file << pch;
								if (index != pos.size() - 1)
									file << ",";
							}
						}
						pch = strtok(NULL, delim);
					}
					file << endl;
				}
			}
		}
		file2.open(inputDir4, ios::out);
		file2 << processStep << endl;
		if (i != regionNum - 1)
			file2 << "bnns," << i + 1 << endl;
		file2.close();
	}
}
