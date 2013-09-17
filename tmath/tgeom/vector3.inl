//-----------------------------------------------------------------------------
// Copyright (C) 2002-2005 Bretton Wade, All rights reserved
//-----------------------------------------------------------------------------

#ifndef     _VECTOR3_INL_
#define     _VECTOR3_INL_

//-----------------------------------------------------------------------------
// includes
//-----------------------------------------------------------------------------
#ifndef     _VECTOR_H_
#include    "vector.h"
#endif      _VECTOR_H_

//-----------------------------------------------------------------------------
// template methods
//-----------------------------------------------------------------------------
template<class Real>
TVector3<Real>
TVector3<Real>::Unit (Real x, Real y, Real z)
{
    return (TVector3<Real> (x, y, z)).Normalized ();
}

//-----------------------------------------------------------------------------
template<class Real>
/* void */
TVector3<Real>::TVector3 (const TVector3<Real>& vec) :
    x (vec.x),
    y (vec.y),
    z (vec.z)
{
}

//-----------------------------------------------------------------------------
template<class Real>
/* void */  
TVector3<Real>::TVector3 (const TPoint3<Real>& point) :
    x (point.x),
    y (point.y),
    z (point.z)
{
}

//-----------------------------------------------------------------------------
template<class Real>
/* void */  
TVector3<Real>::TVector3 (Real _x, Real _y, Real _z) :
    x (_x),
    y (_y),
    z (_z)
{
}

//-----------------------------------------------------------------------------
template<class Real>
TVector3<Real>    
TVector3<Real>::operator + (const TVector3<Real>& vec) const
{
    return TVector3<Real> (x + vec.x, y + vec.y, z + vec.z);
}

//-----------------------------------------------------------------------------
template<class Real>
TVector3<Real>    
TVector3<Real>::operator - (const TVector3<Real>& vec) const
{
    return TVector3<Real> (x - vec.x, y - vec.y, z - vec.z);
}

//-----------------------------------------------------------------------------
template<class Real>
TVector3<Real>    
TVector3<Real>::operator * (Real scalar) const
{
    return TVector3<Real> (x * scalar, y * scalar, z * scalar);
}

//-----------------------------------------------------------------------------
template<class Real>
TVector3<Real>   
TVector3<Real>::operator / (Real scalar) const
{
    Real    over_scalar = 1 / scalar;
    return TVector3<Real> (x * over_scalar, y * over_scalar, z * over_scalar);
}

//-----------------------------------------------------------------------------
template<class Real>
bool        
TVector3<Real>::operator == (const TVector3<Real>& vec) const
{
    return FuzzyEquals (*this, vec, REAL_EPSILON);
}

//-----------------------------------------------------------------------------
template<class Real>
bool        
TVector3<Real>::operator != (const TVector3<Real>& vec) const
{
    return FuzzyNotEquals (*this, vec, REAL_EPSILON);
}

//-----------------------------------------------------------------------------
template<class Real>
bool        
TVector3<Real>::FuzzyEquals (const TVector3& lhs, const TVector3& rhs, Real rEpsilon)
{
    return ::FuzzyEquals<Real> (x, vec.x, rEpsilon) and 
           ::FuzzyEquals<Real> (y, vec.y, rEpsilon) and 
           ::FuzzyEquals<Real> (z, vec.z, rEpsilon);
}

//-----------------------------------------------------------------------------
template<class Real>
bool        
TVector3<Real>::FuzzyNotEquals (const TVector3& lhs, const TVector3& rhs, Real rEpsilon)
{
    return ::FuzzyNotEquals<Real> (x, vec.x, rEpsilon) or 
           ::FuzzyNotEquals<Real> (y, vec.y, rEpsilon) or 
           ::FuzzyNotEquals<Real> (z, vec.z, rEpsilon);
}

//-----------------------------------------------------------------------------
template<class Real>
TVector3<Real>&   
TVector3<Real>::operator += (const TVector3<Real>& vec)
{
    x += vec.x;
    y += vec.y;
    z += vec.z;
    return *this;
}

//-----------------------------------------------------------------------------
template<class Real>
TVector3<Real>&   
TVector3<Real>::operator -= (const TVector3<Real>& vec)
{
    x -= vec.x;
    y -= vec.y;
    z -= vec.z;
    return *this;
}

//-----------------------------------------------------------------------------
template<class Real>
TVector3<Real>&   
TVector3<Real>::operator *= (Real scalar)
{
    x *= scalar;
    y *= scalar;
    z *= scalar;
    return *this;
}

//-----------------------------------------------------------------------------
template<class Real>
TVector3<Real>&   
TVector3<Real>::operator /= (Real scalar)
{
    Real    over_scalar = 1 / scalar;
    x *= over_scalar;
    y *= over_scalar;
    z *= over_scalar;
    return *this;
}

//-----------------------------------------------------------------------------
template<class Real>
Real        
TVector3<Real>::operator DOT (const TVector3<Real>& vec) const
{
    return (x * vec.x) + (y * vec.y) + (z * vec.z);
}

