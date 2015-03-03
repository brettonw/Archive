//-----------------------------------------------------------------------------
// Copyright (C) 2006 Bretton Wade, All Rights Reserved
//-----------------------------------------------------------------------------

#ifndef     _MASH_INL_
#define     _MASH_INL_

//-----------------------------------------------------------------------------
// includes
//-----------------------------------------------------------------------------
#ifndef     _MASH_H_
#include    "mash.h"
#endif  //  _MASH_H_

//-----------------------------------------------------------------------------
// inlines
//-----------------------------------------------------------------------------
inline
void
Mash::Assign (Word IDA, Word IDB)
{
    if (IDA < IDB)
    {
        m_IDA = IDA;
        m_IDB = IDB;
    }
    else
    {
        m_IDA = IDB;
        m_IDB = IDA;
    }
}

//-----------------------------------------------------------------------------
inline
/* void */
Mash::Mash (Word IDA, Word IDB)
{
    Assign (IDA, IDB);
}

//-----------------------------------------------------------------------------
inline
/* void */      
Mash::Mash (const Thing* pThingA, const Thing* pThingB)
{
    Assign (pThingA->GetID (), pThingB->GetID ());
}

//-----------------------------------------------------------------------------
inline
/* void */      
Mash::Mash (const Mash& mash) : 
    m_word (mash.m_word)
{
}

//-----------------------------------------------------------------------------
inline
Half            
Mash::GetIDA (void) const
{
    return m_IDA;
}

//-----------------------------------------------------------------------------
inline
Half            
Mash::GetIDB (void) const
{
    return m_IDB;
}

//-----------------------------------------------------------------------------
inline
Thing*            
Mash::GetThingA (void) const
{
    return Thing::GetThing (m_IDA);
}

//-----------------------------------------------------------------------------
inline
Thing*            
Mash::GetThingB (void) const
{
    return Thing::GetThing (m_IDB);
}

//-----------------------------------------------------------------------------
inline
Mash&           
Mash::operator = (const Mash& mash)
{
    m_word = mash.m_word;
    return *this;
}

//-----------------------------------------------------------------------------
inline
bool            
Mash::operator < (const Mash& mash) const
{
    return m_word < mash.m_word;
}

//-----------------------------------------------------------------------------
inline
bool            
Mash::operator == (const Mash& mash) const
{
    return m_word == mash.m_word;
}

//-----------------------------------------------------------------------------
inline
bool            
Mash::operator != (const Mash& mash) const
{
    return m_word != mash.m_word;
}

//-----------------------------------------------------------------------------

#endif  //  _MASH_INL_

