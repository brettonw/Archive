//-----------------------------------------------------------------------------
// Copyright (C) 2002-2005 Bretton Wade, All rights reserved
//-----------------------------------------------------------------------------

#ifndef     _CHAIN_INL_
#define     _CHAIN_INL_

//-----------------------------------------------------------------------------
// includes
//-----------------------------------------------------------------------------
#ifndef     _CHAIN_H_
#include    "chain.h"
#endif      _CHAIN_H_

#ifndef     _LINE_TEST_H_
#include    "line_test.h"
#endif  //  _LINE_TEST_H_

#ifndef     _INTERSECT_H_
#include    "intersect.h"
#endif  //  _INTERSECT_H_

//-----------------------------------------------------------------------------
// class static member definitions
//-----------------------------------------------------------------------------
template<class Real>
uInt        TChain<Real>::s_iID = 0;

//-----------------------------------------------------------------------------
// class member definitions
//-----------------------------------------------------------------------------
template<class Real>
/* void */      
TChain<Real>::TChain (bool bClosed) :
    m_bClosed (bClosed),
    m_iID (++s_iID)
{   
}

//-----------------------------------------------------------------------------
template<class Real>
bool
TChain<Real>::GetClosed (void) const
{   
    return m_bClosed;
}

//-----------------------------------------------------------------------------
template<class Real>
uInt
TChain<Real>::GetID (void) const
{   
    return m_iID;
}

//-----------------------------------------------------------------------------
template<class Real>
void
TChain<Real>::SetID (uInt iID)
{   
    m_iID = iID;
}

//-----------------------------------------------------------------------------
template<class Real>
uInt
TChain<Real>::GetNextID (void)
{
    return s_iID + 1;
}

//-----------------------------------------------------------------------------
template<class Real>
void
TChain<Real>::DebugPrint (void) const
{
    uInt2   iVertexCount = size ();
    ::DebugPrint ("Chain %u (%hu vertices - %s)\n", m_iID, iVertexCount, m_bClosed ? "closed" : "open");
    for (uInt2 i = 0; i < iVertexCount; i++)
        ::DebugPrint ("%u\t%f\t%f\n", i, at(i).x, at(i).y);
}

//-----------------------------------------------------------------------------
template<class Real>
Real
TChain<Real>::Area (void) const
{
    // Assert (m_bClosed);
    
    // fetch the size of the input chain
    uInt2                   iVertexCount = uInt2(size ());
    // Assert (iVertexCount > 2);
    
    // the accumulated area starts at 0
    Real                    rArea = 0;

    // loop over all the vertices
    const TPoint2<Real>&    a = at (0);
    TPoint2<Real>           b = at (1);
    for (uInt2 i = 2; i < iVertexCount; i++)
    {
        // get the current vertex
        TPoint2<Real>   c = at (i);
        
        // accumulate the area of triangle ABC, which may be negative if it
        // turns to the right
        rArea += RawLineTest (a, b, c);
        
        // shift the vertices for the next go around
        b = c;
    }
    
    // return the accumulated area
    return rArea;
}

//-----------------------------------------------------------------------------
template<class Real>
TBound2<Real>
TChain<Real>::GetBound (void) const
{
    // loop over all of the points
    TBound2<Real>       bound;
    const_iterator      iter = begin ();
    const_iterator      iterEnd = end ();
    while (iter != iterEnd)
    {
        // fetch the current point and add it to the bound
        const TPoint2<Real>&    point = (*iter++).pt;
        bound.AddPoint (point);
    }
    
    // return the result
    return bound;
}

