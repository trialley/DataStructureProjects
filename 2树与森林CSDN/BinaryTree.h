#pragma once

#include "BinaryTreeNode.h"
#include "TreeNode.h"
#include "tree.h"
#include "LinkedQueue.h"

#include <vector>
using namespace std;

template<class T>
class BinaryTree
{
public:
	BinaryTree();
	BinaryTree(BinaryTreeNode<T> *pRoot, size_t pSize);
	~BinaryTree();

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
BinaryTree<T>::BinaryTree() :
	m_pRoot(NULL),
	m_pSize(0)
{}

template<class T>
BinaryTree<T>::BinaryTree(BinaryTreeNode<T> *pRoot, size_t pSize) :
	m_pRoot(pRoot),
	m_pSize(pSize)
{}


template<class T>
BinaryTree<T>::~BinaryTree()
{
	LinkedQueue<BinaryTreeNode<T> *> pQueue;
	if (m_pRoot != NULL)
		pQueue.push(m_pRoot);

	while (!pQueue.isEmpty())
	{
		BinaryTreeNode<T> *currentNode = pQueue.top();
		pQueue.pop();

		if (currentNode->leftNode != NULL)
			pQueue.push(currentNode->leftNode);
		if (currentNode->rightNode != NULL)
			pQueue.push(currentNode->rightNode);

		delete currentNode;
	}
}



template<class T>
BinaryTreeNode<T>* BinaryTree<T>::root() const
{
	return m_pRoot;
}

template<class T>
bool BinaryTree<T>::isEmpty() const
{
	return !m_pSize;
}

template<class T>
size_t BinaryTree<T>::size() const
{
	return m_pSize;
}



template<class T>
void BinaryTree<T>::initialize(T elements[], size_t pSize)
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
void BinaryTree<T>::levelOrderPrint()
{
	LinkedQueue<BinaryTreeNode<T> *> pQueue;
	if (m_pRoot != NULL)
		pQueue.push(m_pRoot);

	while (!pQueue.isEmpty())
	{
		BinaryTreeNode<T> *targetNode = pQueue.top();
		pQueue.pop();

	//	cout << "parent: " << targetNode->element << endl;
	//	cout << "children: ";
		if (targetNode->leftNode != NULL)
		{
			pQueue.push(targetNode->leftNode);
			//cout << targetNode->leftNode->element << " ";
		}
		if (targetNode->rightNode != NULL)
		{
			pQueue.push(targetNode->rightNode);
			//cout << targetNode->rightNode->element << " ";
		}
	//	cout << endl << endl;
		cout << targetNode->element << " ";
	}
}
