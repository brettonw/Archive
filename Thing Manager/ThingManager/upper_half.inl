//-----------------------------------------------------------------------------
// Copyright (C) 2006 Bretton Wade, All Rights Reserved
//-----------------------------------------------------------------------------

#ifndef     _UPPER_HALF_INL_
#define     _UPPER_HALF_INL_

//-----------------------------------------------------------------------------
// includes
//-----------------------------------------------------------------------------
#ifndef     _UPPER_HALF_H_
#include    "upper_half.h"
#endif  //  _UPPER_HALF_H_

//-----------------------------------------------------------------------------
// inlines
//-----------------------------------------------------------------------------
inline
/* void */
UpperHalf::UpperHalf (const Thing* pThing, Word axis, bool bIsUpper) :
    m_ID (pThing->GetID ()),
    m_axis (axis),
    m_isUpper (bIsUpper)
{
    // in case we have more things than can be represented in 15 bits
    Assert (pThing->GetID () <= m_ID);  
}

//-----------------------------------------------------------------------------
inline
/* void */      
UpperHalf::UpperHalf (const UpperHalf& half) :
    m_half (half.m_half)
{
}

//-----------------------------------------------------------------------------
inline
Word            
UpperHalf::GetID (void) const
{
    return m_ID;
}

//-----------------------------------------------------------------------------
inline
Thing*          
UpperHalf::GetThing (void) const
{
    return Thing::GetThing (m_ID);
}

//-----------------------------------------------------------------------------
inline
bool            
UpperHalf::IsUpper (void) const
{
    return m_isUpper;
}

//-----------------------------------------------------------------------------
inline
float
UpperHalf::GetValue (void) const
{
    return GetThing ()->m_bound[m_axis][m_isUpper ? UPPER_BOUND : LOWER_BOUND];
}

//-----------------------------------------------------------------------------
inline
UpperHalf&
UpperHalf::operator = (const UpperHalf& half)
{
    m_half = half.m_half;
    return *this;
}

//-----------------------------------------------------------------------------
inline
bool
UpperHalf::operator > (const UpperHalf& half) const
{
    if (m_ID != half.m_ID)
        return GetValue () > half.GetValue ();
    return m_isUpper;
}

//-----------------------------------------------------------------------------

#endif  //  _UPPER_HALF_INL_

