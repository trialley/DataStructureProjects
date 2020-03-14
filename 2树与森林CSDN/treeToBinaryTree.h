#pragma once


#include "Tree.h"
#include "BinaryTreeNode.h"

template<class T>
BinaryTreeNode<T> *m_pBinaryTree;


template<class T>
BinaryTreeNode<T> *treeToBinaryTree(const Tree<T> &pRoot)
{
	treeToBinaryTree<T>(NULL, pRoot.root());
	return m_pBinaryTree<T>;
}

/************************************************************************/
/*                           ���������                                  */
/*                  �����ֵܽڵ�֮���һ����								*/
/*                  ���нڵ�ֻ����������ߵ�һ�����ӵ�����                  */
/************************************************************************/
template<class T>
void treeToBinaryTree(BinaryTreeNode<T> *parentNode, TreeNode<T> *pNode)
{
	if (pNode == NULL)
		return;
	
	if (parentNode == NULL)
	{
		parentNode = new BinaryTreeNode<T>(pNode->element);
		m_pBinaryTree<T> = parentNode;
	}

	int pCnt = pNode->pSize;
	if (pCnt == 0)
		return;

	BinaryTreeNode<T> *lhsNode, *rhsNode;
	lhsNode = new BinaryTreeNode<T>(pNode->next[0]->element);
	parentNode->leftNode = lhsNode; //ֻ��������ߵ�һ���ڵ������

	//�ֵܽڵ�֮������
	for (int i = 0; i < pCnt - 1; ++i)
	{
		rhsNode = new BinaryTreeNode<T>(pNode->next[i + 1]->element);
		lhsNode->rightNode = rhsNode;
		lhsNode = rhsNode;
	}

	//�ݹ���ã������е��ֵܽڵ�ΪĿ��ڵ㣬�Զ�������ͬһλ�ýڵ�Ϊ���ڵ�
	lhsNode = parentNode->leftNode;
	for (int i = 0; i < pCnt; ++i)
	{
		treeToBinaryTree<T>(lhsNode, pNode->next[i]);
		lhsNode = lhsNode->rightNode;
	}
}