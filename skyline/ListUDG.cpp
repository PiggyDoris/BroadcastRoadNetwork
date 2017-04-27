#include "stdafx.h"
#include "ListUDG.h"
#include <iomanip>
#include <iostream>
#include <vector>
#include <fstream>
using namespace std;

/*
* 创建邻接表对应的图(自己输入)
*/
ListUDG::ListUDG()
{
	string c1, c2;
	int v, e;
	int i, p1, p2;
	int weight;
	ENode *node1, *node2;

	// 输入"顶点数"和"边数"
	cout << "input vertex number: ";
	cin >> mVexNum;
	cout << "input edge number: ";
	cin >> mEdgNum;
	if (mVexNum < 1 || mEdgNum < 1 || (mEdgNum >(mVexNum * (mVexNum - 1))))
	{
		cout << "input error: invalid parameters!" << endl;
		return;
	}

	// 初始化"邻接表"的顶点
	for (i = 0; i < mVexNum; i++)
	{
		cout << "vertex(" << i << "): ";
		mVexs[i].data = readChar();
		mVexs[i].firstEdge = NULL;
	}

	// 初始化"邻接表"的边
	for (i = 0; i < mEdgNum; i++)
	{
		// 读取边的起始顶点和结束顶点
		cout << "edge(" << i << "): ";
		c1 = readChar();
		c2 = readChar();
		cin >> weight;

		p1 = getPosition(c1);
		p2 = getPosition(c2);
		// 初始化node1
		node1 = new ENode();
		node1->ivex = p2;
		node1->weight = weight;
		// 将node1链接到"p1所在链表的末尾"
		if (mVexs[p1].firstEdge == NULL)
			mVexs[p1].firstEdge = node1;
		else
			linkLast(mVexs[p1].firstEdge, node1);
		// 初始化node2
		node2 = new ENode();
		node2->ivex = p1;
		node2->weight = weight;
		// 将node2链接到"p2所在链表的末尾"
		if (mVexs[p2].firstEdge == NULL)
			mVexs[p2].firstEdge = node2;
		else
			linkLast(mVexs[p2].firstEdge, node2);
	}
}

/*
* 创建邻接表对应的图(用已提供的数据)
*/
ListUDG::ListUDG(vector<string> vexsList, int vlen, vector<EData*> edgesList, int elen)
{
	string c1, c2;
	int i, p1, p2;
	int weight;
	ENode *node1, *node2;

	// 初始化"顶点数"和"边数"
	mVexNum = vlen;
	mEdgNum = elen;
	// 初始化"邻接表"的顶点
	for (i = 0; i < mVexNum; i++)
	{
		mVexs[i].data = vexsList.at(i);
		//cout << i<<":" << vexsList.at(i) << endl;
		mVexs[i].firstEdge = NULL;
		if (i == 9998)
		{
			cout << "cc" << endl;
		}
	}

	// 初始化"邻接表"的边
	for (i = 0; i < mEdgNum; i++)
	{
		// 读取边的起始顶点和结束顶点
		c1 = edgesList.at(i)->start;
		c2 = edgesList.at(i)->end;
		weight = edgesList.at(i)->weight;

		p1 = getPosition(c1);
		p2 = getPosition(c2);
		// 初始化node1
		node1 = new ENode();
		node1->ivex = p2;
		node1->weight = weight;
		// 将node1链接到"p1所在链表的末尾"
		if (mVexs[p1].firstEdge == NULL)
			mVexs[p1].firstEdge = node1;
		else
			linkLast(mVexs[p1].firstEdge, node1);
		// 初始化node2
		node2 = new ENode();
		node2->ivex = p1;
		node2->weight = weight;
		// 将node2链接到"p2所在链表的末尾"
		if (mVexs[p2].firstEdge == NULL)
			mVexs[p2].firstEdge = node2;
		else
			linkLast(mVexs[p2].firstEdge, node2);
	}
}

/*
* 析构函数
*/
ListUDG::~ListUDG()
{
}

void ListUDG::linkLast(ENode *list, ENode *node)
{
	ENode *p = list;

	while (p->nextEdge)
		p = p->nextEdge;
	p->nextEdge = node;
}