//-----------------------------------------------------------------------------
template<class Real>
bool
TChain<Real>::ContainsPoint (const TPoint2<Real>& point) const
{
    // assuming that the chain represents a simple, closed polygon
    // Assert (m_bClosed);
    
    // the algorithm is to count the number of times a ray cast to the right 
    // from the test point crosses an edge of the polygon. We distinguish
    // entrys from exits, and test that the number of exits is greater than
    // the number of entrys. This is necessary to allow for non-simple polys
    // being tested.
    uInt                    iCrossingOutCount = 0;
    uInt                    iCrossingInCount = 0;
    
    // loop over the points of the source chain
    uInt                    iSize = size ();
    for (uInt i = 0; i < iSize; i++)
    {
        // fetch the current and next points in the chain
        uInt                    j = (i + 1) % iSize;
        const TPoint2<Real>&    ptA = at (i).pt;
        const TPoint2<Real>&    ptB = at (j).pt;
    
        // if the edge is horizontal, as we will regard that as neither an 
        // entry nor an exit, so skip them altogether
        if (FuzzyNotEquals (ptA.y, ptB.y))
        {
            // test the edge against a right ray
            Real    rIntersection = IntersectRightRaySegmentAny (point, ptA, ptB);
            if (rIntersection > REAL_EPSILON)
            {
                // the intersection is some distance from the test point, so
                // check to see if it is entry or an exit. Exits are determined 
                // by a line progressing up in the y direction, and entries are 
                // determined by a line progressing down in the y direction.
                if ((ptA.y <= point.y) and (ptB.y >= point.y))
                    iCrossingOutCount++;
                if ((ptA.y >= point.y) and (ptB.y <= point.y))
                    iCrossingInCount++;
            }
            else if (rIntersection > -REAL_EPSILON)
            {
                // the test point is on a line - here we only count exits,
                // because we define on the line as contained by the chain.
                // exits are determined by a line progressing up in the y
                // direction.
                if ((ptA.y <= point.y) and (ptB.y >= point.y))
                    iCrossingOutCount++;
            }
        }
    }
    
    // If the crossing count is odd, then the point is contained    
    return (iCrossingOutCount > iCrossingInCount);
}

//-----------------------------------------------------------------------------
template<class Real>
sInt
TChain<Real>::TestWindingOrder (void) const
{
    // the area of the chain will be positive if it winds to the left, and
    // negative if it winds to the right - it shouldn't be 0, else we have 
    // coincident vertices
    return LineTestResult (Area ());
}

//-----------------------------------------------------------------------------
template<class Real>
bool
TChain<Real>::TestSimple (void) const
{
    // Assert (m_bClosed);
    // nasty n^2 algorithm to test all edges against all other segments
    
    // fetch the size of the input chain as both the edge count and the 
    // vertex count
    uInt2           iEdgeCount = size ();
    uInt2           iVertexCount = iEdgeCount;
    // Assert (iVertexCount > 2);
    
    // loop over all the edges
    for (uInt2 i = 0; i < iEdgeCount; i++)
    {
        uInt2           iNext = (i + 1) % iVertexCount;
        TEdge2<Real>    edgeA (at (i).pt, at (iNext).pt);
        
        uInt2       j = (iNext + 1) % iVertexCount;
        while (j != 0)
        {
            uInt2   k = (j + 1) % iVertexCount;
            if (k != 0)
            {
                TEdge2<Real>    edgeB (at (j).pt, at (k).pt);
                if (IntersectTest (edgeA, edgeB))
                    return false;
            }
            j = k;
        }
    }
    return true;
}

