#pragma once

#include "BinaryTreeNode.h"
#include "TreeNode.h"

#include <iostream>
#include <memory>
using namespace std;

template<class T>
TreeNode<T> *m_pTree = NULL;

template<class T>
BinaryTreeNode<T> *m_pRoot = NULL;

template<class T>
TreeNode<T>* binaryTreeToTree(BinaryTreeNode<T> *pRoot)
{
	m_pRoot<T> = pRoot;
	binaryTreeToTree<T>(NULL, pRoot);
	return m_pTree<T>;
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
void binaryTreeToTree(TreeNode<T> *parentNode, BinaryTreeNode<T> *pNode)
{
	if (pNode == NULL)
		return;

	//判断是否是（注）中情况
	bool isRightNode = false;
	BinaryTreeNode<T> *targetNode = m_pRoot<T>;
	while (targetNode)
	{
		if (targetNode == pNode)
		{
			isRightNode = true;
			break;
		}
		targetNode = targetNode->rightNode;
	}

	//计算树中该节点的孩子数（步骤1中连线个数）
	BinaryTreeNode<T> *pLeftNode = pNode->leftNode;
	int pCnt = 0;
	while (pLeftNode)
	{
		++pCnt;
		pLeftNode = pLeftNode->rightNode;
	}

	//（注）中情况
	if (isRightNode && pNode->rightNode != NULL)
		++pCnt;

	if (parentNode == NULL)
	{
		parentNode = new TreeNode<T>(pNode->element, pCnt);
		m_pTree<T> = parentNode;  //记录树的根节点
	}
	else
		parentNode->setSize(pCnt);  //设置孩子数量

	pLeftNode = pNode->leftNode;
	pCnt = 0;
	while (pLeftNode)
	{
		//为树的新节点添加孩子（用步骤1连线的二叉树结点初始化树节点）
		TreeNode<T> *pChildNode = new TreeNode<T>(pLeftNode->element);
		parentNode->next[pCnt] = pChildNode;
		pLeftNode = pLeftNode->rightNode;
		++pCnt;
	}

	//（注）中情况
	if (isRightNode && pNode->rightNode != NULL)
	{
		TreeNode<T> *pChildNode = new TreeNode<T>(pNode->rightNode->element);
		parentNode->next[pCnt++] = pChildNode;
	}

	//对其孩子进行递归调用函数
	pLeftNode = pNode->leftNode;
	for (int i = 0; i < pCnt; ++i)
	{
		binaryTreeToTree<T>(parentNode->next[i], pLeftNode);
		if (isRightNode && i == pCnt - 1) //（注）中情况
			pLeftNode = pNode->rightNode;
		else
			pLeftNode = pLeftNode->rightNode;
	}
}