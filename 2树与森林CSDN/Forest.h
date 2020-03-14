#pragma once
#include "BinaryTree.h"
#include "Tree.h"


template<class T>
class Forest {
private:
	vector<unique_ptr< Tree<T> > > _trees;
	size_t _size;
public:
	/*从一二叉树中初始化本森林*/
	Forest (const BTree<T>& pRoot) {
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
			Tree<T>* pTree = new Tree<T> (binaryTreeToTree<T> (m_pBinaryTreeVector.at (i)));
			unique_ptr<Tree<T> > uniqueTree (pTree);
			_trees.push_back (std::move (uniqueTree));
		}

		//_trees = m_pTreeVector;
	}
	~Forest (){}

	size_t size () const { return _size; };
	unique_ptr<Tree<T> > getTree (int index)const {
		if (index >= _trees.size ()) {
			return nullptr;
		}

		return new Tree<T> (_trees[index]);
	}

};