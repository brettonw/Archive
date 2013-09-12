//------------------------------------------------------------------------------
//	Copyright (C) 1997-2000 Bretton Wade
//	All Rights Reserved
//------------------------------------------------------------------------------

#ifndef		_BW_HANDLE_TO_H_
#define		_BW_HANDLE_TO_H_

//--------------------------------------------------------------------------
//	includes
//--------------------------------------------------------------------------
#ifndef		_BW_HANDLE_H_
#include	"handle.h"
#endif	//	_BW_HANDLE_H_

//--------------------------------------------------------------------------
//	class definitions
//--------------------------------------------------------------------------
template	<class aType>
class	HandleTo
{
	public:
							HandleTo (void);
							HandleTo (aType* ptr);
							HandleTo (const HandleTo<aType>& handle);
virtual						~HandleTo (void);
							operator aType* (void) const;
		aType&				operator * (void) const;
		aType*				operator -> (void) const;
		aType*				GetPtr (void) const;
		HandleTo<aType>&	operator = (aType* ptr);
		HandleTo<aType>&	operator = (const HandleTo<aType>& handle);
		bool				IsUnique (void) const;
		void				MakeUnique (void);
	protected:
		void				AddRef (void) const;
		void				RemoveRef (void) const;
	protected:
		Handle*				myHandle;
};

//--------------------------------------------------------------------------
//	template methods
//--------------------------------------------------------------------------
template	<class aType>
HandleTo<aType>::HandleTo (void)
{
	myHandle = 0;
}

//--------------------------------------------------------------------------
template	<class aType>
HandleTo<aType>::HandleTo (aType* ptr)
{
	if (ptr)
	{
		myHandle = new Handle (ptr);
		AddRef ();
	}
	else
		myHandle = 0;
}

//--------------------------------------------------------------------------
template	<class aType>
HandleTo<aType>::HandleTo (const HandleTo<aType>& handle)
{
	handle.AddRef ();
	myHandle = handle.myHandle;
}

//--------------------------------------------------------------------------
template	<class aType>
HandleTo<aType>::~HandleTo (void)
{
	RemoveRef ();
}

//--------------------------------------------------------------------------
template	<class aType>
HandleTo<aType>::operator aType* () const
{
	return myHandle ? static_cast<aType*> (myHandle->GetPtr ()) : 0;
}

//--------------------------------------------------------------------------
template	<class aType>
aType&			HandleTo<aType>::operator * (void) const
{
	return *static_cast<aType*> (myHandle->GetPtr ());
}

//--------------------------------------------------------------------------
template	<class aType>
aType*			HandleTo<aType>::operator -> (void) const
{
	return static_cast<aType*> (myHandle->GetPtr ());
}

//--------------------------------------------------------------------------
template	<class aType>
aType*	HandleTo<aType>::GetPtr (void) const
{
	return static_cast<aType*> (myHandle->GetPtr ());
}

//--------------------------------------------------------------------------
template	<class aType>
HandleTo<aType>&	HandleTo<aType>::operator = (aType *ptr)
{
	RemoveRef ();
	if (ptr)
		myHandle = new Handle (ptr);
	else
		myHandle = 0;
	return *this;
}

//--------------------------------------------------------------------------
template	<class aType>
HandleTo<aType>&	HandleTo<aType>::operator = (const HandleTo<aType> &handle)
{
	handle.AddRef ();
	RemoveRef ();
	myHandle = handle.myHandle;
	return *this;
}

//--------------------------------------------------------------------------
template	<class aType>
bool			HandleTo<aType>::IsUnique (void) const
{
	return bool (myHandle->GetCount () == 1);
}

//--------------------------------------------------------------------------
template	<class aType>
void			HandleTo<aType>::MakeUnique (void)
{
	if (not IsUnique ())
	{
		Handle*	ptr = new Handle (new aType (*static_cast<aType*> (myHandle->GetPtr ())));
		RemoveRef ();
		myHandle = ptr;
	}
}

//--------------------------------------------------------------------------
template	<class aType>
void	HandleTo<aType>::AddRef (void) const
{
	if (myHandle)
		myHandle->AddRef ();
}

//--------------------------------------------------------------------------
template	<class aType>
void	HandleTo<aType>::RemoveRef (void) const
{
	if (myHandle)
		if (not myHandle->RemoveRef ())
		{
			delete static_cast<aType*> (myHandle->GetPtr ());
			delete myHandle;
		}
}

//--------------------------------------------------------------------------

#endif	//	_BW_HANDLE_TO_H_
