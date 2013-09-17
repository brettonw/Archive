//-----------------------------------------------------------------------------
// Copyright (C) 2002 Microsoft, All rights reserved
//-----------------------------------------------------------------------------

#ifndef     _HYPERPLANE_H_
#define     _HYPERPLANE_H_

//-----------------------------------------------------------------------------
// includes
//-----------------------------------------------------------------------------
#ifndef     _VECTOR_H_
#include    "vector.h"
#endif      _VECTOR_H_

//-----------------------------------------------------------------------------
// namespace
//-----------------------------------------------------------------------------
BEGIN_NAMESPACE (TMath)

//-----------------------------------------------------------------------------
// template plane class based on a column-vector matrix type
//-----------------------------------------------------------------------------
template<typename Number, unsigned A>
struct THyperplane : public TMatrix<Number, A + 1, 1>
{
        /* void */                  THyperplane (void) {}
        /* void */                  THyperplane (const Number* pNumber);
        /* void */                  THyperplane (const TMatrix<Number, A + 1, 1>& hyperplane);
        /* void */                  THyperplane (const TPoint<Number, A>& point, const TVector<Number, A>& normal);

        // XXX the following needs to be thought about in the context of an n-dimensional hyperplane
        /* void */                  THyperplane (const TPoint<Number, A>& p1, const TPoint<Number, A>& p2, const TPoint<Number, A>& p3);
        // XXX and we need the Initializer notation (operator ,) that works on points
        
static  THyperplane<Number, A>      FitPlane (unsigned iCount, const TPoint<Number, A>* pPoints);
        
        Initializer<Number, A>      operator = (const Number& initializer);
        TVector<Number, A>&         operator = (const TMatrix<Number, A + 1, 1>& vector);

        THyperplane<Number, A>      operator - (void) const;
        THyperplane<Number, A>&     Reverse (void);
        
        Number                      GetDistanceToPoint (const TPoint<Number, A>& point) const;
        Number                      GetDistanceToPoint (const TPoint<Number, A>& point, const TVector<Number, A>& axis) const;
        bool                        TestPointOnPlane (const TPoint<Number, A>& point) const;
        
        TPoint<Number, A>           Point (void) const;
        const TVector<Number, A>&   Normal (void) const;
};

//-----------------------------------------------------------------------------
// inlines
//-----------------------------------------------------------------------------
#ifndef     _HYPERPLANE_INL_
#include    "hyperplane.inl"
#endif  //  _HYPERPLANE_INL_

//-----------------------------------------------------------------------------
// namespace
//-----------------------------------------------------------------------------
END_NAMESPACE (TMath)

//-----------------------------------------------------------------------------

#endif  //  _HYPERPLANE_H_
