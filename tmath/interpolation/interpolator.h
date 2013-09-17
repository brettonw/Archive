//--------------------------------------------------------------------------
// Copyright (C) 2001 Microsoft Corp.
// All rights reserved.
//--------------------------------------------------------------------------

#ifndef     _INTERPOLATOR_H_
#define     _INTERPOLATOR_H_

//--------------------------------------------------------------------------
// includes
//--------------------------------------------------------------------------
#ifndef     _MSR_MATRIX_H_
#include    "interpolation\msr_matrix.h"
#endif  //  _MSR_MATRIX_H_

#ifndef     _RBF_FUNCTOR_H_
#include    "interpolation\rbf_functor.h"
#endif  //  _RBF_FUNCTOR_H_

//--------------------------------------------------------------------------
// useful types
//--------------------------------------------------------------------------
class   Interpolator;
typedef PtrTo<Interpolator> PtrToInterpolator;

//--------------------------------------------------------------------------
// class definitions
//--------------------------------------------------------------------------
class Interpolator : public CountedObject
{
        friend class InterpolatorFactory;
public:
virtual /* void */          ~Interpolator (void);
        
        void                ComputeExampleWeights (uInt dimension, const float* location, 
                                                    uInt exampleCount, float* exampleWeights) const;
        
protected:
        // protected constructor so that only our friend (InteprolatorFactory) can create one
        /* void */          Interpolator (uInt dimension, uInt exampleCount, RBF_Functor* rbf, float* pData);

        float               EvaluateHyperplane (uInt example, const float* location) const;
        float               EvaluateRadialWeights (uInt example, const float* rbfWeightsByLocation) const;

static  float               EvaluateHyperplane (uInt dimension, const float* hyperplane, const float* location);

protected:
        // dimension data
        uInt                m_dimension;
        uInt                m_hyperplaneDimension;
        uInt                m_exampleCount;

        // the Radial Basis Function (RBF) to use
        RBF_Functor*        m_rbf;

        // the memory block we use to keep all the static data together in one space
        float*              m_pStaticData;

        // the indices into the data block we will create
        float*              m_exampleRadii;
        MSR_Matrix          m_exampleLocations;
        MSR_Matrix          m_hyperplaneCoefficients;
        MSR_Matrix          m_rbfWeights;
};

//--------------------------------------------------------------------------

#endif  //  _INTERPOLATOR_H_
