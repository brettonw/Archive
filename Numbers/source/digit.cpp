//------------------------------------------------------------------------------
//	Copyright ©1996-2004 Bretton Wade, All rights reserved.
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifndef     _PRECOMPILE_
#include    "precompile.h"
#endif  //  _PRECOMPILE_

#include    "digit.h"

//------------------------------------------------------------------------------
uDigit	gRadixMax[37] =
				{ 0, 0, 32768, 59049, 16384, 15625, 46656, 16807, 32768, 59049, 10000, 14641, 20736, 28561,
				 38416, 50625, 4096, 4913, 5832, 6859, 8000, 9261, 10648, 12167, 13824, 15625, 17576, 19683,
				 21952, 24389, 27000, 29791, 32768, 35937, 39304, 42875, 46656};
sInt4	gRadixDigits[37] =
				{ 0, 0, 15, 10, 7, 6, 6, 5, 5, 5, 4, 4, 4, 4, 4, 4, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
				 3, 3, 3, 3, 3, 3, 3};
cstr		gDigit2char = "0123456789abcdefghijklmnopqrstuvwxyz";
uDigit	gChar2digit[128] =
				{ 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
					0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
					0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
					0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  0,  0,  0,  0,  0,  0,
					0, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24,
				 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35,  0,  0,  0,  0,  0,
					0, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24,
				 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35,  0,  0,  0,  0,  0};

//------------------------------------------------------------------------------
sInt4		StrToDigit (cstr string, uDigit radix, uDigit &digit, uDigit &multiplier)
{
	digit = 0;
	multiplier = 1;
	sInt4 i;
	for (i = 0; (i < gRadixDigits[radix]) && *string; i++)
	{ 
		digit *= radix;
		digit += gChar2digit[*string++];
		multiplier *= radix;
	}
	return i;
}

//------------------------------------------------------------------------------
sInt4		DigitToStr (uDigit digit, cstr string, uDigit radix)
{
	sInt4	count = 0;
	for (sInt4 i = 0; i < gRadixDigits[radix]; i++)
	{
		uDigit	remainder = digit % radix;
		digit /= radix;
		string[count] = gDigit2char[remainder];
		count--;
	}
	return count;
}

//------------------------------------------------------------------------------
uDigit	Sub (sDigit size, const uDigit *a, const uDigit *b, uDigit *result)
{
	uTwoDigit	borrow = 0;
	for (sDigit i = 0; i < size; i++)
		result[i] = uDigit (borrow = a[i] - b[i] - ((borrow & (0x01 << DIGIT_BITS)) >> DIGIT_BITS));
	return uDigit (borrow);
}

//------------------------------------------------------------------------------
int		Cmp (sDigit size, const uDigit *a, const uDigit *b)
{
	for (sDigit i = size - 1; i >= 0; i--)
	{
		int		comparison = int (a[i]) - int (b[i]);
		if (comparison)
			return comparison;
	}
	return 0;
}

//------------------------------------------------------------------------------
uDigit	SubMult (sDigit size, const uDigit *a, const uDigit *b, uDigit c, uDigit *result)
{
	uTwoDigit	carry = 0,
						borrow = 0;
	for (sDigit i = 0; i < size; i++)
	{
		carry = (b[i] * c) + (carry >> DIGIT_BITS);
		borrow = a[i] - uDigit (carry) - ((borrow & (0x01 << DIGIT_BITS)) >> DIGIT_BITS);
		result[i] = uDigit (borrow);
	}
	return borrow;
}

//------------------------------------------------------------------------------
sInt4		DigitBits (uDigit a)
{
	sInt4	bits = DIGIT_BITS;
	while (a)
	{
		a >>= 1;
		bits--;
	}
	return bits;
}

//------------------------------------------------------------------------------
