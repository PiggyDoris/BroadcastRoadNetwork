// ConsoleApplication1.cpp : 定義主控台應用程式的進入點。
//

#include "stdafx.h"
#include "skylineModel.h"


int _tmain(int argc, _TCHAR* argv[])
{
	int option = 3;
	skylineModel* model = new skylineModel;
	while (option)
	{
		cout << endl;
		cout << "---------------------------------------------" << endl;
		cout << "＝＝＝kNN Search - Border Approximation(BA) Method＝＝＝" << endl << endl;
		cout << "Please choose your option!"<< endl;
		cout << "1. Start skyline computing." << endl;
		cout << "2. Load file." << endl;
		cout << "3. Write TWN_LANDMARK.txt." << endl;
		cout << "4. Write CITY_ROAD.txt." << endl;
		cout << "5. find NonSpatialSkyline" << endl;
		cout << "6. find findIntersection" << endl; 
		cout << "7. write roadOriginDistance" << endl;
		cout << "8. write GridPartition" << endl;
		cout << "9. write roadBorderDistance" << endl;
		cout << "10. write SpotFile" << endl;
		cout << endl << "11. Precomputation & kNN Search(BA & NPI)   <RECOMMENDED!!!!!>" << endl << endl;
		cout << endl << "12. dijkstra" << endl;
		cout << "0. Exit."<<endl;
		cout << "---------------------------------------------" << endl;
		cout << "option:";
		cin >> option;
		switch (option)
		{
		case 1:
			model->display();
			break;
		case 2:
			model->loadFile();
			break;		
		case 3:
			model->writeLandmarkToMBRFile();
			break;
		case 4:
			model->writeRoadToMBRFile();
			break;
		case 5:
			model->findNonSpatialSkyline();
			break;
		case 6:
			model->findIntersection();
			break;
		case 7:
			model->writeAllOriginRoadDistance();
			break;
		//case 8:
		//	model->writeGridPartition();
		//	break;
		//case 9:
		//	model->writeAllBorderRoadDistance();
		//	break;
		//case 10:
		//	model->randomViewPoints();
		//	break;
		case 11:
			model->precomputationIntegrate();
			break;
		case 13:
			model->precomputationIntegrate_RNN();
			break;
		default:
			break;
		}
	}
	system("pause");
	return 0;
}