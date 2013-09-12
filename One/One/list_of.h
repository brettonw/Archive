//------------------------------------------------------------------------------
//	Copyright (C) 1997-2000 Bretton Wade
//	All Rights Reserved
//------------------------------------------------------------------------------

#ifndef		_BW_LIST_OF_H_
#define		_BW_LIST_OF_H_

//--------------------------------------------------------------------------
//	includes
//--------------------------------------------------------------------------
#ifndef		_BW_LIST_NODE_OF_H_
#include	"list_node_of.h"
#endif	//	_BW_LIST_NODE_OF_H_

//--------------------------------------------------------------------------
//	forward class declarations
//--------------------------------------------------------------------------
template	<class aType>
class	IteratorOnListOf;

template	<class aType>
class	ClientForListOf;

//--------------------------------------------------------------------------
//	class definitions
//--------------------------------------------------------------------------
template	<class aType>
class	ListOf
{
friend	class	IteratorOnListOf<aType>;
friend	class	ClientForListOf<aType>;
	public:
						ListOf (void);
						~ListOf (void);
		void			AddToHead (const aType& item);
		void			AddToTail (const aType& item);
		void			RemoveFromHead (void);
		void			RemoveFromTail (void);
		void			RemoveAll (void);
		uInt			GetSize (void) const;
		bool			IsEmpty (void) const;
	private:
		SentinelNode	mySentinel;
		uInt			mySize;
};

//--------------------------------------------------------------------------
//	template methods
//--------------------------------------------------------------------------
template	<class aType>
ListOf<aType>::ListOf (void)
{
	mySize = 0;
}

//--------------------------------------------------------------------------
template	<class aType>
ListOf<aType>::~ListOf (void)
{
	RemoveAll ();
}

//--------------------------------------------------------------------------
template	<class aType>
void		ListOf<aType>::AddToHead (const aType& item)
{
	ListNodeOf<aType>*	tmp = new ListNodeOf<aType> (item);
	tmp->AddToListAfter (&mySentinel);
	mySize++;
}

//--------------------------------------------------------------------------
template	<class aType>
void		ListOf<aType>::AddToTail (const aType& item)
{
	ListNodeOf<aType>*	tmp = new ListNodeOf<aType> (item);
	tmp->AddToListBefore (&mySentinel);
	mySize++;
}

//--------------------------------------------------------------------------
template	<class aType>
void		ListOf<aType>::RemoveFromHead (void)
{
	ListNodeOf<aType>*	tmp = (ListNodeOf<aType>*) mySentinel.GetNextNode (&mySentinel);
	assert (tmp);
	delete tmp;
	mySize--;
}

//--------------------------------------------------------------------------
template	<class aType>
void		ListOf<aType>::RemoveFromTail (void)
{
	ListNodeOf<aType>*	tmp = (ListNodeOf<aType>*) mySentinel.GetPreviousNode (&mySentinel);
	assert (tmp);
	delete tmp;
	mySize--;
}

//--------------------------------------------------------------------------
template	<class aType>
void		ListOf<aType>::RemoveAll (void)
{
	SentinelNode*	tmp;
	while ((tmp = mySentinel.GetNextNode (&mySentinel)) != 0)
		delete tmp;
	mySize = 0;
}

//--------------------------------------------------------------------------
template	<class aType>
uInt		ListOf<aType>::GetSize (void) const
{
	return	mySize;
}

//--------------------------------------------------------------------------
template	<class aType>
bool		ListOf<aType>::IsEmpty (void) const
{
	return (mySize > 0) ? false : true;
}

//--------------------------------------------------------------------------

#endif	//	_BW_LIST_OF_H_
