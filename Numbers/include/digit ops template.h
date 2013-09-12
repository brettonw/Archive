//------------------------------------------------------------------------------
//	Copyright ©1996-2004 Bretton Wade, All rights reserved.
//------------------------------------------------------------------------------

#ifndef     _DIGIT_OPS_TEMPLATE_
#define     _DIGIT_OPS_TEMPLATE_

//------------------------------------------------------------------------------
#ifndef     _DIGIT_
#include    "digit.h"
#endif  //  _DIGIT_

//------------------------------------------------------------------------------
template<sInt4 N>
struct  DigitOp
{
static	sInt4		Top (const uDigit *a);
static	void		Set (uTwoDigit *dst, uTwoDigit src);
static	void		Cop (uTwoDigit *dst, const uTwoDigit *src);
static	uDigit		Tst (const uTwoDigit *a);
static	sInt4		Cmp (const uTwoDigit *a, const uTwoDigit *b);
static	uDigit		LSh (const uDigit *a, uDigit *result, uDigit fill = 0);
static	uDigit		LSh (const uDigit *a, uDigit bits, uDigit *result);
static	uDigit		RSh (const uDigit *a, uDigit *result, uDigit fill = 0);
static	uDigit		RSh (const uDigit *a, uDigit bits, uDigit *result);
static	uDigit		Add (const uDigit *a, const uDigit *b, uDigit *result, uDigit carry);
static	uDigit		Sub (const uDigit *a, const uDigit *b, uDigit *result, uDigit borrow);
static	sInt4		Cmp_Digit (const uDigit *a, uDigit b);
static	uDigit		Add_Digit (const uDigit *a, uDigit b, uDigit *result, uDigit carry);
static	uDigit		Sub_Digit (const uDigit *a, uDigit b, uDigit *result, uDigit borrow);
static	uDigit		Mul_Digit (const uDigit *a, uDigit b, uDigit *result, uDigit carry);
static	uDigit		Div_Digit (const uDigit *a, uDigit b, uDigit *result, uDigit remainder);
static	uDigit		Mul_Digit_Add (const uDigit *a, const uDigit *b, uDigit c, uDigit *result, uDigit carry);
static	void		Bit_And (const uTwoDigit *a, const uTwoDigit *b, uTwoDigit *result);
static	void		Bit_Or (const uTwoDigit *a, const uTwoDigit *b, uTwoDigit *result);
static	void		Bit_Xor (const uTwoDigit *a, const uTwoDigit *b, uTwoDigit *result);
static	void		Bit_Complement (const uTwoDigit *a, uTwoDigit *result);
};

//------------------------------------------------------------------------------
template<sInt4 N>
inline	sInt4	DigitOp<N>::Top (const uDigit *a)
{
	if (a[N - 1])
		return N;
	return DigitOp<N - 1>::Top (a);
}

//------------------------------------------------------------------------------
template<>
inline	sInt4	DigitOp<0>::Top (const uDigit*)
{
	return 0;
}

//------------------------------------------------------------------------------
template<sInt4 N>
inline	void	DigitOp<N>::Set (uTwoDigit *dst, uTwoDigit src)
{
	dst[N - 1] = src;
	DigitOp<N - 1>::Set (dst, src);
}

//------------------------------------------------------------------------------
template<>
inline	void	DigitOp<0>::Set (uTwoDigit*, uTwoDigit)
{
}

//------------------------------------------------------------------------------
template<sInt4 N>
inline	void	DigitOp<N>::Cop (uTwoDigit *dst, const uTwoDigit *src)
{
	*dst++ = *src++;
	DigitOp<N - 1>::Cop (dst, src);
}

//------------------------------------------------------------------------------
template<>
inline	void	DigitOp<0>::Cop (uTwoDigit*, const uTwoDigit*)
{
}

//------------------------------------------------------------------------------
template<sInt4 N>
inline	uDigit	DigitOp<N>::Tst (const uTwoDigit *a)
{
	return a[N - 1] ? 1 : DigitOp<N - 1>::Tst (a);
}

//------------------------------------------------------------------------------
template<>
inline	uDigit	DigitOp<0>::Tst (const uTwoDigit*)
{
	return 0;
}

//------------------------------------------------------------------------------
template<sInt4 N>
inline	sInt4	DigitOp<N>::Cmp (const uTwoDigit *a, const uTwoDigit *b)
{
	if (a[N - 1] > b[N - 1])
		return 1;
	if (a[N - 1] < b[N - 1])
		return -1;
	return DigitOp<N - 1>::Cmp (a, b);
}

//------------------------------------------------------------------------------
template<>
inline	sInt4	DigitOp<0>::Cmp (const uTwoDigit*, const uTwoDigit*)
{
	return 0;
}

