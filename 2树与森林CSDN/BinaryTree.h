#pragma once

//#include "BinaryTreeNode.h"
//#include "TreeNode.h"
#include "tree.h"
//#include "queue.h"
#include<queue>
#include<queue>
#include <vector>
using namespace std;
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
	BTree();
	BTree(BinaryTreeNode<T> *pRoot, size_t pSize);
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
