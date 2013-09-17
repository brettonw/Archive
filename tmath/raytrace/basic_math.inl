//-----------------------------------------------------------------------------
// Copyright (C) 2002 Microsoft, All rights reserved
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// includes
//-----------------------------------------------------------------------------
#ifndef     _BASIC_MATH_HL_
#include    "basic_math.h"
#endif  //  _BASIC_MATH_H_

//-----------------------------------------------------------------------------
#ifndef     _BASIC_MATH_INL_
#define     _BASIC_MATH_INL_

//-----------------------------------------------------------------------------
// functions
//-----------------------------------------------------------------------------
template<class Number>
Number
Tangent (const Number& angle)
{
    return tan (angle);
}

//-----------------------------------------------------------------------------
template<class Number>
Number
ArcTangent (const Number& value)
{
    return atan (value);
}

//-----------------------------------------------------------------------------
template<class Number>
Number
ArcTangent (const Number& x, const Number& y)
{
    return atan2 (x, y);
}

//-----------------------------------------------------------------------------
template<class Number>
Number
Cosine (const Number& value)
{
    return cos (value);
}

//-----------------------------------------------------------------------------
template<class Number>
Number
ArcCosine (const Number& value)
{
    return acos (value);
}

//-----------------------------------------------------------------------------
template<class Number>
Number
Sine (const Number& value)
{
    return sin (value);
}

//-----------------------------------------------------------------------------
template<class Number>
Number
ArcSine (const Number& value)
{
    return asin (value);
}

//-----------------------------------------------------------------------------
template<class Number>
Number
Modulo (const Number& numerator, const Number& denominator)
{
    return fmod (numerator, denominator);
}

//-----------------------------------------------------------------------------
template<class Number>
Number
SquareRoot (const Number& value)
{
    return sqrt (value);
}

//-----------------------------------------------------------------------------
template<class Number>
Number
Power (const Number& base, const Number& power)
{
    return pow (base, power);
}

//-----------------------------------------------------------------------------
template<class Number>
Number
Sign (const Number& value)
{
    return (value < 0) ? Number (-1) : ((value > 0) ? Number (1) : Number (0));
}

//-----------------------------------------------------------------------------
template<class Number>
Number
Sign (const Number& value, const Number& epsilon)
{
    return (value < -epsilon) ? Number (-1) : ((value > epsilon) ? Number (1) : Number (0));
}

//-----------------------------------------------------------------------------
template<class Number>
Number
AbsoluteValue (const Number& value)
{
    //return (value < 0) ? -value : value;
    return fabs (value);
}

//-----------------------------------------------------------------------------
template<class Number>
Number
Floor (const Number& value)
{
    return floor (AbsoluteValue (value)) * Sign (value);
}

//-----------------------------------------------------------------------------
template<class Number>
Number
Ceiling (const Number& value)
{
    return ceil (AbsoluteValue (value)) * Sign (value);
}

//-----------------------------------------------------------------------------
template<class Number>
Number
RoundOff (const Number& value)
{
    return floor (value + Number (0.5));
}

//-----------------------------------------------------------------------------
template<class Number>
Number
Maximum (const Number& valueA, const Number& valueB)
{
    return (valueA > valueB) ? valueA : valueB;
}

//-----------------------------------------------------------------------------
template<class Number>
Number
Minimum (const Number& valueA, const Number& valueB)
{
    return (valueA < valueB) ? valueA : valueB;
}

//-----------------------------------------------------------------------------

#endif  //  _BASIC_MATH_INL_
