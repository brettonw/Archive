//-----------------------------------------------------------------------------
// Copyright (C) 2006 Bretton Wade, All Rights Reserved
//-----------------------------------------------------------------------------

#ifndef     _THING_INL_
#define     _THING_INL_

//-----------------------------------------------------------------------------
// includes
//-----------------------------------------------------------------------------
#ifndef     _THING_H_
#include    "thing.h"
#endif  //  _THING_H_

//-----------------------------------------------------------------------------
// inlines
//-----------------------------------------------------------------------------
inline
bool
Thing::IsValid (void) const
{
    return GetPrev () != 0;
}

//-----------------------------------------------------------------------------
inline
Word
Thing::GetID (void) const
{
    return s_cast<Word> (this - s_pThingBase);
}
   
//-----------------------------------------------------------------------------
inline
Thing*
Thing::GetThing (Word id)
{
    return s_pThingBase + id;
}
   
//-----------------------------------------------------------------------------
inline
void
Thing::operator delete (void* ptr)
{
    Thing*  pThing = r_cast<Thing*> (ptr);
    pThing->m_pNext = s_pFreeList;
    pThing->m_pPrev = 0;
    s_pFreeList = pThing;
}
   
//-----------------------------------------------------------------------------

#endif  //  _THING_INL_

