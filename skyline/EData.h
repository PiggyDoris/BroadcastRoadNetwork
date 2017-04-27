#pragma once
// 示例类：边的结构体(用来演示)
#include <string>
#include <iostream>
using namespace std;
class EData
{
public:
	string start; // 边的起点
	string end;   // 边的终点
	double weight; // 边的权重

public:
	EData(){}
	EData(string s, string e, double w) :start(s), end(e), weight(w){}
};