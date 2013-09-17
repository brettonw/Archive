//-----------------------------------------------------------------------------
// Copyright (C) 2002-2005 Bretton Wade, All rights reserved
//-----------------------------------------------------------------------------

#ifndef     _PLANE_INL_
#define     _PLANE_INL_

//-----------------------------------------------------------------------------
// includes
//-----------------------------------------------------------------------------
#ifndef     _PLANE_H_
#include    "plane.h"
#endif      _PLANE_H_

#ifndef     _MSR_SVD_H_
#include    "msr_svd.h"
#endif  //  _MSR_SVD_H_

//-----------------------------------------------------------------------------
// template methods
//-----------------------------------------------------------------------------
template<class Real>
/* void */
TPlane<Real>::TPlane (const TPlane<Real>& plane) :
    normal (plane.normal),
    d (plane.d)
{
}

//-----------------------------------------------------------------------------
template<class Real>
/* void */
TPlane<Real>::TPlane (Real _a, Real _b, Real _c, Real _d) :
    normal (_a, _b, _c),
    d (_d)
{
    Assert (normal.IsUnit ());
}

//-----------------------------------------------------------------------------
template<class Real>
/* void */
TPlane<Real>::TPlane (const TVector3<Real>& _normal, const TPoint3<Real>& point_on_plane) :
    normal (_normal),
    d (-(normal DOT point_on_plane))
{
    Assert (normal.IsUnit ());
}

//-----------------------------------------------------------------------------
template<class Real>
/* void */
TPlane<Real>::TPlane (const std::vector<TPoint3<Real> >& fit_points)
{
    // compute the median point of the fit point set - this point will be on
    // the plane that is fit
    uInt                fit_points_size = fit_points.size ();
    Assert (fit_points_size > 2);
    TVector3<Real>      medianVec (0, 0, 0);
    for (uInt i = 0; i < fit_points_size; i++)
        medianVec += TVector3<Real> (fit_points[i]);
    medianVec /= static_cast<Real> (fit_points_size);
    
    // we would like to fit a plane to the set of points, but the means of doing 
    // so is very memory intensive. In particular, allocations will be made that
    // are the square of the number of fit points. We limit the size by saying
    // that if any given body of water covers more than a reasonable number of
    // samples, then it must not be sloped at all (it's ocean or a really big 
    // lake).
    if (fit_points_size < 2000)
    {
        // in order to calculate the normal vector, we will set up a set up the 
        // design matrix for a SVD (Singular Value Decomposition). The decomposition
        // itself will produce the normal we need without having to do the back
        // substitution.
        MSR_Matrix<Real>    design;
        design.SetDimension (fit_points_size, 3);
        for (uInt i = 0 ; i < fit_points_size ; i++)
        {
            // compute the delta from the median point to the current point
            const TPoint3<Real>  fit_point = fit_points[i] - medianVec;
            
            // set the design matrix to the value locations we want to fit
            design (i, 0) = fit_point.x;
            design (i, 1) = fit_point.y;
            design (i, 2) = fit_point.z;
        }

        // use the SVD to calculate the decomposition of the design matrix
        MSR_Matrix<Real>    U;
        MSR_Matrix<Real>    w;
        MSR_Matrix<Real>    Vt;
        ConstructSVD (design, U, w, Vt);
        
        // Vt contains what we need, the normal to the fit plane. In particular, it's
        // the row with the lowest singular value, as determined by examining the W
        // matrix.
        Real                w1 = AbsoluteValue (w (0, 0));
        Real                w2 = AbsoluteValue (w (1, 0));
        Real                w3 = AbsoluteValue (w (2, 0));
        Real                selectValue = w1;
        normal =  TVector3<Real> (Vt (0, 0), Vt (0, 1), Vt (0, 2));
        if (w2 < selectValue)
        {
            selectValue = w2;
            normal = TVector3<Real> (Vt (1, 0), Vt (1, 1), Vt (1, 2));
        }
        if (w3 < selectValue)
            normal = TVector3<Real> (Vt (2, 0), Vt (2, 1), Vt (2, 2));

        Assert (normal.IsUnit ());
    }
    else
    {
        // it's really big, so give it very little slope
        normal = TVector3<Real> (0, 1, 0);
    }
    
    // compute the plane equation from the found normal and median point
    d = -(normal DOT medianVec);
}

//-----------------------------------------------------------------------------
template<class Real>
/* void */
TPlane<Real>::TPlane (const std::vector<TPoint3<Real> >& fit_points, const TVector3<Real>& _normal)
{
    // compute the median point of the fit point set - this point will be on
    // the plane that is fit
    uInt                fit_points_size = fit_points.size ();
    Assert (fit_points_size > 2);
    TVector3<Real>      medianVec (0, 0, 0);
    for (uInt i = 0; i < fit_points_size; i++)
        medianVec += TVector3<Real> (fit_points[i]);
    medianVec /= static_cast<Real> (fit_points_size);
    
    // copy the passed normal
    normal = _normal;
    
    // compute the plane equation from the given normal and median point
    d = -(normal DOT medianVec);
}

//-----------------------------------------------------------------------------
template<class Real>
Real
TPlane<Real>::DistanceToPoint (const TPoint3<Real>& point, const TVector3<Real>& axis) const
{
    Assert (axis.IsUnit ());
    Real    rDenominator = normal DOT axis;
    if (FuzzyNotEquals<Real> (0, rDenominator))
        return -((normal DOT point) + d) / rDenominator;
    else
        return 0;
}

//-----------------------------------------------------------------------------
template<class Real>
TPoint3<Real>
TPlane<Real>::PointOnPlane (void) const
{
    TPoint3<Real>   point = TPoint3<Real> (0, 0, 0) + (normal * -d);
    Assert (FuzzyEquals<Real> (DistanceToPoint (point, normal), 0, static_cast<Real> (1.0e-4)));
    return point;
}

//-----------------------------------------------------------------------------

#endif  //  _PLANE_INL_
