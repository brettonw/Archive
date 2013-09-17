//-----------------------------------------------------------------------------
// Copyright (C) 2003 Bretton Wade, All rights reserved
//-----------------------------------------------------------------------------

#ifndef     _MATRIX_H_
#define     _MATRIX_H_

//-----------------------------------------------------------------------------
// namespace
//-----------------------------------------------------------------------------
BEGIN_NAMESPACE (TMath)

//-----------------------------------------------------------------------------
// basic matrix initializer type
//-----------------------------------------------------------------------------
template<typename Number, unsigned A>
struct Initializer
{
    /* void */                      Initializer (Number* pDst, const Number& initializer);
    Initializer<Number, A - 1>      operator , (const Number& initializer);
    
    private:
    Number*                         m_pDst;
};

//-----------------------------------------------------------------------------
template<typename Number>
struct Initializer<Number, 1>
{
    /* void */                      Initializer (Number* pDst, const Number& initializer);
    
    // by declaring the comma operator as private in this specialization, we
    // cause a compiler error if there are too many initializers on the line.
    // the error is ambiguous, but hopefully it got you here looking at this
    // comment to see why you got that error. now you know.
    private:
    Initializer<Number, 0>          operator , (const Number& initializer);
};

//-----------------------------------------------------------------------------
// basic matrix data type
//-----------------------------------------------------------------------------
template<typename Number, unsigned A, unsigned B>
struct TMatrixType
{
    Number                          m_array[A * B];

    /* void */                      TMatrixType (void) {}
    /* void */                      TMatrixType (const Number* pNumber);
    /* void */                      TMatrixType (const TMatrixType<Number, A, B>& matrix);
    
    const Number&                   operator () (unsigned a, unsigned b) const;
    Number&                         operator () (unsigned a, unsigned b);
    
    const Number*                   operator [] (unsigned a) const;
    Number*                         operator [] (unsigned a);
};

//-----------------------------------------------------------------------------
// template function declaration for explicit conversions between number types
//-----------------------------------------------------------------------------
template<typename SourceType, typename DestinationType, unsigned A, unsigned B>
TMatrixType<DestinationType, A, B>& 
Convert (const TMatrixType<SourceType, A, B>& matrix, TMatrixType<DestinationType, A, B>& converted);

//-----------------------------------------------------------------------------
// template function declarations for explicit operations on the matrix type
//-----------------------------------------------------------------------------
template<typename Number, unsigned A, unsigned B>
TMatrixType<Number, A, B>&  
Set (TMatrixType<Number, A, B>& set, const Number& value);

template<typename Number, unsigned A, unsigned B>
TMatrixType<Number, A, B>&  
Zero (TMatrixType<Number, A, B>& zero);

template<typename Number, unsigned A, unsigned B>
TMatrixType<Number, A, B>&  
Copy (const TMatrixType<Number, A, B>& matrix, TMatrixType<Number, A, B>& copy);

template<typename Number, unsigned A, unsigned B>
TMatrixType<Number, B, A>&  
Transpose (const TMatrixType<Number, A, B>& matrix, TMatrixType<Number, B, A>& transpose);

template<typename Number, unsigned A, unsigned B>
TMatrixType<Number, A, B>&  
Negative (const TMatrixType<Number, A, B>& matrix, TMatrixType<Number, A, B>& negative);

template<typename Number, unsigned A, unsigned B>
TMatrixType<Number, A, B>&  
ScalarMultiply (const TMatrixType<Number, A, B>& matrix, const Number& scalar, TMatrixType<Number, A, B>& scaled);

template<typename Number, unsigned A, unsigned B>
TMatrixType<Number, A, B>&  
ScalarDivide (const TMatrixType<Number, A, B>& matrix, const Number& scalar, TMatrixType<Number, A, B>& scaled);

template<typename Number, unsigned A, unsigned B>
TMatrixType<Number, A, B>&  
Add (const TMatrixType<Number, A, B>& lhs, const TMatrixType<Number, A, B>& rhs, TMatrixType<Number, A, B>& sum);

template<typename Number, unsigned A, unsigned B>
TMatrixType<Number, A, B>&  
Subtract (const TMatrixType<Number, A, B>& lhs, const TMatrixType<Number, A, B>& rhs, TMatrixType<Number, A, B>& dif);

template<typename Number, unsigned A, unsigned B, unsigned C>
TMatrixType<Number, A, C>&  
Multiply (const TMatrixType<Number, A, B>& lhs, const TMatrixType<Number, B, C>& rhs, TMatrixType<Number, A, C>& product);

template<typename Number, unsigned A>
Number&                     
Multiply (const TMatrixType<Number, 1, A>& lhs, const TMatrixType<Number, A, 1>& rhs, Number& product);

template<typename Number, unsigned A>
Number                      
Multiply (const TMatrixType<Number, 1, A>& lhs, const TMatrixType<Number, A, 1>& rhs);

