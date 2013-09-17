//-----------------------------------------------------------------------------
// Copyright (C) 2002-2005 Bretton Wade, All rights reserved
//-----------------------------------------------------------------------------

#ifndef     _BOUND_INL_
#define     _BOUND_INL_

//-----------------------------------------------------------------------------
// includes
//-----------------------------------------------------------------------------
#ifndef     _BOUND_H_
#include    "bound.h"
#endif  //  _BOUND_H_

//-----------------------------------------------------------------------------
// template methods
//-----------------------------------------------------------------------------
template<class Real>
/* void */
TBound2<Real>::TBound2 (void) : 
    m_ptMin (FLT_MAX, FLT_MAX),
    m_ptMax (-FLT_MAX, -FLT_MAX)
{
}

//-----------------------------------------------------------------------------
template<class Real>
/* void */
TBound2<Real>::TBound2 (const TBound2<Real>& bound) :
    m_ptMin (bound.m_ptMin),
    m_ptMax (bound.m_ptMax)
{
}

//-----------------------------------------------------------------------------
template<class Real>
/* void */
TBound2<Real>::TBound2 (const TPoint2<Real>& ptMin, const TPoint2<Real>& ptMax) :
    m_ptMin (ptMin),
    m_ptMax (ptMax)
{
}

//-----------------------------------------------------------------------------
template<class Real>
const TPoint2<Real>&
TBound2<Real>::GetMin (void) const
{
    return m_ptMin;
}

//-----------------------------------------------------------------------------
template<class Real>
const TPoint2<Real>&
TBound2<Real>::GetMax (void) const
{
    return m_ptMax;
}

//-----------------------------------------------------------------------------
template<class Real>
const TPoint2<Real>
TBound2<Real>::GetCenter (void) const
{
    return m_ptMin + ((m_ptMax - m_ptMin) / 2);
}

//-----------------------------------------------------------------------------
template<class Real>
TEdge2<Real>
TBound2<Real>::GetEdge (EdgeID edgeID) const
{
    switch (edgeID)
    {
        default:
            Assert (false and "bad data ?!?!?");
            // fall through
            
        case LEFT:
            return TEdge2<Real> (TPoint2<Real> (m_ptMin.x, m_ptMax.y), m_ptMin);
            
        case TOP:
            return TEdge2<Real> (m_ptMax, TPoint2<Real> (m_ptMin.x, m_ptMax.y));

        case RIGHT:
            return TEdge2<Real> (TPoint2<Real> (m_ptMax.x, m_ptMin.y), m_ptMax);

        case BOTTOM:
            return TEdge2<Real> (m_ptMin, TPoint2<Real> (m_ptMax.x, m_ptMin.y));
    }
}

//-----------------------------------------------------------------------------
template<class Real>
void
TBound2<Real>::DebugPrint (void) const
{
    ::DebugPrint ("Bound: min (%g, %g); max (%g, %g); span (%g)\n", m_ptMin.x, m_ptMin.y, m_ptMax.x, m_ptMax.y, (m_ptMax - m_ptMin).Norm ());
}

//-----------------------------------------------------------------------------
template<class Real>
void
TBound2<Real>::AddPoint (const TPoint2<Real>& point)
{
    // update the minimum bound if appropriate
    m_ptMin.x = (point.x < m_ptMin.x) ? point.x : m_ptMin.x;
    m_ptMin.y = (point.y < m_ptMin.y) ? point.y : m_ptMin.y;

    // update the maximum bound if appropriate
    m_ptMax.x = (point.x > m_ptMax.x) ? point.x : m_ptMax.x;
    m_ptMax.y = (point.y > m_ptMax.y) ? point.y : m_ptMax.y;
}

//-----------------------------------------------------------------------------
template<class Real>
void
TBound2<Real>::AddBound (const TBound2<Real>& bound)
{
    // update the minimum bound if appropriate
    const TPoint2<Real>&    ptMin = bound.GetMin ();
    m_ptMin.x = (ptMin.x < m_ptMin.x) ? ptMin.x : m_ptMin.x;
    m_ptMin.y = (ptMin.y < m_ptMin.y) ? ptMin.y : m_ptMin.y;
    
    // update the maximum bound if appropriate
    const TPoint2<Real>&    ptMax = bound.GetMax ();
    m_ptMax.x = (ptMax.x > m_ptMax.x) ? ptMax.x : m_ptMax.x;
    m_ptMax.y = (ptMax.y > m_ptMax.y) ? ptMax.y : m_ptMax.y;
}

//-----------------------------------------------------------------------------
template<class Real>
TBound2<Real>
TBound2<Real>::Scaled (Real rScale) const
{
    // get the center point, so that scale calculations can be done relative to it
    TPoint2<Real>   ptCenter = GetCenter ();
    
    // compute the half width and half height
    Real            rHalfWidth = (m_ptMax.x - m_ptMin.x) / 2;
    Real            rHalfHeight = (m_ptMax.y - m_ptMin.y) / 2;
    
    // compute the new min and max points
    TVector2<Real>  halfSize (rHalfWidth, rHalfHeight);
    TPoint2<Real>   ptMin = ptCenter - (halfSize * rScale);
    TPoint2<Real>   ptMax = ptCenter + (halfSize * rScale);
    
    // return the scaled bound
    return TBound2<Real> (ptMin, ptMax);
}

//-----------------------------------------------------------------------------
template<class Real>
TBound2<Real>
TBound2<Real>::Expanded (Real rAdd) const
{
    // pass the add parameter twice to the core version of this method
    return Expanded (rAdd, rAdd);
}

//-----------------------------------------------------------------------------
template<class Real>
TBound2<Real>
TBound2<Real>::Expanded (Real rAddX, Real rAddY) const
{
    // create a vector representing the addition to make
    TVector2<Real>  vAdd (rAddX, rAddY);
    
    // create and return a new bound with the min and max points translated
    // by the add vector
    return TBound2<Real> (m_ptMin - vAdd, m_ptMax + vAdd);
}

//-----------------------------------------------------------------------------
template<class Real>
bool
TBound2<Real>::ContainsPoint (const TPoint2<Real>& point) const
{
    return  (point.x >= m_ptMin.x) and
            (point.x <= m_ptMax.x) and
            (point.y >= m_ptMin.y) and
            (point.y <= m_ptMax.y);
}

//-----------------------------------------------------------------------------
template<class Real>
bool
TBound2<Real>::IntersectsBound (const TBound2<Real>& bound)
{
    // get some data from the test bound
    const TPoint2<Real>&    ptMin = bound.GetMin ();
    const TPoint2<Real>&    ptMax = bound.GetMax ();

    // for each axis, check to see if the bounds are disjoint
    if ((m_ptMin.x > ptMax.x) or (m_ptMax.x < ptMin.x))
        return false;
    if ((m_ptMin.y > ptMax.y) or (m_ptMax.y < ptMin.y))
        return false;
        
    // the bounds aren't disjoint on any axis, there must be some overlap
    return true;
}

//-----------------------------------------------------------------------------

#endif  //  _BOUND_INL_
