//-----------------------------------------------------------------------------
// Copyright (C) 2006 Bretton Wade, All Rights Reserved
//-----------------------------------------------------------------------------

#ifndef     _BOUND_INL_
#define     _BOUND_INL_

//-----------------------------------------------------------------------------
// includes
//-----------------------------------------------------------------------------
#ifndef     _BOUND_H_
#include    "bound.h"
#endif  //  _BOUND_H_

//-----------------------------------------------------------------------------
// inlines
//-----------------------------------------------------------------------------
inline
/* void */      
Bound::Bound (const Bound& bound)
{
    m_bounds[X_AXIS][LOWER_BOUND] = bound.m_bounds[X_AXIS][LOWER_BOUND];
    m_bounds[X_AXIS][UPPER_BOUND] = bound.m_bounds[X_AXIS][UPPER_BOUND];
    m_bounds[Y_AXIS][LOWER_BOUND] = bound.m_bounds[Y_AXIS][LOWER_BOUND];
    m_bounds[Y_AXIS][UPPER_BOUND] = bound.m_bounds[Y_AXIS][UPPER_BOUND];
}

//-----------------------------------------------------------------------------
inline
Bound&      
Bound::operator = (const Bound& bound)
{
    m_bounds[X_AXIS][LOWER_BOUND] = bound.m_bounds[X_AXIS][LOWER_BOUND];
    m_bounds[X_AXIS][UPPER_BOUND] = bound.m_bounds[X_AXIS][UPPER_BOUND];
    m_bounds[Y_AXIS][LOWER_BOUND] = bound.m_bounds[Y_AXIS][LOWER_BOUND];
    m_bounds[Y_AXIS][UPPER_BOUND] = bound.m_bounds[Y_AXIS][UPPER_BOUND];
}

//-----------------------------------------------------------------------------
inline
const float*
Bound::operator [] (Word axis) const
{
    return m_bounds[axis];
}

//-----------------------------------------------------------------------------
inline
float*
Bound::operator [] (Word axis)
{
    return m_bounds[axis];
}

//-----------------------------------------------------------------------------
inline
void
Bound::Set (float left, float right, float bottom, float top)
{
    m_bounds[X_AXIS][LOWER_BOUND] = left;
    m_bounds[X_AXIS][UPPER_BOUND] = right;
    m_bounds[Y_AXIS][LOWER_BOUND] = bottom;
    m_bounds[Y_AXIS][UPPER_BOUND] = top;
}

//-----------------------------------------------------------------------------
inline
void
Bound::SetEmpty (void)
{
    float   infinity = 1.0e6;
    m_bounds[X_AXIS][LOWER_BOUND] = infinity;
    m_bounds[X_AXIS][UPPER_BOUND] = -infinity;
    m_bounds[Y_AXIS][LOWER_BOUND] = infinity;
    m_bounds[Y_AXIS][UPPER_BOUND] = -infinity;
}

//-----------------------------------------------------------------------------
inline
float
Bound::GetSize (Word axis) const
{
    return m_bounds[axis][UPPER_BOUND] - m_bounds[axis][LOWER_BOUND];
}

//-----------------------------------------------------------------------------
inline
bool
Bound::ContainsPtAxis (Word axis, const Vector2& pt) const
{
    return (pt[axis] > m_bounds[axis][LOWER_BOUND]) and (pt[axis] < m_bounds[axis][UPPER_BOUND]);
}

//-----------------------------------------------------------------------------
inline
bool
Bound::ContainsPt (const Vector2& pt) const
{
    return ContainsPtAxis (X_AXIS, pt) and ContainsPtAxis (Y_AXIS, pt);
}

//-----------------------------------------------------------------------------
inline
bool
Bound::OverlapAxis (Word axis, const Bound& a, const Bound& b)
{
    return (a[axis][UPPER_BOUND] > b[axis][LOWER_BOUND]) and (b[axis][UPPER_BOUND] > a[axis][LOWER_BOUND]);
}

//-----------------------------------------------------------------------------
inline
bool      
Bound::Overlap (const Bound& a, const Bound& b)
{
    return OverlapAxis (X_AXIS, a, b) and OverlapAxis (Y_AXIS, a, b);
}

//-----------------------------------------------------------------------------

#endif  //  _BOUND_INL_

