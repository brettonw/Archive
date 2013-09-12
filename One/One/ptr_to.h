//------------------------------------------------------------------------------
//	Copyright (C) 1997-2000 Bretton Wade
//	All Rights Reserved
//------------------------------------------------------------------------------

#ifndef		_BW_PTR_TO_H_
#define		_BW_PTR_TO_H_

//--------------------------------------------------------------------------
//	includes
//--------------------------------------------------------------------------
#ifndef		_BW_COUNTED_OBJECT_H_
#include	"counted_object.h"
#endif	//	_BW_COUNTED_OBJECT_H_

//--------------------------------------------------------------------------
//	class definitions
//--------------------------------------------------------------------------
template	<class aType>
class	PtrTo
{
	public:
							PtrTo (void);
							PtrTo (aType* ptr);
							PtrTo (const PtrTo<aType>& ptr);
virtual						~PtrTo (void);
							operator aType* (void) const;
		aType&				operator * (void) const;
		aType*				operator -> (void) const;
		PtrTo<aType>&		operator = (aType* ptr);
		PtrTo<aType>&		operator = (const PtrTo<aType>& ptr);
		bool				IsUnique (void) const;
		void				MakeUnique (void);
	protected:
		aType*				myPtr;
};

//--------------------------------------------------------------------------
//	inline functions
//--------------------------------------------------------------------------
inline
void	AddRefToPtr (CountedObject* ptr)
{
	if (ptr) ptr->AddRef ();
}

//--------------------------------------------------------------------------
inline
void	RemoveRefFromPtr (CountedObject* ptr)
{
	if (ptr and (ptr->RemoveRef () == 0)) delete ptr;
}

//--------------------------------------------------------------------------
//	template methods
//--------------------------------------------------------------------------
template	<class aType>
PtrTo<aType>::PtrTo (void)
{
	myPtr = 0;
}

//--------------------------------------------------------------------------
template	<class aType>
PtrTo<aType>::PtrTo (aType* ptr)
{
	myPtr = ptr;
	AddRefToPtr (myPtr);
}

//--------------------------------------------------------------------------
template	<class aType>
PtrTo<aType>::PtrTo (const PtrTo<aType>& ptr)
{
	myPtr = ptr.myPtr;
	AddRefToPtr (myPtr);
}

//--------------------------------------------------------------------------
template	<class aType>
PtrTo<aType>::~PtrTo (void)
{
	RemoveRefFromPtr (myPtr);
}

//--------------------------------------------------------------------------
template	<class aType>
PtrTo<aType>::operator aType* () const
{
	return myPtr;
}

//--------------------------------------------------------------------------
template	<class aType>
aType&			PtrTo<aType>::operator * (void) const
{
	return *myPtr;
}

//--------------------------------------------------------------------------
template	<class aType>
aType*			PtrTo<aType>::operator -> (void) const
{
	return myPtr;
}

//--------------------------------------------------------------------------
template	<class aType>
PtrTo<aType>&	PtrTo<aType>::operator = (aType *ptr)
{
	AddRefToPtr (ptr);
	RemoveRefFromPtr (myPtr);
	myPtr = ptr;
	return *this;
}

//--------------------------------------------------------------------------
template	<class aType>
PtrTo<aType>&	PtrTo<aType>::operator = (const PtrTo<aType> &ptr)
{
	AddRefToPtr (ptr.myPtr);
	RemoveRefFromPtr (myPtr);
	myPtr = ptr.myPtr;
	return *this;
}

//--------------------------------------------------------------------------
template	<class aType>
bool			PtrTo<aType>::IsUnique (void) const
{
	return myPtr->GetCount () == 1;
}

//--------------------------------------------------------------------------
template	<class aType>
void			PtrTo<aType>::MakeUnique (void)
{
	if (not IsUnique ())
	{
		aType*	ptr = new aType (*myPtr);
		AddRefToPtr (ptr);
		RemoveRefFromPtr (myPtr);
		myPtr = ptr;
	}
}

//--------------------------------------------------------------------------

#endif	//	_BW_PTR_TO_H_
