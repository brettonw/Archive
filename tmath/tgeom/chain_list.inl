//-----------------------------------------------------------------------------
// Copyright (C) 2002-2005 Bretton Wade, All rights reserved
//-----------------------------------------------------------------------------

#ifndef     _CHAIN_LIST_INL_
#define     _CHAIN_LIST_INL_

//-----------------------------------------------------------------------------
// includes
//-----------------------------------------------------------------------------
#ifndef     _CHAIN_LIST_H_
#include    "chain_list.h"
#endif      _CHAIN_LIST_H_

//-----------------------------------------------------------------------------
// class definitions
//-----------------------------------------------------------------------------
template<class Real>
void
TChainList<Real>::DebugPrint (void) const
{
    ::DebugPrint ("Chain List (%d chains)\n", size ());
    const_iterator  iter = begin ();
    const_iterator  iterEnd = end ();
    while (iter != iterEnd)
    {
        iter->DebugPrint ();
        ++iter;
    }
}

//-----------------------------------------------------------------------------
template<class Real>
void
TChainList<Real>::ValidatePolygon (void)
{
    // loop over all of the chains in the chain list
    iterator    iter = begin ();
    while (iter != end ())
    {
        // get the current chain
        TChain<Real>&   chain = *iter;
        
        // validate the chain, and check the result. We drop the chain if it's
        // not valid, 
        if (chain.ValidatePolygon ())
            ++iter;
        else
            iter = erase (iter);
    }
}

//-----------------------------------------------------------------------------
template<class Real>
TBound2<Real>
TChainList<Real>::GetBound (void) const
{
    // set up a bound to accumulate
    TBound2<Real>   bound;
    
    // loop over all the chains in the list
    const_iterator  iter = begin ();
    const_iterator  iterEnd = end ();
    while (iter != iterEnd)
    {
        // fetch the current chain
        const TChain<Real>& chain = *iter++;
        
        // get the chain's bound
        TBound2<Real>       chainBound = chain.GetBound ();
        
        // add that bound into the one I'm accumulating
        bound.AddBound (chainBound);
    }
    
    // return the accumulated bound
    return bound;
}

//-----------------------------------------------------------------------------
template<class Real>
bool
TChainList<Real>::ContainsPoint (const TPoint2<Real>& point) const
{
    // loop over all the chains in the list
    const_iterator  iter = begin ();
    const_iterator  iterEnd = end ();
    while (iter != iterEnd)
    {
        // fetch the current chain
        const TChain<Real>& chain = *iter++;
        
        // check to see if the chain contains the point
        if (chain.ContainsPoint (point))
            return true;
    }
    
    // we got here, nobody contained the point
    return false;
}

//-----------------------------------------------------------------------------
template<class Real>
void
TChainList<Real>::ReverseChains (void)
{
    // loop over all the chains in the list
    iterator    iter = begin ();
    iterator    iterEnd = end ();
    while (iter != iterEnd)
    {
        // fetch the current chain
        TChain<Real>& chain = *iter++;
        
        // reverse the chain
        std::reverse (chain.begin (), chain.end ());
    }
}

//-----------------------------------------------------------------------------
template<class Real>
void
TChainList<Real>::Duplicate (const TChainList<Real>& src)
{
    // clear the current contents of the list
    clear ();

    // loop over all of the source chains
    const_iterator  srcIter = src.begin ();
    const_iterator  srcIterEnd = src.end ();
    while (srcIter != srcIterEnd)
    {
        // fetch the current chain
        const TChain<Real>& srcChain = *srcIter++;
        
        // duplicate the source chain, and push the result onto the list
        TChain<Real>    dstChain (srcChain.GetClosed ());
        dstChain.insert (dstChain.begin (), srcChain.begin (), srcChain.end ());
        push_back (dstChain);
    }
}

//-----------------------------------------------------------------------------
template<class Real>
void
TChainList<Real>::Expanded (const TChainList<Real>& src, Real rAdd, Real rTesselationLimit)
{
    // clear the current contents of the list
    clear ();
    
    // loop over all of the source chains
    const_iterator  srcIter = src.begin ();
    const_iterator  srcIterEnd = src.end ();
    while (srcIter != srcIterEnd)
    {
        // fetch the current chain
        const TChain<Real>& srcChain = *srcIter++;
        
        // expand the source chain, and push the result onto the list
        push_back (srcChain.Expanded (rAdd, rTesselationLimit));
    }
}

//-----------------------------------------------------------------------------
template<class Real>
void
TChainList<Real>::SetUVCoordinates (const TBound2<Real>& uvBound)
{
    // loop over all the chains in the list
    iterator    iter = begin ();
    iterator    iterEnd = end ();
    while (iter != iterEnd)
    {
        // fetch the current chain
        TChain<Real>& chain = *iter++;
        
        // set the uv coordinates on the chain
        chain.SetUVCoordinates (uvBound);
    }
}

//-----------------------------------------------------------------------------

#endif  //  _CHAIN_LIST_INL_
