#pragma once

#include "BinaryTreeNode.h"
#include "TreeNode.h"
#include "tree.h"
#include "LinkedQueue.h"

#include <vector>
using namespace std;

template<class T>
class BTree
{
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

	BinaryTreeNode<T> *m_pRoot;
	size_t m_pSize;
};

template<class T>
BTree<T>::BTree() :
	m_pRoot(nullptr),
	m_pSize(0)
{}

template<class T>
BTree<T>::BTree(BinaryTreeNode<T> *pRoot, size_t pSize) :
	m_pRoot(pRoot),
	m_pSize(pSize)
{}


template<class T>
BTree<T>::~BTree()
{
	LinkedQueue<BinaryTreeNode<T> *> pQueue;
	if (m_pRoot != nullptr)
		pQueue.push(m_pRoot);

	while (!pQueue.isEmpty())
	{
		BinaryTreeNode<T> *currentNode = pQueue.top();
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
	return m_pRoot;
}

template<class T>
bool BTree<T>::isEmpty() const
{
	return !m_pSize;
}

template<class T>
size_t BTree<T>::size() const
{
	return m_pSize;
}



template<class T>
void BTree<T>::initialize(T elements[], size_t pSize)
{
	if (pSize <= 0)
		return;

	LinkedQueue<BinaryTreeNode<T> *> pQueue;
	m_pRoot = new BinaryTreeNode<T>(elements[0]);
	pQueue.push(m_pRoot);

	size_t i = 1;
	while (i < pSize)
	{
		BinaryTreeNode<T> *pCurNode = pQueue.top();
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
	LinkedQueue<BinaryTreeNode<T> *> pQueue;
	if (m_pRoot != nullptr)
		pQueue.push(m_pRoot);

	while (!pQueue.isEmpty())
	{
		BinaryTreeNode<T> *targetNode = pQueue.top();
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
