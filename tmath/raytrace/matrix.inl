//-----------------------------------------------------------------------------
// Copyright (C) 2003 Bretton Wade, All rights reserved
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// includes
//-----------------------------------------------------------------------------
#ifndef     _MATRIX_H_
#include    "matrix.h"
#endif      _MATRIX_H_

//-----------------------------------------------------------------------------
#ifndef     _MATRIX_INL_
#define     _MATRIX_INL_

//-----------------------------------------------------------------------------
// includes
//-----------------------------------------------------------------------------
#ifndef     _META_H_
#include    "meta.h"
#endif      _META_H_

//-----------------------------------------------------------------------------
// template member definitions for the basic matrix initializer type
//-----------------------------------------------------------------------------
template<typename Number, unsigned A>
/* void */                      
Initializer<Number, A>::Initializer (Number* pDst, const Number& initializer) :
    m_pDst (pDst)
{
    *m_pDst = initializer;
}

//-----------------------------------------------------------------------------
template<typename Number, unsigned A>
Initializer<Number, A - 1>                      
Initializer<Number, A>::operator , (const Number& initializer)
{
    return Initializer<Number, A - 1> (m_pDst + 1, initializer);
}

//-----------------------------------------------------------------------------
template<typename Number>
/* void */                      
Initializer<Number, 1>::Initializer (Number* pDst, const Number& initializer)
{
    *pDst = initializer;
}

//-----------------------------------------------------------------------------
// template member definitions for the basic matrix data type
//-----------------------------------------------------------------------------
template<typename Number, unsigned A, unsigned B>
/* void */                      
TMatrixType<Number, A, B>::TMatrixType (const Number* pSrc)
{
    Meta0<Number>::Copy<A * B> (pSrc, m_array);
}

//-----------------------------------------------------------------------------
template<typename Number, unsigned A, unsigned B>
/* void */                      
TMatrixType<Number, A, B>::TMatrixType (const TMatrixType<Number, A, B>& matrix)
{
    Meta0<Number>::Copy<A * B> (matrix.m_array, m_array);
}

//-----------------------------------------------------------------------------
template<typename Number, unsigned A, unsigned B>
const Number&                      
TMatrixType<Number, A, B>::operator () (unsigned iRow, unsigned iCol) const
{
    return *(m_array + (iRow * B) + iCol);
}

//-----------------------------------------------------------------------------
template<typename Number, unsigned A, unsigned B>
Number&                      
TMatrixType<Number, A, B>::operator () (unsigned iRow, unsigned iCol)
{
    return *(m_array + (iRow * B) + iCol);
}

//-----------------------------------------------------------------------------
template<typename Number, unsigned A, unsigned B>
const Number*                   
TMatrixType<Number, A, B>::operator [] (unsigned iRow) const
{
    return m_array + (iRow * B);
}

//-----------------------------------------------------------------------------
template<typename Number, unsigned A, unsigned B>
Number*                         
TMatrixType<Number, A, B>::operator [] (unsigned iRow)
{
    return m_array + (iRow * B);
}

//-----------------------------------------------------------------------------
// template function definitions for explicit conversions between number types
//-----------------------------------------------------------------------------
template<typename SourceType, typename DestinationType, unsigned A, unsigned B>
TMatrixType<DestinationType, A, B>& 
Convert (const TMatrixType<SourceType, A, B>& matrix, TMatrixType<DestinationType, A, B>& converted)
{
    ArrayConversionMeta<SourceType, DestinationType>::Copy<A * B> (matrix.m_array, converted. m_array);
    return converted;
}

//-----------------------------------------------------------------------------
// template function definitions for explicit operations on the matrix type
//-----------------------------------------------------------------------------
template<typename Number, unsigned A, unsigned B>
TMatrixType<Number, A, B>&  
Set (TMatrixType<Number, A, B>& set, const Number& value)
{
    Meta0<Number>::Set<A * B> (set.m_array, value);
    return set;
}

//-----------------------------------------------------------------------------
template<typename Number, unsigned A, unsigned B>
TMatrixType<Number, A, B>&  
Zero (TMatrixType<Number, A, B>& zero)
{
    return Set (zero, Number (0));
}