//-----------------------------------------------------------------------------
template<class Real>
bool
TChain<Real>::EliminateRedundantVertices (Real rEpsilon)
{
    // fetch the size of the input chain
    uInt2           iVertexCount = size ();
    
    // set up an output chain, the same size as the input
    TChain<Real>    outputChain (m_bClosed);
    outputChain.reserve (iVertexCount);

    // common parameters for the main loop    
    TVertex2<Real>  a;
    TVertex2<Real>  b;
    uInt2           iStart;

    // check to see if the chain is closed
    if (m_bClosed)
    {
        // make sure this is a valid polygon
        // Assert (iVertexCount > 2);
        if (iVertexCount < 3)
            return false;
        
        // set up the cached pair accounting for the closed loop at the end - we
        // wind backwards looking for a point that is not equal to the first
        // point
        uInt    i = iVertexCount;
        do
            b = at (--i);
        while ((TPoint2<Real>::FuzzyEquals (a.pt, b.pt, rEpsilon)) and (i > 1));
        a = at (--i);
        // Assert (i != 0);
        iStart = 0;
    }
    else
    {
        // make sure this is a valid polyline
        // Assert (iVertexCount > 1);
        if (iVertexCount < 2)
            return false;
        
        // this is a line, not a poly - start at the front - with the cached pair
        // from the beginning of the line
        a = at (0);
        b = at (1);
        iStart = 2;
        
        // emit the first point
        outputChain.push_back (a);
    }
    
    // this code walks the chain checking each sequence of 3 points for
    // redundancy (duplicate or colinear points).
    for (uInt2 i = iStart; i < iVertexCount; i++)
    {
        // make sure this is a valid polyline
        // Assert (iVertexCount > 1);
        
        // fetch the next point
        TVertex2<Real>& c = at (i);
        
        // check if the new vertex is not colinear with the line 
        // formed by the previous two vertices
        if ((NormalizedLineTest (a.pt, b.pt, c.pt, rEpsilon) != COLINEAR) and
            (TPoint2<Real>::FuzzyNotEquals (a.pt, b.pt, rEpsilon)) and
            (TPoint2<Real>::FuzzyNotEquals (a.pt, c.pt, rEpsilon)) and
            (TPoint2<Real>::FuzzyNotEquals (b.pt, c.pt, rEpsilon)))
        {
            // output the current vertex
            outputChain.push_back (b);
            
            // shift the walking vertices for the next go around
            a = b;
            b = c;
        }
        else
        {
            // shift the current vertex for the next go around
            b = c;
        }
    }

    // check to see if the chain is closed
    if (m_bClosed)
    {
        // move the output chain back to the input if it is a valid polygon
        if (outputChain.size () > 2)
        {
            assign (outputChain.begin (), outputChain.end ());
            return true;
        }
    }
    else
    {
        // finish by emitting the "last" point. If the last point is "equal" 
        // to the first one, we emit the first one to make sure that's the case
        if (TPoint2<Real>::FuzzyEquals (front ().pt, back ().pt, rEpsilon))
            outputChain.push_back (front());
        else
            outputChain.push_back (back ());
            
        // move the output chain back to the input if it is a valid polyline
        if (outputChain.size () > 1)
        {
            assign (outputChain.begin (), outputChain.end ());
            return true;
        }
    }
    
    // return failure if we get here
    return false;
}

//-----------------------------------------------------------------------------
template<class Real>
bool
TChain<Real>::ValidatePolygon (Real rEpsilon)
{
    // Assert (m_bClosed);
    
    // eliminate redundant vertices, and be sure the result has enough vertices
    // to be a valid polygon
    if (EliminateRedundantVertices (rEpsilon))
    {
        // check the winding order, and return success if the chain winds left
        if ((TestWindingOrder () == TURNS_LEFT))
            return true;
    }
        
#if DEBUG_GEOMETRY
    // squawk about the bad poly
    ::DebugPrint ("WARNING: invalid chain\n");
    DebugPrint ();
#endif
    
    // return the failure
    return false;
}