//------------------------------------------------------------------------------
template<sInt4 N>
inline	uDigit	DigitOp<N>::LSh (const uDigit *a, uDigit *result, uDigit fill)
{
	uDigit r = a[N - 1];
	result[N - 1] = DigitOp<N - 1>::LSh (a, result, fill);
	return r;
}

//------------------------------------------------------------------------------
template<>
inline	uDigit	DigitOp<0>::LSh (const uDigit*, uDigit*, uDigit fill)
{
	return fill;
}

//------------------------------------------------------------------------------
template<sInt4 N>
inline	uDigit	DigitOp<N>::LSh (const uDigit *a, uDigit bits, uDigit *result)
{
	uTwoDigit	r = a[N - 1] << bits;
	result[N - 1] = uDigit (r) | DigitOp<N - 1>::LSh (a, bits, result);
	return uDigit (r >> DIGIT_BITS);
}

//------------------------------------------------------------------------------
template<>
inline	uDigit	DigitOp<0>::LSh (const uDigit*, uDigit, uDigit*)
{
	return 0;
}

//------------------------------------------------------------------------------
template<sInt4 N>
inline	uDigit	DigitOp<N>::RSh (const uDigit *a, uDigit *result, uDigit fill)
{
	uDigit r = *a;
	*result = DigitOp<N - 1>::RSh (a + 1, result + 1, fill);
	return r;
}

//------------------------------------------------------------------------------
template<>
inline	uDigit	DigitOp<0>::RSh (const uDigit*, uDigit*, uDigit fill)
{
	return fill;
}

//------------------------------------------------------------------------------
template<sInt4 N>
inline	uDigit	DigitOp<N>::RSh (const uDigit *a, uDigit bits, uDigit *result)
{
	uTwoDigit	r = *a;
	*result = uDigit (r >> bits) | DigitOp<N - 1>::RSh (a + 1, bits, result + 1);
	return uDigit ((r << DIGIT_BITS) >> bits);
}

//------------------------------------------------------------------------------
template<>
inline	uDigit	DigitOp<0>::RSh (const uDigit*, uDigit, uDigit*)
{
	return 0;
}

//------------------------------------------------------------------------------
template<sInt4 N>
inline	uDigit	DigitOp<N>::Add (const uDigit *a, const uDigit *b, uDigit *result, uDigit carry)
{
	uTwoDigit	r = *a + *b + carry;
	*result = uDigit (r);
	return DigitOp<N - 1>::Add (a + 1, b + 1, result + 1, r >> DIGIT_BITS);
}

//------------------------------------------------------------------------------
template<>
inline	uDigit	DigitOp<0>::Add (const uDigit*, const uDigit*, uDigit*, uDigit carry)
{
	return carry;
}

//------------------------------------------------------------------------------
template<sInt4 N>
inline	uDigit	DigitOp<N>::Sub (const uDigit *a, const uDigit *b, uDigit *result, uDigit borrow)
{
	uTwoDigit	r = *a - *b - borrow;
	*result = uDigit (r);
	return DigitOp<N - 1>::Sub (a + 1, b + 1, result + 1, (r & (1 << DIGIT_BITS)) >> DIGIT_BITS);
}

//------------------------------------------------------------------------------
template<>
inline	uDigit	DigitOp<0>::Sub (const uDigit*, const uDigit*, uDigit*, uDigit borrow)
{
	return borrow;
}

//------------------------------------------------------------------------------
template<sInt4 N>
inline	sInt4	DigitOp<N>::Cmp_Digit (const uDigit *a, uDigit b)
{
	if (a[N - 1])
		return 1;
	return DigitOp<N - 1>::Cmp_Digit (a, b);
}

//------------------------------------------------------------------------------
template<>
inline	sInt4	DigitOp<1>::Cmp_Digit (const uDigit *a, uDigit b)
{
	return a[0] - b;
}

//------------------------------------------------------------------------------
template<sInt4 N>
inline	uDigit	DigitOp<N>::Add_Digit (const uDigit *a, uDigit b, uDigit *result, uDigit carry)
{
	uTwoDigit	r = *a + b + carry;
	*result = uDigit (r);
	return DigitOp<N - 1>::Add_Digit (a + 1, 0, result + 1, r >> DIGIT_BITS);
}

//------------------------------------------------------------------------------
template<>
inline	uDigit	DigitOp<0>::Add_Digit (const uDigit*, uDigit, uDigit*, uDigit carry)
{
	return carry;
}

