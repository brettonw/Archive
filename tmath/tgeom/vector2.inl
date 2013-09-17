//-----------------------------------------------------------------------------
// Copyright (C) 2002-2005 Bretton Wade, All rights reserved
//-----------------------------------------------------------------------------

#ifndef     _VECTOR2_INL_
#define     _VECTOR2_INL_

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
TVector2<Real>
TVector2<Real>::Unit (Real x, Real y)
{
    return (TVector2<Real> (x, y)).Normalized ();
}

//-----------------------------------------------------------------------------
template<class Real>
/* void */
TVector2<Real>::TVector2 (const TVector2<Real>& vec) :
    x (vec.x),
    y (vec.y)
{
}

//-----------------------------------------------------------------------------
template<class Real>
/* void */  
TVector2<Real>::TVector2 (const TPoint2<Real>& point) :
    x (point.x),
    y (point.y)
{
}

//-----------------------------------------------------------------------------
template<class Real>
/* void */  
TVector2<Real>::TVector2 (Real _x, Real _y) :
    x (_x),
    y (_y)
{
}

//-----------------------------------------------------------------------------
template<class Real>
TVector2<Real>    
TVector2<Real>::operator + (const TVector2<Real>& vec) const
{
    return TVector2<Real> (x + vec.x, y + vec.y);
}

//-----------------------------------------------------------------------------
template<class Real>
TVector2<Real>    
TVector2<Real>::operator - (const TVector2<Real>& vec) const
{
    return TVector2<Real> (x - vec.x, y - vec.y);
}

//-----------------------------------------------------------------------------
template<class Real>
TVector2<Real>    
TVector2<Real>::operator * (Real scalar) const
{
    return TVector2<Real> (x * scalar, y * scalar);
}

//-----------------------------------------------------------------------------
template<class Real>
TVector2<Real>   
TVector2<Real>::operator / (Real scalar) const
{
    return TVector2<Real> (x / scalar, y / scalar);
}

//-----------------------------------------------------------------------------
template<class Real>
bool        
TVector2<Real>::operator == (const TVector2<Real>& vec) const
{
    return FuzzyEquals (*this, vec, REAL_EPSILON);
}

//-----------------------------------------------------------------------------
template<class Real>
bool        
TVector2<Real>::operator != (const TVector2<Real>& vec) const
{
    return FuzzyNotEquals (*this, vec, REAL_EPSILON);
}

//-----------------------------------------------------------------------------
template<class Real>
bool        
TVector2<Real>::FuzzyEquals (const TVector2& lhs, const TVector2& rhs, Real rEpsilon)
{
    return ::FuzzyEquals<Real> (lhs.x, rhs.x, rEpsilon) and 
           ::FuzzyEquals<Real> (lhs.y, rhs.y, rEpsilon);
}

//-----------------------------------------------------------------------------
template<class Real>
bool        
TVector2<Real>::FuzzyNotEquals (const TVector2& lhs, const TVector2& rhs, Real rEpsilon)
{
    return ::FuzzyNotEquals<Real> (lhs.x, rhs.x, rEpsilon) or 
           ::FuzzyNotEquals<Real> (lhs.y, rhs.y, rEpsilon);
}

//-----------------------------------------------------------------------------
template<class Real>
TVector2<Real>&   
TVector2<Real>::operator += (const TVector2<Real>& vec)
{
    x += vec.x;
    y += vec.y;
    return *this;
}

//-----------------------------------------------------------------------------
template<class Real>
TVector2<Real>&   
TVector2<Real>::operator -= (const TVector2<Real>& vec)
{
    x -= vec.x;
    y -= vec.y;
    return *this;
}

//-----------------------------------------------------------------------------
template<class Real>
TVector2<Real>&   
TVector2<Real>::operator *= (Real scalar)
{
    x *= scalar;
    y *= scalar;
    return *this;
}

//-----------------------------------------------------------------------------
template<class Real>
TVector2<Real>&   
TVector2<Real>::operator /= (Real scalar)
{
    x /= scalar;
    y /= scalar;
    return *this;
}

//-----------------------------------------------------------------------------
template<class Real>
Real        
TVector2<Real>::operator DOT (const TVector2<Real>& vec) const
{
    return (x * vec.x) + (y * vec.y);
}

//-----------------------------------------------------------------------------
template<class Real>
Real        
TVector2<Real>::operator CROSS (const TVector2<Real>& vec) const
{
    return (x * vec.y) - (y * vec.x);
}

