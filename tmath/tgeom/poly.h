//-----------------------------------------------------------------------------
// Copyright (C) 2002-2005 Bretton Wade, All rights reserved
//-----------------------------------------------------------------------------

#ifndef     _POLY_H_
#define     _POLY_H_

//-----------------------------------------------------------------------------
// includes
//-----------------------------------------------------------------------------
#ifndef     _CHAIN_LIST_H_
#include    "chain_list.h"
#endif  //  _CHAIN_LIST_H_

//-----------------------------------------------------------------------------
// class definitions
//-----------------------------------------------------------------------------
template<class Real>
class TPoly2
{
    public:
        /* void */              TPoly2 (void) {}
        /* void */              TPoly2 (const TPoly2<Real>& poly);
        /* void */              TPoly2 (const TChain<Real>& chain);
        /* void */              TPoly2 (const TChainList<Real>& chainList);
        
        bool                    IsEmpty (void) const;
        TBound2<Real>           GetBound (void) const;
        bool                    ContainsPoint (const TPoint2<Real>& point) const;
        TChainList<Real>&       GetChains (void);
        const TChainList<Real>& GetChains (void) const;
        
        TPoly2<Real>            ClipToEdge (const TEdge2<Real>& edge) const;
        TPoly2<Real>            ClipToBound (const TBound2<Real>& bound) const;
        void                    Reverse (void);
        TPoly2<Real>            Duplicate (void);
        
        uInt                    OutputTriangles (const TPoint2<Real>& ptOrigin, FILE* fp) const;
        
        void                    SetUVCoordinates (const TPoint2<Real>& uvOrigin, const TBound2<Real>& uvBound);
        const TPoint2<Real>     GetUVOrigin (void) const;

static  void                    Merge (TPoly2<Real>& dest, const TPoly2<Real>& src);
        
    protected:
        TChainList<Real>        m_chainList;
        TPoint2<Real>           m_uvOrigin;
};

//-----------------------------------------------------------------------------
// inlines
//-----------------------------------------------------------------------------
#ifndef     _POLY_INL_
#include    "poly.inl"
#endif  //  _POLY_INL_

//-----------------------------------------------------------------------------

#endif  //  _POLY_H_
