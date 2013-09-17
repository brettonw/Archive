//------------------------------------------------------------------------------
// Copyright (C) 2001 Microsoft Corp.
// All rights reserved.
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// includes
//------------------------------------------------------------------------------
#include    "pch.cpp"
#include    "interpolator_factory.h"
#include    "fuzzy_compare.h"
#include    "msr_svd.h"
#include    "msr_svd_templates.h"
#include    "util.h"

//------------------------------------------------------------------------------
// class methods
//------------------------------------------------------------------------------
/* void */
InterpolatorFactory::InterpolatorFactory (void) :
    m_pData (0)
{
}

//------------------------------------------------------------------------------
/* void */
InterpolatorFactory::~InterpolatorFactory (void)
{
    // deallocate the memory block if it has been allocated
    assert (not m_pData);
    delete[] m_pData;
}

//------------------------------------------------------------------------------
PtrToInterpolator
InterpolatorFactory::CreateInterpolator (const PtrToXMLNode& xmlNode, RBF_Functor* rbf)
{
    // store the RBF
    m_rbf = rbf;

    // fetch the data lists and read the dimension and example count from them
    const XMLNodeList*          adverbList = xmlNode->GetChildren ("Adverb");
    const XMLNodeList*          exampleList = xmlNode->GetChildren ("Example");
    m_dimension = adverbList->size ();
    m_exampleCount = exampleList->size ();

    // compute the sizes and allocate the memory block
    AllocateMemory ();
    
    // read over each example node, building the example
    XMLNodeList::const_iterator exampleIterator = exampleList->begin ();
    for (uInt i = 0; i < m_exampleCount; i++)
    {
        // extract the example location, not that this assumes that the locations
        // are in the same order they are declared in the adverb list
        PtrToXMLNode        exampleNode = *exampleIterator++;
        const XMLNodeList*  locationList = exampleNode->GetChildren ("Location");
        assert (locationList->size () == m_dimension);
        XMLNodeList::const_iterator adverbIterator = adverbList->begin ();
        XMLNodeList::const_iterator locationIterator = locationList->begin ();
        for (uInt j = 0; j < m_dimension; j++)
        {
            PtrToXMLNode    adverbNode = *adverbIterator++;
            PtrToXMLNode    locationNode = *locationIterator++;
            assert (adverbNode->GetAttribute ("name") == locationNode->GetAttribute ("name"));
            m_exampleLocations (i, j) = (locationNode->GetAttribute ("value")).ConvertToFloat ();
        }
    }

    // extract the radius scale
    float                       scale = 0.5f;
    Text                        scaleText = xmlNode->GetAttribute ("radius_scale");
    if (scaleText != Text ())
        scale = scaleText.ConvertToFloat ();

    // create the interpolator
    return CreateInterpolator (scale);
}

//------------------------------------------------------------------------------
void
InterpolatorFactory::AllocateMemory (void)
{
    assert (m_exampleCount >= 2);
    assert (m_dimension > 0);
    assert (m_rbf);
    
    // figure out the dimension of the hyperplane
    m_hyperplaneDimension = m_dimension + 1;
    
    // figure out how big the data block should be
    uInt    dataSize = m_exampleCount + 
                        (m_dimension * m_exampleCount) + 
                        (m_hyperplaneDimension * m_exampleCount) + 
                        (m_exampleCount * m_exampleCount);
                        
    // allocate the block
    m_pData = new float[dataSize];
    
    // set up the matrices
    m_exampleRadii = m_pData;
    m_exampleLocations.SetDimension (m_exampleCount, m_dimension, m_exampleRadii + m_exampleCount);
    m_hyperplaneCoefficients.SetDimension (m_exampleCount, m_hyperplaneDimension, m_exampleLocations.GetData () + (m_dimension * m_exampleCount));
    m_rbfWeights.SetDimension (m_exampleCount, m_exampleCount, m_hyperplaneCoefficients.GetData () + (m_hyperplaneDimension * m_exampleCount));
}

//------------------------------------------------------------------------------
PtrToInterpolator
InterpolatorFactory::CreateInterpolator (float scale)
{
    assert (m_pData);
    
    // Calculate the radius to use for each example
    CalculateExampleRadii (scale);
    
    // solve the systems
    SolveForHyperplaneCoefficients ();
    SolveForRBFWeights ();
    
    // create the interpolator
    PtrToInterpolator   interpolator = new Interpolator (m_dimension, m_exampleCount, m_rbf, m_pData);
    
    // zero the data block for the next go around
    m_pData = 0;
        
    // return the interpolator
    return interpolator;
}

//------------------------------------------------------------------------------
float
InterpolatorFactory::ComputeDistanceBetweenExamples (uInt a, uInt b) const
{
    // compute the vector between the two points and its squared length
    float   squaredDistance = 0.0f;
    for (uInt i = 0; i < m_dimension; i++)
    {
        float   delta = m_exampleLocations (a, i) - m_exampleLocations (b, i);
        squaredDistance += delta * delta;
    }
    
    // decide whether or not to take the square root, and return the result
    if (squaredDistance > FLOAT_EPSILON)
        return sqrtf (squaredDistance);
    return squaredDistance;
}

