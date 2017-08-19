#include "stdafx.h"
#include "Destination.h"
#include "BinaryTree.h"
#include <queue>
#include <string>
#include <iostream>
using namespace std;

void TreeNode::setTreeNodeData(double data)
{
	this->treeData = data;
}
double TreeNode::getTreeNodeData()
{
	return  treeData;
}
void TreeNode::setRegionNum(int num)
{
	this->regionNum = num;
	i++;
}
int TreeNode::getRegionNum()
{
	return  regionNum;
}
void TreeNode::setTreeNodeLeftChild(TreeNode* node)
{
	this->leftchild = node;
}
TreeNode* TreeNode::getTreeNodeLeftChild()
{
	return this->leftchild;
}
void TreeNode::setTreeNodeRightChild(TreeNode* node)
{
	this->rightchild = node;
}
TreeNode* TreeNode::getTreeNodeRightChild()
{
	return this->rightchild;
}
void TreeNode::setTreeNodeParent(TreeNode* node)
{
	this->parent = node;
}
TreeNode* TreeNode::getTreeNodeParent()
{
	return this->parent;
}
void TreeNode::setTreeNodeIsLeaf(bool isLeaf)
{
	this->isleaf = isLeaf;
}
bool TreeNode::getTreeNodeIsLeaf()
{
	return this->isleaf;
}
void TreeNode::setTreeLeafData(Node* spot)
{
	this->leafSpot = spot;
}
Node* TreeNode::getTreeLeafData()
{
	return this->leafSpot;
}
void TreeNode::setTreeNodeAxis(char axis)
{
	this->treeAxis = axis;
}
char TreeNode::getTreeNodeAxis()
{
	return this->treeAxis;
}
void TreeNode::setLeafRegion(Region* inputRegion)
{
	this->leafRegion = inputRegion;
}
Region* TreeNode::getLeafRegion()
{
	return this->leafRegion;
}