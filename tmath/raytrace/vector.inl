//-----------------------------------------------------------------------------
// Copyright (C) 2002 Microsoft, All rights reserved
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// includes
//-----------------------------------------------------------------------------
#ifndef     _VECTOR_H_
#include    "vector.h"
#endif      _VECTOR_H_

//-----------------------------------------------------------------------------
#ifndef     _VECTOR_INL_
#define     _VECTOR_INL_

//-----------------------------------------------------------------------------
// template member definitions for the basic vector data type
//-----------------------------------------------------------------------------
template<typename Number, int hDefault, unsigned A>
/* void */
TVectorType<Number, hDefault, A>::TVectorType (const Number* pNumber) :
    TMatrix<Number, 1, A> (pNumber)
{
}
//-----------------------------------------------------------------------------
template<typename Number, int hDefault, unsigned A>
/* void */
TVectorType<Number, hDefault, A>::TVectorType (const TMatrix<Number, 1, A>& vectorType) :
    TMatrix<Number, 1, A> (vectorType)
{
}
//-----------------------------------------------------------------------------
template<typename Number, int hDefault, unsigned A>
Initializer<Number, A>
TVectorType<Number, hDefault, A>::operator = (const Number& initializer)
{
    return TMatrix<Number, 1, A>::operator = (initializer);
}
//-----------------------------------------------------------------------------
template<typename Number, int hDefault, unsigned A>
TMatrix<Number, 1, A>&
TVectorType<Number, hDefault, A>::operator = (const TMatrix<Number, 1, A>& vectorType)
{
    TMatrix<Number, 1, A>::operator = (vectorType);
    return *this;
}
//-----------------------------------------------------------------------------
template<typename Number, int hDefault, unsigned A>
const Number&
TVectorType<Number, hDefault, A>::operator () (unsigned a) const
{
    return m_array[a];
}
//-----------------------------------------------------------------------------
template<typename Number, int hDefault, unsigned A>
Number&
TVectorType<Number, hDefault, A>::operator () (unsigned a)
{
    return m_array[a];
}
//-----------------------------------------------------------------------------
template<typename Number, int hDefault, unsigned A>
const Number&
TVectorType<Number, hDefault, A>::operator [] (unsigned a) const
{
    return m_array[a];
}
//-----------------------------------------------------------------------------
template<typename Number, int hDefault, unsigned A>
Number&
TVectorType<Number, hDefault, A>::operator [] (unsigned a)
{
    return m_array[a];
}
    
//-----------------------------------------------------------------------------
template<typename Number, int hDefault, unsigned A>
template<unsigned B>
TMatrixType<Number, 1, B>
TVectorType<Number, hDefault, A>::operator * (const TMatrixType<Number, A + 1, B>& matrix) const
{
    TMatrixType<Number, 1, B>   product;
    MultiplyH<Number, hDefault, A, B> (*this, matrix, product);
    return product;
}

//-----------------------------------------------------------------------------
template<typename Number, int hDefault, unsigned A>
Number
TVectorType<Number, hDefault, A>::operator * (const TMatrixType<Number, A + 1, 1>& matrix) const
{
    return MultiplyH<Number, hDefault, A> (*this, matrix);
}

//-----------------------------------------------------------------------------
// template function definitions for explicit operations on the vector type
//-----------------------------------------------------------------------------
template<typename Number, int hDefault, unsigned A, unsigned B>
TMatrixType<Number, 1, B>&          
MultiplyH (const TVectorType<Number, hDefault, A>& lhs, const TMatrixType<Number, A + 1, B>& rhs, TMatrixType<Number, 1, B>& product)
{
    Meta2_Homo<Number, hDefault, B, B>::RowOfInnerProducts<A + 1> (product.m_array, lhs.m_array, rhs.m_array);
    return product;
}

