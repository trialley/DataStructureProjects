#pragma once


#include "TreeNode.h"
#include "BinaryTree.h"
#include "LinkedQueue.h"

template<class T>
class Tree
{
public:
	Tree(TreeNode<T>* pRoot = nullptr);
	~Tree();
	
	size_t size() const;
	TreeNode<T> *root() const;
	void levelOrderPrint() const;
private:
	void destroy(TreeNode<T> *pNode);
private:
	TreeNode<T> *m_pRoot;
	size_t m_pSize;
};


template<class T>
Tree<T>::Tree(TreeNode<T> * pRoot) :
	m_pRoot(pRoot),
	m_pSize(0)
{
	LinkedQueue<TreeNode<T> *> pQueue;
	if (m_pRoot != nullptr)
	{
		pQueue.push(m_pRoot);
		++m_pSize;
	}

	while (!pQueue.isEmpty())
	{
		TreeNode<T> *targetNode = pQueue.top();
		pQueue.pop();
		m_pSize += targetNode->pSize;
		for (int i = 0; i < targetNode->pSize; ++i)
			pQueue.push(targetNode->next[i]);
	}
}

template<class T>
Tree<T>::~Tree()
{
	destroy(m_pRoot);
}


template<class T>
void Tree<T>::destroy(TreeNode<T> *pNode)
{
	for (int i = 0; i < pNode->pSize; ++i)
	{
		destroy(pNode->next[i]);
	}
	delete pNode;
}

template<class T>
size_t Tree<T>::size() const
{
	return m_pSize;
}


template<class T>
TreeNode<T> *Tree<T>::root() const
{
	return m_pRoot;
}


template<class T>
void Tree<T>::levelOrderPrint() const
{
	LinkedQueue<TreeNode<T> *> pQueue;
	if (m_pRoot != nullptr)
		pQueue.push(m_pRoot);

	while (!pQueue.isEmpty())
	{
		TreeNode<T> *targetNode = pQueue.top();
		pQueue.pop();

		cout << "parent: " << targetNode->element << endl;
		cout << "children: ";
		for (size_t i = 0; i < targetNode->pSize; ++i)
		{
			pQueue.push(targetNode->next[i]);
			cout << targetNode->next[i]->element << " ";
		}
		cout << endl << endl;
	//	cout << targetNode->element << " ";
	}
}