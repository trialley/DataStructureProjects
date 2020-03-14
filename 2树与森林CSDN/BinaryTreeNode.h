#pragma once

#include <iostream>
using namespace std;

template<class T>
class BinaryTreeNode
{
public:
	BinaryTreeNode(const T& theElement) :
		element(theElement),
		leftNode(NULL),
		rightNode(NULL)
	{}

	T element;
	BinaryTreeNode<T> *leftNode;
	BinaryTreeNode<T> *rightNode;
};