//-----------------------------------------------------------------------------
// Copyright (C) 2002-2005 Bretton Wade, All rights reserved
//-----------------------------------------------------------------------------

#ifndef     _VECTOR2_H_
#define     _VECTOR2_H_

//-----------------------------------------------------------------------------
// includes
//-----------------------------------------------------------------------------
#ifndef     _VECTOR_H_
#include    "vector.h"
#endif      _VECTOR_H_

//-----------------------------------------------------------------------------
// forward definitions
//-----------------------------------------------------------------------------
template<class Real>
struct TVector2;

template<class Real>
struct TPoint2;

//-----------------------------------------------------------------------------
// class definitions
//-----------------------------------------------------------------------------
template<class Real>
struct TVector2
{
        // the basic data
        Real            x;
        Real            y;
    
        // static constructors
static  TVector2        Unit (Real x, Real y);
    
        // constructors
        /* void */      TVector2 (void) {}
        /* void */      TVector2 (const TVector2& vec);
        /* void */      TVector2 (const TPoint2<Real>& point);
        /* void */      TVector2 (Real x, Real y);
        
        // operators
        TVector2        operator + (const TVector2& vec) const;
        TVector2        operator - (const TVector2& vec) const;
        TVector2        operator * (Real scalar) const;
        TVector2        operator / (Real scalar) const;
        bool            operator == (const TVector2& vec) const;
        bool            operator != (const TVector2& vec) const;
static  bool            FuzzyEquals (const TVector2& lhs, const TVector2& rhs, Real rEpsilon);
static  bool            FuzzyNotEquals (const TVector2& lhs, const TVector2& rhs, Real rEpsilon);
        
        // self operators
        TVector2&       operator += (const TVector2& vec);
        TVector2&       operator -= (const TVector2& vec);
        TVector2&       operator *= (Real scalar);
        TVector2&       operator /= (Real scalar);
        
        // vector operations
        Real            operator DOT (const TVector2& vec) const;
        Real            operator CROSS (const TVector2& vec) const;
        Real            Norm (void) const;
        TVector2        Normalized (Real* pfLength = 0) const;
        TVector2&       Normalize (Real* pfLength = 0);
        TVector2        Perpendicular (void) const;
        
        // tests
        bool            IsUnit (void) const;

        // special constructor for alternate Real types
        template<class alternate>
        /* void */      TVector2 (const TVector2<alternate>& vec) :
            x (static_cast<Real> (vec.x)),
            y (static_cast<Real> (vec.y))
        {}
};

//-----------------------------------------------------------------------------
template<class Real>
struct TPoint2
{
        // the basic data
        Real            x;
        Real            y;
        
        // constructors
        /* void */      TPoint2 (void) {}
        /* void */      TPoint2 (const TPoint2& point);
        /* void */      TPoint2 (const TVector2<Real>& vec);
        /* void */      TPoint2 (Real x, Real y);
        
        // operators
        TPoint2         operator + (const TVector2<Real>& vec) const;
        TPoint2         operator - (const TVector2<Real>& vec) const;
        TVector2<Real>  operator - (const TPoint2& point) const;
        bool            operator == (const TPoint2& vec) const;
        bool            operator != (const TPoint2& vec) const;
static  bool            FuzzyEquals (const TPoint2& lhs, const TPoint2& rhs, Real rEpsilon);
static  bool            FuzzyNotEquals (const TPoint2& lhs, const TPoint2& rhs, Real rEpsilon);

        // self operators
        TPoint2&        operator += (const TVector2<Real>& vec);
        TPoint2&        operator -= (const TVector2<Real>& vec);

        // special constructor for alternate Real types
        template<class alternate>
        /* void */      TPoint2 (const TPoint2<alternate>& point) :
            x (static_cast<Real> (point.x)),
            y (static_cast<Real> (point.y))
        {}
};

//-----------------------------------------------------------------------------
// global operators
//-----------------------------------------------------------------------------
template<class Real>
TVector2<Real>  operator * (Real scalar, const TVector2<Real>& vec);
template<class Real>
TPoint2<Real>   operator + (const TVector2<Real>& vec, const TPoint2<Real>& point);

//-----------------------------------------------------------------------------
// inlines
//-----------------------------------------------------------------------------
#ifndef     _VECTOR2_INL_
#include    "vector2.inl"
#endif  //  _VECTOR2_INL_

//-----------------------------------------------------------------------------

#endif  //  _VECTOR2_H_
