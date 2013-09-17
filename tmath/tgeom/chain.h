//-----------------------------------------------------------------------------
// Copyright (C) 2002-2005 Bretton Wade, All rights reserved
//-----------------------------------------------------------------------------

#ifndef     _CHAIN_H_
#define     _CHAIN_H_

//-----------------------------------------------------------------------------
// includes
//-----------------------------------------------------------------------------
#ifndef     _VECTOR_H_
#include    "vector.h"
#endif      _VECTOR_H_

#ifndef     _BOUND_H_
#include    "bound.h"
#endif      _BOUND_H_

//-----------------------------------------------------------------------------
// class definitions
//-----------------------------------------------------------------------------
template<class Real>
class TChain : public std::vector<TPoint2<Real> >
{
    public:
        /* void */      TChain (bool bClosed = true);
        
        bool            GetClosed (void) const;
        
        uInt            GetID (void) const;
        void            SetID (uInt iID);
static  uInt            GetNextID (void);
        
        void            DebugPrint (void) const;
        
        Real            Area (void) const;
        TBound2<Real>   GetBound (void) const;
        bool            ContainsPoint (const TPoint2<Real>& point) const;
        
        sInt            TestWindingOrder (void) const;
        bool            TestSimple (void) const;
        
        bool            EliminateRedundantVertices (Real rEpsilon = REAL_EPSILON);
        bool            ValidatePolygon (Real rEpsilon = REAL_EPSILON);
        
        void            PunchHole (const TChain<Real>& holeChain);
        Real            ReorderRight (void);

        void            SetUVCoordinates (const TBound2<Real>& uvBound);

        TChain<Real>    Expanded (Real rAdd, Real rTesselationLimit = static_cast<Real> (0.9)) const;
        
    protected:
static  void            TesselateAngle (TChain<Real>& chainOut, TPoint2<Real>& ptSource, Real rScale, TVector2<Real>& vA, TVector2<Real>& vB, Real rTesselationLimit);
        
    protected:
        bool            m_bClosed;
        uInt            m_iID;
static  uInt            s_iID;
};

//-----------------------------------------------------------------------------
// inlines
//-----------------------------------------------------------------------------
#ifndef     _CHAIN_INL_
#include    "chain.inl"
#endif  //  _CHAIN_INL_

//-----------------------------------------------------------------------------

#endif  //  _CHAIN_H_
