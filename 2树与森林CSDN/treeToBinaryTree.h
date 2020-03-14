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
/*                           树变二叉树                                  */
/*                  所有兄弟节点之间加一条线								*/
/*                  所有节点只保留与其左边第一个孩子的连线                  */
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
	parentNode->leftNode = lhsNode; //只保留与左边第一个节点的连线

	//兄弟节点之间连线
	for (int i = 0; i < pCnt - 1; ++i)
	{
		rhsNode = new BinaryTreeNode<T>(pNode->next[i + 1]->element);
		lhsNode->rightNode = rhsNode;
		lhsNode = rhsNode;
	}

	//递归调用，以树中的兄弟节点为目标节点，以二叉树中同一位置节点为根节点
	lhsNode = parentNode->leftNode;
	for (int i = 0; i < pCnt; ++i)
	{
		treeToBinaryTree<T>(lhsNode, pNode->next[i]);
		lhsNode = lhsNode->rightNode;
	}
}