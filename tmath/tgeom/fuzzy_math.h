//-----------------------------------------------------------------------------
// Copyright (C) 2002-2005 Bretton Wade, All rights reserved
//-----------------------------------------------------------------------------

#ifndef     _FUZZY_MATH_H_
#define     _FUZZY_MATH_H_

//-----------------------------------------------------------------------------
// constants - done as a macro to support multiple types
//-----------------------------------------------------------------------------
#define FLOAT_EPSILON   (1.0e-6f)
#define DOUBLE_EPSILON  (1.0e-12)
#define REAL_EPSILON    GetEpsilon<Real> (static_cast<Real> (FLOAT_EPSILON))

#define FLOAT_INFINITY  (1.0e6f)
#define DOUBLE_INFINITY (1.0e12)
#define REAL_INFINITY    GetInfinity<Real> (static_cast<Real> (FLOAT_INFINITY))

//-----------------------------------------------------------------------------
// functions
//-----------------------------------------------------------------------------
template<class Real>
Real
GetEpsilon (Real rDefaultEpsilon)
{
    return rDefaultEpsilon;
}

//-----------------------------------------------------------------------------
inline
double
GetEpsilon (double /* dUnused */)
{
    return DOUBLE_EPSILON;
}

//-----------------------------------------------------------------------------
template<class Real>
Real
GetInfinity (Real rDefaultInfinity)
{
    return rDefaultInfinity;
}

//-----------------------------------------------------------------------------
inline
double
GetInfinity (double /* dUnused */)
{
    return DOUBLE_INFINITY;
}

//-----------------------------------------------------------------------------
template<class Real>
Real
Modulo (Real rValue, Real rDenominator)
{
    return fmod (rValue, rDenominator);
}

//-----------------------------------------------------------------------------
inline
float
Modulo (float fValue, float fDenominator)
{
    return fmodf (fValue, fDenominator);
}

//-----------------------------------------------------------------------------
template<class Real>
Real
Tangent (Real rValue)
{
    return tan (rValue);
}

//-----------------------------------------------------------------------------
inline
float
Tangent (float fValue)
{
    return tanf (fValue);
}

//-----------------------------------------------------------------------------
template<class Real>
Real
ArcTangent (Real rValue)
{
    return atan (rValue);
}

//-----------------------------------------------------------------------------
inline
float
ArcTangent (float fValue)
{
    return atanf (fValue);
}

//-----------------------------------------------------------------------------
template<class Real>
Real
Cosine (Real rValue)
{
    return cos (rValue);
}

//-----------------------------------------------------------------------------
inline
float
Cosine (float fValue)
{
    return cosf (fValue);
}

//-----------------------------------------------------------------------------
template<class Real>
Real
Sine (Real rValue)
{
    return sin (rValue);
}

//-----------------------------------------------------------------------------
inline
float
Sine (float fValue)
{
    return sinf (fValue);
}

//-----------------------------------------------------------------------------
template<class Real>
Real
Sign (Real rValue, Real rEpsilon = REAL_EPSILON)
{
    return (rValue < -rEpsilon) ? static_cast<Real> (-1) : ((rValue > rEpsilon) ? static_cast<Real> (1) : static_cast<Real> (0));
}

//-----------------------------------------------------------------------------
template<class Real>
Real
AbsoluteValue (Real rValue)
{
    return (rValue < 0) ? -rValue : rValue;
}

//-----------------------------------------------------------------------------
template<class Real>
Real
Floor (Real rValue)
{
    return floor (AbsoluteValue (rValue)) * Sign (rValue);
}

//-----------------------------------------------------------------------------
inline
float
Floor (float fValue)
{
    return floorf (AbsoluteValue (fValue)) * Sign (fValue);
}

//-----------------------------------------------------------------------------
template<class Real>
Real
Ceiling (Real rValue)
{
    return ceil (AbsoluteValue (rValue)) * Sign (rValue);
}

//-----------------------------------------------------------------------------
inline
float
Ceiling (float fValue)
{
    return ceilf (AbsoluteValue (fValue)) * Sign (fValue);
}

//-----------------------------------------------------------------------------
template<class Real>
Real
RoundOff (Real rValue)
{
    return Floor (rValue + (static_cast<Real> (0.5) * Sign (rValue)));
}

//-----------------------------------------------------------------------------
template<class Real>
Real
Maximum (Real rValueA, Real rValueB)
{
    return (rValueA > rValueB) ? rValueA : rValueB;
}

//-----------------------------------------------------------------------------
template<class Real>
Real
Minimum (Real rValueA, Real rValueB)
{
    return (rValueA < rValueB) ? rValueA : rValueB;
}

//-----------------------------------------------------------------------------
template<class Real>
Real
SquareRoot (Real rValue)
{
    return sqrt (rValue);
}

//-----------------------------------------------------------------------------
inline
float
SquareRoot (float fValue)
{
    return sqrtf (fValue);
}

//-----------------------------------------------------------------------------
template<class Real>
bool
FuzzyEquals (Real rLeft, Real rRight, Real rEpsilon = REAL_EPSILON)
{
    return AbsoluteValue (rLeft - rRight) <= rEpsilon;
}

//-----------------------------------------------------------------------------
template<class Real>
bool
FuzzyNotEquals (Real rLeft, Real rRight, Real rEpsilon = REAL_EPSILON)
{
    return AbsoluteValue (rLeft - rRight) > rEpsilon;
}

//-----------------------------------------------------------------------------

#endif  //  _FUZZY_MATH_H_