//-----------------------------------------------------------------------------
template<class Real>
Real        
TVector2<Real>::Norm (void) const
{
    Real    normSquared = operator DOT (*this);
    if (::FuzzyNotEquals<Real> (normSquared, 0))
        return SquareRoot (normSquared);
    return 0;
}

//-----------------------------------------------------------------------------
template<class Real>
TVector2<Real>
TVector2<Real>::Normalized (Real* pfLength = 0) const
{
    Real    norm = Norm ();
    if (pfLength)
        *pfLength = norm;
    if (::FuzzyNotEquals<Real> (norm, 0))
        return operator / (norm);
    return  TVector2<Real> (0, 0);
}

//-----------------------------------------------------------------------------
template<class Real>
TVector2<Real>&   
TVector2<Real>::Normalize (Real* pfLength)
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
TVector2<Real>
TVector2<Real>::Perpendicular (void) const
{
    return TVector2<Real> (y, -x);
}

//-----------------------------------------------------------------------------
template<class Real>
bool
TVector2<Real>::IsUnit (void) const
{
    return ::FuzzyEquals (Norm (), 1);
}

//-----------------------------------------------------------------------------
template<class Real>
/* void */
TPoint2<Real>::TPoint2 (const TPoint2<Real>& point) :
    x (point.x),
    y (point.y)
{
}

//-----------------------------------------------------------------------------
template<class Real>
/* void */
TPoint2<Real>::TPoint2 (const TVector2<Real>& vec) :
    x (vec.x),
    y (vec.y)
{
}

//-----------------------------------------------------------------------------
template<class Real>
/* void */
TPoint2<Real>::TPoint2 (Real _x, Real _y) :
    x (_x),
    y (_y)
{
}

//-----------------------------------------------------------------------------
template<class Real>
TPoint2<Real>
TPoint2<Real>::operator + (const TVector2<Real>& vec) const
{
    return TPoint2<Real> (x + vec.x, y + vec.y);
}

//-----------------------------------------------------------------------------
template<class Real>
TPoint2<Real>
TPoint2<Real>::operator - (const TVector2<Real>& vec) const
{
    return TPoint2<Real> (x - vec.x, y - vec.y);
}

//-----------------------------------------------------------------------------
template<class Real>
TVector2<Real>
TPoint2<Real>::operator - (const TPoint2<Real>& point) const
{
    return TVector2<Real> (x - point.x, y - point.y);
}

//-----------------------------------------------------------------------------
template<class Real>
bool        
TPoint2<Real>::operator == (const TPoint2<Real>& point) const
{
    return FuzzyEquals (*this, point, REAL_EPSILON);
}

//-----------------------------------------------------------------------------
template<class Real>
bool        
TPoint2<Real>::operator != (const TPoint2<Real>& point) const
{
    return FuzzyNotEquals (*this, point, REAL_EPSILON);
}

//-----------------------------------------------------------------------------
template<class Real>
bool        
TPoint2<Real>::FuzzyEquals (const TPoint2& lhs, const TPoint2& rhs, Real rEpsilon)
{
    return ::FuzzyEquals<Real> (lhs.x, rhs.x, rEpsilon) and 
           ::FuzzyEquals<Real> (lhs.y, rhs.y, rEpsilon);
}

//-----------------------------------------------------------------------------
template<class Real>
bool        
TPoint2<Real>::FuzzyNotEquals (const TPoint2& lhs, const TPoint2& rhs, Real rEpsilon)
{
    return ::FuzzyNotEquals<Real> (lhs.x, rhs.x, rEpsilon) or 
           ::FuzzyNotEquals<Real> (lhs.y, rhs.y, rEpsilon);
}

//-----------------------------------------------------------------------------
template<class Real>
TPoint2<Real>&
TPoint2<Real>::operator += (const TVector2<Real>& vec)
{
    x += vec.x;
    y += vec.y;
    return *this;
}

//-----------------------------------------------------------------------------
template<class Real>
TPoint2<Real>&
TPoint2<Real>::operator -= (const TVector2<Real>& vec)
{
    x -= vec.x;
    y -= vec.y;
    return *this;
}

//-----------------------------------------------------------------------------
// global operators
//-----------------------------------------------------------------------------
template<class Real>
TVector2<Real>  operator * (Real scalar, const TVector2<Real>& vec)
{
    return vec * scalar;
}

//-----------------------------------------------------------------------------
template<class Real>
TPoint2<Real>   operator + (const TVector2<Real>& vec, const TPoint2<Real>& point)
{
    return point + vec;
}

//-----------------------------------------------------------------------------

#endif  //  _VECTOR2_INL_
