//-----------------------------------------------------------------------------
// Copyright (C) 2002-2005 Bretton Wade, All rights reserved
//-----------------------------------------------------------------------------

#ifndef     _BOUND_H_
#define     _BOUND_H_

//-----------------------------------------------------------------------------
// includes
//-----------------------------------------------------------------------------
#ifndef     _LINE_TEST_H_
#include    "line_test.h"
#endif  //  _LINE_TEST_H_

//-----------------------------------------------------------------------------
// class definitions
//-----------------------------------------------------------------------------
template<class Real>
class TBound2
{
    public:
        enum EdgeID
        {
            LEFT,
            TOP,
            RIGHT,
            BOTTOM
        };
        
    public:
        /* void */              TBound2 (void);
        /* void */              TBound2 (const TBound2<Real>& bound);
        /* void */              TBound2 (const TPoint2<Real>& ptMin, const TPoint2<Real>& ptMax);
        
        // special constructor for alternate Real types
        template<class alternate>
        /* void */              TBound2 (const TBound2<alternate>& bound) :
                                    m_ptMin (bound.GetMin ()), m_ptMax (bound.GetMax ()) {}

        const TPoint2<Real>&    GetMin (void) const;
        const TPoint2<Real>&    GetMax (void) const;
        const TPoint2<Real>     GetCenter (void) const;
        TEdge2<Real>            GetEdge (EdgeID edgeID) const;
        
        void                    DebugPrint (void) const;

        void                    AddPoint (const TPoint2<Real>& point);
        void                    AddBound (const TBound2<Real>& bound);
        TBound2<Real>           Scaled (Real rScale) const;
        TBound2<Real>           Expanded (Real rAdd) const;
        TBound2<Real>           Expanded (Real rAddX, Real rAddY) const;
        
        bool                    ContainsPoint (const TPoint2<Real>& point) const;
        bool                    IntersectsBound (const TBound2<Real>& bound);
        
    protected:
        TPoint2<Real>           m_ptMin;
        TPoint2<Real>           m_ptMax;
};

//-----------------------------------------------------------------------------
// inlines
//-----------------------------------------------------------------------------
#ifndef     _BOUND_INL_
#include    "bound.inl"
#endif  //  _BOUND_INL_

//-----------------------------------------------------------------------------

#endif  //  _BOUND_H_
