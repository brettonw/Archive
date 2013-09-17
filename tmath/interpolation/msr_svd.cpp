//--------------------------------------------------------------------------
// Copyright (C) 2001 Microsoft Corp.
// All rights reserved.
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
// includes
//--------------------------------------------------------------------------
#include    "pch.cpp"
#include    "msr_svd.h"
#include    "msr_svd_templates.h"

//--------------------------------------------------------------------------
void
ConstructSVD (const MSR_Matrix& A, MSR_Matrix& U, MSR_Matrix& W, MSR_Matrix& Vt)
{
    uInt    m = A.GetRowCount();
    uInt    n = A.GetColumnCount();
    uInt    min = (m < n) ? m : n;
    assert (min != 0);

    U.SetDimension (m, min);
    W.SetDimension (min, 1);
    Vt.SetDimension (min, n);

    num_svd<float> (A.GetData (), m, n, U.GetData (), W.GetData (), Vt.GetData ());
}

//--------------------------------------------------------------------------
void
ApplySVD (const MSR_Matrix& U, const MSR_Matrix& W, const MSR_Matrix& Vt, const MSR_Matrix& B, MSR_Matrix& X)
{
    // setup the output matrix
    uInt    n = Vt.GetColumnCount ();
    X.SetDimension (n, 1);

    // compute the "zero" value for dealing with singularities
    float   max = 0.0f;
    for (uInt i = 0; i < W.GetRowCount () ; i++)
        max = (max > W (i, 0)) ? max : W (i, 0);
    float   threshold = max * 1.0e-5f;

    // calculate the output matrix
    uInt    m = U.GetRowCount ();
    num_svd_backsubst<float> (m, n, U.GetData (), W.GetData (), Vt.GetData (), B.GetData (), X.GetData (), threshold);

}

//--------------------------------------------------------------------------
