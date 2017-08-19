#include <iostream>
#include <cstdlib>
#include "Node.h"
#include "Region.h"

class TreeNode{
private:
	TreeNode* leftchild;
	TreeNode* rightchild;
	TreeNode* parent;
	Node* leafSpot;
	char treeAxis=' ';
	double treeData;
	int regionNum = 0;
	bool isleaf = false;
	Region* leafRegion;
	int i = 0;
public:
	TreeNode() :leftchild(0), rightchild(0), parent(0), treeData(0){};
	//TreeNode(char s) :leftchild(0), rightchild(0), parent(0), data(s){};
	void setRegionNum(int);
	int getRegionNum();
	void setTreeNodeData(double);
	double getTreeNodeData();
	void setTreeNodeLeftChild(TreeNode*);
	TreeNode* getTreeNodeLeftChild();
	void setTreeNodeRightChild(TreeNode*);
	TreeNode* getTreeNodeRightChild();
	void setTreeNodeParent(TreeNode*);
	TreeNode* getTreeNodeParent();
	void setTreeNodeIsLeaf(bool);
	bool getTreeNodeIsLeaf();
	void setTreeLeafData(Node*);
	Node* getTreeLeafData();
	void setTreeNodeAxis(char);
	char getTreeNodeAxis();
	void setLeafRegion(Region* );
	Region* getLeafRegion();
};