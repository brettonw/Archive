//-----------------------------------------------------------------------------
// Copyright (C) 2002-2005 Bretton Wade, All rights reserved
//-----------------------------------------------------------------------------

#ifndef     _CLIP_H_
#define     _CLIP_H_

//-----------------------------------------------------------------------------
// includes
//-----------------------------------------------------------------------------
#ifndef     _CHAIN_LIST_H_
#include    "chain_list.h"
#endif  //  _CHAIN_LIST_H_

//-----------------------------------------------------------------------------
// functions
//-----------------------------------------------------------------------------
template<class Real>
void    ClipToLine (TChainList<Real>& chainsOut, const TChainList<Real>& chainsIn, const TEdge2<Real>& edge);
template<class Real>
void    ClipToBound (TChainList<Real>& chainsOut, const TChainList<Real>& chainsIn, const TBound2<Real>& bound);

//-----------------------------------------------------------------------------
// inlines
//-----------------------------------------------------------------------------
#ifndef     _CLIP_INL_
#include    "clip.inl"
#endif  //  _CLIP_INL_

//-----------------------------------------------------------------------------

#endif  //  _CLIP_H_
