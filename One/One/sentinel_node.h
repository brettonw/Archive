//------------------------------------------------------------------------------
//	Copyright (C) 1997-2000 Bretton Wade
//	All Rights Reserved
//------------------------------------------------------------------------------

#ifndef		_BW_SENTINEL_NODE_H_
#define		_BW_SENTINEL_NODE_H_

//--------------------------------------------------------------------------
//	class definitions
//--------------------------------------------------------------------------
class	SentinelNode
{
	public:
						SentinelNode (void);
virtual					~SentinelNode (void);
		SentinelNode*	GetNextNode (const SentinelNode* sentinel) const;
		SentinelNode*	GetPreviousNode (const SentinelNode* sentinel) const;
		void			AddToListAfter (SentinelNode* after);
		void			AddToListBefore (SentinelNode* before);
		void			RemoveFromList (void);
	protected:
		SentinelNode*	myNext;
		SentinelNode*	myPrevious;
};

//--------------------------------------------------------------------------

#endif	//	_BW_SENTINEL_NODE_H_
