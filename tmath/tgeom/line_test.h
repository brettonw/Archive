//-----------------------------------------------------------------------------
// Copyright (C) 2002-2005 Bretton Wade, All rights reserved
//-----------------------------------------------------------------------------

#ifndef     _LINE_TEST_H_
#define     _LINE_TEST_H_

//-----------------------------------------------------------------------------
// includes
//-----------------------------------------------------------------------------
#ifndef     _VECTOR_H_
#include    "vector.h"
#endif      _VECTOR_H_

//-----------------------------------------------------------------------------
// types
//-----------------------------------------------------------------------------
template<class Real>
struct TEdge2
{
    TPoint2<Real>   a;
    TPoint2<Real>   b;
    
    TEdge2 (void) {}
    TEdge2 (const TPoint2<Real>& _a, const TPoint2<Real>& _b) : a (_a), b (_b) {}
};

//-----------------------------------------------------------------------------
// constants
//-----------------------------------------------------------------------------
enum
{
    TURNS_LEFT =    1,
    TURNS_RIGHT =   -1,
    SPANNING =      TURNS_LEFT * TURNS_RIGHT,
    COLINEAR =      0
};

//-----------------------------------------------------------------------------
// functions
//-----------------------------------------------------------------------------
template<class Real>
sInt
LineTestResult (Real rLineTestResult, Real rEpsilon = REAL_EPSILON)
{
    return  (rLineTestResult > rEpsilon) ? TURNS_LEFT : 
            (rLineTestResult < -rEpsilon) ? TURNS_RIGHT : 
            COLINEAR;
}

//-----------------------------------------------------------------------------
template<class Real>
Real
RawLineTest (const TPoint2<Real>& a, const TPoint2<Real>& b, const TPoint2<Real>& c)
{
    return (b - a) CROSS (c - a);
}

//-----------------------------------------------------------------------------
template<class Real>
sInt
LineTest (const TPoint2<Real>& a, const TPoint2<Real>& b, const TPoint2<Real>& c, Real rEpsilon = REAL_EPSILON)
{
    return LineTestResult (RawLineTest (a, b, c), rEpsilon);
}

//-----------------------------------------------------------------------------
template<class Real>
sInt
LineTest (const TEdge2<Real>& edge, const TPoint2<Real>& c, Real rEpsilon = REAL_EPSILON)
{
    return LineTest (edge.a, edge.b, c, rEpsilon);
}

//-----------------------------------------------------------------------------
template<class Real>
Real
NormalizedRawLineTest (const TPoint2<Real>& a, const TPoint2<Real>& b, const TPoint2<Real>& c)
{
    return (b - a).Normalized () CROSS (c - a).Normalized ();
}

//-----------------------------------------------------------------------------
template<class Real>
sInt
NormalizedLineTest (const TPoint2<Real>& a, const TPoint2<Real>& b, const TPoint2<Real>& c, Real rEpsilon = REAL_EPSILON)
{
    return LineTestResult (NormalizedRawLineTest (a, b, c), rEpsilon);
}

//-----------------------------------------------------------------------------
template<class Real>
sInt
NormalizedLineTest (const TEdge2<Real>& edge, const TPoint2<Real>& c, Real rEpsilon = REAL_EPSILON)
{
    return NormalizedLineTest (edge.a, edge.b, c, rEpsilon);
}

//-----------------------------------------------------------------------------

#endif  //  _LINE_TEST_H_
