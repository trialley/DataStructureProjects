#pragma once


template<class T>
class LinkedQueueNode
{
public:
	LinkedQueueNode(const T& theElement, LinkedQueueNode<T> *theNext = nullptr) :
		element(theElement),
		next(theNext)
	{}

	T element;
	LinkedQueueNode<T> *next;
};