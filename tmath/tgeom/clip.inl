//-----------------------------------------------------------------------------
// Copyright (C) 2002-2005 Bretton Wade, All rights reserved
//-----------------------------------------------------------------------------

#ifndef     _CLIP_INL_
#define     _CLIP_INL_

//-----------------------------------------------------------------------------
// includes
//-----------------------------------------------------------------------------
#ifndef     _CLIP_H_
#include    "clip.h"
#endif  //  _CLIP_H_

#ifndef     _INTERSECT_H_
#include    "intersect.h"
#endif  //  _INTERSECT_H_

//-----------------------------------------------------------------------------
// intersection case table - the line intersection code will test three points
// against the intersecting line, and use those three values to build a code
// that is looked up to find the sequence of operations that should be done
// to handle that case. Here, we define the operations, the lookup table, and
// some macros to support the operation
//-----------------------------------------------------------------------------
enum
{
    DO_NOTHING =    0,
    PRE_EMIT =      1,
    PRE_SECT =      2,
    NEW_CHAIN =     4,
    POST_SECT =     8,
    POST_EMIT =     16,
    SPECIAL_CASE =  PRE_EMIT bit_or NEW_CHAIN bit_or POST_EMIT
};

//-----------------------------------------------------------------------------
extern  stdext::hash_map<int, int>  g_IntersectionCode;

//-----------------------------------------------------------------------------
#define MAKE_INTERSECTION_CODE(last, current, next)                             \
    (   ((*(int*)&last) bit_and 0x00ff)           bit_or                        \
        (((*(int*)&current) bit_and 0x00ff) << 8) bit_or                        \
        (((*(int*)&next) bit_and 0x00ff) << 16))
    
//-----------------------------------------------------------------------------
// types
//-----------------------------------------------------------------------------
template<class Real>
struct EndPoint
{
    typename TChainList<Real>::iterator  chainIter;
    Real                        rOffsetA;
    Real                        rOffsetB;
    bool                        bFront;
    /* void */                  EndPoint (typename TChainList<Real>::iterator _chainIter, Real _rOffsetA, Real _rOffsetB, bool _bFront) :
                                    chainIter (_chainIter), rOffsetA (_rOffsetA), rOffsetB (_rOffsetB), bFront (_bFront) {}
    void                        DebugPrint (void) const
                                    { ::DebugPrint ("EndPoint A=%g, B=%g, front=%s, chain=%d\n", rOffsetA, rOffsetB, bFront ? "true" : "false", chainIter->GetID ()); }
};

//-----------------------------------------------------------------------------
template<class Real>
class EndPointVec : public std::vector<EndPoint<Real> >
{
    public:
        void                    DebugPrint (void) const
        {
            ::DebugPrint ("EndPointVec (%d endpoints):\n", size ());
            EndPointVec<Real>::const_iterator   iter = begin ();
            EndPointVec<Real>::const_iterator   iterEnd = end ();
            while (iter != iterEnd)
            {
                const EndPoint<Real>&   endPoint = *iter++;
                endPoint.DebugPrint ();
            }
        }
};

