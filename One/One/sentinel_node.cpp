//------------------------------------------------------------------------------
//	Copyright (C) 1997-2000 Bretton Wade
//	All Rights Reserved
//------------------------------------------------------------------------------

//--------------------------------------------------------------------------
//	includes
//--------------------------------------------------------------------------
#include	"precompile.h"
#include	"sentinel_node.h"

//--------------------------------------------------------------------------
SentinelNode::SentinelNode
(
	void
)
{
	myNext = myPrevious = this;
}

//--------------------------------------------------------------------------
SentinelNode::~SentinelNode
(
	void
)
{
	assert ((myPrevious == this) and (myNext == this));
}

//--------------------------------------------------------------------------
SentinelNode*
SentinelNode::GetNextNode
(
	const SentinelNode*	sentinel
) const
{
	return (myNext != sentinel) ? myNext : 0;
}

//--------------------------------------------------------------------------
SentinelNode*
SentinelNode::GetPreviousNode
(
	const SentinelNode* sentinel
) const
{
	return (myPrevious != sentinel) ? myPrevious : 0;
}

//--------------------------------------------------------------------------
void
SentinelNode::AddToListAfter
(
	SentinelNode*	after
)
{
	assert (after);
	myNext = after->myNext;
	myPrevious = after;
	after->myNext = this;
	myNext->myPrevious = this;
}

//--------------------------------------------------------------------------
void
SentinelNode::AddToListBefore
(
	SentinelNode*	before
)
{
	assert (before);
	myNext = before;
	myPrevious = before->myPrevious;
	myPrevious->myNext = this;
	before->myPrevious = this;
}

//--------------------------------------------------------------------------
void
SentinelNode::RemoveFromList
(
	void
)
{
	myNext->myPrevious = myPrevious;
	myPrevious->myNext = myNext;
	myNext = myPrevious = this;
}

//--------------------------------------------------------------------------
