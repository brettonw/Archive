//-----------------------------------------------------------------------------
// Copyright (C) 2002 Microsoft, All rights reserved
//-----------------------------------------------------------------------------

#ifndef     _VECTOR_H_
#define     _VECTOR_H_

//-----------------------------------------------------------------------------
// includes
//-----------------------------------------------------------------------------
#ifndef     _MATRIX_H_
#include    "matrix.h"
#endif      _MATRIX_H_

//-----------------------------------------------------------------------------
// macros
//-----------------------------------------------------------------------------
#define     DOT         |
#define     CROSS       ^

//-----------------------------------------------------------------------------
// namespace
//-----------------------------------------------------------------------------
BEGIN_NAMESPACE (TMath)

//-----------------------------------------------------------------------------
// basic vector data type
//-----------------------------------------------------------------------------
template<typename Number, int hDefault, unsigned A>
struct TVectorType : public TMatrix<Number, 1, A>
{
    /* void */                      TVectorType (void) {}
    /* void */                      TVectorType (const Number* pNumber);
    /* void */                      TVectorType (const TMatrix<Number, 1, A>& vectorType);
    
    Initializer<Number, A>          operator = (const Number& initializer);
    TMatrix<Number, 1, A>&          operator = (const TMatrix<Number, 1, A>& vectorType);

    const Number&                   operator () (unsigned a) const;
    Number&                         operator () (unsigned a);
    
    const Number&                   operator [] (unsigned a) const;
    Number&                         operator [] (unsigned a);

    template<unsigned B>
    TMatrixType<Number, 1, B>       operator * (const TMatrixType<Number, A + 1, B>& matrix) const;
    Number                          operator * (const TMatrixType<Number, A + 1, 1>& matrix) const;
};

//-----------------------------------------------------------------------------
// template function declarations for explicit operations on the vector type
//-----------------------------------------------------------------------------
template<typename Number, int hDefault, unsigned A, unsigned B>
TMatrixType<Number, 1, B>&          
MultiplyH (const TVectorType<Number, hDefault, A>& lhs, const TMatrixType<Number, A + 1, B>& rhs, TMatrixType<Number, 1, B>& product);

template<typename Number, int hDefault, unsigned A>
Number&                             
MultiplyH (const TVectorType<Number, hDefault, A>& lhs, const TMatrixType<Number, A + 1, 1>& rhs, Number& product);

template<typename Number, int hDefault, unsigned A>
Number                              
MultiplyH (const TVectorType<Number, hDefault, A>& lhs, const TMatrixType<Number, A + 1, 1>& rhs);

//-----------------------------------------------------------------------------
// forward definitions
//-----------------------------------------------------------------------------
template<typename Number, unsigned A>
struct TVector;

template<typename Number, unsigned A>
struct TPoint;

//-----------------------------------------------------------------------------
// template function declarations for explicit operations on the vector type
//-----------------------------------------------------------------------------
template<typename Number, unsigned A>
Number                  
ZeroNorm (const TVector<Number, A>& vector);

template<typename Number, unsigned A>
Number                  
DotProduct (const TVector<Number, A>& lhs, const TVector<Number, A>& rhs);

template<typename Number, unsigned A>
Number                  
OneNorm (const TVector<Number, A>& vector);

template<typename Number, unsigned A>
Number                  
OneNormSquared (const TVector<Number, A>& vector);

template<typename Number, unsigned A>
Number                  
Length (const TVector<Number, A>& vector);

template<typename Number, unsigned A>
Number                  
LengthSquared (const TVector<Number, A>& vector);

template<typename Number, unsigned A>
TVector<Number, A>&     
Normalize (const TVector<Number, A>& vector, TVector<Number, A>& normalized);

template<typename Number, unsigned A>
TVector<Number, A>&     
Normalize (const TVector<Number, A>& vector, TVector<Number, A>& normalized, Number& norm);

template<typename Number>
Number                  
CrossProduct (const TVector<Number, 2>& lhs, const TVector<Number, 2>& rhs);

template<typename Number>
TVector<Number, 3>&     
CrossProduct (const TVector<Number, 3>& lhs, const TVector<Number, 3>& rhs, TVector<Number, 3>& xproduct);

//-----------------------------------------------------------------------------
// template vector classes based on a row-vector matrix type
//-----------------------------------------------------------------------------
template<typename Number, unsigned A>
struct TVector : public TVectorType<Number, 0, A>
{
    /* void */                  TVector (void) {}
    /* void */                  TVector (const Number* pNumber);
    /* void */                  TVector (const TMatrix<Number, 1, A>& vector);
    
    Initializer<Number, A>      operator = (const Number& initializer);
    TVector<Number, A>&         operator = (const TMatrix<Number, 1, A>& vector);

    TVector<Number, A>          operator - (void) const;

