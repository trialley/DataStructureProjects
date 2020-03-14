#pragma once

//#include "BinaryTreeNode.h"
//#include "TreeNode.h"
#include "tree.h"
#include"Forest.h"
#include<queue>
#include<queue>
#include <vector>
using namespace std;
template<class T>
class Forest;
template<class T>
class BinaryTreeNode {
public:
	BinaryTreeNode (const T& theElement) :
		element (theElement),
		leftNode (nullptr),
		rightNode (nullptr) {}

	T element;
	BinaryTreeNode<T>* leftNode;
	BinaryTreeNode<T>* rightNode;
};
template<class T>
class BTree{
public:
	BinaryTreeNode<T>* m_pBinaryTree;
	BTree();
	BTree(BinaryTreeNode<T> *pRoot, size_t pSize);
	BTree (const Forest<T>& pForest) {
			if (pForest.size () == 0)
				return;

			//将每一棵树转换成二叉树，然后连起来
			BinaryTreeNode<T>* pRoot = (new BTree<T> (*pForest.at (0)))->root ();
			BinaryTreeNode<T>* pCurRoot = pRoot;
			BinaryTreeNode<T>* pNextRoot = nullptr;
			size_t pSize = pForest.at (0)->size ();

			for (int i = 1; i < pForest.size (); ++i) {
				pSize += pForest.at (i)->size ();
				pNextRoot = (new BTree<T> (*pForest.at (i)))->root ();
				pCurRoot->rightNode = pNextRoot;
				pCurRoot = pNextRoot;
			}

			_root = pRoot;
			_size = pSize;
	}
	BTree (const Tree<T>& pRoot) {
		treeToBinaryTree (nullptr, pRoot.root ());
		_root = m_pBinaryTree;
	}
	/************************************************************************/
	/*                           树变二叉树                                  */
	/*                  所有兄弟节点之间加一条线								*/
	/*                  所有节点只保留与其左边第一个孩子的连线                  */
	/************************************************************************/
	void treeToBinaryTree (BinaryTreeNode<T>* parentNode, TreeNode<T>* pNode) {
		if (pNode == nullptr)
			return;

		if (parentNode == nullptr) {
			parentNode = new BinaryTreeNode<T> (pNode->element);
			m_pBinaryTree = parentNode;
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
			treeToBinaryTree(lhsNode, pNode->next[i]);
			lhsNode = lhsNode->rightNode;
		}
	}
	~BTree();
	size_t size() const;
	bool isEmpty() const;
	void initialize(T elements[], size_t pSize);
	void levelOrderPrint();
	BinaryTreeNode<T> *root() const;

private:
	BinaryTreeNode<T> *_root;
	size_t _size;
};

template<class T>
BTree<T>::BTree() :
	_root(nullptr),
	_size(0)
{}

template<class T>
BTree<T>::BTree(BinaryTreeNode<T> *pRoot, size_t pSize) :
	_root(pRoot),
	_size(pSize)
{}


template<class T>
BTree<T>::~BTree()
{
	queue<BinaryTreeNode<T> *> pQueue;
	if (_root != nullptr)
		pQueue.push(_root);

	while (!pQueue.empty())
	{
		BinaryTreeNode<T> *currentNode = pQueue.front();
		pQueue.pop();

		if (currentNode->leftNode != nullptr)
			pQueue.push(currentNode->leftNode);
		if (currentNode->rightNode != nullptr)
			pQueue.push(currentNode->rightNode);

		delete currentNode;
	}
}



template<class T>
BinaryTreeNode<T>* BTree<T>::root() const
{
	return _root;
}

template<class T>
bool BTree<T>::isEmpty() const
{
	return !_size;
}

template<class T>
size_t BTree<T>::size() const
{
	return _size;
}



template<class T>
void BTree<T>::initialize(T elements[], size_t pSize)
{
	if (pSize <= 0)
		return;

	queue<BinaryTreeNode<T> *> pQueue;
	_root = new BinaryTreeNode<T>(elements[0]);
	pQueue.push(_root);

	size_t i = 1;
	while (i < pSize)
	{
		BinaryTreeNode<T> *pCurNode = pQueue.front();
		pQueue.pop();
		pCurNode->leftNode = new BinaryTreeNode<T>(elements[i++]);
		pQueue.push(pCurNode->leftNode);

		if (i == pSize)
			break;
		pCurNode->rightNode = new BinaryTreeNode<T>(elements[i++]);
		pQueue.push(pCurNode->rightNode);
	}
}

template<class T>
void BTree<T>::levelOrderPrint()
{
	queue<BinaryTreeNode<T> *> pQueue;
	if (_root != nullptr)
		pQueue.push(_root);

	while (!pQueue.empty())
	{
		BinaryTreeNode<T> *targetNode = pQueue.front();
		pQueue.pop();

	//	cout << "parent: " << targetNode->element << endl;
	//	cout << "children: ";
		if (targetNode->leftNode != nullptr)
		{
			pQueue.push(targetNode->leftNode);
			//cout << targetNode->leftNode->element << " ";
		}
		if (targetNode->rightNode != nullptr)
		{
			pQueue.push(targetNode->rightNode);
			//cout << targetNode->rightNode->element << " ";
		}
	//	cout << endl << endl;
		cout << targetNode->element << " ";
	}
}
