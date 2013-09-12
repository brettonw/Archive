//------------------------------------------------------------------------------
//	Copyright (C) 1997-2000 Bretton Wade
//	All Rights Reserved
//------------------------------------------------------------------------------

#ifndef		_BW_PRIORITY_QUEUE_OF_H_
#define		_BW_PRIORITY_QUEUE_OF_H_

//--------------------------------------------------------------------------
//	includes
//--------------------------------------------------------------------------
#ifndef		_BW_RESIZABLE_VECTOR_OF_H_
#include	"resizable_vector_of.h"
#endif	//	_BW_RESIZABLE_VECTOR_OF_H_

//--------------------------------------------------------------------------
//	class definitions
//--------------------------------------------------------------------------
template	<class aType>
class	PriorityQueueOf
{
	public:
									PriorityQueueOf (void);
									~PriorityQueueOf (void);
		void						Insert (aType item);
		void						InsertLazy (aType item);
		aType						GetMinimum (void);
		aType						ExtractMinimum (void);
		void						Clear (void);
		uInt						GetQueueSize (void) const;
		bool						IsEmpty (void) const;
	private:
		void						PercolateDown (uInt parent);
		void						RestoreHeapOrder (void);
	private:
		bool						myIsInHeapOrder;
		uInt						myHeapSize;
		ResizableVectorOf<aType>	myHeap;
};

//--------------------------------------------------------------------------
//	template methods
//--------------------------------------------------------------------------
template	<class aType>
PriorityQueueOf<aType>::PriorityQueueOf (void)
{
	myIsInHeapOrder = true;
	myHeapSize = 0;
}

//--------------------------------------------------------------------------
template	<class aType>
PriorityQueueOf<aType>::~PriorityQueueOf (void)
{
}

//--------------------------------------------------------------------------
template	<class aType>
void	PriorityQueueOf<aType>::Insert (aType item)
{
	if (myIsInHeapOrder)
	{
		uInt	child = ++myHeapSize,
				parent = child / 2;
		myHeap[child] = item;
		while ((parent > 0) and (item < myHeap[parent]))
		{
			aType	tmp = myHeap[child];
			myHeap[child] = myHeap[parent];
			myHeap[parent] = tmp;
			child = parent;
			parent /= 2;
		}
	}
	else
		InsertLazy (item);
}

//--------------------------------------------------------------------------
template	<class aType>
void	PriorityQueueOf<aType>::InsertLazy (aType item)
{
	uInt	child = ++myHeapSize,
			parent = child / 2;
	myHeap[child] = item;
	if (myIsInHeapOrder and (parent > 0))
		if (item < myHeap[parent])
			myIsInHeapOrder = false;
}

//--------------------------------------------------------------------------
template	<class aType>
aType	PriorityQueueOf<aType>::GetMinimum (void)
{
	if (!myIsInHeapOrder)
		RestoreHeapOrder ();
	return myHeap[1];
}

//--------------------------------------------------------------------------
template	<class aType>
aType	PriorityQueueOf<aType>::ExtractMinimum (void)
{
	if (!myIsInHeapOrder)
		RestoreHeapOrder ();
	aType   min = myHeap[1];
	myHeap[1] = myHeap[myHeapSize--];
	PercolateDown (1);
	return min;
}

//--------------------------------------------------------------------------
template	<class aType>
void	PriorityQueueOf<aType>::Clear (void)
{
	myHeapSize = 0;
	myIsInHeapOrder = true;
}

//--------------------------------------------------------------------------
template	<class aType>
uInt	PriorityQueueOf<aType>::GetQueueSize (void) const
{
	return myHeapSize;
}

//--------------------------------------------------------------------------
template	<class aType>
bool	PriorityQueueOf<aType>::IsEmpty (void) const
{
	return (myHeapSize > 0) ? true : false;
}

//--------------------------------------------------------------------------
template	<class aType>
void	PriorityQueueOf<aType>::PercolateDown (uInt parent)
{
	aType	item = myHeap[parent];
	uInt	child = parent * 2;
	while (child <= myHeapSize)
	{
		if (child < myHeapSize)
		{
			uInt	sibling = child + 1;
			if (myHeap[sibling] < myHeap[child])
			child = sibling;
		}
		if (myHeap[child] >= item)
			break;
		aType	tmp = myHeap[child];
		myHeap[child] = myHeap[parent];
		myHeap[parent] = tmp;
		parent = child;
		child = parent * 2;
	}
}

//--------------------------------------------------------------------------
template	<class aType>
void	PriorityQueueOf<aType>::RestoreHeapOrder (void)
{
	for (uInt i = myHeapSize / 2; i > 0; i--)
		PercolateDown (i);
	myIsInHeapOrder = true;
}

//--------------------------------------------------------------------------

#endif	//	_BW_PRIORITY_QUEUE_OF_H_
