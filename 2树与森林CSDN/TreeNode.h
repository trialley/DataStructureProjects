#pragma once

template<class T>
class TreeNode
{
public:
	TreeNode(const T& theElement, size_t pCnt = 0) :
		element(theElement),
		pSize(pCnt)
	{
		next = new TreeNode<T>*[pCnt];
		for (size_t i = 0; i < pCnt; ++i)
			next[i] = nullptr;
		if (pCnt == 0)
			next = nullptr;
	}


	~TreeNode()
	{
		delete[]next;
	}

	void setSize(size_t pCnt)
	{
		delete []next;
		next = new TreeNode<T>*[pCnt];
		pSize = pCnt;
	}

	size_t pSize;
	T element;
	TreeNode<T> **next;
};