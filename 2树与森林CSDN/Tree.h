#pragma once


//#include "TreeNode.h"
#include "BinaryTree.h"
#include<queue>
using namespace std;
//#include "queue.h"

template<class T>
class TreeNode {
public:
	TreeNode (const T& theElement, size_t pCnt = 0) :
		element (theElement),
		pSize (pCnt) {
		next = new TreeNode<T> * [pCnt];
		for (size_t i = 0; i < pCnt; ++i)
			next[i] = nullptr;
		if (pCnt == 0)
			next = nullptr;
	}


	~TreeNode () {
		delete[]next;
	}

	void setSize (size_t pCnt) {
		delete[]next;
		next = new TreeNode<T> * [pCnt];
		pSize = pCnt;
	}

	size_t pSize;
	T element;
	TreeNode<T>** next;
};
template<class T>
class Tree {
public:
	Tree (TreeNode<T>* pRoot = nullptr);
	~Tree ();

	size_t size () const;
	TreeNode<T>* root () const;
	void levelOrderPrint () const;
private:
	void destroy (TreeNode<T>* pNode);
private:
	TreeNode<T>* _root;
	size_t _size;
};

template<class T>
Tree<T>::Tree (TreeNode<T>* pRoot) :
	_root (pRoot),
	_size (0) {
	queue<TreeNode<T>*> pQueue;
	if (_root != nullptr) {
		pQueue.push (_root);
		++_size;
	}

	while (!pQueue.empty ()) {
		TreeNode<T>* targetNode = pQueue.front ();
		pQueue.pop ();
		_size += targetNode->pSize;
		for (int i = 0; i < targetNode->pSize; ++i)
			pQueue.push (targetNode->next[i]);
	}
}

template<class T>
Tree<T>::~Tree () {
	destroy (_root);
}


template<class T>
void Tree<T>::destroy (TreeNode<T>* pNode) {
	for (int i = 0; i < pNode->pSize; ++i) {
		destroy (pNode->next[i]);
	}
	delete pNode;
}

template<class T>
size_t Tree<T>::size () const {
	return _size;
}


template<class T>
TreeNode<T>* Tree<T>::root () const {
	return _root;
}


template<class T>
void Tree<T>::levelOrderPrint () const {
	queue<TreeNode<T>*> pQueue;
	if (_root != nullptr)
		pQueue.push (_root);

	while (!pQueue.empty ()) {
		TreeNode<T>* targetNode = pQueue.front ();
		pQueue.pop ();

		cout << "parent: " << targetNode->element << endl;
		cout << "children: ";
		for (size_t i = 0; i < targetNode->pSize; ++i) {
			pQueue.push (targetNode->next[i]);
			cout << targetNode->next[i]->element << " ";
		}
		cout << endl << endl;
		//	cout << targetNode->element << " ";
	}
}