//-----------------------------------------------------------------------------
template<typename Number, int hDefault, unsigned A>
Number&                             
MultiplyH (const TVectorType<Number, hDefault, A>& lhs, const TMatrixType<Number, A + 1, 1>& rhs, Number& product)
{
    TMatrixType<Number, 1, 1>   result;
    Multiply (lhs, rhs, result);
    product = result.m_array[0];
    return product;
}

//-----------------------------------------------------------------------------
template<typename Number, int hDefault, unsigned A>
Number                              
MultiplyH (const TVectorType<Number, hDefault, A>& lhs, const TMatrixType<Number, A + 1, 1>& rhs)
{
    Number  product;
    Multiply (lhs, rhs, product);
    return product;
}

//-----------------------------------------------------------------------------
// template function definitions for explicit operations on the vector type
//-----------------------------------------------------------------------------
template<typename Number, unsigned A>
Number                  
ZeroNorm (const TVector<Number, A>& vector)
{
    return Meta0<Number>::Sum<A> (vector.m_array);
}

//-----------------------------------------------------------------------------
template<typename Number, unsigned A>
Number                  
DotProduct (const TVector<Number, A>& lhs, const TVector<Number, A>& rhs)
{
    return Meta0<Number>::MultiplyAndAdd<A> (lhs.m_array, rhs.m_array);
}

//-----------------------------------------------------------------------------
template<typename Number, unsigned A>
Number                  
OneNorm (const TVector<Number, A>& vector)
{
    return SquareRoot (OneNormSquared (vector));
}

//-----------------------------------------------------------------------------
template<typename Number, unsigned A>
Number                  
OneNormSquared (const TVector<Number, A>& vector)
{
    return Meta0<Number>::SquareAndAdd<A> (vector.m_array);
}

//-----------------------------------------------------------------------------
template<typename Number, unsigned A>
Number                  
Length (const TVector<Number, A>& vector)
{
    return OneNorm (vector);
}

//-----------------------------------------------------------------------------
template<typename Number, unsigned A>
Number                  
LengthSquared (const TVector<Number, A>& vector)
{
    return OneNormSquared (vector);
}

//-----------------------------------------------------------------------------
template<typename Number, unsigned A>
TVector<Number, A>&     
Normalize (const TVector<Number, A>& vector, TVector<Number, A>& normalized)
{
    Number  unused;
    return Normalize (vector, normalized, unused);
}

//-----------------------------------------------------------------------------
template<typename Number, unsigned A>
TVector<Number, A>&     
Normalize (const TVector<Number, A>& vector, TVector<Number, A>& normalized, Number& norm)
{
    norm = OneNorm (vector);
    if (norm != Number (0))
        normalized = vector / norm;
    // what do we want to do if norm *IS* close to 0?
    return normalized;
}

//-----------------------------------------------------------------------------
template<typename Number>
Number                  
CrossProduct (const TVector<Number, 2>& lhs, const TVector<Number, 2>& rhs)
{
    return (lhs.m_array[0] * rhs.m_array[1]) - (lhs.m_array[1] * rhs.m_array[0]);
}

//-----------------------------------------------------------------------------
template<typename Number>
TVector<Number, 3>&     
CrossProduct (const TVector<Number, 3>& lhs, const TVector<Number, 3>& rhs, TVector<Number, 3>& xproduct)
{
    xproduct.m_array[0] = (lhs.m_array[1] * rhs.m_array[2]) - (lhs.m_array[2] * rhs.m_array[1]);
    xproduct.m_array[1] = (lhs.m_array[2] * rhs.m_array[0]) - (lhs.m_array[0] * rhs.m_array[2]);
    xproduct.m_array[2] = (lhs.m_array[0] * rhs.m_array[1]) - (lhs.m_array[1] * rhs.m_array[0]);
    return xproduct;
}

//-----------------------------------------------------------------------------
// template vector classes based on a row-vector matrix type
//-----------------------------------------------------------------------------
template<typename Number, unsigned A>
/* void */
TVector<Number, A>::TVector (const Number* pNumber) :
    TVectorType<Number, 0, A> (pNumber)
{
}

