//-----------------------------------------------------------------------------
// Copyright (C) 1997-2006 Bretton Wade
// All rights reserved.
//-----------------------------------------------------------------------------

#ifndef     _RESETTER_INL_
#define     _RESETTER_INL_

//-----------------------------------------------------------------------------
// include files
//-----------------------------------------------------------------------------
#ifndef     _RESETTER_H_
#include    "resetter.h"
#endif  //  _RESETTER_H_

//-----------------------------------------------------------------------------
// class methods
//-----------------------------------------------------------------------------
template <class aType>
/* void */
Resetter<aType>::Resetter (aType& targetVariable) :
    m_targetVariable (targetVariable),
    m_originalValue (targetVariable)
{
}

//-----------------------------------------------------------------------------
template <class aType>
/* void */
Resetter<aType>::Resetter (aType& targetVariable, aType newValue) :
    m_targetVariable (targetVariable),
    m_originalValue (targetVariable)
{
    targetVariable = newValue;
}

//-----------------------------------------------------------------------------
template <class aType>
/* void */
Resetter<aType>::~Resetter (void)
{
    m_targetVariable = m_originalValue;
}

//-----------------------------------------------------------------------------

#endif  //  _RESETTER_INL_
