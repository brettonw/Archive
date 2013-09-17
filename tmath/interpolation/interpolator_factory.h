//--------------------------------------------------------------------------
// Copyright (C) 2001 Microsoft Corp.
// All rights reserved.
//--------------------------------------------------------------------------

#ifndef     _INTERPOLATOR_FACTORY_H_
#define     _INTERPOLATOR_FACTORY_H_

//--------------------------------------------------------------------------
// includes
//--------------------------------------------------------------------------
#ifndef     _INTERPOLATOR_H_
#include    "interpolation\interpolator.h"
#endif  //  _INTERPOLATOR_H_

#ifndef     _XML_NODE_H_
#include    "xml_node.h"
#endif  //  _XML_NODE_H_

//--------------------------------------------------------------------------
// class definitions
//--------------------------------------------------------------------------
class InterpolatorFactory
{
public:
        /* void */          InterpolatorFactory (void);
        /* void */          ~InterpolatorFactory (void);
        
        // method for creating an interpolator
        PtrToInterpolator   CreateInterpolator (const PtrToXMLNode& xmlNode, RBF_Functor* rbf);
        
protected:
        // setup methods
        void                AllocateMemory (void);
        PtrToInterpolator   CreateInterpolator (float scale);

        // precalculation methods
        float               ComputeDistanceBetweenExamples (uInt a, uInt b) const;
        void                CalculateExampleRadii (float scale);
        void                SolveForHyperplaneCoefficients (void);
        void                SolveForRBFWeights (void);
        
protected:
        // dimension data
        uInt                m_dimension;
        uInt                m_hyperplaneDimension;
        uInt                m_exampleCount;
        
        // the Radial Basis Function (RBF) to use
        RBF_Functor*        m_rbf;

        // the memory block we use to keep it all together in one space
        float*              m_pData;

        // the indices into the data block we will create
        float*              m_exampleRadii;
        MSR_Matrix          m_exampleLocations;
        MSR_Matrix          m_hyperplaneCoefficients;
        MSR_Matrix          m_rbfWeights;
};

//--------------------------------------------------------------------------

#endif  //  _INTERPOLATOR_FACTORY_H_