//------------------------------------------------------------------------------
void
InterpolatorFactory::CalculateExampleRadii (float scale)
{
    // loop over all the examples
    for (uInt i = 0; i < m_exampleCount; i++)
    {
        // calculate a sum of distances for all other example locations
        float   totalDistance = 0.0f;
        for (uInt j = 0; j < m_exampleCount; j++)
        {
            // make sure we aren't taking the distance from an example to itself
            if (j != i)
            {
                // accumulate the distance
                totalDistance += ComputeDistanceBetweenExamples (i, j);
            }
        }
        
        // compute and store the final size as the scaled average distance to
        // all other example locations
        m_exampleRadii[i] = scale * totalDistance / static_cast<float> (m_exampleCount - 1);
    }
}

//------------------------------------------------------------------------------
void
InterpolatorFactory::SolveForHyperplaneCoefficients (void)
{
    // set up the design matrix we'll use for solving the coefficients
    MSR_Matrix  design;
    design.SetDimension (m_exampleCount, m_hyperplaneDimension);

    // initialize the design matrix with the example locations
    for (uInt i = 0 ; i < m_exampleCount ; i++)
    {
        design (i, 0) = 1.0f;
        for (uInt2 j = 0 ; j < m_dimension ; j++)
            design (i, j + 1) = m_exampleLocations (i, j);
    }

    // set up the SVD (Single Value Decomposition) matrices
    MSR_Matrix  U;
    MSR_Matrix  w;
    MSR_Matrix  Vt;
    ConstructSVD (design, U, w, Vt);

    // for each example, we want to calculate a best fit hyperplane that interpolates
    // the range of values at all examples. In the case of the cardinal basis, this
    // is impossible, and we're just getting the best approximation we can. The rest
    // will be filled in by the RBF evaluations.
    MSR_Matrix  targetValues;
    MSR_Matrix  hyperplaneCoefficients;
    targetValues.SetDimension (m_exampleCount, 1);
    hyperplaneCoefficients.SetDimension (m_hyperplaneDimension, 1);
    for (i = 0 ; i < m_exampleCount ; i++)
    {
        // for the cardinal basis, we want a value of
        // 1 at the example, and 0 everywhere else
        for (uInt2 j = 0 ; j < m_exampleCount ; j++)
            targetValues (j, 0) = (j == i) ? 1.0f : 0.0f;

        // compute the best fit hyperplane using the SVD
        ApplySVD (U, w, Vt, targetValues, hyperplaneCoefficients);

        // store the results back to the hyperplane coefficients
        for (j = 0 ; j < m_hyperplaneDimension ; j++)
            m_hyperplaneCoefficients (i, j) = hyperplaneCoefficients (j, 0);
    }
}

//------------------------------------------------------------------------------
void
InterpolatorFactory::SolveForRBFWeights (void)
{
    // set up the design matrix we'll use for solving the weights
    MSR_Matrix    design;
    design.SetDimension (m_exampleCount, m_exampleCount);

    // initialize the design matrix with the values of the RBF 
    // evaluated at every example for all other examples
    for (uInt i = 0 ; i < m_exampleCount ; i++)
        for (uInt j = 0 ; j < m_exampleCount ; j++)
            design (i, j) = m_rbf->Evaluate (m_dimension, &m_exampleLocations (i, 0), &m_exampleLocations (j, 0), m_exampleRadii[j]);

    // set up the Single Value Decomposition (SVD)
    MSR_Matrix    U;
    MSR_Matrix    w;
    MSR_Matrix    Vt;
    ConstructSVD (design, U, w, Vt);

    // evaluate the hyperplanes at all examples for all other examples
    // so we know what values have to be cancelled out by the radial
    // basis functions.
    MSR_Matrix    targetValues;
    MSR_Matrix    rbfWeights;
    targetValues.SetDimension (m_exampleCount, 1);
    rbfWeights.SetDimension (m_exampleCount, 1);
    for (i = 0 ; i < m_exampleCount ; i++)
    {
        // fill the values matrix
        for (uInt j = 0 ; j < m_exampleCount ; j++)
        {
            // evaluate the ith hyperplane at the kth location, and use
            // that value to calculate the target value of the weighted
            // RBF at the example location
            float   linearWeight = Interpolator::EvaluateHyperplane (m_hyperplaneDimension, &m_hyperplaneCoefficients (i, 0), &m_exampleLocations (j, 0));

            // for the cardinal basis, the target value is 1.0 at the
            // example location, and 0 everywhere else
            float   target = (j == i) ? 1.0f : 0.0f;
            targetValues (j, 0) = target - linearWeight;
        }

        // compute the weight on the RBFs that cause our evaluations to be
        // the target values (0 or 1) at the desired locations. Really, all 
        // this does for the runtime system is let it exactly interpolate 
        // example values at their locations. As in the hyperplane calcs,
        // this is a best fit solution.
        ApplySVD (U, w, Vt, targetValues, rbfWeights);

        // store the results back to the radial weights
        for (j = 0 ; j < m_exampleCount ; j++)
            m_rbfWeights (i, j) = rbfWeights (j, 0);
    }
}

//------------------------------------------------------------------------------