/*
* 返回ch的位置
*/
int ListUDG::getPosition(string ch)
{
	int i;
	for (i = 0; i<mVexNum; i++)
	if (mVexs[i].data == ch)
		return i;
	return -1;
}

/*
* 读取一个输入字符
*/
string ListUDG::readChar()
{
	string ch;
	cin >> ch;
	return ch;
}


/*
* 深度优先搜索遍历图的递归实现
*/
void ListUDG::DFS(int i, int *visited)
{
	ENode *node;

	visited[i] = 1;
	cout << mVexs[i].data << " ";
	node = mVexs[i].firstEdge;
	while (node != NULL)
	{
		if (!visited[node->ivex])
			DFS(node->ivex, visited);
		node = node->nextEdge;
	}
}

/*
* 深度优先搜索遍历图
*/
void ListUDG::DFS()
{
	int i;
	int visited[MAX];       // 顶点访问标记

	// 初始化所有顶点都没有被访问
	for (i = 0; i < mVexNum; i++)
		visited[i] = 0;

	cout << "DFS: ";
	for (i = 0; i < mVexNum; i++)
	{
		if (!visited[i])
			DFS(i, visited);
	}
	cout << endl;
}

/*
* 广度优先搜索（类似于树的层次遍历）
*/
void ListUDG::BFS()
{
	int head = 0;
	int rear = 0;
	int queue[MAX];     // 辅组队列
	int visited[MAX];   // 顶点访问标记
	int i, j, k;
	ENode *node;

	for (i = 0; i < mVexNum; i++)
		visited[i] = 0;

	cout << "BFS: ";
	for (i = 0; i < mVexNum; i++)
	{
		if (!visited[i])
		{
			visited[i] = 1;
			cout << mVexs[i].data << " ";
			queue[rear++] = i;  // 入队列
		}
		while (head != rear)
		{
			j = queue[head++];  // 出队列
			node = mVexs[j].firstEdge;
			while (node != NULL)
			{
				k = node->ivex;
				if (!visited[k])
				{
					visited[k] = 1;
					cout << mVexs[k].data << " ";
					queue[rear++] = k;
				}
				node = node->nextEdge;
			}
		}
	}
	cout << endl;
}

/*
* 打印邻接表图
*/
void ListUDG::print()
{
	int i, j;
	ENode *node;

	cout << "List Graph:" << endl;
	for (i = 0; i < mVexNum; i++)
	{
		cout << i << "(" << mVexs[i].data << "): ";
		node = mVexs[i].firstEdge;
		while (node != NULL)
		{
			cout << node->ivex << "(" << mVexs[node->ivex].data << ") ";
			node = node->nextEdge;
		}
		cout << endl;
	}
}

/*
* 获取边<start, end>的权值；若start和end不是连通的，则返回无穷大。
*/
int ListUDG::getWeight(int start, int end)
{
	ENode *node;

	if (start == end)
		return 0;

	node = mVexs[start].firstEdge;
	while (node != NULL)
	{
		if (end == node->ivex)
			return node->weight;
		node = node->nextEdge;
	}

	return INF;
}

