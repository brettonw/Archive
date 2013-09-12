//------------------------------------------------------------------------------
//	Copyright (C) 1997-2000 Bretton Wade
//	All Rights Reserved
//------------------------------------------------------------------------------

#ifndef		_BW_HASH_TABLE_OF_H_
#define		_BW_HASH_TABLE_OF_H_

//--------------------------------------------------------------------------
//	includes
//--------------------------------------------------------------------------
#ifndef		_BW_CLIENT_FOR_LIST_OF_H_
#include	"client_for_list_of.h"
#endif	//	_BW_CLIENT_FOR_LIST_OF_H_

#ifndef		_BW_HASH_TABLE_NODE_OF_H_
#include	"hash_table_node_of.h"
#endif	//	_BW_HASH_TABLE_NODE_OF_H_

#ifndef		_BW_BITS_H_
#include	"bits.h"
#endif	//	_BW_BITS_H_

#ifndef		_BW_HASH_H_
#include	"hash.h"
#endif	//	_BW_HASH_H_

#ifndef		_BW_EXCEPTION_H_
#include	"exception.h"
#endif	//	_BW_EXCEPTION_H_

//--------------------------------------------------------------------------
//	exception definitions
//--------------------------------------------------------------------------
DEFINE_EXCEPTION (KeyNotFound);

//--------------------------------------------------------------------------
//	forward class declarations
//--------------------------------------------------------------------------
template	<class keyType, class aType>
class	IteratorOnHashTableOf;

//--------------------------------------------------------------------------
//	class definitions
//--------------------------------------------------------------------------
template	<class keyType, class aType>
class	HashTableOf
{
friend	class	IteratorOnHashTableOf<keyType, aType>;
	public:
													HashTableOf (uInt maxCost = 3, uInt maxSize = 1 << 15);
virtual												~HashTableOf (void);
		bool										Contains (keyType key) const;
		aType&										GetItem (keyType key) const;
		void										PutItem (keyType key, const aType& item);
		void										RemoveItem (keyType key);
		uInt										GetTableSize (void) const;
	protected:
		void										Resize (void);
	protected:
		uInt										myMaxCost;
		uInt										myMaxSize;
		uInt										myMask;
		ListOf<HashTableNodeOf<keyType, aType> >*	myList;
};

//--------------------------------------------------------------------------
//	template methods
//--------------------------------------------------------------------------
template	<class keyType, class aType>
HashTableOf<keyType, aType>::HashTableOf (uInt maxCost, uInt maxSize)
{
	myMaxCost = maxCost;
	myMaxSize = static_cast<uInt> (HighestBit (static_cast<sInt4> (maxSize)));
	myMask = 0;
	myList = new ListOf<HashTableNodeOf<keyType, aType> >[1];
}

//--------------------------------------------------------------------------
template	<class keyType, class aType>
HashTableOf<keyType, aType>::~HashTableOf (void)
{
	delete[] myList;
}

//--------------------------------------------------------------------------
template	<class keyType, class aType>
bool		HashTableOf<keyType, aType>::Contains (keyType key) const
{
	uInt												listIndex = Hash (key) bitand myMask;
	IteratorOnListOf<HashTableNodeOf<keyType, aType> >	iterator (myList[listIndex]);
	while (not iterator.IsDone ())
	{
		HashTableNodeOf<keyType, aType>*	node = &(iterator.GetCurrentItem ());
		if (node->GetKey () == key)
			return true;
		iterator.GotoNextItem ();
	}
	return false;
}

//--------------------------------------------------------------------------
template	<class keyType, class aType>
aType&		HashTableOf<keyType, aType>::GetItem (keyType key) const
{
	uInt												listIndex = Hash (key) bitand myMask;
	IteratorOnListOf<HashTableNodeOf<keyType, aType> >	iterator (myList[listIndex]);
	while (not iterator.IsDone ())
	{
		HashTableNodeOf<keyType, aType>*	node = &(iterator.GetCurrentItem ());
		if (node->GetKey () == key)
			return node->GetItem ();
		iterator.GotoNextItem ();
	}
	throw KeyNotFound ();
}

//--------------------------------------------------------------------------
template	<class keyType, class aType>
void		HashTableOf<keyType, aType>::PutItem (keyType key, const aType& item)
{
	uInt												listIndex = Hash (key) bitand myMask;
	ClientForListOf<HashTableNodeOf<keyType, aType> >	client (myList[listIndex]);
	while (not client.IsDone ())
	{
		HashTableNodeOf<keyType, aType>*	node = &(client.GetCurrentItem ());
		if (node->GetKey () == key)
		{
			node->SetItem (item);
			return;
		}
		client.GotoNextItem ();
	}
	client.InsertBeforeCurrentItem (HashTableNodeOf<keyType, aType> (key, item));
	if (myList[listIndex].GetSize () > myMaxCost)
		Resize ();
}

//--------------------------------------------------------------------------
template	<class keyType, class aType>
void		HashTableOf<keyType, aType>::RemoveItem (keyType key)
{
	uInt												listIndex = Hash (key) bitand myMask;
	ClientForListOf<HashTableNodeOf<keyType, aType> >	client (myList[listIndex]);
	while (not client.IsDone ())
	{
		HashTableNodeOf<keyType, aType>*	node = &(client.GetCurrentItem ());
		if (node->GetKey () == key)
		{
			client.RemoveCurrentItem ();
			return;
		}
		client.GotoNextItem ();
	}
	throw KeyNotFound ();
}

//--------------------------------------------------------------------------
template	<class keyType, class aType>
uInt4		HashTableOf<keyType, aType>::GetTableSize (void) const
{
	return myMask + 1;
}

//--------------------------------------------------------------------------
template	<class keyType, class aType>
void		HashTableOf<keyType, aType>::Resize (void)
{
	if (myMask + 1 < myMaxSize)
	{
		ListOf<HashTableNodeOf<keyType, aType> >*	oldList = myList;
		uInt										oldSize = myMask + 1;
		myMask = (myMask * 2) + 1;
		myList = new ListOf<HashTableNodeOf<keyType, aType> >[myMask + 1];
		for (uInt i = 0; i < oldSize; i++)
		{
			IteratorOnListOf<HashTableNodeOf<keyType, aType> >	iterator (oldList[i]);
			while (not iterator.IsDone ())
			{
				HashTableNodeOf<keyType, aType>&	node = iterator.GetCurrentItem ();
				PutItem (node.GetKey (), node.GetItem ());
				iterator.GotoNextItem ();
			}
		}
		delete[] oldList;
	}
}

//--------------------------------------------------------------------------

#endif	//	_BW_HASH_TABLE_OF_H_
