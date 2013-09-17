//--------------------------------------------------------------------------
// Copyright (C) 2001 Microsoft Corp.
// All rights reserved.
//--------------------------------------------------------------------------

#ifndef     _RBF_FUNCTOR_H_
#define     _RBF_FUNCTOR_H_

//--------------------------------------------------------------------------
// class definitions
//--------------------------------------------------------------------------
class RBF_Functor
{
public:
virtual float   Evaluate (uInt dimension, const float* sample, const float* location, float radius) = 0;
};

//--------------------------------------------------------------------------

#endif  //  _RBF_FUNCTOR_H_
