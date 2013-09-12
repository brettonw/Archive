//------------------------------------------------------------------------------
//	Copyright (C) 1997-2000 Bretton Wade
//	All Rights Reserved
//------------------------------------------------------------------------------

#ifndef		_BW_CLIENT_FOR_LIST_OF_H_
#define		_BW_CLIENT_FOR_LIST_OF_H_

//--------------------------------------------------------------------------
//	includes
//--------------------------------------------------------------------------
#ifndef		_BW_ITERATOR_ON_LIST_OF_H_
#include	"iterator_on_list_of.h"
#endif	//	_BW_ITERATOR_ON_LIST_OF_H_

//--------------------------------------------------------------------------
//	class definitions
//--------------------------------------------------------------------------
template	<class aType>
class	ClientForListOf : public IteratorOnListOf<aType>
{
	public:
							ClientForListOf (ListOf<aType>& list);
							ClientForListOf (ListOf<aType>* list);
		void				InsertBeforeCurrentItem (const aType& item);
		void				InsertAfterCurrentItem (const aType& item);
		void				RemoveCurrentItem (void);
};

//--------------------------------------------------------------------------
//	template methods
//--------------------------------------------------------------------------
template	<class aType>
ClientForListOf<aType>::ClientForListOf (ListOf<aType>& list) : IteratorOnListOf<aType> (list)
{
}

//--------------------------------------------------------------------------
template	<class aType>
ClientForListOf<aType>::ClientForListOf (ListOf<aType>* list) : IteratorOnListOf<aType> (list)
{
}

//--------------------------------------------------------------------------
template	<class aType>
void	ClientForListOf<aType>::InsertBeforeCurrentItem (const aType& item)
{
	ListNodeOf<aType>	*tmp = new ListNodeOf<aType> (item);
	tmp->AddToListBefore (myCurrent ? myCurrent : &(myList->mySentinel));
	myCurrent = tmp;
	myList->mySize++;
}

//--------------------------------------------------------------------------
template	<class aType>
void	ClientForListOf<aType>::InsertAfterCurrentItem (const aType& item)
{
	ListNodeOf<aType>	*tmp = new ListNodeOf<aType> (item);
	tmp->AddToListAfter (myCurrent ? myCurrent : &(myList->mySentinel));
	myCurrent = tmp;
	myList->mySize++;
}

//--------------------------------------------------------------------------
template	<class aType>
void	ClientForListOf<aType>::RemoveCurrentItem (void)
{
	assert (myCurrent);
	delete myCurrent;
	GotoHead ();
	myList->mySize--;
}

//--------------------------------------------------------------------------

#endif	//	_BW_CLIENT_FOR_LIST_OF_H_
