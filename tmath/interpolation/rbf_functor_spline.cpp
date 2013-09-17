//--------------------------------------------------------------------------
// Copyright (C) 2001 Microsoft Corp.
// All rights reserved.
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
// includes
//--------------------------------------------------------------------------
#include    "pch.cpp"
#include    "rbf_functor_spline.h"
#include    "fuzzy_compare.h"

//--------------------------------------------------------------------------
// class methods
//--------------------------------------------------------------------------
float
RBF_FunctorSpline::Evaluate (uInt dimension, const float* sample, const float* location, float radius)
{
    // compute the squared distance from the sample to the cone center
    float   distanceSquared = 0.0f; 
    for (uInt i = 0; i < dimension; i++)
    {
        float   delta = sample[i] - location[i];
        distanceSquared += delta * delta;
    }
    
    // check to see if the sample is at the location and return the center value
    // of the RBF, which is 4.0
    if (distanceSquared < FLOAT_EPSILON)
        return 4.0f;
        
    // compute the distance
    float   distance = sqrtf (distanceSquared);
    
    // compare it to the radius of the RBF
    if (distance >= radius)
        return 0.0f;
        
    // we compute the distance in normalized coordinates now to simplify the
    // math. This value is in the range 0 .. 1.
    float   normalizedDistance = distance / radius;

    // the bspline function is separable, depending on which half of the support
    // the sample lies in.
    if (normalizedDistance > 0.5f)
    {
        // the sample is in the outer half of the support, so we return a
        // value that is converging on the line f(x)=0.0 with C2 continuity
        // as x goes to 1, and with the inner half
        float   u = (1.0f - normalizedDistance) * 2.0f;
        return u * u * u;
    }
    else
    {
        // the sample is in the inner half of the support, so we return
        // a value that is C2 continuous with the outer half, and with 
        // f(x) = 4.0 as x goes to 0
        float   u = (0.5f - normalizedDistance) * 2.0f;
        return 1.0f + ((3.0f * u) * (1 + u - (u * u)));
    }
}

//--------------------------------------------------------------------------
