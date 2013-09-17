//-----------------------------------------------------------------------------
// Copyright (C) 2002-2005 Bretton Wade, All rights reserved
//-----------------------------------------------------------------------------

#ifndef     _INTERSECT_H_
#define     _INTERSECT_H_

//-----------------------------------------------------------------------------
// includes
//-----------------------------------------------------------------------------
#ifndef     _LINE_TEST_H_
#include    "line_test.h"
#endif  //  _LINE_TEST_H_

//-----------------------------------------------------------------------------
// functions
//-----------------------------------------------------------------------------
template<class Real>
bool            IntersectTest (const TPoint2<Real>& a, const TPoint2<Real>& b, const TPoint2<Real>& c, const TPoint2<Real>& d);

template<class Real>
bool            IntersectTest (const TEdge2<Real>& edgeA, const TEdge2<Real>& edgeB);

template<class Real>
TPoint2<Real>   IntersectSegments (const TPoint2<Real>& a, const TPoint2<Real>& b, const TPoint2<Real>& c, const TPoint2<Real>& d);

template<class Real>
TPoint2<Real>   IntersectSegments (const TEdge2<Real>& edgeA, const TEdge2<Real>& edgeB);

template<class Real>
Real            IntersectRightRaySegmentAny (const TPoint2<Real>& ptRayOrigin, const TPoint2<Real>& a, const TPoint2<Real>& b);

template<class Real>
Real            IntersectRightRaySegment (const TPoint2<Real>& ptRayOrigin, const TPoint2<Real>& a, const TPoint2<Real>& b);

//-----------------------------------------------------------------------------
// inlines
//-----------------------------------------------------------------------------
#ifndef     _INTERSECT_INL_
#include    "intersect.inl"
#endif  //  _INTERSECT_INL_

//-----------------------------------------------------------------------------

#endif  //  _INTERSECT_H_
