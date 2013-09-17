//--------------------------------------------------------------------------
// Copyright (C) 2001 Microsoft Corp.
// All rights reserved.
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
// includes
//--------------------------------------------------------------------------
#include    "pch.cpp"
#include    "rbf_functor_cone.h"
#include    "fuzzy_compare.h"

//--------------------------------------------------------------------------
// class methods
//--------------------------------------------------------------------------
float
RBF_FunctorCone::Evaluate (uInt dimension, const float* sample, const float* location, float radius)
{
    // compute the squared distance from the sample to the cone center
    float   distanceSquared = 0.0f; 
    for (uInt i = 0; i < dimension; i++)
    {
        float   delta = sample[i] - location[i];
        distanceSquared += delta * delta;
    }
    
    // check to see if the sample is at the location
    if (distanceSquared < FLOAT_EPSILON)
        return radius;
        
    // compute the distance
    float   distance = sqrtf (distanceSquared);
    
    // compare it to the radius of the RBF
    if (distance >= radius)
        return 0.0f;
        
    // this is a linear filter kernel with limited support
    return radius - distance;
}

//--------------------------------------------------------------------------
