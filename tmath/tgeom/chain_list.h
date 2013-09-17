//-----------------------------------------------------------------------------
// Copyright (C) 2002-2005 Bretton Wade, All rights reserved
//-----------------------------------------------------------------------------

#ifndef     _CHAIN_LIST_H_
#define     _CHAIN_LIST_H_

//-----------------------------------------------------------------------------
// includes
//-----------------------------------------------------------------------------
#ifndef     _CHAIN_H_
#include    "chain.h"
#endif      _CHAIN_H_

//-----------------------------------------------------------------------------
// class definitions
//-----------------------------------------------------------------------------
template<class Real>
class TChainList : public std::list<TChain <Real> >
{
    public:
        void                DebugPrint (void) const;
        void                ValidatePolygon (void);
        TBound2<Real>       GetBound (void) const;
        bool                ContainsPoint (const TPoint2<Real>& point) const;
        void                ReverseChains (void);
        void                Duplicate (const TChainList<Real>& src);
        void                Expanded (const TChainList<Real>& src, Real rAdd, Real rTesselationLimit = static_cast<Real> (0.9));
        void                SetUVCoordinates (const TBound2<Real>& uvBound);
};

//-----------------------------------------------------------------------------
// inlines
//-----------------------------------------------------------------------------
#ifndef     _CHAIN_LIST_INL_
#include    "chain_list.inl"
#endif  //  _CHAIN_LIST_INL_

//-----------------------------------------------------------------------------

#endif  //  _CHAIN_LIST_H_