//-----------------------------------------------------------------------------
template<typename Number, unsigned A, unsigned B>
TMatrixType<Number, A, B>&  
Copy (const TMatrixType<Number, A, B>& matrix, TMatrixType<Number, A, B>& copy)
{
    Meta0<Number>::Copy<A * B> (matrix.m_array, copy.m_array);
    return copy;
}

//-----------------------------------------------------------------------------
template<typename Number, unsigned A, unsigned B>
TMatrixType<Number, B, A>&  
Transpose (const TMatrixType<Number, A, B>& matrix, TMatrixType<Number, B, A>& transpose)
{
    Meta2<Number, A, B>::TransposeOuterLoop<A> (matrix.m_array, transpose.m_array);
    return transpose;
}

//-----------------------------------------------------------------------------
template<typename Number, unsigned A, unsigned B>
TMatrixType<Number, A, B>&  
Negative (const TMatrixType<Number, A, B>& matrix, TMatrixType<Number, A, B>& negative)
{
    Meta0<Number>::CopyAndNegate<A * B> (matrix.m_array, negative.m_array);
    return negative;
}

//-----------------------------------------------------------------------------
template<typename Number, unsigned A, unsigned B>
TMatrixType<Number, A, B>&  
ScalarMultiply (const TMatrixType<Number, A, B>& matrix, const Number& scalar, TMatrixType<Number, A, B>& scaled)
{
    Meta0<Number>::Multiply<A * B> (matrix.m_array, scalar, scaled.m_array);
    return scaled;
}

//-----------------------------------------------------------------------------
template<typename Number, unsigned A, unsigned B>
TMatrixType<Number, A, B>&  
ScalarDivide (const TMatrixType<Number, A, B>& matrix, const Number& scalar, TMatrixType<Number, A, B>& scaled)
{
    Meta0<Number>::Divide<A * B> (matrix.m_array, scalar, scaled.m_array);
    return scaled;
}

//-----------------------------------------------------------------------------
template<typename Number, unsigned A, unsigned B>
TMatrixType<Number, A, B>&  
Add (const TMatrixType<Number, A, B>& lhs, const TMatrixType<Number, A, B>& rhs, TMatrixType<Number, A, B>& sum)
{
    Meta0<Number>::Add<A * B> (lhs.m_array, rhs.m_array, sum.m_array);
    return sum;
}

//-----------------------------------------------------------------------------
template<typename Number, unsigned A, unsigned B>
TMatrixType<Number, A, B>&  
Subtract (const TMatrixType<Number, A, B>& lhs, const TMatrixType<Number, A, B>& rhs, TMatrixType<Number, A, B>& dif)
{
    Meta0<Number>::Sub<A * B> (lhs.m_array, rhs.m_array, dif.m_array);
    return dif;
}

//-----------------------------------------------------------------------------
template<typename Number, unsigned A, unsigned B, unsigned C>
TMatrixType<Number, A, C>&  
Multiply (const TMatrixType<Number, A, B>& lhs, const TMatrixType<Number, B, C>& rhs, TMatrixType<Number, A, C>& product)
{
    Meta2<Number, C, B>::MatMultOuterLoop<A> (product.m_array, lhs.m_array, rhs.m_array);
    return product;
}

//-----------------------------------------------------------------------------
template<typename Number, unsigned A>
Number&                     
Multiply (const TMatrixType<Number, 1, A>& lhs, const TMatrixType<Number, A, 1>& rhs, Number& product)
{
    TMatrix<Number, 1, 1>&  matrix = *(reinterpret_cast<TMatrix<Number, 1, 1> > (&product));
    Multiply (lhs, rhs, matrix);    
    return product;
}

//-----------------------------------------------------------------------------
template<typename Number, unsigned A>
Number                      
Multiply (const TMatrixType<Number, 1, A>& lhs, const TMatrixType<Number, A, 1>& rhs)
{
    Number  product;
    Multiply (lhs, rhs, product);
    return product;
}