//-----------------------------------------------------------------------------
template<typename Number, unsigned A>
/* void */
TVector<Number, A>::TVector (const TMatrix<Number, 1, A>& vector) :
    TVectorType<Number, 0, A> (vector)
{
}

//-----------------------------------------------------------------------------
template<typename Number, unsigned A>
Initializer<Number, A>
TVector<Number, A>::operator = (const Number& initializer)
{
    return TVectorType<Number, 0, A>::operator = (initializer);
}

//-----------------------------------------------------------------------------
template<typename Number, unsigned A>
TVector<Number, A>&
TVector<Number, A>::operator = (const TMatrix<Number, 1, A>& vector)
{
    TVectorType<Number, 0, A>::operator = (vector);
    return *this;
}

//-----------------------------------------------------------------------------
template<typename Number, unsigned A>
TVector<Number, A>
TVector<Number, A>::operator - (void) const
{
    return TVector<Number, A> (TMatrix<Number, 1, A>::operator - ());
}

//-----------------------------------------------------------------------------
template<typename Number, unsigned A>
TVector<Number, A>
TVector<Number, A>::operator + (const TVector<Number, A>& vec) const
{
    return TVector<Number, A> (TMatrix<Number, 1, A>::operator + (vec));
}

//-----------------------------------------------------------------------------
template<typename Number, unsigned A>
TPoint<Number, A>
TVector<Number, A>::operator + (const TPoint<Number, A>& pt) const
{
    return TPoint<Number, A> (TMatrix<Number, 1, A>::operator + (pt));
}

//-----------------------------------------------------------------------------
template<typename Number, unsigned A>
TVector<Number, A>
TVector<Number, A>::operator - (const TVector<Number, A>& vec) const
{
    return TVector<Number, A> (TMatrix<Number, 1, A>::operator - (vec));
}

//-----------------------------------------------------------------------------
template<typename Number, unsigned A>
TVector<Number, A>&
TVector<Number, A>::operator += (const TVector<Number, A>& vec)
{
    TMatrix<Number, 1, A>::operator += (vec);
    return *this;
}

//-----------------------------------------------------------------------------
template<typename Number, unsigned A>
TVector<Number, A>&
TVector<Number, A>::operator -= (const TVector<Number, A>& vec)
{
    TMatrix<Number, 1, A>::operator -= (vec);
    return *this;
}

//-----------------------------------------------------------------------------
template<typename Number, unsigned A>
TVector<Number, A>
TVector<Number, A>::operator * (const Number& scalar) const
{
    return TVector<Number, A> (TMatrix<Number, 1, A>::operator * (scalar));
}

//-----------------------------------------------------------------------------
template<typename Number, unsigned A>
TVector<Number, A>
TVector<Number, A>::operator / (const Number& scalar) const
{
    return TVector<Number, A> (TMatrix<Number, 1, A>::operator / (scalar));
}

//-----------------------------------------------------------------------------
template<typename Number, unsigned A>
TVector<Number, A>&
TVector<Number, A>::operator *= (const Number& scalar)
{
    TMatrix<Number, 1, A>::operator *= (scalar);
    return *this;
}

//-----------------------------------------------------------------------------
template<typename Number, unsigned A>
TVector<Number, A>&
TVector<Number, A>::operator /= (const Number& scalar)
{
    TMatrix<Number, 1, A>::operator /= (scalar);
    return *this;
}

//-----------------------------------------------------------------------------
template<typename Number, unsigned A>
template<unsigned B>
TMatrixType<Number, 1, B>
TVector<Number, A>::operator * (const TMatrixType<Number, A + 1, B>& matrix) const 
{
    return TVectorType<Number, 0, A>::operator * (matrix);
}

