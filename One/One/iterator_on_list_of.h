//------------------------------------------------------------------------------
//	Copyright (C) 1997-2000 Bretton Wade
//	All Rights Reserved
//------------------------------------------------------------------------------

#ifndef		_BW_ITERATOR_ON_LIST_OF_H_
#define		_BW_ITERATOR_ON_LIST_OF_H_

//--------------------------------------------------------------------------
//	includes
//--------------------------------------------------------------------------
#ifndef		_BW_LIST_OF_H_
#include	"list_of.h"
#endif	//	_BW_LIST_OF_H_

//--------------------------------------------------------------------------
//	class definitions
//--------------------------------------------------------------------------
template	<class aType>
class	IteratorOnListOf
{
	public:
							IteratorOnListOf (ListOf<aType>& list);
							IteratorOnListOf (ListOf<aType>* list);
		void				GotoHead (void);
		void				GotoTail (void);
		void				GotoNextItem (void);
		void				GotoPreviousItem (void);
		aType&				GetCurrentItem (void) const;
		bool				IsDone (void) const;
	protected:
		ListOf<aType>*		myList;
		ListNodeOf<aType>*	myCurrent;
};

//--------------------------------------------------------------------------
//	template methods
//--------------------------------------------------------------------------
template	<class aType>
IteratorOnListOf<aType>::IteratorOnListOf (ListOf<aType>& list)
{
	myList = &list;
	myCurrent = (ListNodeOf<aType>*) myList->mySentinel.GetNextNode (&(myList->mySentinel));
}

//--------------------------------------------------------------------------
template	<class aType>
IteratorOnListOf<aType>::IteratorOnListOf (ListOf<aType>* list)
{
	assert (list);
	myList = list;
	myCurrent = (ListNodeOf<aType>*) myList->mySentinel.GetNextNode (&(myList->mySentinel));
}

//--------------------------------------------------------------------------
template	<class aType>
void	IteratorOnListOf<aType>::GotoHead (void)
{
	myCurrent = (ListNodeOf<aType>*) myList->mySentinel.GetNextNode (&(myList->mySentinel));
}

//--------------------------------------------------------------------------
template	<class aType>
void	IteratorOnListOf<aType>::GotoTail (void)
{
	myCurrent = (ListNodeOf<aType>*) myList->mySentinel.GetPreviousNode (&(myList->mySentinel));
}

//--------------------------------------------------------------------------
template	<class aType>
void	IteratorOnListOf<aType>::GotoNextItem (void)
{
	assert (myCurrent);
	myCurrent = myCurrent->GetNextNode (&(myList->mySentinel));
}

//--------------------------------------------------------------------------
template	<class aType>
void	IteratorOnListOf<aType>::GotoPreviousItem (void)
{
	assert (myCurrent);
	myCurrent = myCurrent->GetPreviousNode (&(myList->mySentinel));
}

//--------------------------------------------------------------------------
template	<class aType>
aType&	IteratorOnListOf<aType>::GetCurrentItem (void) const
{
	assert (myCurrent);
	return myCurrent->GetItem ();
}

//--------------------------------------------------------------------------
template	<class aType>
bool	IteratorOnListOf<aType>::IsDone (void) const
{
	return myCurrent ? false : true;
}

//--------------------------------------------------------------------------

#endif	//	_BW_ITERATOR_ON_LIST_OF_H_
