//-----------------------------------------------------------------------------
// Copyright (C) 2002-2005 Bretton Wade, All rights reserved
//-----------------------------------------------------------------------------

#ifndef     _PLANE_H_
#define     _PLANE_H_

//-----------------------------------------------------------------------------
// includes
//-----------------------------------------------------------------------------
#ifndef     _VECTOR_H_
#include    "vector.h"
#endif      _VECTOR_H_

//-----------------------------------------------------------------------------
// class definitions
//-----------------------------------------------------------------------------
template<class Real>
struct TPlane
{
    // the basic data
    TVector3<Real>  normal;
    Real            d;
    
    // constructors
    /* void */      TPlane (void) {}
    /* void */      TPlane (const TPlane& plane);
    /* void */      TPlane (Real a, Real b, Real c, Real d);
    /* void */      TPlane (const TVector3<Real>& normal, const TPoint3<Real>& point_on_plane);
    /* void */      TPlane (const std::vector<TPoint3<Real> >& fit_points);
    /* void */      TPlane (const std::vector<TPoint3<Real> >& fit_points, const TVector3<Real>& normal);
    
    // plane operations
    Real            DistanceToPoint (const TPoint3<Real>& point, const TVector3<Real>& axis = normal) const;
    TPoint3<Real>   PointOnPlane (void) const;
    
    // special constructor for alternate Real types
    template<class alternate>
    /* void */      TPlane (const TPlane<alternate>& plane) :
        normal (static_cast<Real> (plane.normal)),
        d (static_cast<Real> (plane.d))
    {}
};

//-----------------------------------------------------------------------------
// inlines
//-----------------------------------------------------------------------------
#ifndef     _PLANE_INL_
#include    "plane.inl"
#endif  //  _PLANE_INL_

//-----------------------------------------------------------------------------

#endif  //  _PLANE_H_