//-----------------------------------------------------------------------------
template<typename Number, unsigned A, unsigned B>
bool                        
Equal (const TMatrixType<Number, A, B>& lhs, const TMatrixType<Number, A, B>& rhs)
{
    return Meta0<Number>::Equality<A * B> (lhs.m_array, rhs.m_array);
}

//-----------------------------------------------------------------------------
// template function definitions for explicit operations on square matrices
//-----------------------------------------------------------------------------
template<typename Number, unsigned A>
TMatrixType<Number, A, A>&  
Identity (TMatrixType<Number, A, A>& identity)
{
    Zero (identity);
    Meta1<Number, A>::IdentityDiagonal<A> (identity.m_array);
    return identity;
}

//-----------------------------------------------------------------------------
template<typename Number, unsigned A>
Number&  
Determinant (const TMatrixType<Number, A, A>& matrix, Number& determinant)
{
    // Determinant is deliberately left undefined for matrices of size greater 
    // than 4x4 due to the computational complexity of the algorithm used
    // here. This library is intended for graphics use, meaning TMatrixType
    // of dimensions <= 4 are expected. If you need a bigger matrix, this
    // is probably not the right library.
}

//-----------------------------------------------------------------------------
template<typename Number>
Number&  
Determinant (const TMatrixType<Number, 2, 2>& matrix, Number& determinant)
{
    const Number* a = matrix.m_array;
    determinant = (a[0] * a[3]) - (a[1] * a[2]);
    return determinant;
}

//-----------------------------------------------------------------------------
template<typename Number>
Number&  
Determinant (const TMatrixType<Number, 3, 3>& matrix, Number& determinant)
{
    const Number* a = matrix.m_array;
    determinant =   ((a[1] * a[5] * a[6]) + 
                     (a[2] * a[3] * a[7]) + 
                     (a[0] * a[4] * a[8])) - 
                    ((a[2] * a[4] * a[6]) + 
                     (a[1] * a[3] * a[8]) + 
                     (a[0] * a[5] * a[7]));
    return determinant;
}

//-----------------------------------------------------------------------------
template<typename Number>
Number&  
Determinant (const TMatrixType<Number, 4, 4>& matrix, Number& determinant)
{
    const Number* a = matrix.m_array;
    determinant =  ((a[ 1] * a[11] * a[14] * a[ 4]) + (a[10] * a[13] * a[ 3] * a[ 4]) +
                    (a[ 0] * a[10] * a[15] * a[ 5]) + (a[11] * a[12] * a[ 2] * a[ 5]) +
                    (a[ 0] * a[11] * a[13] * a[ 6]) + (a[ 1] * a[10] * a[12] * a[ 7]) +
                    (a[14] * a[ 3] * a[ 5] * a[ 8]) + (a[ 1] * a[15] * a[ 6] * a[ 8]) +
                    (a[13] * a[ 2] * a[ 7] * a[ 8]) + (a[15] * a[ 2] * a[ 4] * a[ 9]) +
                    (a[12] * a[ 3] * a[ 6] * a[ 9]) + (a[ 0] * a[14] * a[ 7] * a[ 9])) - 
                   ((a[ 1] * a[10] * a[15] * a[ 4]) + (a[11] * a[13] * a[ 2] * a[ 4]) +
                    (a[ 0] * a[11] * a[14] * a[ 5]) + (a[10] * a[12] * a[ 3] * a[ 5]) +
                    (a[ 1] * a[11] * a[12] * a[ 6]) + (a[ 0] * a[10] * a[13] * a[ 7]) +
                    (a[15] * a[ 2] * a[ 5] * a[ 8]) + (a[13] * a[ 3] * a[ 6] * a[ 8]) +
                    (a[ 1] * a[14] * a[ 7] * a[ 8]) + (a[14] * a[ 3] * a[ 4] * a[ 9]) +
                    (a[ 0] * a[15] * a[ 6] * a[ 9]) + (a[12] * a[ 2] * a[ 7] * a[ 9]));
    return determinant;
}

//-----------------------------------------------------------------------------
template<typename Number, unsigned A>
Number&  
Cofactor (const TMatrixType<Number, A, A>& matrix, unsigned i, unsigned j, Number& cofactor)
{
    // Cofactor is deliberately left undefined for matrices of size greater 
    // than 4x4 due to the computational complexity of the algorithm used
    // here to compute the sub-matrix determinant. See the comment in 
    // Determinant (above) for an explanation.
}

