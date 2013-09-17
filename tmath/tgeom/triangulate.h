//-----------------------------------------------------------------------------
// Copyright (C) 2002-2005 Bretton Wade, All rights reserved
//-----------------------------------------------------------------------------

#ifndef     _TRIANGULATE_H_
#define     _TRIANGULATE_H_

//-----------------------------------------------------------------------------
// includes
//-----------------------------------------------------------------------------
#ifndef     _CHAIN_LIST_H_
#include    "chain_list.h"
#endif  //  _CHAIN_LIST_H_

//-----------------------------------------------------------------------------
// types
//-----------------------------------------------------------------------------
struct Triangle
{
    uInt2       a, b, c;
    Triangle (uInt2 _a, uInt2 _b, uInt2 _c) :
        a (_a), b (_b), c (_c) {}
};

class TriangleVec : public std::vector<Triangle>
{
    public:
        void    DebugPrint (void) const
        {
            const_iterator    iter = begin ();
            const_iterator    iterEnd = end ();
            while (iter != iterEnd)
            {
                const Triangle& tri = *iter++;
                ::DebugPrint ("Triangle: %u, %u, %u\n", tri.a, tri.b, tri.c);
            }   
        }
};

//-----------------------------------------------------------------------------
// functions
//-----------------------------------------------------------------------------
template<class Real>
void    Triangulate (const TChainList<Real>& chainList, TChain<Real>& chainOut, TriangleVec& trianglesOut);

template<class Real>
void    ClampEdgeLengths (TChain<Real>& chainInOut, const TriangleVec& trianglesIn, TriangleVec& trianglesOut, Real rMaxEdgeLength);

//-----------------------------------------------------------------------------
// inlines
//-----------------------------------------------------------------------------
#ifndef     _TRIANGULATE_INL_
#include    "triangulate.inl"
#endif  //  _TRIANGULATE_INL_

//-----------------------------------------------------------------------------

#endif  //  _TRIANGULATE_H_
