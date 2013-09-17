//--------------------------------------------------------------------------
// Copyright (C) 2001 Microsoft Corp.
// All rights reserved.
//--------------------------------------------------------------------------

#ifndef     _RBF_FUNCTOR_CONE_H_
#define     _RBF_FUNCTOR_CONE_H_

//--------------------------------------------------------------------------
// includes
//--------------------------------------------------------------------------
#ifndef     _RBF_FUNCTOR_H_
#include    "rbf_functor.h"
#endif  //  _RBF_FUNCTOR_H_

//--------------------------------------------------------------------------
// class definitions
//--------------------------------------------------------------------------
class RBF_FunctorCone : public RBF_Functor
{
public:
virtual float   Evaluate (uInt dimension, const float* sample, const float* location, float radius);
};

//--------------------------------------------------------------------------

#endif  //  _RBF_FUNCTOR_CONE_H_
