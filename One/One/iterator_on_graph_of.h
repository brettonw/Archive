//------------------------------------------------------------------------------
//	Copyright (C) 1997-2000 Bretton Wade
//	All Rights Reserved
//------------------------------------------------------------------------------

#ifndef		_BW_ITERATOR_ON_GRAPH_OF_H_
#define		_BW_ITERATOR_ON_GRAPH_OF_H_

//------------------------------------------------------------------------------
//	includes
//------------------------------------------------------------------------------
#ifndef		_BW_GRAPH_OF_H_
#include	"graph_of.h"
#endif	//	_BW_GRAPH_OF_H_

#ifndef		_BW_EXCEPTION_H_
#include	"exception.h"
#endif	//	_BW_EXCEPTION_H_

//--------------------------------------------------------------------------
//	exception definitions
//--------------------------------------------------------------------------
DEFINE_EXCEPTION (CycleAtNode);

//------------------------------------------------------------------------------
//	class definitions
//------------------------------------------------------------------------------
template	<class aType>
class	IteratorOnGraphOf
{
	public:
									IteratorOnGraphOf (GraphOf<aType>& graph);
									IteratorOnGraphOf (GraphOf<aType>* graph);
		void						WalkTopology (void);
		void						GotoHead (void);
		void						GotoTail (void);
		void						GotoNextItem (void);
		void						GotoPreviousItem (void);
		aType&						GetCurrentItem (void) const;
		bool						IsDone (void) const;
	private:
		GraphOf<aType>*				myGraph;
		ListOf<aType>				myList;
		IteratorOnListOf<aType>		myIterator;
};

//------------------------------------------------------------------------------
//	template methods
//------------------------------------------------------------------------------
template	<class aType>
IteratorOnGraphOf<aType>::IteratorOnGraphOf (GraphOf<aType>& graph) : myIterator (myList)
{
	myGraph = &graph;
	WalkTopology ();
}

//------------------------------------------------------------------------------
template	<class aType>
IteratorOnGraphOf<aType>::IteratorOnGraphOf (GraphOf<aType>* graph) : myIterator (myList)
{
	assert (graph);
	myGraph = graph;
	WalkTopology ();
}

//------------------------------------------------------------------------------
template	<class aType>
void	IteratorOnGraphOf<aType>::WalkTopology (void)
{
	myList.RemoveAll ();
	IteratorOnHashTableOf<Text, GraphNodeOf<aType>*>	iterator (myGraph->myNodes);
	while (not iterator.IsDone ())
	{
		GraphNodeOf<aType>*		currentNode = iterator.GetCurrentItem ();
		currentNode->Clear ();
		iterator.GotoNextItem ();
	}
	iterator.GotoHead ();
	while (not iterator.IsDone ())
	{
		GraphNodeOf<aType>*		currentNode = iterator.GetCurrentItem ();
		if (currentNode->GetColor () == GraphNodeOf<aType>::WHITE)
		{
		    try
		    {
			    currentNode->Visit (myList);
			}
			catch (GraphNotAcyclic)
			{
			    throw CycleAtNode (cString (iterator.GetCurrentKey ()));
			}
		}
		iterator.GotoNextItem ();
	}
	myIterator.GotoHead ();
}

//------------------------------------------------------------------------------
template	<class aType>
void	IteratorOnGraphOf<aType>::GotoHead (void)
{
	myIterator->GotoHead ();
}

//------------------------------------------------------------------------------
template	<class aType>
void	IteratorOnGraphOf<aType>::GotoTail (void)
{
	myIterator.GotoTail ();
}

//------------------------------------------------------------------------------
template	<class aType>
void	IteratorOnGraphOf<aType>::GotoNextItem (void)
{
	myIterator.GotoNextItem ();
}

//------------------------------------------------------------------------------
template	<class aType>
void	IteratorOnGraphOf<aType>::GotoPreviousItem (void)
{
	myIterator.GotoPreviousItem ();
}

//------------------------------------------------------------------------------
template	<class aType>
aType&	IteratorOnGraphOf<aType>::GetCurrentItem (void) const
{
	return myIterator.GetCurrentItem ();
}

//------------------------------------------------------------------------------
template	<class aType>
bool	IteratorOnGraphOf<aType>::IsDone (void) const
{
	return myIterator.IsDone ();
}

//------------------------------------------------------------------------------

#endif	//	_BW_ITERATOR_ON_GRAPH_OF_H_