//-----------------------------------------------------------------------------
template<class Real>
void
TChain<Real>::PunchHole (const TChain<Real>& holeChain)
{
    // we assume that the hole chain has been reordered so that the farthest
    // right vertex is the first one in the chain. We cast a ray from that
    // point directly to the right, and find the first edge intersected. We
    // then insert the hole chain into the source chain at that location, with
    // some vertex duplication to make sure the resulting chain is valid
    
    // set up storage for the best intersection found
    Real                    rBestT = REAL_INFINITY;
    uInt                    iBestIndex = 0xffffffff;
    
    // fetch the origin for the ray we will fire
    const TVertex2<Real>&   ptRayOrigin = holeChain[0];
    TVector2<Real>          vRayDirection (1, 0);
    
    // loop over the points of the source chain
    uInt                    iSize = size ();
    for (uInt i = 0; i < iSize; i++)
    {
        // fetch the current and next points in the chain
        uInt    j = (i + 1) % iSize;
        TVertex2<Real>& ptA = at (i);
        TVertex2<Real>& ptB = at (j);
        
        // compute the intersection "time" for a right ray with the edge AB
        Real            rT = IntersectRightRaySegment (ptRayOrigin.pt, ptA.pt, ptB.pt);
        
        // if the intersection "time" is positive, store the intersection 
        // time, along with the edge index for the second point
        if ((rT > 0) and (rT < rBestT))
        {
            rBestT = rT;
            iBestIndex = j;
            // Assert (LineTest (ptA.pt, ptRayOrigin.pt + (vRayDirection * rBestT), ptB.pt) == COLINEAR);
        }
    }
    
    // compute the intersection point from the best found intersection
    // Assert (iBestIndex != 0xffffffff);
    TPoint2<Real>           ptIntersection = ptRayOrigin.pt + (vRayDirection * rBestT);
    
    // use the intersection point to compute the new UV coordinates
    TVertex2<Real>&         ptA = at (((iBestIndex - 1) + iSize) % iSize);
    TVertex2<Real>&         ptB = at (iBestIndex);
    Real                    rT;
    Real                    rDenominator = ptB.pt.y - ptA.pt.y;
    if (FuzzyNotEquals<Real> (rDenominator, 0))
        rT = (ptIntersection.y - ptA.pt.y) / rDenominator;
    else
    {
        rDenominator = ptB.pt.x - ptA.pt.x;
        // Assert (FuzzyNotEquals<Real> (rDenominator, 0));
        rT = (ptIntersection.x - ptA.pt.x) / rDenominator;
    }
    // Assert ((rT >= 0) and (rT <= 1));
    // Assert (ptA.pt + ((ptB.pt - ptA.pt) * rT) == ptIntersection);
    TPoint2<Real>           uv = ptA.uv + ((ptB.uv - ptA.uv) * rT);
    
    // insert that point into the source chain twice
    iterator                insertionIter = begin () + iBestIndex;
    insertionIter = insert (insertionIter, TVertex2<Real> (ptIntersection, uv));
    insertionIter = insert (insertionIter, TVertex2<Real> (ptIntersection, uv));
    insertionIter++;
    
    // insert the first point of the hole poly into the source chain
    insertionIter = insert (insertionIter, ptRayOrigin);
    
    // now insert the whole hole poly chain
    insert (insertionIter, holeChain.begin (), holeChain.end ());
}

//-----------------------------------------------------------------------------
template<class Real>
Real
TChain<Real>::ReorderRight (void)
{
    // initialize the maximal point storage with the first coordinate
    Real    rMaxX = at (0).pt.x;
    uInt    iMaxIndex = 0;
    
    // loop over all of the points
    uInt    iEnd = size ();
    for (uInt i = iMaxIndex + 1; i < iEnd; i++)
    {
        //fetch the current point
        TPoint2<Real>&  pt = at (i).pt;
        
        // check to see if it is farther to the right than the current point
        if (pt.x > rMaxX)
        {
            // store the x coordinate and the index
            rMaxX = pt.x;
            iMaxIndex = i;
        }
    }
    
    // now roll the vector so that the first point in the chain is the farthest
    // to the right
    std::rotate (begin (), begin () + iMaxIndex, end ());
    
    // return the maximal point
    return rMaxX;
}