template<typename Number, unsigned A, unsigned B>
bool                        
Equal (const TMatrixType<Number, A, B>& lhs, const TMatrixType<Number, A, B>& rhs);

//-----------------------------------------------------------------------------
// template function declarations for explicit operations on square matrices
//-----------------------------------------------------------------------------
template<typename Number, unsigned A>
TMatrixType<Number, A, A>&  
Identity (TMatrixType<Number, A, A>& identity);

template<typename Number, unsigned A>
Number&                     
Determinant (const TMatrixType<Number, A, A>& matrix, Number& determinant);

template<typename Number, unsigned A>
Number&                     
Cofactor (const TMatrixType<Number, A, A>& matrix, unsigned i, unsigned j, Number& cofactor);

template<typename Number, unsigned A>
TMatrixType<Number, A, A>&  
Inverse (const TMatrixType<Number, A, A>& matrix, Number& determinant, TMatrixType<Number, A, A>& inverse);

//-----------------------------------------------------------------------------
// template types for objects based on the matrix type
//-----------------------------------------------------------------------------
template<typename Number, unsigned A, unsigned B>
struct TMatrix : public TMatrixType<Number, A, B>
{
    /* void */                      TMatrix (void) {}
    /* void */                      TMatrix (const Number* pSrc);
    /* void */                      TMatrix (const TMatrixType<Number, A, B>& matrix);

    Initializer<Number, A * B>      operator = (const Number& initializer);
    TMatrix<Number, A, B>&          operator = (const TMatrix<Number, A, B>& matrix);

    bool                            operator == (const TMatrix<Number, A, B>& matrix) const;
    bool                            operator != (const TMatrix<Number, A, B>& matrix) const;
    
    TMatrix<Number, A, B>           operator - (void) const;

    TMatrix<Number, A, B>           operator + (const TMatrix<Number, A, B>& matrix) const;
    TMatrix<Number, A, B>           operator - (const TMatrix<Number, A, B>& matrix) const;
    TMatrix<Number, A, B>&          operator += (const TMatrix<Number, A, B>& matrix);
    TMatrix<Number, A, B>&          operator -= (const TMatrix<Number, A, B>& matrix);

    TMatrix<Number, A, B>           operator * (const Number& scalar) const;
    TMatrix<Number, A, B>           operator / (const Number& scalar) const;
    TMatrix<Number, A, B>&          operator *= (const Number& scalar);
    TMatrix<Number, A, B>&          operator /= (const Number& scalar);
};

//-----------------------------------------------------------------------------
// template function declarations for assignment based operations on matrices
//-----------------------------------------------------------------------------
template<typename Number, unsigned A, unsigned B>
TMatrix<Number, A, B>       
Zero (void);

template<typename Number, unsigned A, unsigned B>
TMatrixType<Number, B, A>   
Transpose (const TMatrixType<Number, A, B>& matrix);

template<typename Number, unsigned A>
TMatrix<Number, A, A>       
Identity (void);

template<typename Number, unsigned A>
Number                      
Determinant (const TMatrix<Number, A, A>& matrix);

template<typename Number, unsigned A>
Number                      
Cofactor (const TMatrix<Number, A, A>& matrix, unsigned i, unsigned j);

template<typename Number, unsigned A>
TMatrix<Number, A, A>       
Inverse (const TMatrix<Number, A, A>& matrix, Number& determinant);

template<typename Number, unsigned A>
TMatrix<Number, A, A>       
Inverse (const TMatrix<Number, A, A>& matrix);

//-----------------------------------------------------------------------------
// global operators
//-----------------------------------------------------------------------------
template<typename Number, unsigned A>
Number                      
operator * (const TMatrixType<Number, 1, A>& lhs, const TMatrixType<Number, A, 1>& rhs);
    
template<typename Number, unsigned A, unsigned B, unsigned C>
TMatrix<Number, A, C>       
operator * (const TMatrix<Number, A, B>& lhs, const TMatrix<Number, B, C>& rhs);
    
template<typename Number, unsigned A, unsigned B>
TMatrix<Number, A, B>       
operator * (Number scalar, const TMatrix<Number, A, B>& matrix);

//-----------------------------------------------------------------------------
// stream operator for matrix types
//-----------------------------------------------------------------------------
template<typename Number, unsigned A, unsigned B>
std::ostream& 
operator << (std::ostream& stream, const TMatrixType<Number, A, B>& matrix);

//-----------------------------------------------------------------------------
// inlines
//-----------------------------------------------------------------------------
#ifndef     _MATRIX_INL_
#include    "matrix.inl"
#endif  //  _MATRIX_INL_

//-----------------------------------------------------------------------------
// namespace
//-----------------------------------------------------------------------------
END_NAMESPACE (TMath)

//-----------------------------------------------------------------------------

#endif  //  _MATRIX_H_
