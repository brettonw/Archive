//-----------------------------------------------------------------------------
// Copyright (C) 1997-2002 Bretton Wade
// All rights reserved.
//-----------------------------------------------------------------------------

#ifndef     _RESETTER_INL_
#define     _RESETTER_INL_

//-----------------------------------------------------------------------------
// includes
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
    myTargetVariable (targetVariable),
    myOriginalValue (targetVariable)
{
}

//-----------------------------------------------------------------------------
template <class aType>
/* void */
Resetter<aType>::Resetter (aType& targetVariable, aType newValue) :
    myTargetVariable (targetVariable),
    myOriginalValue (targetVariable)
{
    targetVariable = newValue;
}

//-----------------------------------------------------------------------------
template <class aType>
/* void */
Resetter<aType>::~Resetter (void)
{
    myTargetVariable = myOriginalValue;
}

//-----------------------------------------------------------------------------

#endif  //  _RESETTER_INL_