/*
* prim最小生成树
*
* 参数说明：
*   start -- 从图中的第start个元素开始，生成最小树
*/
void ListUDG::prim(int start)
{
	int min, i, j, k, m, n, tmp, sum;
	int index = 0;         // prim最小树的索引，即prims数组的索引
	string prims[MAX];     // prim最小树的结果数组
	int weights[MAX];    // 顶点间边的权值

	// prim最小生成树中第一个数是"图中第start个顶点"，因为是从start开始的。
	prims[index++] = mVexs[start].data;

	// 初始化"顶点的权值数组"，
	// 将每个顶点的权值初始化为"第start个顶点"到"该顶点"的权值。
	for (i = 0; i < mVexNum; i++)
		weights[i] = getWeight(start, i);

	for (i = 0; i < mVexNum; i++)
	{
		// 由于从start开始的，因此不需要再对第start个顶点进行处理。
		if (start == i)
			continue;

		j = 0;
		k = 0;
		min = INF;
		// 在未被加入到最小生成树的顶点中，找出权值最小的顶点。
		while (j < mVexNum)
		{
			// 若weights[j]=0，意味着"第j个节点已经被排序过"(或者说已经加入了最小生成树中)。
			if (weights[j] != 0 && weights[j] < min)
			{
				min = weights[j];
				k = j;
			}
			j++;
		}

		// 经过上面的处理后，在未被加入到最小生成树的顶点中，权值最小的顶点是第k个顶点。
		// 将第k个顶点加入到最小生成树的结果数组中
		prims[index++] = mVexs[k].data;
		// 将"第k个顶点的权值"标记为0，意味着第k个顶点已经排序过了(或者说已经加入了最小树结果中)。
		weights[k] = 0;
		// 当第k个顶点被加入到最小生成树的结果数组中之后，更新其它顶点的权值。
		for (j = 0; j < mVexNum; j++)
		{
			// 获取第k个顶点到第j个顶点的权值
			tmp = getWeight(k, j);
			// 当第j个节点没有被处理，并且需要更新时才被更新。
			if (weights[j] != 0 && tmp < weights[j])
				weights[j] = tmp;
		}
	}

	// 计算最小生成树的权值
	sum = 0;
	for (i = 1; i < index; i++)
	{
		min = INF;
		// 获取prims[i]在矩阵表中的位置
		n = getPosition(prims[i]);
		// 在vexs[0...i]中，找出到j的权值最小的顶点。
		for (j = 0; j < i; j++)
		{
			m = getPosition(prims[j]);
			tmp = getWeight(m, n);
			if (tmp < min)
				min = tmp;
		}
		sum += min;
	}
	// 打印最小生成树
	cout << "PRIM(" << mVexs[start].data << "frown表情符號" << sum << ": ";
	for (i = 0; i < index; i++)
		cout << prims[i] << " ";
	cout << endl;
}

/*
* 获取图中的边
*/
vector<EData*> ListUDG::getEdges()
{
	int i, j;
	int index = 0;
	ENode *node;
	EData *edges;
	vector<EData*> edgeList;
	edges = new EData[mEdgNum];
	for (i = 0; i < mVexNum; i++)
	{
		node = mVexs[i].firstEdge;
		while (node != NULL)
		{
			if (node->ivex > i)
			{
				EData *edge = new EData(mVexs[i].data, mVexs[node->ivex].data, node->weight);
				edgeList.push_back(edge);
				//edgeList[index].start = mVexs[i].data;           // 起点
				//edges[index].end = mVexs[node->ivex].data;  // 终点
				//edges[index].weight = node->weight;            // 权
				index++;
			}
			node = node->nextEdge;
		}
	}

	//return edges;
	return edgeList;
}

/*
* 对边按照权值大小进行排序(由小到大)
*/
void ListUDG::sortEdges(vector<EData*> edgesList, int elen)
{
	int i, j;

	for (i = 0; i<elen; i++)
	{
		for (j = i + 1; j<elen; j++)
		{
			if (edgesList.at(i)->weight > edgesList.at(j)->weight)
			{
				// 交换"边i"和"边j"
				swap(edgesList.at(i), edgesList.at(j));
			}
		}
	}
}

/*
* 获取i的终点
*/
int ListUDG::getEnd(int vends[], int i)
{
	while (vends[i] != 0)
		i = vends[i];
	return i;
}

/*
* 克鲁斯卡尔（Kruskal)最小生成树
*/
void ListUDG::kruskal()
{
	int i, m, n, p1, p2;
	int length;
	int index = 0;          // rets数组的索引
	int vends[MAX] = { 0 };     // 用于保存"已有最小生成树"中每个顶点在该最小树中的终点。
	EData* rets[MAX];        // 结果数组，保存kruskal最小生成树的边
//	vector<EData*> edgesList;
	EData *edges;           // 图对应的所有边
	vector<EData*> edgesList;
	// 获取"图中所有的边"
	edgesList = getEdges();
	// 将边按照"权"的大小进行排序(从小到大)
	sortEdges(edgesList, mEdgNum);

	for (i = 0; i < mEdgNum; i++)
	{
		p1 = getPosition(edgesList.at(i)->start);      // 获取第i条边的"起点"的序号
		p2 = getPosition(edgesList.at(i)->end);        // 获取第i条边的"终点"的序号

		m = getEnd(vends, p1);                 // 获取p1在"已有的最小生成树"中的终点
		n = getEnd(vends, p2);                 // 获取p2在"已有的最小生成树"中的终点
		// 如果m!=n，意味着"边i"与"已经添加到最小生成树中的顶点"没有形成环路
		if (m != n)
		{
			vends[m] = n;                       // 设置m在"已有的最小生成树"中的终点为n
			rets[index++] = edgesList.at(i);           // 保存结果
		}
	}
//	delete[] edges;

	// 统计并打印"kruskal最小生成树"的信息
	length = 0;
	for (i = 0; i < index; i++)
		length += rets[i]->weight;
	cout << "Kruskal=" << length << ": ";
	for (i = 0; i < index; i++)
		cout << "(" << rets[i]->start << "," << rets[i]->end << ") ";
	cout << endl;
}

