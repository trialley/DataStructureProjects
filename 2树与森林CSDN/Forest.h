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
	//����ʼΪɭ�֣�ÿ�еĸ�ʽΪ A B[nodes], ��ʾ��� A ӵ�� B �����ӽ��, ���ӽ��ļ���Ϊ nodes, i.e. 1 2 3 4 ��ʾ��� 1 ӵ�� 2 ������, �ֱ�Ϊ��� 3, 4��
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

	/*��һ�������г�ʼ����ɭ��*/
	Forest (BTree<T>& pRoot) {
		vector<Tree<T>* > m_pTreeVector;
		vector<BinaryTreeNode<T>*> m_pBinaryTreeVector;

		//��ֶ�����
		BinaryTreeNode<T>* pNode = pRoot.root ();
		while (pNode != nullptr) {
			BinaryTreeNode<T>* pRightNode = pNode->rightNode;
			pNode->rightNode = nullptr;
			m_pBinaryTreeVector.push_back (pNode);
			pNode = pRightNode;
		}

		//��ÿһ�ö�������ת������
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