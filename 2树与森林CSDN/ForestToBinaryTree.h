#pragma once

#include "BinaryTree.h"
#include "Tree.h"
//#include "treeToBinaryTree.h"

#include <vector>
#include <memory>
using namespace std;

template<class T>
unique_ptr<BTree<T> > forestToBinaryTree(const vector<Tree<T> *> &pForest)
{
	if (pForest.size() == 0)
		return nullptr;

	//将每一棵树转换成二叉树，然后连起来
	BinaryTreeNode<T> *pRoot =(new BTree<T>(*pForest.at(0)))->root();
	BinaryTreeNode<T> *pCurRoot = pRoot;
	BinaryTreeNode<T> *pNextRoot = nullptr;
	size_t pSize = pForest.at(0)->size();

	for (int i = 1; i < pForest.size(); ++i)
	{
		pSize += pForest.at(i)->size();
		pNextRoot = (new BTree<T>(*pForest.at(i)))->root();
		pCurRoot->rightNode = pNextRoot;
		pCurRoot = pNextRoot;
	}

	BTree<T>* binaryTree = new BTree<T>(pRoot, pSize);
	unique_ptr<BTree<T> > uniqueBinaryTree(binaryTree);
	return uniqueBinaryTree;
}



