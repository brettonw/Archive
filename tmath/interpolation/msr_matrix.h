//--------------------------------------------------------------------------
// Copyright (C) 2001 Microsoft Corp.
// All rights reserved.
//--------------------------------------------------------------------------

#ifndef     _MSR_MATRIX_H_
#define     _MSR_MATRIX_H_

//--------------------------------------------------------------------------
class MSR_Matrix
{
public:
        /* void */      MSR_Matrix (void);
        /* void */      ~MSR_Matrix (void);

        // set up the matrix
        void            SetDimension (uInt rowCount, uInt columnCount);
        void            SetDimension (uInt rowCount, uInt columnCount, float* pData);

        // accessors
        uInt            GetRowCount (void) const;
        uInt            GetColumnCount (void) const;
        const float*    GetData (void) const;
        float*          GetData (void);

        // matrix accessors
        float&          operator () (uInt row, uInt column);
	    const float&    operator () (uInt row, uInt column) const;

protected:
        uInt            m_rowCount;
        uInt            m_columnCount;
        float*          m_pData;
        bool            m_delete;
};

//--------------------------------------------------------------------------
// inlines
//--------------------------------------------------------------------------
#ifndef     _MSR_MATRIX_INL_
#include    "msr_matrix.inl"
#endif  //  _MSR_MATRIX_INL_

//--------------------------------------------------------------------------

#endif  //  _MSR_MATRIX_H_
