//-----------------------------------------------------------------------------
// Copyright (C) 2002 Microsoft, All rights reserved
//-----------------------------------------------------------------------------

#ifndef     _TRIANGULATE_INL_
#define     _TRIANGULATE_INL_

//-----------------------------------------------------------------------------
// includes
//-----------------------------------------------------------------------------
#ifndef     _TRIANGULATE_H_
#include    "triangulate.h"
#endif  //  _TRIANGULATE_H_

#ifndef     _INTERSECT_H_
#include    "intersect.h"
#endif  //  _INTERSECT_H_

//-----------------------------------------------------------------------------
// types
//-----------------------------------------------------------------------------
template<class Real>
struct EarPoint
{
    TPoint2<Real>   point;
    uInt2           next;
    
    EarPoint (const TPoint2<Real>& _point, uInt2 _next) :
        point (_point), next (_next) {}
};
template<class Real>
class EarPointVec : public std::vector<EarPoint <Real> > {};

//-----------------------------------------------------------------------------
// functions
//-----------------------------------------------------------------------------
template<class Real>
bool
IsInterior (EarPointVec<Real>& earPoints, uInt2 a, uInt2 b)
{
    // figure out the vertices before and after A
    uInt    iPointCount = earPoints.size ();
    uInt    aPrev = ((a - 1) + iPointCount) % iPointCount;
    uInt    aNext = (a + 1) % iPointCount;
    
    // if b is not equal to aPrev or aNext, then we have some work to do
    if ((b != aPrev) and (b != aNext))
    {
        // compute the three conditions used to determine if the edge AB is
        // interior to the angle formed by aPrev-A-aNext. If two or more of
        // these conditions is true, then the proposed edge is interior
        uInt    iConditionCount = 0;
        if (LineTest (earPoints[a].point, earPoints[aPrev].point, earPoints[aNext].point) == TURNS_LEFT)
            iConditionCount++;
        if (LineTest (earPoints[a].point, earPoints[aNext].point, earPoints[b].point) == TURNS_LEFT)
            iConditionCount++;
        if (LineTest (earPoints[a].point, earPoints[b].point, earPoints[aPrev].point) == TURNS_LEFT)
            iConditionCount++;
            
        // return true if 2 or more of the conditions are true
        return (iConditionCount >= 2);
    }
    
    // the line AB is an edge of the polygon, and we define that as interior
    return true;
}

//-----------------------------------------------------------------------------
template<class Real>
bool
IsDiagonal (EarPointVec<Real>& earPoints, uInt2 a, uInt2 b)
{
    // check to see if the edge is an interior edge
    if (IsInterior (earPoints, a, b) and IsInterior (earPoints, b, a))
    {
        // given a segment AB, we want to test against all segments CD such that
        // C != A, C != B, D != A, and D != B
        uInt2   c = earPoints[b].next;
        uInt2   d = earPoints[c].next;
        while (d != a)
        {
            // if the two segments intersect, this is not a valid diagonal
            if (IntersectTest (earPoints[a].point, earPoints[b].point, earPoints[c].point, earPoints[d].point))
                return false;
            
            // advance to the next edge
            c = d;
            d = earPoints[d].next;
        }
        
        // we got through all of the segments and didn't intersect any of them, 
        // so AB is a valid diagonal
        return true;
    }
    
    // the proposed edge is not an interior edge, return failure
    return false;
}

//-----------------------------------------------------------------------------
template<class Real>
bool
SnipAnEar (TriangleVec& triangles, EarPointVec<Real>& earPoints, uInt2 iOffset, uInt2 iStart = 0, uInt iStop = 0)
{
    // set a flag about whether or not we did any work
    bool    bDidWork = false;
    
    // we come into this function wanting to use the "start" vertex as the 
    // first corner of a triangle to snip an ear off the doggie. We proceed
    // by finding the next vertex in the array that is unsnipped - this is 
    // guaranteed to succeed, and then a third, which might be where we stop.
    uInt2   a = iStart;
    uInt2   b = earPoints[a].next;
    uInt2   c = earPoints[b].next;
            
    // we want to loop as long as we are doing some work
    while (c != iStop)
    {
        // we want to examine whether or not it is valid to connect a and c to 
        // snip b. The first check is that ABC turns left, and the second is
        // to confirm that AC is a valid diagonal
        sInt    fTestABC = LineTest (earPoints[a].point, earPoints[b].point, earPoints[c].point);
        if ((fTestABC == TURNS_LEFT) and IsDiagonal (earPoints, a, c))
        {
            // AC is a valid diagonal, output the results
            triangles.push_back (Triangle (a + iOffset, b + iOffset, c + iOffset));
            
            // snip b, and advance c
            earPoints[a].next = b = c;
            c = earPoints[c].next;
            
            // say we did some work
            bDidWork = true;
        }
        else
        {
            // AC is not a valid diagonal or we couldn't snip b due 
            // to concavity in the polygon, so use b as the starting 
            // point for a new search
            if (SnipAnEar (triangles, earPoints, iOffset, b))
            {
                // update c, in case our old c was snipped
                c = earPoints[b].next;
                
                // say we did some work
                bDidWork = true;
            }
            else
            {
                // no work was done, and we recurred because we couldn't
                // proceed from our current vertex, so we need to drop back
                // to our caller rather than keep trying to loop
                return bDidWork;
            }
        }
    }

    // we got to the end of the TChain array, so we're done
    return bDidWork;
}

//-----------------------------------------------------------------------------
// public functions
//-----------------------------------------------------------------------------
template<class Real>
void
Triangulate (const TChainList<Real>& chainList, TChain<Real>& chainOut, TriangleVec& trianglesOut)
{
    // loop over the input chains, pushing the points into the vertex buffer, 
    // and generating triangle indices with the appropriate offset
    uInt                                iOffset = 0;
    TChainList<Real>::const_iterator    chainListIter = chainList.begin ();
    TChainList<Real>::const_iterator    chainListEnd = chainList.end ();
    while (chainListIter != chainListEnd)
    {
        // fetch the chain
        const TChain<Real>&             chain = *chainListIter++;
        
        // copy the chain onto the output chain
        chainOut.insert (chainOut.end (), chain.begin (), chain.end ());
        
        // build an EarPointVec from the chain
        EarPointVec<Real>               earPoints;
        TChain<Real>::const_iterator    pointIter = chain.begin ();
        TChain<Real>::const_iterator    pointEnd = chain.end ();
        uInt2                           iNext = 0;
        uInt                            iPointCount = chain.size ();
        earPoints.reserve (iPointCount);
        while (pointIter != pointEnd)
        {
            iNext = (iNext + 1) % iPointCount;
            earPoints.push_back (EarPoint<Real> ((*pointIter++).pt, iNext));
        }
        
        // start snipping ears to build the triangles
        SnipAnEar (trianglesOut, earPoints, static_cast<uInt2> (iOffset));
        
        // increment offset by the number of vertices in this chain
        iOffset += iPointCount;
    }
    
    // test that we have valid numbers here - this is why iOffset is a uInt, 
    // and not a uInt2
    Assert (iOffset < SHRT_MAX);
}

//-----------------------------------------------------------------------------

#endif  //  _TRIANGULATE_INL_
