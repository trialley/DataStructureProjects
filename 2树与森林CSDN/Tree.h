#pragma once


//#include "TreeNode.h"
#include "BinaryTree.h"
#include<queue>
using namespace std;
//#include "queue.h"

template<class T>
class BinaryTreeNode;

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

	BinaryTreeNode<T>* t_root = nullptr;

	Tree (BinaryTreeNode<T>* pRoot) {
		t_root= pRoot;
		binaryTreeToTree<T> (nullptr, pRoot);
	}


	/************************************************************************/
	/* 二叉树->树															*/
	/* 1.若某个节点有左孩子，将该节点与其左孩子，左孩子的右孩子，					*/
	/*   左孩子的右孩子的右孩子，左孩子的右孩子的右孩子的右孩子....连线			*/
	/* 2.删除原有的那些线中，节点与右孩子的连线									*/
	/************************************************************************/
	/* 注：根节点，根节点的右孩子，根节点的右孩子的右孩子...也算在步骤1中          */
	/************************************************************************/

	/*parentNode: 二叉树中此时子树根节点*/
	/*pNode: 树中此时子树根节点*/
	template<class T>
	void binaryTreeToTree (TreeNode<T>* parentNode, BinaryTreeNode<T>* pNode) {
		if (pNode == nullptr)
			return;

		//判断是否是（注）中情况
		bool isRightNode = false;
		BinaryTreeNode<T>* targetNode = t_root;
		while (targetNode) {
			if (targetNode == pNode) {
				isRightNode = true;
				break;
			}
			targetNode = targetNode->rightNode;
		}

		//计算树中该节点的孩子数（步骤1中连线个数）
		BinaryTreeNode<T>* pLeftNode = pNode->leftNode;
		int pCnt = 0;
		while (pLeftNode) {
			++pCnt;
			pLeftNode = pLeftNode->rightNode;
		}

		//（注）中情况
		if (isRightNode && pNode->rightNode != nullptr)
			++pCnt;

		if (parentNode == nullptr) {
			parentNode = new TreeNode<T> (pNode->element, pCnt);
			_root = parentNode;  //记录树的根节点
		} else
			parentNode->setSize (pCnt);  //设置孩子数量

		pLeftNode = pNode->leftNode;
		pCnt = 0;
		while (pLeftNode) {
			//为树的新节点添加孩子（用步骤1连线的二叉树结点初始化树节点）
			TreeNode<T>* pChildNode = new TreeNode<T> (pLeftNode->element);
			parentNode->next[pCnt] = pChildNode;
			pLeftNode = pLeftNode->rightNode;
			++pCnt;
		}

		//（注）中情况
		if (isRightNode && pNode->rightNode != nullptr) {
			TreeNode<T>* pChildNode = new TreeNode<T> (pNode->rightNode->element);
			parentNode->next[pCnt++] = pChildNode;
		}

		//对其孩子进行递归调用函数
		pLeftNode = pNode->leftNode;
		for (int i = 0; i < pCnt; ++i) {
			binaryTreeToTree (parentNode->next[i], pLeftNode);
			if (isRightNode && i == pCnt - 1) //（注）中情况
				pLeftNode = pNode->rightNode;
			else
				pLeftNode = pLeftNode->rightNode;
		}
	}

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



