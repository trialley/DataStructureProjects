#pragma once

#include "BinaryTree.h"
#include "Tree.h"
//#include "treeToBinaryTree.h"

#include <vector>
#include <memory>
using namespace std;
template<class T>
BinaryTreeNode<T>* m_pBinaryTree;
template<class T>
BinaryTreeNode<T>* treeToBinaryTree (const Tree<T>& pRoot) {
	treeToBinaryTree<T> (nullptr, pRoot.root ());
	return m_pBinaryTree<T>;
}

/************************************************************************/
/*                           树变二叉树                                  */
/*                  所有兄弟节点之间加一条线								*/
/*                  所有节点只保留与其左边第一个孩子的连线                  */
/************************************************************************/
template<class T>
void treeToBinaryTree (BinaryTreeNode<T>* parentNode, TreeNode<T>* pNode) {
	if (pNode == nullptr)
		return;

	if (parentNode == nullptr) {
		parentNode = new BinaryTreeNode<T> (pNode->element);
		m_pBinaryTree<T> = parentNode;
	}

	int pCnt = pNode->pSize;
	if (pCnt == 0)
		return;

	BinaryTreeNode<T>* lhsNode, * rhsNode;
	lhsNode = new BinaryTreeNode<T> (pNode->next[0]->element);
	parentNode->leftNode = lhsNode; //只保留与左边第一个节点的连线

	//兄弟节点之间连线
	for (int i = 0; i < pCnt - 1; ++i) {
		rhsNode = new BinaryTreeNode<T> (pNode->next[i + 1]->element);
		lhsNode->rightNode = rhsNode;
		lhsNode = rhsNode;
	}

	//递归调用，以树中的兄弟节点为目标节点，以二叉树中同一位置节点为根节点
	lhsNode = parentNode->leftNode;
	for (int i = 0; i < pCnt; ++i) {
		treeToBinaryTree<T> (lhsNode, pNode->next[i]);
		lhsNode = lhsNode->rightNode;
	}
}
template<class T>
unique_ptr<BTree<T> > forestToBinaryTree(const vector<Tree<T> *> &pForest)
{
	if (pForest.size() == 0)
		return nullptr;

	//将每一棵树转换成二叉树，然后连起来
	BinaryTreeNode<T> *pRoot = treeToBinaryTree<T>(*pForest.at(0));
	BinaryTreeNode<T> *pCurRoot = pRoot;
	BinaryTreeNode<T> *pNextRoot = nullptr;
	size_t pSize = pForest.at(0)->size();

	for (int i = 1; i < pForest.size(); ++i)
	{
		pSize += pForest.at(i)->size();
		pNextRoot = treeToBinaryTree<T>(*pForest.at(i));
		pCurRoot->rightNode = pNextRoot;
		pCurRoot = pNextRoot;
	}

	BTree<T>* binaryTree = new BTree<T>(pRoot, pSize);
	unique_ptr<BTree<T> > uniqueBinaryTree(binaryTree);
	return uniqueBinaryTree;
}



