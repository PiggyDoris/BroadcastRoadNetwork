#pragma once

#include "stdafx.h"
#include "ListUDG.h"
#include "EData.h"
#include <iomanip>
#include <iostream>
#include <vector>

// 邻接表
class ListUDG
{
#define MAX 20000
#define INF         (~(0x1<<31))        // 最大值(即0X7FFFFFFF)
private: // 内部类
	// 邻接表中表对应的链表的顶点
	class ENode
	{
		int ivex;           // 该边所指向的顶点的位置
		int weight;         // 该边的权
		ENode *nextEdge;    // 指向下一条弧的指针
		friend class ListUDG;
	};

	// 邻接表中表的顶点
	class VNode
	{
		string data;          // 顶点信息
		ENode *firstEdge;   // 指向第一条依附该顶点的弧
		friend class ListUDG;
	};

private: // 私有成员
	int mVexNum;             // 图的顶点的数目
	int mEdgNum;             // 图的边的数目
	VNode mVexs[MAX];

public:
	// 创建邻接表对应的图(自己输入)
	ListUDG();
	// 创建邻接表对应的图(用已提供的数据)
	ListUDG(vector<string> vexs, int vlen, vector<EData*> edgesList, int elen);
	~ListUDG();

	// 深度优先搜索遍历图
	void DFS();
	// 广度优先搜索（类似于树的层次遍历）
	void BFS();
	// 打印邻接表图
	void print();
	// prim最小生成树
	void prim(int start);
	// 克鲁斯卡尔（Kruskal)最小生成树
	void kruskal();
	// Dijkstra最短路径
	void dijkstra(int vs, int vexs[], int dist[],string outputDir);
	int dijkstra(int vs, int prev[], int dist[], int);
	vector<int> bottomDijkstra(int vs, int prev[], int dist[], int);

private:
	// 读取一个输入字符
	string readChar();
	// 返回ch的位置
	int getPosition(string ch);
	// 深度优先搜索遍历图的递归实现
	void DFS(int i, int *visited);
	// 将node节点链接到list的最后
	void linkLast(ENode *list, ENode *node);
	// 获取边<start, end>的权值；若start和end不是连通的，则返回无穷大。
	int getWeight(int start, int end);
	// 获取图中的边
	vector<EData*> getEdges();
	// 对边按照权值大小进行排序(由小到大)
	void sortEdges(vector<EData*> edgesList, int elen);
	// 获取i的终点
	int getEnd(int vends[], int i);
};