/*
* Dijkstra最短路径。
* 即，统计图中"顶点vs"到其它各个顶点的最短路径。
*
* 参数说明：
*       vs -- 起始顶点(start vertex)。即计算"顶点vs"到其它顶点的最短路径。
*     prev -- 前驱顶点数组。即，prev[i]的值是"顶点vs"到"顶点i"的最短路径所经历的全部顶点中，位于"顶点i"之前的那个顶点。
*     dist -- 长度数组。即，dist[i]是"顶点vs"到"顶点i"的最短路径的长度。
*/
void ListUDG::dijkstra(int vs, int prev[], int dist[],string outputDir)
{
	ofstream file;
	file.open(outputDir, ios::out|ios::app);

	int i, j, k;
	int min;
	int tmp;
	int flag[MAX];      // flag[i]=1表示"顶点vs"到"顶点i"的最短路径已成功获取。

	// 初始化
	for (i = 0; i < mVexNum; i++)
	{
		flag[i] = 0;                // 顶点i的最短路径还没获取到。
		prev[i] = 0;                // 顶点i的前驱顶点为0。
		dist[i] = getWeight(vs, i);  // 顶点i的最短路径为"顶点vs"到"顶点i"的权。
	}

	// 对"顶点vs"自身进行初始化
	flag[vs] = 1;
	dist[vs] = 0;

	// 遍历mVexNum-1次；每次找出一个顶点的最短路径。
	for (i = 1; i < mVexNum; i++)
	{
		// 寻找当前最小的路径；
		// 即，在未获取最短路径的顶点中，找到离vs最近的顶点(k)。
		min = INF;
		for (j = 0; j < mVexNum; j++)
		{
			if (flag[j] == 0 && dist[j] < min)
			{
				min = dist[j];
				k = j;
			}
		}
		// 标记"顶点k"为已经获取到最短路径
		flag[k] = 1;

		// 修正当前最短路径和前驱顶点
		// 即，当已经"顶点k的最短路径"之后，更新"未获取最短路径的顶点的最短路径和前驱顶点"。
		for (j = 0; j < mVexNum; j++)
		{
			tmp = getWeight(k, j);
			tmp = (tmp == INF ? INF : (min + tmp)); // 防止溢出
			if (flag[j] == 0 && (tmp < dist[j]))
			{
				dist[j] = tmp;
				prev[j] = k;
			}
		}
	}

	// 打印dijkstra最短路径的结果
	//cout << "dijkstra(" << mVexs[vs].data << "): " << endl;
	file << mVexs[vs].data << ",";
	for (i = 0; i < mVexNum; i++)
	{
//		cout << "  shortest(" << mVexs[vs].data << ", " << mVexs[i].data << ")=" << dist[i] << endl;
		if (i != mVexNum-1)
		{
			file << dist[i] << ",";
		}
		else
			file << dist[i] << endl;
	}
}

