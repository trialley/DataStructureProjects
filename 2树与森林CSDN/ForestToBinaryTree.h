#pragma once

#include "BinaryTree.h"
#include "Tree.h"
#include "treeToBinaryTree.h"

#include <vector>
#include <memory>
using namespace std;

template<class T>
unique_ptr<BinaryTree<T> > forestToBinaryTree(const vector<Tree<T> *> &pForest)
{
	if (pForest.size() == 0)
		return NULL;

	//将每一棵树转换成二叉树，然后连起来
	BinaryTreeNode<T> *pRoot = treeToBinaryTree<T>(*pForest.at(0));
	BinaryTreeNode<T> *pCurRoot = pRoot;
	BinaryTreeNode<T> *pNextRoot = NULL;
	size_t pSize = pForest.at(0)->size();

	for (int i = 1; i < pForest.size(); ++i)
	{
		pSize += pForest.at(i)->size();
		pNextRoot = treeToBinaryTree<T>(*pForest.at(i));
		pCurRoot->rightNode = pNextRoot;
		pCurRoot = pNextRoot;
	}

	BinaryTree<T>* binaryTree = new BinaryTree<T>(pRoot, pSize);
	unique_ptr<BinaryTree<T> > uniqueBinaryTree(binaryTree);
	return uniqueBinaryTree;
}



