//------------------------------------------------------------------------------
//	Copyright ©1996-2004 Bretton Wade, All rights reserved.
//------------------------------------------------------------------------------

#ifndef     _DIGIT_
#define     _DIGIT_

//------------------------------------------------------------------------------
#if         1
typedef uInt2               uDigit;
typedef sInt2               sDigit;
//typedef	uInt1								uHalfDigit;
//typedef	sInt1								sHalfDigit;
typedef	uInt4               uTwoDigit;
typedef	sInt4               sTwoDigit;
#else
typedef uInt4               uDigit;
typedef sInt4               sDigit;
typedef	uInt2			    uHalfDigit;
typedef	sInt2			    sHalfDigit;
typedef	uInt8               uTwoDigit;
typedef	sInt8               sTwoDigit;
#endif

//------------------------------------------------------------------------------
#define     DIGIT_BITS      (sizeof (uDigit) * 8)
#define	    DIGIT_SHIFT     4
#define	    DIGIT_MASK      ((1 << DIGIT_BITS) - 1)

//------------------------------------------------------------------------------
extern  uDigit  gRadixMax[37];

//------------------------------------------------------------------------------
sInt4	    StrToDigit (cstr string, uDigit radix, uDigit &digit, uDigit &multiplier);
sInt4		DigitToStr (uDigit digit, cstr string, uDigit radix);
sInt4		Cmp (sDigit size, const uDigit *a, const uDigit *b);
uDigit      Sub (sDigit size, const uDigit *a, const uDigit *b, uDigit *result);
uDigit      SubMult (sDigit size, const uDigit *a, const uDigit *b, uDigit c, uDigit *result);
sInt4		DigitBits (uDigit a);

//------------------------------------------------------------------------------

#endif  //  _DIGIT_