//-----------------------------------------------------------------------------
template<class Real>
void
TChain<Real>::SetUVCoordinates (const TBound2<Real>& uvBound)
{
    // cache the lower left point and compute the bound span
    const TPoint2<Real>&    ptLowerLeft = uvBound.GetMin ();
    TVector2<Real>          vSpan = uvBound.GetMax () - ptLowerLeft;
    // Assert (vSpan.x == vSpan.y);
    
    // loop over all of the vertices
    iterator                iter = begin ();
    iterator                iterEnd = end ();
    while (iter != iterEnd)
    {
        // fetch the current vertex
        TVertex2<Real>&     vertex = *iter++;
        
        // compute the uv coordinate relative to the bound and set it
        TVector2<Real>      vDelta = vertex.pt - ptLowerLeft;
        vertex.uv = TPoint2<Real> (vDelta.x / vSpan.x, vDelta.y / vSpan.y);
    }
}

//-----------------------------------------------------------------------------
template<class Real>
TChain<Real>
TChain<Real>::Expanded (Real rAdd, Real rTesselationLimit) const
{
    //#pragma TODO (brettonw, "BETTER EXPANDING ALGORITHM")
    // only closed polygons expand into bigger polygons
    // Assert (m_bClosed);

    // set up the target chain
    TChain<Real>    expandedChain (m_bClosed);
    
    // loop over the vertices of the polygon, we compute the vector perpendicular
    // to the previous edge, and carry that along repeatedly
    uInt            iSize = size ();
    TVertex2<Real>  ptA = at (0);
    TVector2<Real>  vXA = ptA.pt - at (iSize - 1).pt;
    TVector2<Real>  vPrev = vXA.Normalized ().Perpendicular ();
    for (uInt i = 0; i < iSize; i++)
    {
        // compute the index for the next point, and set up the subsequent
        // points for processing
        const TVertex2<Real>&   ptB = at ((i + 1) % iSize);
    
        // compute the vector that will be used for expansion
        TVector2<Real>          vAB = ptB.pt - ptA.pt;
        Real                    rNextLength = vAB.Norm ();
        TVector2<Real>          vNext = vAB.Normalized ().Perpendicular ();
        
        // check to see if this is an angle
        Real                    rAngleTest = vPrev CROSS vNext;
        if (FuzzyNotEquals<Real> (rAngleTest, 0))
        {
            //#pragma TODO (brettonw, "FIGURE OUT WHAT TO DO ABOUT UV COORDINATES HERE")
            
            // store the first edge point
            expandedChain.push_back (ptA.pt + (vPrev * rAdd));
            
            // check to see if there will be a gap between the two points
            if (rAngleTest > REAL_EPSILON)
            {
                // tesselate a curve around the current point to maintain the
                // expansion correctly without creating overly large spikes
                TesselateAngle (expandedChain, ptA.pt, rAdd, vPrev, vNext, rTesselationLimit);
            }
            
            // store the second edge point
            expandedChain.push_back (ptA.pt + (vNext * rAdd));
        }

        // propogate the next point to the current point, and the next vector
        // to the previous vector
        ptA = ptB;
        vPrev = vNext;
    }
    
    // return the resulting chain
    return expandedChain;
}

//-----------------------------------------------------------------------------
template<class Real>
void
TChain<Real>::TesselateAngle (TChain<Real>& chainOut, TPoint2<Real>& ptSource, Real rScale, TVector2<Real>& vA, TVector2<Real>& vB, Real rTesselationLimit)
{
    // check to see if the angle between the vectors is greater than the limit
    if ((vA DOT vB) < rTesselationLimit)
    {
        // compute the split vector
        TVector2<Real>  vSplit = (vA + vB).Normalized ();
        
        // recur on the first half
        TesselateAngle (chainOut, ptSource, rScale, vA, vSplit, rTesselationLimit);
        
        // output the split vector
        //#pragma TODO (brettonw, "FIGURE OUT WHAT TO DO ABOUT UV COORDINATES HERE")
        chainOut.push_back (ptSource + (vSplit * rScale));
        
        // recur on the second half
        TesselateAngle (chainOut, ptSource, rScale, vSplit, vB, rTesselationLimit);
    }
}

//-----------------------------------------------------------------------------

#endif  //  _CHAIN_INL_
