//-----------------------------------------------------------------------------
// Copyright (C) 2002 Microsoft, All rights reserved
//-----------------------------------------------------------------------------

#ifndef     _BASIC_MATH_H_
#define     _BASIC_MATH_H_

//-----------------------------------------------------------------------------
// namespace
//-----------------------------------------------------------------------------
BEGIN_NAMESPACE (TMath)

//-----------------------------------------------------------------------------
// functions
//-----------------------------------------------------------------------------
template<class Number>
Number
Tangent (const Number& angle);

template<class Number>
Number
ArcTangent (const Number& value);

template<class Number>
Number
ArcTangent (const Number& x, const Number& y);

template<class Number>
Number
Cosine (const Number& value);

template<class Number>
Number
ArcCosine (const Number& value);

template<class Number>
Number
Sine (const Number& value);

template<class Number>
Number
ArcSine (const Number& value);

template<class Number>
Number
Modulo (const Number& numerator, const Number& denominator);

template<class Number>
Number
SquareRoot (const Number& value);

template<class Number>
Number
Power (const Number& base, const Number& power);

template<class Number>
Number
Sign (const Number& value);

template<class Number>
Number
Sign (const Number& value, const Number& epsilon = 0);

template<class Number>
Number
AbsoluteValue (const Number& value);

template<class Number>
Number
Floor (const Number& value);

template<class Number>
Number
Ceiling (const Number& value);

template<class Number>
Number
RoundOff (const Number& value);

template<class Number>
Number
Maximum (const Number& valueA, const Number& valueB);

template<class Number>
Number
Minimum (const Number& valueA, const Number& valueB);

//-----------------------------------------------------------------------------
// inlines
//-----------------------------------------------------------------------------
#ifndef     _BASIC_MATH_INL_
#include    "basic_math.inl"
#endif  //  _BASIC_MATH_INL_

//-----------------------------------------------------------------------------
// namespace
//-----------------------------------------------------------------------------
END_NAMESPACE (TMath)

//-----------------------------------------------------------------------------

#endif  //  _BASIC_MATH_H_
