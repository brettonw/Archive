//-----------------------------------------------------------------------------
// Copyright (C) 1997-2006 Bretton Wade
// All Rights Reserved
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// include files
//-----------------------------------------------------------------------------
#include    "precompile.h"
#include    "advanced_math.h"

//-----------------------------------------------------------------------------
// globals
//-----------------------------------------------------------------------------
float    g_fEpsilon = c_fEpsilon;

//-----------------------------------------------------------------------------
// functions
//-----------------------------------------------------------------------------
D3DXVECTOR2 Reflect (const D3DXVECTOR2& v, const D3DXVECTOR2& n)
{
	return v - (n * (2 * (v dot n)));
}

//-----------------------------------------------------------------------------
bool
Dist (const D3DXVECTOR2& p, const D3DXVECTOR2& v, float d, float &fRoot1, float &fRoot2)
{
    float   a = v dot v;
    float   b = 2 * (p dot v);
    float   c = (p dot p) - (d * d);
    return Quadratic (a, b, c, fRoot1, fRoot2);
}

//-----------------------------------------------------------------------------
bool
Quadratic (float a, float b, float c, float &fRoot1, float &fRoot2)
{
    float    fRadical = (b * b) - (4 * a * c);
    if (fRadical > 0)
    {
		float   fRoot = sqrtf (fRadical);
		float   q = -0.5f * (b + (Sign (b) * fRoot));
		fRoot1 = c / q;
		fRoot2 = q / a;
		if (fRoot1 > fRoot2)
		{
			float	fTmp = fRoot1;
			fRoot1 = fRoot2;
			fRoot2 = fTmp;
		}
		return true;
	}
	return false;
}

//-----------------------------------------------------------------------------
void        
SetFloatRange (float fMin, float fMax)
{
    // find the greatest absolute value of the range
    float   fRangeMax = max (fabsf (fMin), fabsf (fMax));
    
    // now find the next power of ten greater than the range
    float   fPowerOfTen = 1.0f;
    while (fPowerOfTen < fRangeMax)
        fPowerOfTen *= 10.0f;
        
    // start with an epsilon 6 orders of magnitude smaller than the range
    g_fEpsilon = c_fEpsilon * fPowerOfTen;
    
    // confirm that the epsilon is at least one order of magnitude greater than 
    // the size that the machine is actually capable of recognizing
    float   fNewVal = fMin;
    float   fOldVal = fMin + g_fEpsilon;
    while (fNewVal != fOldVal)
    {
        fOldVal = fNewVal;
        fNewVal += g_fEpsilon;
    }
    Assert (fNewVal > (fPowerOfTen * 10.0f));
}

//-----------------------------------------------------------------------------
