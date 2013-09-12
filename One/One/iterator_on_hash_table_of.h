//------------------------------------------------------------------------------
//	Copyright (C) 1997-2000 Bretton Wade
//	All Rights Reserved
//------------------------------------------------------------------------------

#ifndef		_BW_ITERATOR_ON_HASH_TABLE_OF_H_
#define		_BW_ITERATOR_ON_HASH_TABLE_OF_H_

//--------------------------------------------------------------------------
//	includes
//--------------------------------------------------------------------------
#ifndef		_BW_HASH_TABLE_OF_H_
#include	"hash_table_of.h"
#endif	//	_BW_HASH_TABLE_OF_H_

//--------------------------------------------------------------------------
//	class definitions
//--------------------------------------------------------------------------
template	<class keyType, class aType>
class	IteratorOnHashTableOf
{
	public:
															IteratorOnHashTableOf (HashTableOf<keyType, aType>& table);
															IteratorOnHashTableOf (HashTableOf<keyType, aType>* table);
															~IteratorOnHashTableOf (void);
		void												GotoHead (void);
		void												GotoNextItem (void);
		const keyType&										GetCurrentKey (void) const;
		aType&												GetCurrentItem (void) const;
		bool												IsDone (void) const;
	private:
		HashTableOf<keyType, aType>*						myTable;
		uInt												myCurrentIndex;
		IteratorOnListOf<HashTableNodeOf<keyType, aType> >*	myIterator;
};

//--------------------------------------------------------------------------
//	template methods
//--------------------------------------------------------------------------
template	<class keyType, class aType>
IteratorOnHashTableOf<keyType, aType>::IteratorOnHashTableOf (HashTableOf<keyType, aType>& table)
{
	myTable = &table;
	myIterator = 0;
	GotoHead ();
}

//--------------------------------------------------------------------------
template	<class keyType, class aType>
IteratorOnHashTableOf<keyType, aType>::IteratorOnHashTableOf (HashTableOf<keyType, aType>* table)
{
	assert (table);
	myTable = table;
	myIterator = 0;
	GotoHead ();
}

//--------------------------------------------------------------------------
template	<class keyType, class aType>
IteratorOnHashTableOf<keyType, aType>::~IteratorOnHashTableOf (void)
{
	delete myIterator;
}

//--------------------------------------------------------------------------
template	<class keyType, class aType>
void	IteratorOnHashTableOf<keyType, aType>::GotoHead (void)
{
	myCurrentIndex = 0;
	do
	{
		delete myIterator;
		myIterator = new IteratorOnListOf<HashTableNodeOf<keyType, aType> > (myTable->myList[myCurrentIndex++]);
	}
	while (myIterator->IsDone () and (myCurrentIndex < myTable->GetTableSize ()));
}

//--------------------------------------------------------------------------
template	<class keyType, class aType>
void	IteratorOnHashTableOf<keyType, aType>::GotoNextItem (void)
{
	myIterator->GotoNextItem ();
	while (myIterator->IsDone () and (myCurrentIndex < myTable->GetTableSize ()))
	{
		delete myIterator;
		myIterator = new IteratorOnListOf<HashTableNodeOf<keyType, aType> > (myTable->myList[myCurrentIndex++]);
	}
}

//--------------------------------------------------------------------------
template	<class keyType, class aType>
const keyType&	IteratorOnHashTableOf<keyType, aType>::GetCurrentKey (void) const
{
	assert (not myIterator->IsDone ());
	return myIterator->GetCurrentItem ().GetKey ();
}

//--------------------------------------------------------------------------
template	<class keyType, class aType>
aType&	IteratorOnHashTableOf<keyType, aType>::GetCurrentItem (void) const
{
	assert (not myIterator->IsDone ());
	return myIterator->GetCurrentItem ().GetItem ();
}

//--------------------------------------------------------------------------
template	<class keyType, class aType>
bool	IteratorOnHashTableOf<keyType, aType>::IsDone (void) const
{
	return myIterator->IsDone ();
}

//--------------------------------------------------------------------------

#endif	//	_BW_ITERATOR_ON_HASH_TABLE_OF_H_