int ListUDG::dijkstra(int vs, int prev[], int dist[], int bNNAmount)
{
	int i, j, k;
	int min;
	int tmp;
	int flag[MAX];      // flag[i]=1表示"顶点vs"到"顶点i"的最短路径已成功获取。

	// 初始化
	for (i = 0; i < mVexNum; i++)
	{
		flag[i] = 0;                // 顶点i的最短路径还没获取到。
		prev[i] = 0;                // 顶点i的前驱顶点为0。
		dist[i] = getWeight(vs, i);  // 顶点i的最短路径为"顶点vs"到"顶点i"的权。
	}

	// 对"顶点vs"自身进行初始化
	flag[vs] = 1;
	dist[vs] = 0;

	// 遍历mVexNum-1次；每次找出一个顶点的最短路径。
	for (i = 1; i < mVexNum; i++)
	{
		// 寻找当前最小的路径；
		// 即，在未获取最短路径的顶点中，找到离vs最近的顶点(k)。
		min = INF;
		for (j = 0; j < mVexNum; j++)
		{
			if (flag[j] == 0 && dist[j] < min)
			{
				min = dist[j];
				k = j;
			}
		}
		// 标记"顶点k"为已经获取到最短路径
		flag[k] = 1;

		// 修正当前最短路径和前驱顶点
		// 即，当已经"顶点k的最短路径"之后，更新"未获取最短路径的顶点的最短路径和前驱顶点"。
		for (j = 0; j < mVexNum; j++)
		{
			tmp = getWeight(k, j);
			tmp = (tmp == INF ? INF : (min + tmp)); // 防止溢出
			if (flag[j] == 0 && (tmp < dist[j]))
			{
				dist[j] = tmp;
				prev[j] = k;
			}
		}
	}

	// 打印dijkstra最短路径的结果
	//cout << "dijkstra(" << mVexs[vs].data << "): " << endl;
	// 	file << mVexs[vs].data << ",";
	// 	for (i = 0; i < mVexNum; i++)
	// 	{
	// 		cout << "  shortest(" << mVexs[vs].data << ", " << mVexs[i].data << "frown表情符號" << dist[i] << endl;
	// // 		if (i != mVexNum - 1)
	// // 		{
	// // 			file << dist[i] << ",";
	// // 		}
	// // 		else
	// // 			file << dist[i] << endl;
	// 	}
	int minQueryBNNDist = -1;
	for (int i = mVexNum - bNNAmount; i < mVexNum; i++)
	{
		if (minQueryBNNDist == -1 || minQueryBNNDist > dist[i])
			minQueryBNNDist = dist[i];
	}

	//cout << "(" << mVexs[vs].data << "," << mVexs[mVexNum - 1].data << ") " << dist[mVexNum - 1] << endl;
	return minQueryBNNDist;
}

vector<int> ListUDG::bottomDijkstra(int vs, int prev[], int dist[], int bNNAmount)
{
	int i, j, k;
	int min;
	int tmp;
	int flag[MAX];      // flag[i]=1表示"顶点vs"到"顶点i"的最短路径已成功获取。
	vector<int> bottomDijkstraList;

	// 初始化
	for (i = 0; i < mVexNum; i++)
	{
		flag[i] = 0;                // 顶点i的最短路径还没获取到。
		prev[i] = 0;                // 顶点i的前驱顶点为0。
		dist[i] = getWeight(vs, i);  // 顶点i的最短路径为"顶点vs"到"顶点i"的权。
	}

	// 对"顶点vs"自身进行初始化
	flag[vs] = 1;
	dist[vs] = 0;

	// 遍历mVexNum-1次；每次找出一个顶点的最短路径。
	for (i = 1; i < mVexNum; i++)
	{
		// 寻找当前最小的路径；
		// 即，在未获取最短路径的顶点中，找到离vs最近的顶点(k)。
		min = INF;
		for (j = 0; j < mVexNum; j++)
		{
			if (flag[j] == 0 && dist[j] < min)
			{
				min = dist[j];
				k = j;
			}
		}
		// 标记"顶点k"为已经获取到最短路径
		flag[k] = 1;

		// 修正当前最短路径和前驱顶点
		// 即，当已经"顶点k的最短路径"之后，更新"未获取最短路径的顶点的最短路径和前驱顶点"。
		for (j = 0; j < mVexNum; j++)
		{
			tmp = getWeight(k, j);
			tmp = (tmp == INF ? INF : (min + tmp)); // 防止溢出
			if (flag[j] == 0 && (tmp < dist[j]))
			{
				dist[j] = tmp;
				prev[j] = k;
			}
		}
	}

	// 打印dijkstra最短路径的结果
	int minQueryBNNDist = -1;
	for (int i = mVexNum - bNNAmount; i < mVexNum; i++)
	{
		bottomDijkstraList.push_back(dist[i]);
	}

	//cout << "(" << mVexs[vs].data << "," << mVexs[mVexNum - 1].data << ") " << dist[mVexNum - 1] << endl;
	return bottomDijkstraList;
}