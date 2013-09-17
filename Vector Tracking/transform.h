//-----------------------------------------------------------------------------
// Copyright (C) 2002 Microsoft, All rights reserved
//-----------------------------------------------------------------------------

#ifndef     _TRANSFORM_H_
#define     _TRANSFORM_H_

//-----------------------------------------------------------------------------
// includes
//-----------------------------------------------------------------------------
#ifndef     _VECTOR_H_
#include    "vector.h"
#endif      _VECTOR_H_

#ifndef     _REAL_H_
#include    "real.h"
#endif      _REAL_H_

//-----------------------------------------------------------------------------
// namespace
//-----------------------------------------------------------------------------
BEGIN_NAMESPACE (TMath)

//-----------------------------------------------------------------------------
// basic transform data types
//-----------------------------------------------------------------------------
template<typename Number, unsigned A>
struct TTransform : public TMatrix<Number, A + 1, A + 1> {};

template<typename Number>
struct  TTransform2 : public TTransform<Number, 2>
{
        const TVector<Number, 2>&   GetAxisX (void) const;
        const TVector<Number, 2>&   GetAxisY (void) const;
        const TVector<Number, 2>&   GetPosition (void) const;
};

template<typename Number>
struct  TTransform3 : public TTransform<Number, 3>
{
        const TVector<Number, 3>&   GetAxisX (void) const;
        const TVector<Number, 3>&   GetAxisY (void) const;
        const TVector<Number, 3>&   GetAxisZ (void) const;
        const TVector<Number, 3>&   GetPosition (void) const;
};

//-----------------------------------------------------------------------------
// transform functions for 2D
//-----------------------------------------------------------------------------
template<typename Number>
TTransform2<Number>&
Rotate (TTransform2<Number>& transform, const Number& angle);

template<typename Number>
TTransform2<Number>&
Scale (TTransform2<Number>& transform, const Number& scale);

template<typename Number>
TTransform2<Number>&
Scale (TTransform2<Number>& transform, const Number& xScale, const Number& yScale);

template<typename Number>
TTransform2<Number>&
Translate (TTransform2<Number>& transform, const Number& xTranslate, const Number& yTranslate);

//-----------------------------------------------------------------------------
// transform functions for 3D
//-----------------------------------------------------------------------------
template<typename Number>
TTransform3<Number>&
RotateX (TTransform3<Number>& transform, const Number& angle);

template<typename Number>
TTransform3<Number>&
RotateY (TTransform3<Number>& transform, const Number& angle);

template<typename Number>
TTransform3<Number>&
RotateZ (TTransform3<Number>& transform, const Number& angle);

template<typename Number>
TTransform3<Number>&
Scale (TTransform3<Number>& transform, const Number& scale);

template<typename Number>
TTransform3<Number>&
Scale (TTransform3<Number>& transform, const Number& xScale, const Number& yScale, const Number& zScale);

template<typename Number>
TTransform3<Number>&
Translate (TTransform3<Number>& transform, const Number& xTranslate, const Number& yTranslate, const Number& zTranslate);

template<typename Number>
TTransform3<Number>&
Perspective (TTransform3<Number>& transform, const Number& distance, const Number& nearClip, const Number& farClip);

template<typename Number>
TTransform3<Number>&
View (TTransform3<Number>& transform, const TVector<Number, 3>& u, const TVector<Number, 3>& v, const TVector<Number, 3>& vpn, const TPoint<Number, 3>& vrp);

//-----------------------------------------------------------------------------
// typenames for export
//-----------------------------------------------------------------------------
typedef TVector<Real32, 2>      FVec2;
typedef TPoint<Real32, 2>       FPnt2;
typedef TTransform2<Real32>     FMat2;

typedef TVector<Real32, 3>      FVec3;
typedef TPoint<Real32, 3>       FPnt3;
typedef TTransform3<Real32>     FMat3;

typedef TVector<Real64, 2>      DVec2;
typedef TPoint<Real64, 2>       DPnt2;
typedef TTransform2<Real64>     DMat2;

typedef TVector<Real64, 3>      DVec3;
typedef TPoint<Real64, 3>       DPnt3;
typedef TTransform3<Real64>     DMat3;

//-----------------------------------------------------------------------------
// inlines
//-----------------------------------------------------------------------------
#ifndef     _TRANSFORM_INL_
//#include    "transform.inl"
#endif  //  _TRANSFORM_INL_

//-----------------------------------------------------------------------------
// namespace
//-----------------------------------------------------------------------------
END_NAMESPACE (TMath)

//-----------------------------------------------------------------------------

#endif  //  _TRANSFORM_H_
