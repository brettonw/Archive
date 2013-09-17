//--------------------------------------------------------------------------
// Copyright (C) 2001 Microsoft Corp.
// All rights reserved.
//--------------------------------------------------------------------------

#ifndef     _MSR_MATRIX_INL_
#define     _MSR_MATRIX_INL_

//--------------------------------------------------------------------------
// includes
//--------------------------------------------------------------------------
#ifndef     _MSR_MATRIX_H_
#include    "msr_matrix.h"
#endif  //  _MSR_MATRIX_H_

//--------------------------------------------------------------------------
inline
/* void */
MSR_Matrix::MSR_Matrix (void) :
    m_rowCount (0),
    m_columnCount (0),
    m_pData (0),
    m_delete (false)
{
}

//--------------------------------------------------------------------------
inline
/* void */
MSR_Matrix::~MSR_Matrix (void)
{
    // check to see if there is a pointer to delete
    if (m_delete)
        delete[] m_pData;
}

//--------------------------------------------------------------------------
inline
void
MSR_Matrix::SetDimension (uInt rowCount, uInt columnCount)
{
    assert ((rowCount > 0) and (columnCount > 0));

    // check to see if there is already a pointer
    if (m_delete)
        delete[] m_pData;
        
    // copy the new values
    m_rowCount = rowCount;
    m_columnCount = columnCount;
    
    // allocate the data and set the delete flag
    m_pData = new float[m_rowCount * m_columnCount];
    m_delete = true;
}

//--------------------------------------------------------------------------
inline
void
MSR_Matrix::SetDimension (uInt rowCount, uInt columnCount, float* pData)
{
    assert ((rowCount > 0) and (columnCount > 0));
    
    // check to see if there is already a pointer
    if (m_delete)
        delete[] m_pData;
        
    // copy the new values
    m_rowCount = rowCount;
    m_columnCount = columnCount;
    
    // copy the data pointer and clear the delete flag
    m_pData = pData;
    m_delete = false;
}

//--------------------------------------------------------------------------
inline
uInt
MSR_Matrix::GetRowCount (void) const
{
	return m_rowCount;
}

//--------------------------------------------------------------------------
inline
uInt
MSR_Matrix::GetColumnCount (void) const
{
	return m_columnCount;
}

//--------------------------------------------------------------------------
inline
const float*
MSR_Matrix::GetData (void) const
{
    return m_pData;
}

//--------------------------------------------------------------------------
inline
float*
MSR_Matrix::GetData (void)
{
    return m_pData;
}

//--------------------------------------------------------------------------
inline
float& 
MSR_Matrix::operator () (uInt row, uInt column)
{
    assert (m_pData);
	return m_pData[column + (row * m_columnCount)];
}

//--------------------------------------------------------------------------
inline
const float&
MSR_Matrix::operator () (uInt row, uInt column) const
{
    assert (m_pData);
	return m_pData[column + (row * m_columnCount)];
}

//--------------------------------------------------------------------------

#endif  //  _MSR_MATRIX_INL_
