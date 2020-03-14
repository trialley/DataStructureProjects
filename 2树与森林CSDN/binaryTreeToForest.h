#pragma once

//#include "binaryTreeToTree.h"
#include "Tree.h"
#include "BinaryTree.h"

#include <vector>
#include <memory>
using namespace std;



template<class T>
vector<unique_ptr<Tree<T> > > binaryTreeToForest (const BTree<T>& pRoot) {
	vector<unique_ptr<Tree<T> > > m_pTreeVector;
	vector<BinaryTreeNode<T>*> m_pBinaryTreeVector;

	//拆分二叉树
	BinaryTreeNode<T>* pNode = pRoot.root ();
	while (pNode != nullptr) {
		BinaryTreeNode<T>* pRightNode = pNode->rightNode;
		pNode->rightNode = nullptr;
		m_pBinaryTreeVector.push_back (pNode);
		pNode = pRightNode;
	}

	//将每一棵二叉树都转换成树
	for (int i = 0; i < m_pBinaryTreeVector.size (); ++i) {
		Tree<T>* pTree = new Tree<T> (m_pBinaryTreeVector.at (i));
		unique_ptr<Tree<T> > uniqueTree (pTree);
		m_pTreeVector.push_back (std::move (uniqueTree));
	}

	return m_pTreeVector;
}

