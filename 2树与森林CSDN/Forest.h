#pragma once
#include "BinaryTree.h"
#include "Tree.h"
#include<vector>
#include<memory>
using namespace std;
template<class T>
class BTree;
template<class T>
class Forest {
private:
	vector<Tree<T>* > _trees;
public:
	void initn (int N) {
	//若初始为森林，每行的格式为 A B[nodes], 表示结点 A 拥有 B 个孩子结点, 孩子结点的集合为 nodes, i.e. 1 2 3 4 表示结点 1 拥有 2 个孩子, 分别为结点 3, 4。
		for (int i = 0; i < N; i++) {
			int a,b;
			cin >> a >> b;
			for (int i = 0; i < _trees.size (); i++) {
				if (_trees[i]->root ()->element == a) {
					for (int j=0; j < b; j++) {
						int temp;
						cin >> temp;
						_trees[i]->insert(temp).
					}
				}
				break;
			}
		}
	}
	Forest(){}

	/*从一二叉树中初始化本森林*/
	Forest (BTree<T>& pRoot) {
		vector<Tree<T>* > m_pTreeVector;
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
			_trees.push_back (new Tree<T> (m_pBinaryTreeVector.at (i)));
		}

		//_trees = m_pTreeVector;
	}
	~Forest (){}

	size_t size () const { return _trees.size(); };
	unique_ptr<Tree<T> > getTree (int index)const {
		if (index >= _trees.size ()) {
			return nullptr;
		}

		return new Tree<T> (_trees[index]);
	}
	Tree<T>*  at (int index)const {
		if (index >= _trees.size ()) {
			return nullptr;
		}

		return  _trees.at(index);
	}
};