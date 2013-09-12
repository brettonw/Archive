//------------------------------------------------------------------------------
//	Copyright (C) 1997-2000 Bretton Wade
//	All Rights Reserved
//------------------------------------------------------------------------------


#ifndef		_BW_LIST_NODE_OF_H_
#define		_BW_LIST_NODE_OF_H_

//--------------------------------------------------------------------------
//	includes
//--------------------------------------------------------------------------
#ifndef		_BW_SENTINEL_NODE_H_
#include	"sentinel_node.h"
#endif	//	_BW_SENTINEL_NODE_H_

//--------------------------------------------------------------------------
//	class definitions
//--------------------------------------------------------------------------
template	<class aType>
class	ListNodeOf : public SentinelNode
{
	public:
						ListNodeOf (const aType& item);
						~ListNodeOf (void);
		ListNodeOf*		GetNextNode (SentinelNode* sentinel) const;
		ListNodeOf*		GetPreviousNode (SentinelNode* sentinel) const;
		aType&			GetItem (void);
	private:
		aType			myItem;
};

//--------------------------------------------------------------------------
//	template methods
//--------------------------------------------------------------------------
template	<class aType>
ListNodeOf<aType>::ListNodeOf (const aType& item) : myItem (item)
{
}

//--------------------------------------------------------------------------
template	<class aType>
ListNodeOf<aType>::~ListNodeOf (void)
{
	RemoveFromList ();
}

//--------------------------------------------------------------------------
template	<class aType>
ListNodeOf<aType>*	ListNodeOf<aType>::GetNextNode (SentinelNode* sentinel) const
{
	return (ListNodeOf<aType>*) SentinelNode::GetNextNode (sentinel);
}

//--------------------------------------------------------------------------
template	<class aType>
ListNodeOf<aType>*	ListNodeOf<aType>::GetPreviousNode (SentinelNode* sentinel) const
{
	return (ListNodeOf<aType>*) SentinelNode::GetPreviousNode (sentinel);
}

//--------------------------------------------------------------------------
template	<class aType>
aType&	ListNodeOf<aType>::GetItem (void)
{
	return myItem;
}

//--------------------------------------------------------------------------

#endif	//	_BW_LIST_NODE_OF_H_