//-----------------------------------------------------------------------------
template<typename Number>
Number&  
Cofactor (const TMatrixType<Number, 2, 2>& matrix, unsigned i, unsigned j, Number& cofactor)
{
    // compute the minor, the determinant of the 1x1 submatrix formed by eliminating
    // the ith row and jth column, and negate it if i+j is odd.
    unsigned            odd = (i + j) bit_and 0x01;
    Number              minor = matrix[i xor 1][j xor 1];
    cofactor = odd ? -minor : minor;
    return cofactor;
}

//-----------------------------------------------------------------------------
#define     SET_COFACTOR_3_INDEX(i, a0, a1)                                     \
    switch (i)                                                                  \
    {                                                                           \
        case 0: a0 = 1; a1 = 2; break;                                          \
        case 1: a0 = 0; a1 = 2; break;                                          \
        case 2: a0 = 0; a1 = 1; break;                                          \
    }
    
//-----------------------------------------------------------------------------
template<typename Number>
Number&  
Cofactor (const TMatrixType<Number, 3, 3>& matrix, unsigned i, unsigned j, Number& cofactor)
{
    // compute the minor, the determinant of the 2x2 submatrix formed by eliminating
    // the ith row and jth column, and negate it if i+j is odd.
    unsigned            odd = (i + j) bit_and 0x01;
    unsigned            a0, a1, b0, b1;
    SET_COFACTOR_3_INDEX (i, a0, a1);
    SET_COFACTOR_3_INDEX (j, b0, b1);
    Number              minor = (matrix[a0][b0] * matrix[a1][b1]) - (matrix[a0][b1] * matrix[a1][b0]);
    cofactor = odd ? -minor : minor;
    return cofactor;
}

//-----------------------------------------------------------------------------
#define     SET_COFACTOR_4_INDEX(i, a0, a1, a2)                                 \
    switch (i)                                                                  \
    {                                                                           \
        case 0: a0 = 1; a1 = 2; a2 = 3; break;                                  \
        case 1: a0 = 0; a1 = 2; a2 = 3; break;                                  \
        case 2: a0 = 0; a1 = 1; a2 = 3; break;                                  \
        case 3: a0 = 0; a1 = 1; a2 = 2; break;                                  \
    }
    
//-----------------------------------------------------------------------------
template<typename Number>
Number&  
Cofactor (const TMatrixType<Number, 4, 4>& matrix, unsigned i, unsigned j, Number& cofactor)
{
    // compute the minor, the determinant of the 3x3 submatrix formed by eliminating
    // the ith row and jth column, and negate it if i+j is odd.
    unsigned            odd = (i + j) bit_and 0x01;
    unsigned            a0, a1, a2, b0, b1, b2;
    SET_COFACTOR_4_INDEX (i, a0, a1, a2);
    SET_COFACTOR_4_INDEX (j, b0, b1, b2);
    Number              minor = ((matrix[a0][b1] * matrix[a1][b2] * matrix[a2][b0]) + 
                                 (matrix[a0][b2] * matrix[a1][b0] * matrix[a2][b1]) + 
                                 (matrix[a0][b0] * matrix[a1][b1] * matrix[a2][b2])) -
                                ((matrix[a0][b2] * matrix[a1][b1] * matrix[a2][b0]) + 
                                 (matrix[a0][b1] * matrix[a1][b0] * matrix[a2][b2]) + 
                                 (matrix[a0][b0] * matrix[a1][b2] * matrix[a2][b1]));
    cofactor = odd ? -minor : minor;
    return cofactor;
}

//-----------------------------------------------------------------------------
template<typename Number, unsigned A>
TMatrixType<Number, A, A>&  
Inverse (const TMatrixType<Number, A, A>& matrix, Number& determinant, TMatrixType<Number, A, A>& inverse)
{
    // compute the adjoint matrix
    TMatrixType<Number, A, A>   adjoint;
    for (unsigned i = 0; i < A; i++)
        for (unsigned j = 0; j < A; j++)
            Cofactor (matrix, j, i, adjoint[i][j]);
    
    // compute the determinant of the matrix
    Determinant (matrix, determinant);
    
    // compute the inverse of the matrix
    ScalarDivide (adjoint, determinant, inverse);
    
    // return the result
    return inverse;
}