//-----------------------------------------------------------------------------
// STL helpers
//-----------------------------------------------------------------------------
template<class Real>
bool
EndPointCmp (EndPoint<Real>& lhs, EndPoint<Real>& rhs)
{
    // check to see if the endpoints are equal - this is done conservatively
    if (FuzzyEquals<Real> (lhs.rOffsetA, rhs.rOffsetA, static_cast<Real> (1.0e-5)))
    {
        // check to see if the two endpoints are for the same chain
        if (lhs.chainIter->GetID () == rhs.chainIter->GetID ())
        {
            // We assume that this is a hole, and return  sort order as such
            return (lhs.bFront == true);
        }
        else
        {
            // compute a few values that will be examined to help determine
            // what to do - these are the distances to the opposing end
            // points of the chain along the sort line, and the sign of the 
            // product of those two distances
            Real    rDeltaL = lhs.rOffsetA - lhs.rOffsetB;
            Real    rDeltaR = rhs.rOffsetA - rhs.rOffsetB;
            Real    rSign = Sign (rDeltaL * rDeltaR);
            Assert (FuzzyNotEquals<Real> (rDeltaL, rDeltaR));
            Assert (FuzzyNotEquals<Real> (rDeltaL, 0) or FuzzyNotEquals<Real> (rDeltaR, 0));
            
            // Adjust the deltas by the sign of their product
            rDeltaL *= rSign;
            rDeltaR *= rSign;

            // depending on the sign of the multiplied distances, the following
            // code moves the test points towards or away from their opposing
            // end points - providing the separation that we need in order to
            // clearly determine a sort order. The comparison is done on the
            // moved values
            return (lhs.rOffsetA + rDeltaL) < (rhs.rOffsetA + rDeltaR);
        }
    }
    
    // the normal case, where there is a clear distinction for sorting
    return lhs.rOffsetA < rhs.rOffsetA;
}

//-----------------------------------------------------------------------------
template<class Real>
struct ChainListIteratorCompare
{
    bool    operator () (const typename TChainList<Real>::iterator& lhs, const typename TChainList<Real>::iterator& rhs) const
    {
        return (*(int*)&lhs) < (*(int*)&rhs);
    }
};
template<class Real>
class IteratorSet : public std::set<typename TChainList<Real>::iterator, ChainListIteratorCompare<Real> > {};

//-----------------------------------------------------------------------------
// functions
//-----------------------------------------------------------------------------
template<class Real>
typename EndPointVec<Real>::iterator
JoinChains (TChainList<Real>& chainsOut, typename EndPointVec<Real>::iterator endPointIter)
{
    // We assume that the calling function has called us with the start of a 
    // polygon. This could be followed by the end of this polygon, or by the
    // start of some holes that we must incorporate into the polygon chain
    Assert (endPointIter->bFront == false);
    TChainList<Real>::iterator  targetChainIter = endPointIter->chainIter;
    ++endPointIter;
    Assert (endPointIter->bFront == true);
    
    // we'll need a way to tag chains as being holes so we know not to
    // treat their front as a new polygon
    IteratorSet<Real>           holes;
    
    // loop until we find the end of our starting polygon
    while (endPointIter->chainIter != targetChainIter)
    {
        // check to see if we are looking at the front or the back of a chain
        if (endPointIter->bFront)
        {
            // this is a hole, tag it
            holes.insert (endPointIter->chainIter);
            
            // move the contents of the chain from the hole to the poly
            TChainList<Real>::iterator  holeChainIter = endPointIter->chainIter;
            targetChainIter->insert (targetChainIter->end (), holeChainIter->begin (), holeChainIter->end ());
            chainsOut.erase (holeChainIter);
        }
        else
        {
            // if we haven't seen the back of this chain, it's the start of a 
            // new poly, recur, and pick up where it leaves off
            if (holes.find (endPointIter->chainIter) == holes.end ())
                endPointIter = JoinChains (chainsOut, endPointIter);
        }
        
        // increment the iterator
        ++endPointIter;
    }
    
    // return an iterator to the next end point
    return endPointIter;
}