//------------------------------------------------------------------------------
template<sInt4 N>
inline	uDigit	DigitOp<N>::Sub_Digit (const uDigit *a, uDigit b, uDigit *result, uDigit borrow)
{
	uTwoDigit	r = *a - b - borrow;
	*result = uDigit (r);
	return DigitOp<N - 1>::Sub_Digit (a + 1, 0, result + 1, (r & (1 << DIGIT_BITS)) >> DIGIT_BITS);
}

//------------------------------------------------------------------------------
template<>
inline	uDigit	DigitOp<0>::Sub_Digit (const uDigit*, uDigit, uDigit*, uDigit borrow)
{
	return borrow;
}

//------------------------------------------------------------------------------
template<sInt4 N>
inline	uDigit	DigitOp<N>::Mul_Digit (const uDigit *a, uDigit b, uDigit *result, uDigit carry)
{
	uTwoDigit	r = (*a * b) + carry;
	*result = uDigit (r);
	return DigitOp<N - 1>::Mul_Digit (a + 1, b, result + 1, r >> DIGIT_BITS);
}

//------------------------------------------------------------------------------
template<>
inline	uDigit	DigitOp<0>::Mul_Digit (const uDigit*, uDigit, uDigit*, uDigit carry)
{
	return carry;
}

//------------------------------------------------------------------------------
template<sInt4 N>
inline	uDigit	DigitOp<N>::Div_Digit (const uDigit *a, uDigit b, uDigit *result, uDigit remainder)
{
	uTwoDigit	r = (remainder << DIGIT_BITS) + a[N - 1];
	result[N - 1] = uDigit (r / b);
	return DigitOp<N - 1>::Div_Digit (a, b, result, uDigit (r % b));
}

//------------------------------------------------------------------------------
template<>
inline	uDigit	DigitOp<0>::Div_Digit (const uDigit*, uDigit, uDigit*, uDigit remainder)
{
	return remainder;
}

//------------------------------------------------------------------------------
template<sInt4 N>
inline	uDigit	DigitOp<N>::Mul_Digit_Add (const uDigit *a, const uDigit *b, uDigit c, uDigit *result, uDigit carry)
{
	uTwoDigit	r = *a + (*b * c) + carry;
	*result = uDigit (r);
	return DigitOp<N - 1>::Mul_Digit_Add (a + 1, b + 1, c, result + 1, r >> DIGIT_BITS);
}

//------------------------------------------------------------------------------
template<>
inline	uDigit	DigitOp<0>::Mul_Digit_Add (const uDigit*, const uDigit*, uDigit, uDigit*, uDigit carry)
{
	return carry;
}

//------------------------------------------------------------------------------
template<sInt4 N>
inline	void	DigitOp<N>::Bit_And (const uTwoDigit *a, const uTwoDigit *b, uTwoDigit *result)
{
	result[N - 1] = a[N - 1] & b[N - 1];
	DigitOp<N - 1>::Bit_And (a, b, result);
}

//------------------------------------------------------------------------------
template<>
inline	void	DigitOp<0>::Bit_And (const uTwoDigit*, const uTwoDigit*, uTwoDigit*)
{
}

//------------------------------------------------------------------------------
template<sInt4 N>
inline	void	DigitOp<N>::Bit_Or (const uTwoDigit *a, const uTwoDigit *b, uTwoDigit *result)
{
	result[N - 1] = a[N - 1] | b[N - 1];
	DigitOp<N - 1>::Bit_Or (a, b, result);
}

//------------------------------------------------------------------------------
template<>
inline	void	DigitOp<0>::Bit_Or (const uTwoDigit*, const uTwoDigit*, uTwoDigit*)
{
}

//------------------------------------------------------------------------------
template<sInt4 N>
inline	void	DigitOp<N>::Bit_Xor (const uTwoDigit *a, const uTwoDigit *b, uTwoDigit *result)
{
	result[N - 1] = a[N - 1] ^ b[N - 1];
	DigitOp<N - 1>::Bit_Xor (a, b, result);
}

//------------------------------------------------------------------------------
template<>
inline	void	DigitOp<0>::Bit_Xor (const uTwoDigit*, const uTwoDigit*, uTwoDigit*)
{
}

//------------------------------------------------------------------------------
template<sInt4 N>
inline	void	DigitOp<N>::Bit_Complement (const uTwoDigit *a, uTwoDigit *result)
{
	result[N - 1] = ~a[N - 1];
	DigitOp<N - 1>::Bit_Complement (a, result);
}

//------------------------------------------------------------------------------
template<>
inline	void	DigitOp<0>::Bit_Complement (const uTwoDigit*, uTwoDigit*)
{
}

//------------------------------------------------------------------------------

#endif  //  _DIGIT_OPS_TEMPLATE_
