//-----------------------------------------------------------------------------
// Copyright (C) 2002-2005 Bretton Wade, All rights reserved
//-----------------------------------------------------------------------------

#ifndef     _VECTOR3_H_
#define     _VECTOR3_H_

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
struct TVector3;

template<class Real>
struct TPoint3;

//-----------------------------------------------------------------------------
// class definitions
//-----------------------------------------------------------------------------
template<class Real>
struct TVector3
{
        // the basic data
        Real            x;
        Real            y;
        Real            z;
    
        // static constructors
static  TVector3        Unit (Real x, Real y, Real z);
    
        // constructors
        /* void */      TVector3 (void) {}
        /* void */      TVector3 (const TVector3& vec);
        /* void */      TVector3 (const TPoint3<Real>& point);
        /* void */      TVector3 (Real x, Real y, Real z);
        
        // operators
        TVector3        operator + (const TVector3& vec) const;
        TVector3        operator - (const TVector3& vec) const;
        TVector3        operator * (Real scalar) const;
        TVector3        operator / (Real scalar) const;
        bool            operator == (const TVector3& vec) const;
        bool            operator != (const TVector3& vec) const;
static  bool            FuzzyEquals (const TVector3& lhs, const TVector3& rhs, Real rEpsilon);
static  bool            FuzzyNotEquals (const TVector3& lhs, const TVector3& rhs, Real rEpsilon);
        
        // self operators
        TVector3&       operator += (const TVector3& vec);
        TVector3&       operator -= (const TVector3& vec);
        TVector3&       operator *= (Real scalar);
        TVector3&       operator /= (Real scalar);
        
        // vector operations
        Real            operator DOT (const TVector3& vec) const;
        TVector3        operator CROSS (const TVector3& vec) const;
        Real            Norm (void) const;
        TVector3        Normalized (Real* pfLength = 0) const;
        TVector3&       Normalize (Real* pfLength = 0);

        // tests
        bool            IsUnit (void) const;

        // special constructor for alternate Real types
        template<class alternate>
        /* void */      TVector3 (const TVector3<alternate>& vec) :
            x (static_cast<Real> (vec.x)),
            y (static_cast<Real> (vec.y)),
            z (static_cast<Real> (vec.z))
        {}
};

//-----------------------------------------------------------------------------
template<class Real>
struct TPoint3
{
        // the basic data
        Real            x;
        Real            y;
        Real            z;
        
        // constructors
        /* void */      TPoint3 (void) {}
        /* void */      TPoint3 (const TPoint3& point);
        /* void */      TPoint3 (const TVector3<Real>& vec);
        /* void */      TPoint3 (Real x, Real y, Real z);
        
        // operators
        TPoint3         operator + (const TVector3<Real>& vec) const;
        TPoint3         operator - (const TVector3<Real>& vec) const;
        TVector3<Real>  operator - (const TPoint3& point) const;
        bool            operator == (const TPoint3& vec) const;
        bool            operator != (const TPoint3& vec) const;
static  bool            FuzzyEquals (const TPoint3& lhs, const TPoint3& rhs, Real rEpsilon);
static  bool            FuzzyNotEquals (const TPoint3& lhs, const TPoint3& rhs, Real rEpsilon);

        // self operators
        TPoint3&        operator += (const TVector3<Real>& vec);
        TPoint3&        operator -= (const TVector3<Real>& vec);

        // special constructor for alternate Real types
        template<class alternate>
        /* void */      TPoint3 (const TPoint3<alternate>& point) :
            x (static_cast<Real> (point.x)),
            y (static_cast<Real> (point.y)),
            z (static_cast<Real> (point.z))
        {}
};

//-----------------------------------------------------------------------------
// global operators
//-----------------------------------------------------------------------------
template<class Real>
TVector3<Real>  operator * (Real scalar, const TVector3<Real>& vec);
template<class Real>
TPoint3<Real>   operator + (const TVector3<Real>& vec, const TPoint3<Real>& point);

//-----------------------------------------------------------------------------
// inlines
//-----------------------------------------------------------------------------
#ifndef     _VECTOR3_INL_
#include    "vector3.inl"
#endif  //  _VECTOR3_INL_

//-----------------------------------------------------------------------------

#endif  //  _VECTOR3_H_
