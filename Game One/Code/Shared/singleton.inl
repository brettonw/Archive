//-----------------------------------------------------------------------------
// Copyright (C) 1997-2002 Bretton Wade
// All rights reserved.
//-----------------------------------------------------------------------------

#ifndef     _SINGLETON_INL_
#define     _SINGLETON_INL_

//-----------------------------------------------------------------------------
// include files
//-----------------------------------------------------------------------------
#ifndef     _SINGLETON_H_
#include    "singleton.h"
#endif  //  _SINGLETON_H_

//-----------------------------------------------------------------------------
// template static variables
//-----------------------------------------------------------------------------
template <class aType>
aType*  Singleton<aType>::s_pSingleton = 0;

//-----------------------------------------------------------------------------
// template methods
//-----------------------------------------------------------------------------
template <class aType>
/* void */
Singleton<aType>::Singleton (void)
{
    Assert (s_pSingleton == 0);
    
    // assuming that this isn't multiply inherited with other
    // objects that contain data, we cast down the tree.
    // XXX it might be better to turn on RTTI and assert this
    // XXX is a singleton<aType>, then use dynamic_cast, or
    // XXX rather assert that the dynamic cast was asuccessful.
    s_pSingleton = static_cast<aType*> (this);
}

//-----------------------------------------------------------------------------
template <class aType>
/* void */
Singleton<aType>::~Singleton (void)
{
    // make sure that we can never use this again without crashing
    s_pSingleton = reinterpret_cast<aType*> (-1);
}

//-----------------------------------------------------------------------------
template <class aType>
bool
Singleton<aType>::IsValid (void)
{
    return (s_pSingleton != 0);
}

//-----------------------------------------------------------------------------
template <class aType>
aType*
Singleton<aType>::GetSingleton (void)
{
    Assert (IsValid ());
    return s_pSingleton;
}

//-----------------------------------------------------------------------------

#endif  //  _SINGLETON_INL_
