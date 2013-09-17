//-----------------------------------------------------------------------------
// Copyright (C) 2002-2005 Bretton Wade, All rights reserved
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

//-----------------------------------------------------------------------------
class EarPointVec : public std::vector<EarPoint <Real> > {};

//-----------------------------------------------------------------------------
// functions
//-----------------------------------------------------------------------------
template<class Real>
bool
IsInterior (EarPointVec<Real>& earPoints, uInt2 a, uInt2 b)
{
    // figure out the vertices before and after A
    uInt    iPointCount = uInt(earPoints.size ());
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
template<class Real>
bool
SubdivideTriangle (TChain<Real>& chain, TriangleVec& trianglesOut, const Triangle& triangle, Real rMaxEdgeLength)
{
    // move the "triangle" into an array
    uInt2       iTriangle[3];
    iTriangle[0] = triangle.a;
    iTriangle[1] = triangle.b;
    iTriangle[2] = triangle.c;
    
    // figure out which edge is longest
    uInt2       iLongest = 0xffff;
    Real        rLongest = -REAL_INFINITY;
    for (uInt2 iCurrent = 0; iCurrent < 3; iCurrent++)
    {
        // compute the index for the next vertex on the edge
        uInt2           iNext = (iCurrent + 1) % 3;
        
        // compute the offset vector for the edge and its length
        TVector2<Real>  vDelta = chain[iTriangle[iNext]] - chain[iTriangle[iCurrent]];
        Real            rLength = vDelta.Norm ();
        
        // check to see if it is longer than the stored length
        if (rLength > rLongest)
        {
            rLongest = rLength;
            iLongest = iCurrent;
        }
    }

    // now check to see if the longest edge is longer than our max edge length
    if (rLongest > rMaxEdgeLength)
    {
        // rotate the triangle definition so that the longest edge is the edge
        // starting with the first point in the triangle
        uInt2           iTriangleToSplit[3];
        for (uInt2 iRotateTo = 0; iRotateTo < 3; iRotateTo++)
        {
            uInt2   iRotateFrom = (iRotateTo + iLongest) % 3;
            iTriangleToSplit[iRotateTo] = iTriangle[iRotateFrom];
        }
        
        // compute the split point we want to add
        TPoint2<Real>   ptSplit = chain[iTriangleToSplit[0]] + ((chain[iTriangleToSplit[1]] - chain[iTriangleToSplit[0]]) / 2);
        
        // XXX really ugly search to see if the new point is already in the
        // XXX output list, start from the end, because if it is there, it's
        // XXX most likely to be close to the end because we added it
        uInt            iNewPtIndex;
        for (iNewPtIndex = chain.size () - 1; iNewPtIndex != static_cast<uInt> (-1); --iNewPtIndex)
            if (chain[iNewPtIndex] == ptSplit)
                break;
        
        // check to see if we didn't find the point
        if (iNewPtIndex == static_cast<uInt> (-1))
        {
            // add it to the chain and store off the index
            iNewPtIndex = chain.size ();
            chain.push_back (ptSplit);
        }
    
        // split the triangle into two new triangles along its longest edge
        trianglesOut.push_back (Triangle (iTriangleToSplit[0], iNewPtIndex, iTriangleToSplit[2]));
        trianglesOut.push_back (Triangle (iNewPtIndex, iTriangleToSplit[1], iTriangleToSplit[2]));
        
        // return true to indicate that the triangle was split
        return true;
    }
    
    // return false to indicate that the triangle was not split
    return false;
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
        uInt                            iPointCount = uInt(chain.size ());
        earPoints.reserve (iPointCount);
        while (pointIter != pointEnd)
        {
            iNext = (iNext + 1) % iPointCount;
            earPoints.push_back (EarPoint<Real> ((*pointIter++), iNext));
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
template<class Real>
void    
ClampEdgeLengths (TChain<Real>& chainInOut, const TriangleVec& trianglesIn, TriangleVec& trianglesOut, Real rMaxEdgeLength)
{
    // set up a work array, and copy the input into the work array
    TriangleVec     trianglesToSplit;
    TriangleVec     trianglesThatGotSplit;
    trianglesToSplit.assign (trianglesIn.begin (), trianglesIn.end ());

    // loop until we do no new work
    while (trianglesToSplit.size () > 0)
    {
        // clear the output array
        trianglesThatGotSplit.clear ();
        
        // loop over all the triangles in the work array
        TriangleVec::const_iterator iter = trianglesToSplit.begin ();
        TriangleVec::const_iterator iterEnd = trianglesToSplit.end ();
        while (iter != iterEnd)
        {
            // grab the current triangle
            const Triangle& triangle = *iter;
            
            // try to split the triangle, and if that fails because the triangle is
            // small enough to accept, copy it to the output array
            if (not SubdivideTriangle (chainInOut, trianglesThatGotSplit, triangle, rMaxEdgeLength))
                trianglesOut.push_back (triangle);
            
            // advance to the next triangle
            ++iter;
        }
        
        // copy the split list over to the work list
        trianglesToSplit.assign (trianglesThatGotSplit.begin (), trianglesThatGotSplit.end ());
    }
}


//-----------------------------------------------------------------------------

#endif  //  _TRIANGULATE_INL_
