//-----------------------------------------------------------------------------
// Copyright (C) 1997-2006 Bretton Wade
// All Rights Reserved
//-----------------------------------------------------------------------------

#ifndef     _ADVANCED_MATH_H_
#define     _ADVANCED_MATH_H_

//-----------------------------------------------------------------------------
// constants
//-----------------------------------------------------------------------------
const float     c_f2PI = D3DX_PI * 2;
const float     c_fEpsilon = 1.0e-6f;

//-----------------------------------------------------------------------------
// globals
//-----------------------------------------------------------------------------
extern float    g_fEpsilon;

//-----------------------------------------------------------------------------
// useful vector operator definitions
//-----------------------------------------------------------------------------
#define dot     |
#define cross   ^

//-----------------------------------------------------------------------------
float       operator dot (const D3DXVECTOR2& lhs, const D3DXVECTOR2& rhs);
float       operator cross (const D3DXVECTOR2& lhs, const D3DXVECTOR2& rhs);

//-----------------------------------------------------------------------------
// generally useful routines
//-----------------------------------------------------------------------------
float	    Sign (float fValue);
D3DXVECTOR2 Reflect (const D3DXVECTOR2& v, const D3DXVECTOR2& n);
bool        Dist (const D3DXVECTOR2& p, const D3DXVECTOR2& v, float d, float &root1, float &root2);
bool        Quadratic (float a, float b, float c, float &root1, float &root2);

//-----------------------------------------------------------------------------
// fuzzy comparisons for floats
//-----------------------------------------------------------------------------
bool        CloseMatch (float a, float b);
bool        CloseMatch (const D3DXVECTOR2& A, const D3DXVECTOR2& B);
bool        CloseMatch (const D3DXVECTOR3& A, const D3DXVECTOR3& B);

//-----------------------------------------------------------------------------
// epsilon calculations
//-----------------------------------------------------------------------------
void        SetFloatRange (float fMin, float fMax);

//-----------------------------------------------------------------------------
// inlines
//-----------------------------------------------------------------------------
#ifndef     _ADVANCED_MATH_INL_
#include    "advanced_math.inl"
#endif  //  _ADVANCED_MATH_INL_

//-----------------------------------------------------------------------------

#endif  //  _ADVANCED_MATH_H_
