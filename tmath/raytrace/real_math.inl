//-----------------------------------------------------------------------------
// Copyright (C) 2002 Microsoft, All rights reserved
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// includes
//-----------------------------------------------------------------------------
#ifndef     _REAL_H_
#include    "real.h"
#endif      _REAL_H_

//-----------------------------------------------------------------------------
#ifndef     _REAL_MATH_INL_
#define     _REAL_MATH_INL_

//-----------------------------------------------------------------------------
// specializations of basic math functions for the real type
//-----------------------------------------------------------------------------
template<class Number>
TReal<Number>
Tangent (const TReal<Number>& angle)
{
    return Tangent (angle ());
}

//-----------------------------------------------------------------------------
template<class Number>
TReal<Number>
ArcTangent (const TReal<Number>& value)
{
    return ArcTangent (value ());
}

//-----------------------------------------------------------------------------
template<class Number>
TReal<Number>
ArcTangent (const TReal<Number>& x, const TReal<Number>& y)
{
    return ArcTangent (x (), y ());
}

//-----------------------------------------------------------------------------
template<class Number>
TReal<Number>
Cosine (const TReal<Number>& value)
{
    return Cosine (value ());
}

//-----------------------------------------------------------------------------
template<class Number>
TReal<Number>
ArcCosine (const TReal<Number>& value)
{
    return ArcCosine (value ());
}

//-----------------------------------------------------------------------------
template<class Number>
TReal<Number>
Sine (const TReal<Number>& value)
{
    return Sine (value ());
}

//-----------------------------------------------------------------------------
template<class Number>
TReal<Number>
ArcSine (const TReal<Number>& value)
{
    return ArcSine (value ());
}

//-----------------------------------------------------------------------------
template<class Number>
TReal<Number>
Modulo (const TReal<Number>& numerator, const TReal<Number>& denominator)
{
    return Modulo (numerator (), denominator ());
}

//-----------------------------------------------------------------------------
template<class Number>
TReal<Number>
SquareRoot (const TReal<Number>& value)
{
    return SquareRoot (value ());
}

//-----------------------------------------------------------------------------
template<class Number>
TReal<Number>
Power (const TReal<Number>& base, const TReal<Number>& power)
{
    return Power (base (), power ());
}

//-----------------------------------------------------------------------------
template<class Number>
TReal<Number>
Sign (const TReal<Number>& value)
{
    return Sign (value (), TReal<Number>::GetEpsilon ());
}

//-----------------------------------------------------------------------------
template<class Number>
TReal<Number>
AbsoluteValue (const TReal<Number>& value)
{
    return AbsoluteValue (value ());
}

//-----------------------------------------------------------------------------
template<class Number>
TReal<Number>
Floor (const TReal<Number>& value)
{
    return Floor (value ());
}

//-----------------------------------------------------------------------------
template<class Number>
TReal<Number>
Ceiling (const TReal<Number>& value)
{
    return Ceiling (value ());
}

//-----------------------------------------------------------------------------
template<class Number>
TReal<Number>
RoundOff (const TReal<Number>& value)
{
    return RoundOff (value ());
}

//-----------------------------------------------------------------------------
template<class Number>
TReal<Number>
Maximum (const TReal<Number>& valueA, const TReal<Number>& valueB)
{
    return Maximum (valueA (), valueB ());
}

//-----------------------------------------------------------------------------
template<class Number>
TReal<Number>
Minimum (const TReal<Number>& valueA, const TReal<Number>& valueB)
{
    return Minimum (valueA (), valueB ());
}

//-----------------------------------------------------------------------------

#endif  //  _REAL_MATH_INL_