    TVector<Number, A>          operator + (const TVector<Number, A>& vec) const;
    TPoint<Number, A>           operator + (const TPoint<Number, A>& pt) const;
    TVector<Number, A>          operator - (const TVector<Number, A>& vec) const;
    TVector<Number, A>&         operator += (const TVector<Number, A>& vec);
    TVector<Number, A>&         operator -= (const TVector<Number, A>& vec);

    TVector<Number, A>          operator * (const Number& scalar) const;
    TVector<Number, A>          operator / (const Number& scalar) const;
    TVector<Number, A>&         operator *= (const Number& scalar);
    TVector<Number, A>&         operator /= (const Number& scalar);
    
    template<unsigned B>
    TMatrixType<Number, 1, B>   operator * (const TMatrixType<Number, A + 1, B>& matrix) const;
    Number                      operator * (const TMatrixType<Number, A + 1, 1>& matrix) const;
    
    unsigned                    Minor (void) const;
    unsigned                    Major (void) const;

    // measures
    Number                      ZeroNorm (void) const;
    Number                      OneNorm (void) const;
    Number                      OneNormSquared (void) const;
    Number                      Length (void) const;
    Number                      LengthSquared (void) const;
    TVector<Number, A>&         Normalize (void);
    TVector<Number, A>&         Normalize (Number& norm);
    TVector<Number, A>          Normalized (void) const;
    TVector<Number, A>          Normalized (Number& norm) const;
    bool                        IsUnit (void) const;
};

//-----------------------------------------------------------------------------
template<typename Number, unsigned A>
struct TPoint : public TVectorType<Number, 1, A>
{
    /* void */                  TPoint (void) {}
    /* void */                  TPoint (const Number* pNumber);
    /* void */                  TPoint (const TMatrix<Number, 1, A>& point);
    /* void */                  TPoint (const TMatrix<Number, 1, A + 1>& point);
    
    Initializer<Number, A>      operator = (const Number& initializer);
    TPoint<Number, A>&          operator = (const TMatrix<Number, 1, A>& point);
    TPoint<Number, A>&          operator = (const TMatrix<Number, 1, A + 1>& point);

    TPoint<Number, A>           operator + (const TVector<Number, A>& vec) const;
    TPoint<Number, A>           operator - (const TVector<Number, A>& vec) const;
    TVector<Number, A>          operator - (const TPoint<Number, A>& pt) const;
    TPoint<Number, A>&          operator += (const TVector<Number, A>& vec);
    TPoint<Number, A>&          operator -= (const TVector<Number, A>& vec);

    template<unsigned B>
    TMatrixType<Number, 1, B>   operator * (const TMatrixType<Number, A + 1, B>& matrix) const;
    Number                      operator * (const TMatrixType<Number, A + 1, 1>& matrix) const;

    // the following methods are declared private to hide them from the end
    // user - they are not valid operations on points
    private:
    TPoint<Number, A>           operator - (void) const;
    TPoint<Number, A>           operator + (const TPoint<Number, A>& pt) const;
    TPoint<Number, A>           operator * (const Number& scalar) const;
    TPoint<Number, A>           operator / (const Number& scalar) const;
    TPoint<Number, A>&          operator *= (const Number& scalar);
    TPoint<Number, A>&          operator /= (const Number& scalar);
};

//-----------------------------------------------------------------------------
// template function declarations for assignment based operations on vectors
//-----------------------------------------------------------------------------
template<typename Number, unsigned A>
TVector<Number, A>      
Normalized (const TVector<Number, A>& vector);

template<typename Number, unsigned A>
TVector<Number, A>      
Normalized (const TVector<Number, A>& vector, Number& norm);

template<typename Number>
TVector<Number, 3>      
CrossProduct (const TVector<Number, 3>& lhs, const TVector<Number, 3>& rhs);

//-----------------------------------------------------------------------------
// template function declarations for operators on vectors
//-----------------------------------------------------------------------------
template<typename Number, unsigned A>
Number                  
operator DOT (const TVector<Number, A>& lhs, const TVector<Number, A>& rhs);

template<typename Number>
Number                  
operator CROSS (const TVector<Number, 2>& lhs, const TVector<Number, 2>& rhs);

template<typename Number>
TVector<Number, 3>      
operator CROSS (const TVector<Number, 3>& lhs, const TVector<Number, 3>& rhs);

//-----------------------------------------------------------------------------
// global operators
//-----------------------------------------------------------------------------
template<typename Number, unsigned A>
TVector<Number, A>      
operator * (const Number& scalar, const TVector<Number, A>& vector);

//-----------------------------------------------------------------------------
// inlines
//-----------------------------------------------------------------------------
#ifndef     _VECTOR_INL_
#include    "vector.inl"
#endif  //  _VECTOR_INL_

//-----------------------------------------------------------------------------
// namespace
//-----------------------------------------------------------------------------
END_NAMESPACE (TMath)

//-----------------------------------------------------------------------------

#endif  //  _VECTOR_H_