//-----------------------------------------------------------------------------
template<class Real>
void
ExamineResultingChains (TChainList<Real>& chainsOut, const TEdge2<Real>& edge)
{
    // if there is only one chain, then the clipped chain was entirely to the 
    // left of the clip line. This means we have no more work to do
    if (chainsOut.size () > 1)
    {
        // there's more than one chain, meaning that the clipped chain
        // crossed the clipping line at least twice. An artifact of the
        // Sutherland & Hodgman style clipper we're using is that the last
        // chain built should be connected to the first chain.
        TChain<Real>&   firstChain = chainsOut.front ();
        TChain<Real>&   lastChain = chainsOut.back ();
        firstChain.insert (firstChain.begin (), lastChain.begin (), lastChain.end ());
        chainsOut.pop_back ();
        
        // check to see if there is still more than one chain
        if (chainsOut.size () > 1)
        {
            // the number of chains remaining is the number of endcaps we'll
            // need. In order to establish that the chains aren't independent
            // (e.g. they aren't the tips of a concave object as opposed to 
            // the body of the poly), we have to sort the intersection points 
            // along the clip line. This is a direct result of Jordan's curve
            // theorem. See diagram:
            //
            //               |                |
            //      +--------+                +-----+
            //      |XXXXXXXX|                |XXXXX|
            //      |XX+-----+                +-----+
            //      |XX|     |                |
            //      |XX|     |       vs.      |
            //      |XX|     |                |
            //      |XX+-----+                +-----+
            //      |XXXXXXXX|                |XXXXX|
            //      +--------+                +-----+
            //               |                |
            //
            
            // set up the array that we'll use for sorting along the clip line
            EndPointVec<Real>           endPointVec;
            
            // compute the reference vector that describes the line
            TVector2<Real>              ab = (edge.b - edge.a).Normalized ();
            
            // loop over all of the chains to build the sort array
            TChainList<Real>::iterator  chainIter = chainsOut.begin ();
            TChainList<Real>::iterator  chainEnd = chainsOut.end ();
            while (chainIter != chainEnd)
            {
                Assert (chainIter->size () > 1);
                
                // get the first point of the chain, and qualify it with 
                // respect to the clip line
                Assert (LineTest (edge.a, edge.b, chainIter->front ().pt) == COLINEAR);
                TVector2<Real>  compareVec = chainIter->front ().pt - edge.a;
                Real            rOffsetFront = compareVec DOT ab;
                
                // get the last point of the chain, and qualify it with 
                // respect to the clip line
                Assert (LineTest (edge.a, edge.b, chainIter->back ().pt) == COLINEAR);
                compareVec = chainIter->back ().pt - edge.a;
                Real            rOffsetEnd = compareVec DOT ab;
                
                // push the endpoint references onto the sort array
                endPointVec.push_back (EndPoint<Real> (chainIter, rOffsetFront, rOffsetEnd, true));
                endPointVec.push_back (EndPoint<Real> (chainIter, rOffsetEnd, rOffsetFront, false));
                
                // advance to the next chain
                ++chainIter;
            }
            
            // sort the end point array
            std::sort (endPointVec.begin (), endPointVec.end (), EndPointCmp<Real>);
            DEBUG_PRINT (endPointVec);
            
            // join the chains based on the end point sequence
            EndPointVec<Real>::iterator endPointIter = endPointVec.begin ();
            do
            {
                endPointIter = JoinChains (chainsOut, endPointIter);
                ++endPointIter;
            }
            while (endPointIter != endPointVec.end ());
        }
    }
}