//-----------------------------------------------------------------------------
// template types for objects based on the matrix type
//-----------------------------------------------------------------------------
template<typename Number, unsigned A, unsigned B>
/* void */                      
TMatrix<Number, A, B>::TMatrix (const Number* pSrc) : 
    TMatrixType<Number, A, B> (pSrc)
{
}

//-----------------------------------------------------------------------------
template<typename Number, unsigned A, unsigned B>
/* void */                      
TMatrix<Number, A, B>::TMatrix (const TMatrixType<Number, A, B>& matrix) : 
    TMatrixType<Number, A, B> (matrix)
{
}

//-----------------------------------------------------------------------------
template<typename Number, unsigned A, unsigned B>
Initializer<Number, A * B>
TMatrix<Number, A, B>::operator = (const Number& initializer)
{
    return Initializer<Number, A * B> (m_array, initializer);
}

//-----------------------------------------------------------------------------
template<typename Number, unsigned A, unsigned B>
TMatrix<Number, A, B>&          
TMatrix<Number, A, B>::operator = (const TMatrix<Number, A, B>& matrix)
{
    Copy (matrix, *this);
    return *this;
}

//-----------------------------------------------------------------------------
template<typename Number, unsigned A, unsigned B>
bool                      
TMatrix<Number, A, B>::operator == (const TMatrix<Number, A, B>& matrix) const
{
    return Equal (*this, matrix);
}

//-----------------------------------------------------------------------------
template<typename Number, unsigned A, unsigned B>
bool                      
TMatrix<Number, A, B>::operator != (const TMatrix<Number, A, B>& matrix) const
{
    return not Equal (*this, matrix);
}

//-----------------------------------------------------------------------------
template<typename Number, unsigned A, unsigned B>
TMatrix<Number, A, B>                      
TMatrix<Number, A, B>::operator - (void) const
{
    TMatrix<Number, A, B> negative;
    Negative (*this, negative);
    return negative;
}

//-----------------------------------------------------------------------------
template<typename Number, unsigned A, unsigned B>
TMatrix<Number, A, B>                      
TMatrix<Number, A, B>::operator + (const TMatrix<Number, A, B>& matrix) const
{
    TMatrix<Number, A, B> sum;
    Add (*this, matrix, sum);
    return sum;
}

//-----------------------------------------------------------------------------
template<typename Number, unsigned A, unsigned B>
TMatrix<Number, A, B>                      
TMatrix<Number, A, B>::operator - (const TMatrix<Number, A, B>& matrix) const
{
    TMatrix<Number, A, B> difference;
    Subtract (*this, matrix, difference);
    return difference;
}

//-----------------------------------------------------------------------------
template<typename Number, unsigned A, unsigned B>
TMatrix<Number, A, B>&                      
TMatrix<Number, A, B>::operator += (const TMatrix<Number, A, B>& matrix)
{
    Add (*this, matrix, *this);
    return *this;
}

//-----------------------------------------------------------------------------
template<typename Number, unsigned A, unsigned B>
TMatrix<Number, A, B>&                      
TMatrix<Number, A, B>::operator -= (const TMatrix<Number, A, B>& matrix)
{
    Subtract (*this, matrix, *this);
    return *this;
}

//-----------------------------------------------------------------------------
template<typename Number, unsigned A, unsigned B>
TMatrix<Number, A, B>                      
TMatrix<Number, A, B>::operator * (const Number& scalar) const
{
    TMatrix<Number, A, B>   product;
    ScalarMultiply (*this, scalar, product)
    return product;
}

//-----------------------------------------------------------------------------
template<typename Number, unsigned A, unsigned B>
TMatrix<Number, A, B>                      
TMatrix<Number, A, B>::operator / (const Number& scalar) const
{
    TMatrix<Number, A, B>   result;
    ScalarDivide (*this, scalar, result)
    return result;
}