//-----------------------------------------------------------------------------
template<class Real>
TVector3<Real>
TVector3<Real>::operator CROSS (const TVector3<Real>& vec) const
{
    return TVector3<Real> ((y * vec.z) - (z * vec.y),
                           (z * vec.x) - (x * vec.z),
                           (x * vec.y) - (y * vec.x));
}

//-----------------------------------------------------------------------------
template<class Real>
Real        
TVector3<Real>::Norm (void) const
{
    Real    normSquared = operator DOT (*this);
    if (::FuzzyNotEquals<Real> (normSquared, 0))
        return SquareRoot (normSquared);
    return 0;
}

//-----------------------------------------------------------------------------
template<class Real>
TVector3<Real>
TVector3<Real>::Normalized (Real* pfLength) const
{
    Real    norm = Norm ();
    if (pfLength)
        *pfLength = norm;
    if (::FuzzyNotEquals<Real> (norm, 0))
        return operator / (norm);
    return  TVector3<Real> (0, 0, 0);
}

//-----------------------------------------------------------------------------
template<class Real>
TVector3<Real>&   
TVector3<Real>::Normalize (Real* pfLength)
{
    Real    norm = Norm ();
    if (pfLength)
        *pfLength = norm;
    if (::FuzzyNotEquals<Real> (norm, 0))
        return operator /= (norm);
    return *this;
}

//-----------------------------------------------------------------------------
template<class Real>
bool
TVector3<Real>::IsUnit (void) const
{
    return ::FuzzyEquals<Real> (Norm (), 1);
}

//-----------------------------------------------------------------------------
template<class Real>
/* void */
TPoint3<Real>::TPoint3 (const TPoint3<Real>& point) :
    x (point.x),
    y (point.y),
    z (point.z)
{
}

//-----------------------------------------------------------------------------
template<class Real>
/* void */
TPoint3<Real>::TPoint3 (const TVector3<Real>& vec) :
    x (vec.x),
    y (vec.y),
    z (vec.z)
{
}

//-----------------------------------------------------------------------------
template<class Real>
/* void */
TPoint3<Real>::TPoint3 (Real _x, Real _y, Real _z) :
    x (_x),
    y (_y),
    z (_z)
{
}

//-----------------------------------------------------------------------------
template<class Real>
TPoint3<Real>
TPoint3<Real>::operator + (const TVector3<Real>& vec) const
{
    return TPoint3<Real> (x + vec.x, y + vec.y, z + vec.z);
}

//-----------------------------------------------------------------------------
template<class Real>
TPoint3<Real>
TPoint3<Real>::operator - (const TVector3<Real>& vec) const
{
    return TPoint3<Real> (x - vec.x, y - vec.y, z - vec.z);
}

//-----------------------------------------------------------------------------
template<class Real>
TVector3<Real>
TPoint3<Real>::operator - (const TPoint3<Real>& point) const
{
    return TVector3<Real> (x - point.x, y - point.y, z - point.z);
}

//-----------------------------------------------------------------------------
template<class Real>
bool        
TPoint3<Real>::operator == (const TPoint3<Real>& point) const
{
    return FuzzyEquals (*this, point, REAL_EPSILON);
}

//-----------------------------------------------------------------------------
template<class Real>
bool        
TPoint3<Real>::operator != (const TPoint3<Real>& point) const
{
    return FuzzyNotEquals (*this, point, REAL_EPSILON);
}

//-----------------------------------------------------------------------------
template<class Real>
bool        
TPoint3<Real>::FuzzyEquals (const TPoint3& lhs, const TPoint3& rhs, Real rEpsilon)
{
    return ::FuzzyEquals<Real> (x, point.x, rEpsilon) and 
           ::FuzzyEquals<Real> (y, point.y, rEpsilon) and 
           ::FuzzyEquals<Real> (z, point.z, rEpsilon);
}

//-----------------------------------------------------------------------------
template<class Real>
bool        
TPoint3<Real>::FuzzyNotEquals (const TPoint3& lhs, const TPoint3& rhs, Real rEpsilon)
{
    return ::FuzzyNotEquals<Real> (x, point.x, rEpsilon) or 
           ::FuzzyNotEquals<Real> (y, point.y, rEpsilon) or 
           ::FuzzyNotEquals<Real> (z, point.z, rEpsilon);
}

//-----------------------------------------------------------------------------
template<class Real>
TPoint3<Real>&
TPoint3<Real>::operator += (const TVector3<Real>& vec)
{
    x += vec.x;
    y += vec.y;
    z += vec.z;
    return *this;
}

//-----------------------------------------------------------------------------
template<class Real>
TPoint3<Real>&
TPoint3<Real>::operator -= (const TVector3<Real>& vec)
{
    x -= vec.x;
    y -= vec.y;
    z -= vec.z;
    return *this;
}

//-----------------------------------------------------------------------------
// global operators
//-----------------------------------------------------------------------------
template<class Real>
TVector3<Real>  operator * (Real scalar, const TVector3<Real>& vec)
{
    return vec * scalar;
}

//-----------------------------------------------------------------------------
template<class Real>
TPoint3<Real>   operator + (const TVector3<Real>& vec, const TPoint3<Real>& point)
{
    return point + vec;
}

//-----------------------------------------------------------------------------

#endif  //  _VECTOR3_INL_
