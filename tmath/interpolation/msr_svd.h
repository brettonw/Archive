//--------------------------------------------------------------------------
// Copyright (C) 2001 Microsoft Corp.
// All rights reserved.
//--------------------------------------------------------------------------

#ifndef     _MSR_SVD_H_
#define     _MSR_SVD_H_

//--------------------------------------------------------------------------
// includes
//--------------------------------------------------------------------------
#ifndef     _MSR_MATRIX_H_
#include    "msr_matrix.h"
#endif  //  _MSR_MATRIX_H_

//--------------------------------------------------------------------------
// functions
//--------------------------------------------------------------------------

// compute the SVD (Single Value Decomposition) of the matrix A, where A = U W Vt
void    ConstructSVD (const MSR_Matrix& A, MSR_Matrix& U, MSR_Matrix& W, MSR_Matrix& Vt);

// compute X where A X = B
void    ApplySVD (const MSR_Matrix& U, const MSR_Matrix& W, const MSR_Matrix& Vt, const MSR_Matrix& B, MSR_Matrix& X);

//--------------------------------------------------------------------------

#endif  //  _MSR_SVD_H_
