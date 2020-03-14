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
/* ������->��															*/
/* 1.��ĳ���ڵ������ӣ����ýڵ��������ӣ����ӵ��Һ��ӣ�					*/
/*   ���ӵ��Һ��ӵ��Һ��ӣ����ӵ��Һ��ӵ��Һ��ӵ��Һ���....����			*/
/* 2.ɾ��ԭ�е���Щ���У��ڵ����Һ��ӵ�����									*/
/************************************************************************/
/* ע�����ڵ㣬���ڵ���Һ��ӣ����ڵ���Һ��ӵ��Һ���...Ҳ���ڲ���1��          */
/************************************************************************/

/*parentNode: �������д�ʱ�������ڵ�*/
/*pNode: ���д�ʱ�������ڵ�*/
template<class T>
void binaryTreeToTree(TreeNode<T> *parentNode, BinaryTreeNode<T> *pNode)
{
	if (pNode == NULL)
		return;

	//�ж��Ƿ��ǣ�ע�������
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

	//�������иýڵ�ĺ�����������1�����߸�����
	BinaryTreeNode<T> *pLeftNode = pNode->leftNode;
	int pCnt = 0;
	while (pLeftNode)
	{
		++pCnt;
		pLeftNode = pLeftNode->rightNode;
	}

	//��ע�������
	if (isRightNode && pNode->rightNode != NULL)
		++pCnt;

	if (parentNode == NULL)
	{
		parentNode = new TreeNode<T>(pNode->element, pCnt);
		m_pTree<T> = parentNode;  //��¼���ĸ��ڵ�
	}
	else
		parentNode->setSize(pCnt);  //���ú�������

	pLeftNode = pNode->leftNode;
	pCnt = 0;
	while (pLeftNode)
	{
		//Ϊ�����½ڵ���Ӻ��ӣ��ò���1���ߵĶ���������ʼ�����ڵ㣩
		TreeNode<T> *pChildNode = new TreeNode<T>(pLeftNode->element);
		parentNode->next[pCnt] = pChildNode;
		pLeftNode = pLeftNode->rightNode;
		++pCnt;
	}

	//��ע�������
	if (isRightNode && pNode->rightNode != NULL)
	{
		TreeNode<T> *pChildNode = new TreeNode<T>(pNode->rightNode->element);
		parentNode->next[pCnt++] = pChildNode;
	}

	//���亢�ӽ��еݹ���ú���
	pLeftNode = pNode->leftNode;
	for (int i = 0; i < pCnt; ++i)
	{
		binaryTreeToTree<T>(parentNode->next[i], pLeftNode);
		if (isRightNode && i == pCnt - 1) //��ע�������
			pLeftNode = pNode->rightNode;
		else
			pLeftNode = pLeftNode->rightNode;
	}
}