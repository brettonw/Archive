//-----------------------------------------------------------------------------
// Copyright (C) 2002-2005 Bretton Wade, All rights reserved
//-----------------------------------------------------------------------------

#ifndef     _INTERSECT_INL_
#define     _INTERSECT_INL_

//-----------------------------------------------------------------------------
// includes
//-----------------------------------------------------------------------------
#ifndef     _INTERSECT_H_
#include    "intersect.h"
#endif  //  _INTERSECT_H_

//-----------------------------------------------------------------------------
// functions
//-----------------------------------------------------------------------------
template<class Real>
bool
IntersectTest (const TPoint2<Real>& a, const TPoint2<Real>& b, const TPoint2<Real>& c, const TPoint2<Real>& d)
{
    // this function returns true if ab and cd intersect, start by computing 
    // line tests for the 4 combinations
    sInt    fTestABC = LineTest (a, b, c);
    sInt    fTestABD = LineTest (a, b, d);
    sInt    fTestCDA = LineTest (c, d, a);
    sInt    fTestCDB = LineTest (c, d, b);
    
    // now check to see if the segments span each other
    bool    bABSpansCD = ((fTestCDA * fTestCDB) == SPANNING);
    bool    bCDSpansAB = ((fTestABC * fTestABD) == SPANNING);
    
    // if both segments span each other, it's a trivial accept
    if (bABSpansCD and bCDSpansAB)
        return true;
        
    // otherwise, it's a "between" test, so we check the possible combinations
    // of end of the segments lying on the other line segments
    if (bABSpansCD)
        if ((fTestABC == COLINEAR) or (fTestABD == COLINEAR))
            return true;
    if (bCDSpansAB)
        if ((fTestCDA == COLINEAR) or (fTestCDB == COLINEAR))
            return true;
            
    // if we got all the way to here, the segments don't intersect
    return false;
}

//-----------------------------------------------------------------------------
template<class Real>
bool
IntersectTest (const TEdge2<Real>& edgeA, const TEdge2<Real>& edgeB)
{
    return IntersectTest (edgeA.a, edgeA.b, edgeB.a, edgeB.b);
}

//-----------------------------------------------------------------------------
template<class Real>
TPoint2<Real>
IntersectSegments (const TPoint2<Real>& a, const TPoint2<Real>& b, const TPoint2<Real>& c, const TPoint2<Real>& d)
{
    // compute the denominator of the scale
    Real    rDenominator = (a.x * (d.y - c.y)) + (b.x * (c.y - d.y)) + (d.x * (b.y - a.y)) + (c.x * (a.y - b.y));
    Assert (not FuzzyEquals<Real> (rDenominator, 0));
    
    // compute the numerator
    Real    rNumerator = (a.x * (d.y - c.y)) + (c.x * (a.y - d.y)) + (d.x * (c.y - a.y));
    
    // compute the scale
    Real    rScale = rNumerator / rDenominator;
    
    // compute the result and return it
    return a + ((b - a) * rScale);
}

//-----------------------------------------------------------------------------
template<class Real>
TPoint2<Real>
IntersectSegments (const TEdge2<Real>& edgeA, const TEdge2<Real>& edgeB)
{
    return IntersectSegments (edgeA.a, edgeA.b, edgeB.a, edgeB.b);
}

//-----------------------------------------------------------------------------
template<class Real>
Real            
IntersectRightRaySegmentCore (const TPoint2<Real>& ptRayOrigin, const TPoint2<Real>& a, const TPoint2<Real>& b)
{
    // check that at least one of the points is to the right of the
    // ray origin point
    if ((a.x >= ptRayOrigin.x) or (b.x >= ptRayOrigin.x))
    {
        // compute the normal vector for the line formed by A and B, this
        // will become the Ax + By part of the line equation
        TVector2<Real>  vLine = b - a;
        TVector2<Real>  vNormal (vLine.y, -vLine.x);

        // check to see if the ray is parallel to the edge AB
        if (FuzzyEquals<Real> (vNormal.x, 0))
        {
            // the ray should be interesecting with the endpoint of the edge AB
            // nearest to the origin of the ray, in the ray direction
            Real    rDeltaA = a.x - ptRayOrigin.x;
            Real    rDeltaB = b.x - ptRayOrigin.x;
            if (rDeltaA < -REAL_EPSILON) rDeltaA = REAL_INFINITY;
            if (rDeltaB < -REAL_EPSILON) rDeltaB = REAL_INFINITY;
            return Minimum <Real> (rDeltaA, rDeltaB);
        }
        else
        {
            // compute the constant C for the line equation Ax + By + C = 0
            Real            rC = -(TVector2<Real> (a) DOT vNormal);
            
            // compute the "time" to intersection along the ray, since this is
            // a right ray, we can short-circuit the denominator calculation
            Real            rNumerator = -((TVector2<Real> (ptRayOrigin) DOT vNormal) + rC);
            Real            rT = rNumerator / vNormal.x;
            
            // return the computed intersection "time"
            return rT;
        }
    }
    
    // return failure
    return -1;
}

//-----------------------------------------------------------------------------
template<class Real>
Real            
IntersectRightRaySegmentAny (const TPoint2<Real>& ptRayOrigin, const TPoint2<Real>& a, const TPoint2<Real>& b)
{
    // check to see if this edge spans the ray line going up or down
    if (((a.y <= ptRayOrigin.y) and (b.y >= ptRayOrigin.y)) or ((a.y >= ptRayOrigin.y) and (b.y <= ptRayOrigin.y)))
    {
        // this edge is a candidate, check more thoroughly
        return IntersectRightRaySegmentCore (ptRayOrigin, a, b);
    }
    
    // return failure
    return -1;
}

//-----------------------------------------------------------------------------
template<class Real>
Real            
IntersectRightRaySegment (const TPoint2<Real>& ptRayOrigin, const TPoint2<Real>& a, const TPoint2<Real>& b)
{
    // check to see if this edge spans the ray line going up
    if ((a.y <= ptRayOrigin.y) and (b.y >= ptRayOrigin.y))
    {
        // this edge is a candidate, check more thoroughly
        return IntersectRightRaySegmentCore (ptRayOrigin, a, b);
    }
    
    // return failure
    return -1;
}

//-----------------------------------------------------------------------------

#endif  //  _INTERSECT_INL_
