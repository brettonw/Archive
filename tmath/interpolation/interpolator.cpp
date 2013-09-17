//------------------------------------------------------------------------------
// Copyright (C) 2001 Microsoft Corp.
// All rights reserved.
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// includes
//------------------------------------------------------------------------------
#include    "pch.cpp"
#include    "interpolator.h"
#include    "fuzzy_compare.h"

//------------------------------------------------------------------------------
// class methods
//------------------------------------------------------------------------------
/* void */
Interpolator::~Interpolator (void)
{
    // release the memory block used for the static data
    delete[] m_pStaticData;
    
    // release the pointer to the RBF functor
    delete m_rbf;
}

//------------------------------------------------------------------------------
void
Interpolator::ComputeExampleWeights (uInt dimension, const float* location, 
                                        uInt exampleCount, float* exampleWeights) const
{
    assert (dimension == m_dimension);
    assert (exampleCount == m_exampleCount);
    
#ifndef     NDEBUG
    float   sum = 0.0f;
#endif  //  NDEBUG

    // precalculate the rbf values at the sample location
    float*  rbfWeightsByLocation = static_cast<float*> (alloca (m_exampleCount * sizeof (float)));
    for (uInt i = 0; i < m_exampleCount; i++)
        rbfWeightsByLocation[i] = m_rbf->Evaluate (m_dimension, location, &m_exampleLocations (i, 0), m_exampleRadii[i]);
    
    // loop over all the examples
    for (i = 0; i < m_exampleCount; i++)
    {
        // compute the sum of the linear weights and radial weights        
        float   weight = 0.0f;
        weight += EvaluateHyperplane (i, location);
        weight += EvaluateRadialWeights (i, rbfWeightsByLocation);
        
        // store the computed weight
        exampleWeights[i] = weight;

#ifndef     NDEBUG
        sum += weight;
#endif  //  NDEBUG
    }

    assert (FuzzyCompare (sum, 1.0f, 2.0e-1f));
}

//------------------------------------------------------------------------------
/* void */
Interpolator::Interpolator (uInt dimension, uInt exampleCount, RBF_Functor* rbf, float* pData) :
    m_dimension (dimension),
    m_hyperplaneDimension (dimension + 1),
    m_exampleCount (exampleCount),
    m_rbf (rbf),
    m_pStaticData (pData)
{
    // set up the matrices
    m_exampleRadii = m_pStaticData;
    m_exampleLocations.SetDimension (exampleCount, dimension, m_exampleRadii + exampleCount);
    m_hyperplaneCoefficients.SetDimension (exampleCount, m_hyperplaneDimension, m_exampleLocations.GetData () + (dimension * exampleCount));
    m_rbfWeights.SetDimension (exampleCount, exampleCount, m_hyperplaneCoefficients.GetData () + (m_hyperplaneDimension * exampleCount));
}

//------------------------------------------------------------------------------
float
Interpolator::EvaluateHyperplane (uInt example, const float* location) const
{
    return EvaluateHyperplane (m_dimension, &m_hyperplaneCoefficients (example, 0), location);
}

//------------------------------------------------------------------------------
float
Interpolator::EvaluateRadialWeights (uInt example, const float* rbfWeightsByLocation) const
{
    // start with a zero sum
    float   weight = 0.0f;
    
    // loop over all of the examples
    for (uInt i = 0; i < m_exampleCount; i++)
    {
        // accumulate the weighted rbf value at the sample location
        weight += rbfWeightsByLocation[i] * m_rbfWeights (i, example);
    }
    
    // return the summed weights
    return weight;
}

//------------------------------------------------------------------------------
float
Interpolator::EvaluateHyperplane (uInt dimension, const float* hyperplane, const float* location)
{
    assert (dimension > 0);
    
    // start with the standalone hyperplane coordinate
    float   sum = hyperplane[0];

    // loop over all of the coordinates
    for (uInt i = 0; i < dimension; i++)
    {
        // accumulate the location component multiplied through the hyperplane equation
        sum += hyperplane[i + 1] * location[i];
    }
    
    // return the value of the hyperplane evaluated at the location
    return sum;
}

//------------------------------------------------------------------------------
