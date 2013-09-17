//-----------------------------------------------------------------------------
// Copyright (C) 1997-2006 Bretton Wade
// All Rights Reserved
//-----------------------------------------------------------------------------

#ifndef     _ADVANCED_MATH_INL_
#define     _ADVANCED_MATH_INL_

//-----------------------------------------------------------------------------
// include files
//-----------------------------------------------------------------------------
#ifndef     _ADVANCED_MATH_H_
#include    "advanced_math.h"
#endif  //  _ADVANCED_MATH_H_

//-----------------------------------------------------------------------------
// useful vector operator definitions
//-----------------------------------------------------------------------------
inline
float
operator dot (const D3DXVECTOR2& lhs, const D3DXVECTOR2& rhs)
{
    return D3DXVec2Dot (&lhs, &rhs);
}

//-----------------------------------------------------------------------------
inline
float
operator cross (const D3DXVECTOR2& lhs, const D3DXVECTOR2& rhs)
{
    return (lhs.x * rhs.y) - (lhs.y * rhs.x);
}

//-----------------------------------------------------------------------------
// generally useful routines
//-----------------------------------------------------------------------------
inline	
float	
Sign (float fValue)
{
	return (fValue > 0.0f) ? 1.0f : ((fValue < 0.0f) ? -1.0f : 0.0f);
}

//-----------------------------------------------------------------------------
// fuzzy comparisons for floats
//-----------------------------------------------------------------------------
inline
bool    
CloseMatch (float a, float b)
{
#if 1
    float   delta = fabsf (a - b);
    bool    result = delta <= g_fEpsilon;
    return result;
#else
    float   fa = fabsf (a);
    float   fb = fabsf (b);
    float   delta = (fa > fb) ? (fa / fb) - 1.0f : (fb / fa) - 1.0f;
    bool    result = delta < 1.0e-2f;
    return result;
#endif
}

//-----------------------------------------------------------------------------
inline
bool    
CloseMatch (const D3DXVECTOR2& A, const D3DXVECTOR2& B)
{
    return CloseMatch (A.x, B.x) and CloseMatch (A.y, B.y);
}

//-----------------------------------------------------------------------------
inline
bool    
CloseMatch (const D3DXVECTOR3& A, const D3DXVECTOR3& B)
{
    return CloseMatch (A.x, B.x) and CloseMatch (A.y, B.y) and CloseMatch (A.z, B.z);
}

//-----------------------------------------------------------------------------

#endif  //  _ADVANCED_MATH_INL_
