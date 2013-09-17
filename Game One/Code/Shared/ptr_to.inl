//-----------------------------------------------------------------------------
// Copyright (C) 1997-2006 Bretton Wade
// All rights reserved.
//-----------------------------------------------------------------------------

#ifndef     _PTR_TO_INL_
#define     _PTR_TO_INL_

//-----------------------------------------------------------------------------
// include files
//-----------------------------------------------------------------------------
#ifndef     _PTR_TO_H_
#include    "ptr_to.h"
#endif  //  _PTR_TO_H_

//-----------------------------------------------------------------------------
// inline functions
//-----------------------------------------------------------------------------
inline
void    AddRefToPtr (CountedObject* ptr)
{
    if (ptr) ptr->AddRef ();
}

//-----------------------------------------------------------------------------
inline
void    RemoveRefFromPtr (CountedObject* ptr)
{
    if (ptr and (ptr->RemoveRef () == 0))
        delete ptr;
}

//-----------------------------------------------------------------------------
// template methods
//-----------------------------------------------------------------------------
template <class aType>
/* void */
PtrTo<aType>::PtrTo (void) : 
    m_ptr (0)
{
}

//-----------------------------------------------------------------------------
template <class aType>
/* void */
PtrTo<aType>::PtrTo (aType* ptr) : 
    m_ptr (ptr) 
{ 
    AddRefToPtr (m_ptr); 
}
    
//-----------------------------------------------------------------------------
template <class aType>
/* void */
PtrTo<aType>::PtrTo (const PtrTo<aType>& ptr) : 
    m_ptr (ptr.m_ptr) 
{ 
    AddRefToPtr (m_ptr); 
}
    
//-----------------------------------------------------------------------------
template <class aType>
/* void */
PtrTo<aType>::~PtrTo (void) 
{ 
    RemoveRefFromPtr (m_ptr); 
}

//-----------------------------------------------------------------------------
template <class aType>
const aType*
PtrTo<aType>::GetPtr (void) const 
{ 
    return m_ptr; 
}

//-----------------------------------------------------------------------------
template <class aType>
const aType&
PtrTo<aType>::operator * (void) const 
{ 
    return *m_ptr; 
}

//-----------------------------------------------------------------------------
template <class aType>
const aType*
PtrTo<aType>::operator -> (void) const 
{ 
    return m_ptr; 
}

//-----------------------------------------------------------------------------
template <class aType>
aType*
PtrTo<aType>::GetPtr (void) 
{ 
    return m_ptr; 
}

//-----------------------------------------------------------------------------
template <class aType>
aType&
PtrTo<aType>::operator * (void) 
{ 
    return *m_ptr; 
}

//-----------------------------------------------------------------------------
template <class aType>
aType*
PtrTo<aType>::operator -> (void) 
{ 
    Assert (m_ptr); 
    return m_ptr; 
}

//-----------------------------------------------------------------------------
template <class aType>
/* void* */
PtrTo<aType>::operator const void* (void) const 
{ 
    return m_ptr; 
}

//-----------------------------------------------------------------------------
template <class aType>
PtrTo<aType>&
PtrTo<aType>::operator = (aType* ptr) 
{ 
    AddRefToPtr (ptr); 
    RemoveRefFromPtr (m_ptr); 
    m_ptr = ptr; 
    return *this; 
}

//-----------------------------------------------------------------------------
template <class aType>
PtrTo<aType>&
PtrTo<aType>::operator = (const PtrTo<aType>& ptr)
{ 
    AddRefToPtr (ptr.m_ptr); 
    RemoveRefFromPtr (m_ptr); 
    m_ptr = ptr.m_ptr; 
    return *this; 
}

//-----------------------------------------------------------------------------
template <class aType>
bool
PtrTo<aType>::operator == (const PtrTo<aType>& ptr)
{
    return m_ptr == ptr.m_ptr;
}

//-----------------------------------------------------------------------------
template <class aType>
bool
PtrTo<aType>::operator != (const PtrTo<aType>& ptr)
{
    return m_ptr != ptr.m_ptr;
}

//-----------------------------------------------------------------------------
template <class aType>
bool
PtrTo<aType>::IsUnique (void) const 
{ 
    return m_ptr->GetCount () == 1; 
}

//-----------------------------------------------------------------------------
template <class aType>
void
PtrTo<aType>::MakeUnique (void) 
{ 
    if (not IsUnique ()) 
    { 
        aType* ptr = NewCall aType (*m_ptr); 
        AddRefToPtr (ptr); 
        RemoveRefFromPtr (m_ptr); 
        m_ptr = ptr; 
    } 
}

//-----------------------------------------------------------------------------

#endif  //  _PTR_TO_INL_
