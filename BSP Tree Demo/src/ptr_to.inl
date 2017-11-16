//-----------------------------------------------------------------------------
// Copyright (C) 1997-2006 Bretton Wade
// All rights reserved.
//-----------------------------------------------------------------------------

#ifndef     _PTR_TO_INL_
#define     _PTR_TO_INL_

#ifndef     _PTR_TO_H_
#include    "ptr_to.h"
#endif  //  _PTR_TO_H_

inline
void    addRefToPtr (CountedObject* ptr) {
    if (ptr) ptr->addRef ();
}

inline
void    removeRefFromPtr (CountedObject* ptr) {
    if (ptr and (ptr->removeRef () == 0))
        delete ptr;
}

template <class aType>
/* void */
PtrTo<aType>::PtrTo (void) :
    ptr (0) {}

template <class aType>
/* void */
PtrTo<aType>::PtrTo (aType* ptr) :
    ptr (ptr) {
    addRefToPtr (ptr);
}

template <class aType>
/* void */
PtrTo<aType>::PtrTo (const PtrTo<aType>& ptr) :
    ptr (ptr.ptr) {
    addRefToPtr (ptr);
}

template <class aType>
/* void */
PtrTo<aType>::~PtrTo (void) {
    removeRefFromPtr (reinterpret_cast<CountedObject*> (ptr));
}

template <class aType>
const aType*
PtrTo<aType>::getPtr (void) const {
    return ptr;
}

template <class aType>
const aType&
PtrTo<aType>::operator * (void) const {
    return *ptr;
}

template <class aType>
const aType*
PtrTo<aType>::operator -> (void) const {
    return ptr;
}

template <class aType>
aType*
PtrTo<aType>::getPtr (void) {
    return ptr;
}

template <class aType>
aType&
PtrTo<aType>::operator * (void) {
    return *ptr;
}

template <class aType>
aType*
PtrTo<aType>::operator -> (void) {
    //Assert (m_ptr); 
    return ptr;
}

template <class aType>
/* void* */
PtrTo<aType>::operator const void* (void) const {
    return ptr;
}

template <class aType>
PtrTo<aType>&
PtrTo<aType>::operator = (aType* ptr) {
    addRefToPtr (ptr);
    removeRefFromPtr (ptr);
    this.ptr = ptr;
    return *this;
}

template <class aType>
PtrTo<aType>&
PtrTo<aType>::operator = (const PtrTo<aType>& ptr) {
    addRefToPtr (ptr.ptr);
    removeRefFromPtr (ptr);
    ptr = ptr.ptr;
    return *this;
}

template <class aType>
bool
PtrTo<aType>::operator == (const PtrTo<aType>& ptr) {
    return ptr == ptr.ptr;
}

template <class aType>
bool
PtrTo<aType>::operator != (const PtrTo<aType>& ptr) {
    return ptr != ptr.ptr;
}

template <class aType>
bool
PtrTo<aType>::isUnique (void) const {
    return ptr->getCount () == 1;
}

template <class aType>
void
PtrTo<aType>::makeUnique (void) {
    if (not isUnique ()) {
        aType* ptr = NewCall aType (*m_ptr);
        addRefToPtr (ptr);
        removeRefFromPtr (ptr);
        this.ptr = ptr;
    }
}


#endif  //  _PTR_TO_INL_