//-----------------------------------------------------------------------------
template<typename Number, unsigned A>
Number
TVector<Number, A>::operator * (const TMatrixType<Number, A + 1, 1>& matrix) const 
{
    return TVectorType<Number, 0, A>::operator * (matrix);
}

//-----------------------------------------------------------------------------
template<typename Number, unsigned A>
Number
TVector<Number, A>::ZeroNorm (void) const
{
    return ZeroNorm (*this);
}

//-----------------------------------------------------------------------------
template<typename Number, unsigned A>
Number
TVector<Number, A>::OneNorm (void) const
{
    return OneNorm (*this);
}

//-----------------------------------------------------------------------------
template<typename Number, unsigned A>
Number
TVector<Number, A>::OneNormSquared (void) const
{
    return OneNormSquared (*this);
}

//-----------------------------------------------------------------------------
template<typename Number, unsigned A>
Number
TVector<Number, A>::Length (void) const
{
    return OneNorm (*this);
}

//-----------------------------------------------------------------------------
template<typename Number, unsigned A>
Number
TVector<Number, A>::LengthSquared (void) const
{
    return OneNormSquared (*this);
}

//-----------------------------------------------------------------------------
template<typename Number, unsigned A>
TVector<Number, A>&
TVector<Number, A>::Normalize (void)
{
    return Normalize (*this, *this);
}

//-----------------------------------------------------------------------------
template<typename Number, unsigned A>
TVector<Number, A>&
TVector<Number, A>::Normalize (Number& norm)
{
    return Normalize (*this, *this, norm);
}

//-----------------------------------------------------------------------------
template<typename Number, unsigned A>
TVector<Number, A>
TVector<Number, A>::Normalized (void) const
{
    return Normalized (*this);
}

//-----------------------------------------------------------------------------
template<typename Number, unsigned A>
TVector<Number, A>
TVector<Number, A>::Normalized (Number& norm) const
{
    return Normalized (*this, norm);
}

//-----------------------------------------------------------------------------
template<typename Number, unsigned A>
bool
TVector<Number, A>::IsUnit (void) const
{
    return (OneNorm (*this) == Number (1));
}

//-----------------------------------------------------------------------------
template<typename Number, unsigned A>
/* void */
TPoint<Number, A>::TPoint (const Number* pNumber) :
    TVectorType<Number, 1, A> (pNumber)
{
}

//-----------------------------------------------------------------------------
template<typename Number, unsigned A>
/* void */
TPoint<Number, A>::TPoint (const TMatrix<Number, 1, A>& point) :
    TVectorType<Number, 1, A> (point)
{
}

//-----------------------------------------------------------------------------
template<typename Number, unsigned A>
/* void */
TPoint<Number, A>::TPoint (const TMatrix<Number, 1, A + 1>& point) :
    TVectorType<Number, 1, A> (point.m_array)
{
    if (point.m_array[A] != Number (0))
        ScalarDivide (*this, point.m_array[A], *this);
}

//-----------------------------------------------------------------------------
template<typename Number, unsigned A>
Initializer<Number, A>
TPoint<Number, A>::operator = (const Number& initializer)
{
    return TVectorType<Number, 1, A>::operator = (initializer);
}

//-----------------------------------------------------------------------------
template<typename Number, unsigned A>
TPoint<Number, A>&
TPoint<Number, A>::operator = (const TMatrix<Number, 1, A>& point)
{
    TVectorType<Number, 1, A>::operator = (point);
    return *this;
}

//-----------------------------------------------------------------------------
template<typename Number, unsigned A>
TPoint<Number, A>&
TPoint<Number, A>::operator = (const TMatrix<Number, 1, A + 1>& point)
{
    TVectorType<Number, 1, A>::operator = (*(const TMatrix<Number, 1, A>*) (&point));
    if (point.m_array[A] != Number (0))
        ScalarDivide (*this, point.m_array[A], *this);
    return *this;
}

