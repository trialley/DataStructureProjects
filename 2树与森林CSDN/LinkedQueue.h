#pragma once

#include "LinkedQueueNode.h"

#include <iostream>
using namespace std;

template<class T>
class LinkedQueue
{
public:
	LinkedQueue();
	~LinkedQueue();

	bool isEmpty() const;
	size_t size() const;

	void push(const T& element);
	void pop();

	const T& top() const;
private:
	LinkedQueueNode<T> *m_pHeaderNode;
	size_t m_pSize;
};

template<class T>
LinkedQueue<T>::LinkedQueue() :
	m_pHeaderNode(NULL),
	m_pSize(0)
{}


template<class T>
LinkedQueue<T>::~LinkedQueue()
{
	LinkedQueueNode<T>* nextNode = NULL;
	LinkedQueueNode<T>* targetNode = m_pHeaderNode;
	while (targetNode)
	{
		nextNode = targetNode->next;
		delete targetNode;
		targetNode = nextNode;
	}
}


template<class T>
bool LinkedQueue<T>::isEmpty() const
{
	return !m_pSize;
}

template<class T>
size_t LinkedQueue<T>::size() const
{
	return m_pSize;
}


template<class T>
void LinkedQueue<T>::push(const T& element)
{
	LinkedQueueNode<T> *pNewNode = new LinkedQueueNode<T>(element);
	if (m_pSize == 0)
	{
		m_pHeaderNode = pNewNode;
	}
	else
	{
		LinkedQueueNode<T> *targetNode = m_pHeaderNode;
		while (targetNode->next != NULL)
		{
			targetNode = targetNode->next;
		}
		targetNode->next = pNewNode;
	}

	++m_pSize;
}


template<class T>
void LinkedQueue<T>::pop()
{
	if (m_pSize == 0)
		return;

	LinkedQueueNode<T> *pNextNode = m_pHeaderNode->next;
	delete m_pHeaderNode;
	m_pHeaderNode = pNextNode;
	--m_pSize;
}


template<class T>
const T& LinkedQueue<T>::top() const
{
	return m_pHeaderNode->element;
}

