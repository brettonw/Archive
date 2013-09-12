//-----------------------------------------------------------------------------
// Copyright (C) 1997-2002 Bretton Wade
// All rights reserved.
//-----------------------------------------------------------------------------

#ifndef     _RESETTER_H_
#define     _RESETTER_H_

//-----------------------------------------------------------------------------
// class definitions
//-----------------------------------------------------------------------------
template <class aType>
class   Resetter
{
    public:
        /* void */              Resetter (aType& targetVariable);
        /* void */              Resetter (aType& targetVariable, aType newValue);
        /* void */              ~Resetter (void);

    protected:
        aType&                  myTargetVariable;
        aType                   myOriginalValue;
};

//-----------------------------------------------------------------------------
// inlines
//-----------------------------------------------------------------------------
#ifndef     _RESETTER_INL_
#include    "resetter.inl"
#endif  //  _RESETTER_INL_

//-----------------------------------------------------------------------------

#endif  //  _RESETTER_H_