//-----------------------------------------------------------------------------
template<typename Number, unsigned A, unsigned B>
TMatrix<Number, A, B>&                      
TMatrix<Number, A, B>::operator *= (const Number& scalar)
{
    ScalarMultiply (*this, scalar, *this);
    return *this;
}

//-----------------------------------------------------------------------------
template<typename Number, unsigned A, unsigned B>
TMatrix<Number, A, B>&                      
TMatrix<Number, A, B>::operator /= (const Number& scalar)
{
    ScalarDivide (*this, scalar, *this);
    return *this;
}

//-----------------------------------------------------------------------------
// template function definitions
//-----------------------------------------------------------------------------
template<typename Number, unsigned A, unsigned B>
TMatrix<Number, A, B>
Zero (void)  
{
    TMatrix<Number, A, B>     zero;
    Zero (zero);
    return zero;
}

//-----------------------------------------------------------------------------
template<typename Number, unsigned A, unsigned B>
TMatrixType<Number, B, A>   
Transpose (const TMatrixType<Number, A, B>& matrix)
{
    TMatrixType<Number, B, A>   transpose;
    Transpose (matrix, transpose);
    return transpose;
}

//-----------------------------------------------------------------------------
template<typename Number, unsigned A>
TMatrix<Number, A, A>     
Identity (void)
{
    TMatrix<Number, A, A>     identity;
    Identity (identity);
    return identity;
}

//-----------------------------------------------------------------------------
template<typename Number, unsigned A>
Number
Determinant (const TMatrix<Number, A, A>& matrix)
{
    Number  determinant;
    Determinant (matrix, determinant);
    return determinant;
}

//-----------------------------------------------------------------------------
template<typename Number, unsigned A>
Number
Cofactor (const TMatrix<Number, A, A>& matrix, unsigned i, unsigned j)
{
    Number  cofactor;
    Cofactor (matrix, i, j, cofactor);
    return cofactor;
}

//-----------------------------------------------------------------------------
template<typename Number, unsigned A>
TMatrix<Number, A, A>     
Inverse (const TMatrix<Number, A, A>& matrix, Number& determinant)
{
    TMatrix<Number, A, A>   inverse;
    Inverse (matrix, determinant, inverse);
    return inverse;
}

//-----------------------------------------------------------------------------
template<typename Number, unsigned A>
TMatrix<Number, A, A>     
Inverse (const TMatrix<Number, A, A>& matrix)
{
    TMatrix<Number, A, A>   inverse;
    Number                  determinant;
    Inverse (matrix, determinant, inverse);
    return inverse;
}

//-----------------------------------------------------------------------------
// global operator
//-----------------------------------------------------------------------------
template<typename Number, unsigned A>
Number                  
operator * (const TMatrixType<Number, 1, A>& lhs, const TMatrixType<Number, A, 1>& rhs)
{
    return Multiply (lhs, rhs);
}

//-----------------------------------------------------------------------------
template<typename Number, unsigned A, unsigned B, unsigned C>
TMatrix<Number, A, C>   
operator * (const TMatrix<Number, A, B>& lhs, const TMatrix<Number, B, C>& rhs)
{
    TMatrix<Number, A, C>   product;
    return Multiply (lhs, rhs, product);
    return product;
}

//-----------------------------------------------------------------------------
template<typename Number, unsigned A, unsigned B>
TMatrix<Number, A, B>   operator * (Number scalar, const TMatrix<Number, A, B>& matrix)
{
    return matrix * scalar;
}

//-----------------------------------------------------------------------------
// stream operator for matrix types
//-----------------------------------------------------------------------------
template<typename Number, unsigned A, unsigned B>
std::ostream& 
operator << (std::ostream& stream, const TMatrixType<Number, A, B>& matrix)
{
    const Number* pSrc = matrix.m_array;
    for (unsigned i = 0; i < A; i++)
    {
        Meta0<Number>::Print<B> (stream, pSrc);
        pSrc += B;
    }
    return stream;
}

//-----------------------------------------------------------------------------

#endif  //  _MATRIX_INL_
