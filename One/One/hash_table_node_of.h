//------------------------------------------------------------------------------
//	Copyright (C) 1997-2000 Bretton Wade
//	All Rights Reserved
//------------------------------------------------------------------------------

#ifndef		_BW_HASH_TABLE_NODE_OF_H_
#define		_BW_HASH_TABLE_NODE_OF_H_

//--------------------------------------------------------------------------
//	class definitions
//--------------------------------------------------------------------------
template	<class keyType, class aType>
class	HashTableNodeOf
{
	public:
		HashTableNodeOf (keyType key, const aType& item);
		HashTableNodeOf (const HashTableNodeOf<keyType, aType>& node);
		const keyType&	GetKey (void) const;
		aType&			GetItem (void);
		void			SetItem (const aType& item);
	protected:
		keyType			myKey;
		aType			myItem;
};

//--------------------------------------------------------------------------
//	template methods
//--------------------------------------------------------------------------
template	<class keyType, class aType>
HashTableNodeOf<keyType, aType>::HashTableNodeOf (keyType key, const aType& item) : myKey (key), myItem (item)
{
}

//--------------------------------------------------------------------------
template	<class keyType, class aType>
HashTableNodeOf<keyType, aType>::HashTableNodeOf (const HashTableNodeOf<keyType, aType>& node) : myKey (node.myKey), myItem (node.myItem)
{
}

//--------------------------------------------------------------------------
template	<class keyType, class aType>
const	keyType&	HashTableNodeOf<keyType, aType>::GetKey (void) const
{
	return myKey;
}

//--------------------------------------------------------------------------
template	<class keyType, class aType>
aType&		HashTableNodeOf<keyType, aType>::GetItem (void)
{
	return myItem;
}

//--------------------------------------------------------------------------
template	<class keyType, class aType>
void		HashTableNodeOf<keyType, aType>::SetItem (const aType& item)
{
	myItem = item;
}

//--------------------------------------------------------------------------

#endif	//	_BW_HASH_TABLE_NODE_OF_H_