//-----------------------------------------------------------------------------
// public functions
//-----------------------------------------------------------------------------
template<class Real>
void
ClipToLine (TChainList<Real>& chainsOut, const TChainList<Real>& chainsIn, const TEdge2<Real>& edge)
{
    // loop over all of the chains in the input set
    TChainList<Real>::const_iterator    inputChainIter = chainsIn.begin ();
    TChainList<Real>::const_iterator    inputChainEnd = chainsIn.end ();
    while (inputChainIter != inputChainEnd)
    {
        // fetch the current chain, and make sure it's valid
        const TChain<Real>& chain = *inputChainIter++;
        Assert (chain.GetClosed ());
        Assert (chain.TestWindingOrder () == TURNS_LEFT);
        Assert (chain.TestSimple ());
        
#if DEBUG_GEOMETRY
        DEBUG_PRINT_SCOPE;
        if (0)
        //if ((TChain<Real>::GetNextID () >= 135436) and (TChain<Real>::GetNextID () < 135441))
        {
            DEBUG_PRINT_ON;
            DEBUG_PRINT (chain);
        }
#endif
        
        // set up the vertex count value
        uInt2               iVertexCount = chain.size ();
        
        // set up the chain list that we will build, and push a new chain
        // onto it for starters
        TChainList<Real>    chainList;
        chainList.push_back (TChain<Real> ());
        
        // set up the vertices and test values
        TVertex2<Real>      c = chain[iVertexCount - 1];
        TVertex2<Real>      d = chain[0];
        sInt                fTestC = LineTest (edge, c.pt);
        sInt                fTestD = LineTest (edge, d.pt);
        
        // loop over all of the edges testing them against the line
        for (uInt2 i = 1; i <= iVertexCount; i++)
        {
            // copy the cached vertices and their tests against the line
            TVertex2<Real>  lastC = c;
            sInt            fLastTestC = fTestC;
            c = d;
            fTestC = fTestD;

            // get the next point, and test it against the line
            d = chain[i % iVertexCount];
            fTestD = LineTest (edge, d.pt);
            
            // build an intersection code, and look up the action for it
            sInt            iCode = MAKE_INTERSECTION_CODE (fLastTestC, fTestC, fTestD);
            sInt            iAction = g_IntersectionCode[iCode];

            // there is one special case if c is colinear, and last and d
            // are both to the left of the clipping line. In order to
            // decide what to do, we need to check to see if d is to the
            // right or left of c
            if ((iAction == SPECIAL_CASE) and (LineTest (lastC.pt, c.pt, d.pt) == TURNS_LEFT))
                iAction = PRE_EMIT;

            // walk through the possible actions doing them if we should
            if (iAction bit_and PRE_EMIT)
                chainList.back ().push_back (c);
            if (iAction bit_and PRE_SECT)
                chainList.back ().push_back (IntersectSegments (edge.a, edge.b, c, d));
            if (iAction bit_and NEW_CHAIN)
                chainList.push_back (TChain<Real> ());
            if (iAction bit_and POST_SECT)
                chainList.back ().push_back (IntersectSegments (edge.a, edge.b, c, d));
            if (iAction bit_and POST_EMIT)
                chainList.back ().push_back (c);
        }
        
        // if a chain is completely to the right of the line, we'll get here
        // with an empty chain on the chain list. It's possible that an empty
        // chain is on the list if we happened to work with points that are
        // colinear with the clipping line, so we test for these situations
        // to avoid sending empty lists to the output
        if ((chainList.size () > 1) or (chainList.front ().size () > 0))
        {
            // examine our results to remove artifacts
            ExamineResultingChains (chainList, edge);
            
            // validate the results
            chainList.ValidatePolygon ();
            
            // append our chain list to the output list
            chainsOut.insert (chainsOut.end (), chainList.begin (), chainList.end ());
        }
    }
}

//-----------------------------------------------------------------------------
template<class Real>
void    
ClipToBound (TChainList<Real>& chainsOut, const TChainList<Real>& chainsIn, const TBound2<Real>& bound)
{
    // we traverse the bound one edge at a time in Sutherland & Hodgman fashion, 
    // clipping the input chains to the edges
    TChainList<Real>    tmpChainListClippedLEFT;
    TChainList<Real>    tmpChainListClippedTOP;
    TChainList<Real>    tmpChainListClippedRIGHT;
    
    if (chainsIn.size () > 0)
        ClipToLine (tmpChainListClippedLEFT, chainsIn, bound.GetEdge (TBound2<Real>::LEFT));
    if (tmpChainListClippedLEFT.size () > 0)
        ClipToLine (tmpChainListClippedTOP, tmpChainListClippedLEFT, bound.GetEdge (TBound2<Real>::TOP));
    if (tmpChainListClippedTOP.size () > 0)
        ClipToLine (tmpChainListClippedRIGHT, tmpChainListClippedTOP, bound.GetEdge (TBound2<Real>::RIGHT));
    if (tmpChainListClippedRIGHT.size () > 0)
        ClipToLine (chainsOut, tmpChainListClippedRIGHT, bound.GetEdge (TBound2<Real>::BOTTOM));
}

//-----------------------------------------------------------------------------

#endif  //  _CLIP_INL_
