// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GenericPlatform/GenericPlatformMisc.h"

/**
 * 
 */

template<typename Element>
class Beacon_QueueNode
{
public:
	Beacon_QueueNode() {}
	Beacon_QueueNode(Element* ele)
	{
		this->_ElePtr = ele;
	}
	Beacon_QueueNode(const Beacon_QueueNode<Element>& node)
	{
		this->_ElePtr = node._ElePtr;
	}

public:
	Beacon_QueueNode<Element>& operator=(const Beacon_QueueNode<Element>& node)
	{
		this->_ElePtr = node._ElePtr;
		return *this;
	}

	Element& operator*()
	{
		return *_ElePtr;
	}

	Element* operator->()
	{
		return _ElePtr;
	}

	bool operator< (const Beacon_QueueNode<Element>& node) const
	{
		return *_ElePtr < *(node._ElePtr);
	}

	bool operator> (const Beacon_QueueNode<Element>& node) const
	{
		return *_ElePtr > *(node._ElePtr);
	}

public:
	Element* _ElePtr;
};

template<typename Element>
class BEACON_API Beacon_PriorityQueue
{
public:
	/*Beacon_PriorityQueue()
	{
		F_Compare = [](const Beacon_QueueNode<Element>& a, const Beacon_QueueNode<Element>& b) -> bool {return a < b; };
	}*/
	
	Beacon_PriorityQueue(TFunction<bool(Element* a, Element* b)> compare = [=](Element* a, Element* b) -> bool {return *a < *b; })
	{
		F_Compare = [=](const Beacon_QueueNode<Element>& a, const Beacon_QueueNode<Element>& b) -> bool {return compare(a._ElePtr, b._ElePtr); };
	}

	~Beacon_PriorityQueue() = default;

public:
	inline void Push(Element* e)
	{
		m_TArray.HeapPush(Beacon_QueueNode<Element>(e), F_Compare);
	}

	inline Element* Pop()
	{
		Beacon_QueueNode<Element> node;
		m_TArray.HeapPop(node, F_Compare);
		return node._ElePtr;
	}

	inline void RemoveTop()
	{
		m_TArray.HeapPopDiscard();
	}

	inline bool IsEmpty() const
	{
		return m_TArray.Num() == 0;
	}

	inline void Empty()
	{
		m_TArray.Empty();
	}

	inline uint32 Num() const
	{
		return m_TArray.Num();
	}

protected:
	TArray<Beacon_QueueNode<Element>> m_TArray;

	TFunction<bool(const Beacon_QueueNode<Element>& a, const Beacon_QueueNode<Element>& b)> F_Compare;
};