//-----------------------------------------------------------------------------
template<typename Number, unsigned A>
TPoint<Number, A>
TPoint<Number, A>::operator + (const TVector<Number, A>& vec) const
{
    return TPoint (TMatrix<Number, 1, A>::operator + (vec));
}

//-----------------------------------------------------------------------------
template<typename Number, unsigned A>
TPoint<Number, A>
TPoint<Number, A>::operator - (const TVector<Number, A>& vec) const
{
    return TPoint (TMatrix<Number, 1, A>::operator - (vec));
}

//-----------------------------------------------------------------------------
template<typename Number, unsigned A>
TVector<Number, A>
TPoint<Number, A>::operator - (const TPoint<Number, A>& pt) const
{
    return TVector (TMatrix<Number, 1, A>::operator - (pt));
}

//-----------------------------------------------------------------------------
template<typename Number, unsigned A>
TPoint<Number, A>&
TPoint<Number, A>::operator += (const TVector<Number, A>& vec)
{
    TMatrix<Number, 1, A>::operator += (vec);
    return *this;
}

//-----------------------------------------------------------------------------
template<typename Number, unsigned A>
TPoint<Number, A>&
TPoint<Number, A>::operator -= (const TVector<Number, A>& vec)
{
    TMatrix<Number, 1, A>::operator -= (vec);
    return *this;
}

//-----------------------------------------------------------------------------
template<typename Number, unsigned A>
template<unsigned B>
TMatrixType<Number, 1, B>
TPoint<Number, A>::operator * (const TMatrixType<Number, A + 1, B>& matrix) const 
{
    return TVectorType<Number, 1, A>::operator * (matrix);
}

//-----------------------------------------------------------------------------
template<typename Number, unsigned A>
Number
TPoint<Number, A>::operator * (const TMatrixType<Number, A + 1, 1>& matrix) const 
{
    return TVectorType<Number, 1, A>::operator * (matrix);
}

//-----------------------------------------------------------------------------
// template function definitions for assignment based operations on vectors
//-----------------------------------------------------------------------------
template<typename Number, unsigned A>
TVector<Number, A>      
Normalized (const TVector<Number, A>& vector)
{
    // this could cause a problem for integral types
    Number  unused;
    return Normalized (vector, unused);
}

//-----------------------------------------------------------------------------
template<typename Number, unsigned A>
TVector<Number, A>      
Normalized (const TVector<Number, A>& vector, Number& norm)
{
    TVector<Number, A>  normalized;
    Normalize (vector, normalized, norm);
    return normalized;
}

//-----------------------------------------------------------------------------
template<typename Number>
TVector<Number, 3>      
CrossProduct (const TVector<Number, 3>& lhs, const TVector<Number, 3>& rhs)
{
    TVector<Number, 3>  xproduct;
    CrossProduct (lhs, rhs, xproduct);
    return xproduct;
}

//-----------------------------------------------------------------------------
// template function definitions for operators on vectors
//-----------------------------------------------------------------------------
template<typename Number, unsigned A>
Number                  
operator DOT (const TVector<Number, A>& lhs, const TVector<Number, A>& rhs)
{
    return DotProduct (lhs, rhs);
}

//-----------------------------------------------------------------------------
template<typename Number>
Number                  
operator CROSS (const TVector<Number, 2>& lhs, const TVector<Number, 2>& rhs)
{
    return CrossProduct (lhs, rhs);
}

//-----------------------------------------------------------------------------
template<typename Number>
TVector<Number, 3>      
operator CROSS (const TVector<Number, 3>& lhs, const TVector<Number, 3>& rhs)
{
    return CrossProduct (lhs, rhs);
}

//-----------------------------------------------------------------------------
// global operators
//-----------------------------------------------------------------------------
template<typename Number, unsigned A>
TVector<Number, A>      
operator * (const Number& scalar, const TVector<Number, A>& vector)
{
    return vector * scalar;
}

//-----------------------------------------------------------------------------

#endif  //  _VECTOR_INL_
