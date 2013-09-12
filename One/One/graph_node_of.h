//------------------------------------------------------------------------------
//	Copyright (C) 1997-2000 Bretton Wade
//	All Rights Reserved
//------------------------------------------------------------------------------

#ifndef		_BW_GRAPH_NODE_OF_H_
#define		_BW_GRAPH_NODE_OF_H_

//--------------------------------------------------------------------------
//	includes
//--------------------------------------------------------------------------
#ifndef		_BW_ITERATOR_ON_LIST_OF_H_
#include	"iterator_on_list_of.h"
#endif	//	_BW_ITERATOR_ON_LIST_OF_H_

#ifndef		_BW_TEXT_H_
#include	"text.h"
#endif	//	_BW_TEXT_H_

#ifndef		_BW_EXCEPTION_H_
#include	"exception.h"
#endif	//	_BW_EXCEPTION_H_

//--------------------------------------------------------------------------
//	exception definitions
//--------------------------------------------------------------------------
DEFINE_EXCEPTION (GraphNotAcyclic);

//--------------------------------------------------------------------------
//	class definitions
//--------------------------------------------------------------------------
template	<class aType>
class GraphNodeOf
{
	public:
		enum	Color {WHITE, GRAY, BLACK};
	public:
									GraphNodeOf (const aType& item);
		aType&						GetItem (void);
		Color						GetColor (void) const;
		void						AddEdgeTo (GraphNodeOf<aType>* toNode);
		void						Clear (void);
		void						Visit (ListOf<aType>& list);
	protected:
		ListOf<GraphNodeOf<aType>*>	myEdges;
		aType						myItem;
		Color						myColor;
};

//--------------------------------------------------------------------------
//	template methods
//--------------------------------------------------------------------------
template	<class aType>
GraphNodeOf<aType>::GraphNodeOf (const aType& item) : myItem (item)
{
}

//--------------------------------------------------------------------------
template	<class aType>
aType&	GraphNodeOf<aType>::GetItem (void)
{
	return myItem;
}

//--------------------------------------------------------------------------
template	<class aType>
typename GraphNodeOf<aType>::Color	GraphNodeOf<aType>::GetColor (void) const
{
	return myColor;
}

//--------------------------------------------------------------------------
template	<class aType>
void	GraphNodeOf<aType>::AddEdgeTo (GraphNodeOf<aType>* toNode)
{
	myEdges.AddToTail (toNode);
}

//--------------------------------------------------------------------------
template	<class aType>
void	GraphNodeOf<aType>::Clear (void)
{
	myColor = WHITE;
}

//--------------------------------------------------------------------------
template	<class aType>
void	GraphNodeOf<aType>::Visit (ListOf<aType>& list)
{
	IteratorOnListOf<GraphNodeOf<aType>*>	iterator (myEdges);
	myColor = GRAY;
	while (not iterator.IsDone ())
	{
		GraphNodeOf<aType>*		currentNode = iterator.GetCurrentItem ();
		if (currentNode->myColor == GRAY)
		    throw GraphNotAcyclic ();
		if (currentNode->myColor == WHITE)
			currentNode->Visit (list);
		iterator.GotoNextItem ();
	}
	myColor = BLACK;
	list.AddToHead (myItem);
}

//--------------------------------------------------------------------------

#endif	//	_BW_GRAPH_NODE_OF_H_
