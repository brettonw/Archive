//----------------------------------------------------------------------------------------------------
//	Copyright ©1996-2004 Bretton Wade, All rights reserved.
//----------------------------------------------------------------------------------------------------

#ifndef	_INTEGER_TEMPLATE_
#define	_INTEGER_TEMPLATE_

//----------------------------------------------------------------------------------------------------
#include "digit ops template.h"

//----------------------------------------------------------------------------------------------------
#define	NUM_DIGITS			(BITS / DIGIT_BITS)

//----------------------------------------------------------------------------------------------------
template<sInt4 BITS>
class	Integer
{
	public:
		/* void */	Integer (void);
		/* void */	Integer (sTwoDigit a);
		/* void */	Integer (cstr string);
		/* void */	Integer (cstr string, uDigit radix);
		/* void */	Integer (const Integer& a);
		Integer&	operator = (sTwoDigit a);
		Integer&	operator = (cstr string);
		Integer&	operator = (const Integer& a);
		Integer		operator - (void) const;
		Integer		operator + (const Integer& a) const;
		Integer		operator - (const Integer& a) const;
		Integer		operator * (const Integer& a) const;
		Integer		operator / (const Integer& a) const;
		Integer		operator % (const Integer& a) const;
		Integer		operator + (sDigit a) const;
		Integer		operator - (sDigit a) const;
		Integer		operator * (sDigit a) const;
		Integer		operator / (sDigit a) const;
		sDigit		operator % (sDigit a) const;
		Integer		operator >> (sInt4 bits) const;
		Integer		operator << (sInt4 bits) const;
		Integer&	operator += (const Integer& a);
		Integer&	operator -= (const Integer& a);
		Integer&	operator *= (const Integer& a);
		Integer&	operator /= (const Integer& a);
		Integer&	operator %= (const Integer& a);
		Integer&	operator += (sDigit a);
		Integer&	operator -= (sDigit a);
		Integer&	operator *= (sDigit a);
		Integer&	operator /= (sDigit a);
		Integer&	operator %= (sDigit a);
		Integer&	operator >>= (sInt4 bits);
		Integer&	operator <<= (sInt4 bits);
		Integer&	operator ++ (void);
		Integer&	operator -- (void);
		Integer		operator ++ (int postincrement);
		Integer		operator -- (int postdecrement);
		Integer		operator bit_and (const Integer& a) const;
		Integer		operator bit_or (const Integer& a) const;
		Integer		operator exclusive_or (const Integer& a) const;
		Integer		operator complement (void) const;
		sInt4		operator == (const Integer& a) const;
		sInt4		operator != (const Integer& a) const;
		sInt4		operator < (const Integer& a) const;
		sInt4		operator <= (const Integer& a) const;
		sInt4		operator > (const Integer& a) const;
		sInt4		operator >= (const Integer& a) const;
		/* void* */	operator void* (void) const;
		cstr		ConvertToString (uDigit radix) const;
		void		ConvertFromString (cstr string, uDigit radix);
static	Integer		PowerMod (Integer a, const Integer& b, const Integer& n);
static	sInt4		IsComposite (const Integer& a, const Integer& n);

	protected:
		/* void */	Integer (uDigit *array, uDigit negative, uDigit overflow);
	private:
		uDigit		negative:1;
		uDigit		overflow:1;
		uDigit		digit[NUM_DIGITS];
static	char		buffer[BITS + 5];
};

//----------------------------------------------------------------------------------------------------
template<sInt4 BITS>
inline	Integer<BITS>::Integer (void)
{
	overflow = FALSE;
}

//----------------------------------------------------------------------------------------------------
template<sInt4 BITS>
Integer<BITS>::Integer (sTwoDigit a)
{
	operator = (a);
}

//----------------------------------------------------------------------------------------------------
template<sInt4 BITS>
inline	Integer<BITS>::Integer (cstr string)
{
	operator = (string);
}

//----------------------------------------------------------------------------------------------------
template<sInt4 BITS>
inline	Integer<BITS>::Integer (cstr string, uDigit radix)
{
	ConvertFromString (string, radix);
}

//----------------------------------------------------------------------------------------------------
template<sInt4 BITS>
Integer<BITS>::Integer (const Integer& a)
{
	operator = (a);
}

//----------------------------------------------------------------------------------------------------
template<sInt4 BITS>
Integer<BITS>	&Integer<BITS>::operator = (sTwoDigit a)
{
	DigitOp<NUM_DIGITS / 2>::Set ((uTwoDigit *) digit, 0);
	if (a < 0)
	{
		negative = TRUE;
		a = -a;
	}
	else
		negative = FALSE;
	digit[0] = uDigit (a);
	digit[1] = uDigit (a >> DIGIT_BITS);
	overflow = FALSE;
	return *this;
}

//----------------------------------------------------------------------------------------------------
template<sInt4 BITS>
Integer<BITS>	&Integer<BITS>::operator = (cstr string)
{
	sInt4		sign = '+',
					index = 0;
	if (!isalnum (string[0]))
	{
		sign = string[0];
		index++;
	}
	if (string[index] == '0')
	{
		strncpy (buffer, string, BITS + 4);
		switch (buffer[index + 1])
		{
			case 'b':
				buffer[index + 1] = sign;
				ConvertFromString (&buffer[index + 1], 2);
				break;
			case 'x':
				buffer[index + 1] = sign;
				ConvertFromString (&buffer[index + 1], 16);
				break;
			default:
				buffer[index] = sign;
				ConvertFromString (&buffer[index], 8);
				break;
		}
	}
	else
		ConvertFromString (string, 10);
	return *this;
}

//----------------------------------------------------------------------------------------------------
template<sInt4 BITS>
Integer<BITS>	&Integer<BITS>::operator = (const Integer& a)
{
	negative = a.negative;
	overflow = a.overflow;
	DigitOp<NUM_DIGITS / 2>::Cop ((uTwoDigit *) digit, (uTwoDigit *) a.digit);
	return *this;
}

//----------------------------------------------------------------------------------------------------
template<sInt4 BITS>
Integer<BITS>	Integer<BITS>::operator - (void) const
{
	Integer	result;
	//DigitOp<NUM_DIGITS / 2>::Bit_Complement ((uTwoDigit *) digit, (uTwoDigit *) result.digit);
	//DigitOp<NUM_DIGITS>::Add_Digit (result.digit, 1, result.digit, 0);
	negative ^= 1;
	return result;
}

//----------------------------------------------------------------------------------------------------
template<sInt4 BITS>
Integer<BITS>	Integer<BITS>::operator + (const Integer& a) const
{
	Integer	result;
	result.overflow = DigitOp<NUM_DIGITS>::Add (digit, a.digit, result.digit, 0) or overflow or a.overflow;
	return result;
}

//----------------------------------------------------------------------------------------------------
template<sInt4 BITS>
Integer<BITS>	Integer<BITS>::operator - (const Integer& a) const
{
	Integer	result;
	DigitOp<NUM_DIGITS>::Sub (digit, a.digit, result.digit, 0);
	result.overflow = overflow or a.overflow;
	return result;
}

//----------------------------------------------------------------------------------------------------
template<sInt4 BITS>
Integer<BITS>	Integer<BITS>::operator * (const Integer& a) const
{
	uDigit	product[NUM_DIGITS * 2];
	DigitOp<NUM_DIGITS>::Set ((uTwoDigit *) product, 0);
	sInt4		compare = DigitOp<NUM_DIGITS / 2>::Cmp ((uTwoDigit *) digit, (uTwoDigit *) a.digit);
	if (compare >= 0)
	{
		sInt4		top = DigitOp<NUM_DIGITS>::Top (a.digit);
		for (sInt4 i = 0; i < top; i++)
			DigitOp<NUM_DIGITS>::Mul_Digit_Add (product + i, digit, a.digit[i], product + i, 0);
	}
	else
	{
		sInt4		top = DigitOp<NUM_DIGITS>::Top (digit);
		for (sInt4 i = 0; i < top; i++)
			DigitOp<NUM_DIGITS>::Mul_Digit_Add (product + i, a.digit, digit[i], product + i, 0);
	}
	uDigit	over = DigitOp<NUM_DIGITS / 2>::Tst ((uTwoDigit *) (product + NUM_DIGITS));
	Integer	result (product, negative ^ a.negative, over || overflow || a.overflow);
	if (result.negative && (!DigitOp<NUM_DIGITS>::Tst ((uTwoDigit *) digit)))
		result.negative = FALSE;
	return result;
}

//----------------------------------------------------------------------------------------------------
template<sInt4 BITS>
Integer<BITS>	Integer<BITS>::operator / (const Integer& a) const
{
	Integer	quotient (0);
	sInt4		top = DigitOp<NUM_DIGITS>::Top (a.digit);
	uDigit	remainder[NUM_DIGITS + 1],
					divisor[NUM_DIGITS],
					shift = DigitBits (a.digit[top - 1]);
	remainder[NUM_DIGITS] = DigitOp<NUM_DIGITS>::LSh (digit, shift, remainder);
	DigitOp<NUM_DIGITS>::LSh (a.digit, shift, divisor);
	uDigit	v1 = divisor[top - 1];
	for (sInt4 j = NUM_DIGITS - top; j >= 0; j--)
	{
		uDigit *uj = &remainder[j + top];
		if (v1 == DIGIT_MASK)
			quotient.digit[j] = uj[0];
		else
			quotient.digit[j] = (*(uTwoDigit *) &uj[-1]) / (v1 + 1);
		if (quotient.digit[j])
			SubMult (top + 1, remainder + j, divisor, quotient.digit[j], remainder + j);
		while (Cmp (top + 1, remainder + j, divisor) >= 0)
		{
			quotient.digit[j]++;
			Sub (top + 1, remainder + j, divisor, remainder + j);
		}
	}
	quotient.overflow = overflow || a.overflow;
	quotient.negative = negative ^ a.negative;
	if (quotient.negative && (!DigitOp<NUM_DIGITS>::Tst ((uTwoDigit *) quotient.digit)))
		quotient.negative = FALSE;
	return quotient;
}

//----------------------------------------------------------------------------------------------------
template<sInt4 BITS>
Integer<BITS>	Integer<BITS>::operator % (const Integer& a) const
{
	Integer	quotient (0);
	uDigit	remainder[NUM_DIGITS + 1],
					divisor[NUM_DIGITS],
					top = DigitOp<NUM_DIGITS>::Top (a.digit),
					shift = DigitBits (a.digit[top - 1]);
	remainder[NUM_DIGITS] = DigitOp<NUM_DIGITS>::LSh (digit, shift, remainder);
	DigitOp<NUM_DIGITS>::LSh (a.digit, shift, divisor);
	uDigit	v1 = divisor[top - 1];
	for (sInt4 j = NUM_DIGITS - top; j >= 0; j--)
	{
		uDigit *uj = &remainder[j + top];
		if (v1 == DIGIT_MASK)
			quotient.digit[j] = uj[0];
		else
			quotient.digit[j] = (*(uTwoDigit *) &uj[-1]) / (v1 + 1);
		if (quotient.digit[j])
			SubMult (top + 1, remainder + j, divisor, quotient.digit[j], remainder + j);
		while (Cmp (top + 1, remainder + j, divisor) >= 0)
		{
			quotient.digit[j]++;
			Sub (top + 1, remainder + j, divisor, remainder + j);
		}
	}
	DigitOp<NUM_DIGITS + 1>::RSh (remainder, shift, remainder);
	Integer	residue (remainder, negative, overflow || a.overflow);
	if (residue.negative && (!DigitOp<NUM_DIGITS>::Tst ((uTwoDigit *) residue.digit)))
		residue.negative = FALSE;
	return residue;
}

//----------------------------------------------------------------------------------------------------
template<sInt4 BITS>
Integer<BITS>	Integer<BITS>::operator + (sDigit a) const
{
	Integer	result;
	uDigit	anegative = (a < 0);
	if (anegative)
		a = -a;
	if (negative == anegative)
	{
		result.overflow = DigitOp<NUM_DIGITS>::Add_Digit (digit, a, result.digit, 0);
		result.negative = negative;
	}
	else
	{
		sInt4	compare = DigitOp<NUM_DIGITS / 2>::Cmp_Digit (digit, a);
		if (compare == 0)
		{
			DigitOp<NUM_DIGITS / 2>::Set ((uTwoDigit *) result.digit, 0);
			result.negative = FALSE;
		}
		else if (compare > 0)
		{
			DigitOp<NUM_DIGITS>::Sub_Digit (digit, a, result.digit, 0);
			result.negative = negative;
		}
		else
		{
			uDigit	array[NUM_DIGITS];
			DigitOp<NUM_DIGITS / 2>::Set ((uTwoDigit *) array, 0);
			array[0] = a;
			DigitOp<NUM_DIGITS>::Sub (array, digit, result.digit, 0);
			result.negative = anegative;
		}
	}
	result.overflow = result.overflow || overflow;
	return result;
}

//----------------------------------------------------------------------------------------------------
template<sInt4 BITS>
Integer<BITS>	Integer<BITS>::operator - (sDigit a) const
{
	Integer	result;
	uDigit	anegative = (a < 0);
	if (anegative)
		a = -a;
	if (negative != anegative)
	{
		result.overflow = DigitOp<NUM_DIGITS>::Add_Digit (digit, a, result.digit, 0);
		result.negative = negative;
	}
	else
	{
		sInt4	compare = DigitOp<NUM_DIGITS / 2>::Cmp_Digit (digit, a);
		if (compare == 0)
		{
			DigitOp<NUM_DIGITS / 2>::Set ((uTwoDigit *) result.digit, 0);
			result.negative = FALSE;
		}
		else if (compare > 0)
		{
			DigitOp<NUM_DIGITS>::Sub_Digit (digit, a, result.digit, 0);
			result.negative = negative;
		}
		else
		{
			uDigit	array[NUM_DIGITS];
			DigitOp<NUM_DIGITS / 2>::Set ((uTwoDigit *) array, 0);
			array[0] = a;
			DigitOp<NUM_DIGITS>::Sub (array, digit, result.digit, 0);
			result.negative = !negative;
		}
	}
	result.overflow = overflow;
	return result;
}

//----------------------------------------------------------------------------------------------------
template<sInt4 BITS>
Integer<BITS>	Integer<BITS>::operator * (sDigit a) const
{
	Integer	result;
	uDigit	anegative = (a < 0);
	if (anegative)
		a = -a;
	result.overflow = DigitOp<NUM_DIGITS>::Mul_Digit (digit, a, result.digit, 0) || overflow;
	result.negative = negative ^ anegative;
	if (result.negative && (!DigitOp<NUM_DIGITS>::Tst ((uTwoDigit *) digit)))
		result.negative = FALSE;
	return result;
}

//----------------------------------------------------------------------------------------------------
template<sInt4 BITS>
Integer<BITS>	Integer<BITS>::operator / (sDigit a) const
{
	Integer	result;
	uDigit	anegative = (a < 0);
	if (anegative)
		a = -a;
	DigitOp<NUM_DIGITS>::Div_Digit (digit, a, result.digit, 0);
	result.overflow = overflow;
	result.negative = negative ^ anegative;
	if (result.negative && (!DigitOp<NUM_DIGITS>::Tst ((uTwoDigit *) digit)))
		result.negative = FALSE;
	return result;
}

//----------------------------------------------------------------------------------------------------
template<sInt4 BITS>
sDigit	Integer<BITS>::operator % (sDigit a) const
{
	uDigit	quotient[NUM_DIGITS];
	uDigit	anegative = (a < 0);
	if (anegative)
		a = -a;
	return DigitOp<NUM_DIGITS>::Div_Digit (digit, a, quotient, 0) * (negative ? -1 : 1);
}

//----------------------------------------------------------------------------------------------------
template<sInt4 BITS>
Integer<BITS>	Integer<BITS>::operator >> (sInt4 bits) const
{
	if (bits == 0)
		return Integer (*this);
	if (bits > 0)
	{
		Integer	result = *this;
		while (bits >= DIGIT_BITS)
		{
			DigitOp<NUM_DIGITS>::RSh (result.digit, result.digit);
			bits -= DIGIT_BITS;
		}
		if (bits)
			DigitOp<NUM_DIGITS>::RSh (result.digit, bits, result.digit);
		return result;
	}
	return operator << (-bits);
}

//----------------------------------------------------------------------------------------------------
template<sInt4 BITS>
Integer<BITS>	Integer<BITS>::operator << (sInt4 bits) const
{
	if (bits == 0)
		return Integer (*this);
	if (bits > 0)
	{
		Integer	result = *this;
		while (bits >= DIGIT_BITS)
		{
			DigitOp<NUM_DIGITS>::LSh (result.digit, result.digit);
			bits -= DIGIT_BITS;
		}
		if (bits)
			DigitOp<NUM_DIGITS>::LSh (result.digit, bits, result.digit);
		return result;
	}
	return operator >> (-bits);
}

//----------------------------------------------------------------------------------------------------
template<sInt4 BITS>
Integer<BITS>	&Integer<BITS>::operator += (const Integer& a)
{
	(*this) = (*this) + a;
	return *this;
}

//----------------------------------------------------------------------------------------------------
template<sInt4 BITS>
Integer<BITS>	&Integer<BITS>::operator -= (const Integer& a)
{
	(*this) = (*this) - a;
	return *this;
}

//----------------------------------------------------------------------------------------------------
template<sInt4 BITS>
Integer<BITS>	&Integer<BITS>::operator *= (const Integer& a)
{
	(*this) = (*this) * a;
	return *this;
}

//----------------------------------------------------------------------------------------------------
template<sInt4 BITS>
Integer<BITS>	&Integer<BITS>::operator /= (const Integer& a)
{
	(*this) = (*this) / a;
	return *this;
}

//----------------------------------------------------------------------------------------------------
template<sInt4 BITS>
Integer<BITS>	&Integer<BITS>::operator %= (const Integer& a)
{
	(*this) = (*this) % a;
	return *this;
}

//----------------------------------------------------------------------------------------------------
template<sInt4 BITS>
Integer<BITS>	&Integer<BITS>::operator += (sDigit a)
{
	(*this) = (*this) + a;
	return *this;
}

//----------------------------------------------------------------------------------------------------
template<sInt4 BITS>
Integer<BITS>	&Integer<BITS>::operator -= (sDigit a)
{
	(*this) = (*this) - a;
	return *this;
}

//----------------------------------------------------------------------------------------------------
template<sInt4 BITS>
Integer<BITS>	&Integer<BITS>::operator *= (sDigit a)
{
	(*this) = (*this) * a;
	return *this;
}

//----------------------------------------------------------------------------------------------------
template<sInt4 BITS>
Integer<BITS>	&Integer<BITS>::operator /= (sDigit a)
{
	(*this) = (*this) / a;
	return *this;
}

//----------------------------------------------------------------------------------------------------
template<sInt4 BITS>
Integer<BITS>	&Integer<BITS>::operator %= (sDigit a)
{
	(*this) = (*this) % a;
	return *this;
}

//----------------------------------------------------------------------------------------------------
template<sInt4 BITS>
Integer<BITS>	&Integer<BITS>::operator >>= (sInt4 bits)
{
	(*this) = (*this) >> bits;
	return *this;
}

//----------------------------------------------------------------------------------------------------
template<sInt4 BITS>
Integer<BITS>	&Integer<BITS>::operator <<= (sInt4 bits)
{
	(*this) = (*this) << bits;
	return *this;
}

//----------------------------------------------------------------------------------------------------
template<sInt4 BITS>
inline	Integer<BITS>	&Integer<BITS>::operator ++ (void)
{
	return operator += (1);
}

//----------------------------------------------------------------------------------------------------
template<sInt4 BITS>
inline	Integer<BITS>	&Integer<BITS>::operator -- (void)
{
	return operator -= (1);
}

//----------------------------------------------------------------------------------------------------
template<sInt4 BITS>
Integer<BITS>	Integer<BITS>::operator ++ (int postincrement)
{
	Integer	temp (*this);
	(*this) = (*this) + 1;
	return temp;
}

//----------------------------------------------------------------------------------------------------
template<sInt4 BITS>
Integer<BITS>	Integer<BITS>::operator -- (int postdecrement)
{
	Integer	temp (*this);
	(*this) = (*this) - 1;
	return temp;
}

//----------------------------------------------------------------------------------------------------
template<sInt4 BITS>
Integer<BITS>	Integer<BITS>::operator bit_and (const Integer& a) const
{
	Integer	result;
	result.negative = negative;
	DigitOp<NUM_DIGITS / 2>::Bit_And ((uTwoDigit *) digit, (uTwoDigit *) a.digit, (uTwoDigit *) result.digit);
	return result;
}

//----------------------------------------------------------------------------------------------------
template<sInt4 BITS>
Integer<BITS>	Integer<BITS>::operator bit_or (const Integer& a) const
{
	Integer	result;
	result.negative = negative;
	DigitOp<NUM_DIGITS / 2>::Bit_Or ((uTwoDigit *) digit, (uTwoDigit *) a.digit, (uTwoDigit *) result.digit);
	return result;
}

//----------------------------------------------------------------------------------------------------
template<sInt4 BITS>
Integer<BITS>	Integer<BITS>::operator exclusive_or (const Integer& a) const
{
	Integer	result;
	result.negative = negative;
	DigitOp<NUM_DIGITS / 2>::Bit_Xor ((uTwoDigit *) digit, (uTwoDigit *) a.digit, (uTwoDigit *) result.digit);
	return result;
}

//----------------------------------------------------------------------------------------------------
template<sInt4 BITS>
Integer<BITS>	Integer<BITS>::operator complement (void) const
{
	Integer	result;
	result.negative = negative;
	DigitOp<NUM_DIGITS / 2>::Bit_Complement ((uTwoDigit *) digit, (uTwoDigit *) result.digit);
	return result;
}

//----------------------------------------------------------------------------------------------------
template<sInt4 BITS>
sInt4	Integer<BITS>::operator == (const Integer& a) const
{
	if (negative == a.negative)
		return (DigitOp<NUM_DIGITS / 2>::Cmp ((uTwoDigit *) digit, (uTwoDigit *) a.digit) == 0);
	return FALSE;
}

//----------------------------------------------------------------------------------------------------
template<sInt4 BITS>
sInt4	Integer<BITS>::operator != (const Integer& a) const
{
	if (negative == a.negative)
		return (DigitOp<NUM_DIGITS / 2>::Cmp ((uTwoDigit *) digit, (uTwoDigit *) a.digit) != 0);
	return TRUE;
}

//----------------------------------------------------------------------------------------------------
template<sInt4 BITS>
sInt4	Integer<BITS>::operator < (const Integer& a) const
{
	sInt4	cmp = DigitOp<NUM_DIGITS / 2>::Cmp ((uTwoDigit *) digit, (uTwoDigit *) a.digit);
	if (negative)
		if (a.negative)
			return (cmp > 0);
		else
			return TRUE;
	else
		if (a.negative)
			return FALSE;
		else
			return (cmp < 0);
}

//----------------------------------------------------------------------------------------------------
template<sInt4 BITS>
sInt4		Integer<BITS>::operator <= (const Integer& a) const
{
	sInt4	cmp = DigitOp<NUM_DIGITS / 2>::Cmp ((uTwoDigit *) digit, (uTwoDigit *) a.digit);
	if (negative)
		if (a.negative)
			return (cmp >= 0);
		else
			return TRUE;
	else
		if (a.negative)
			return FALSE;
		else
			return (cmp <= 0);
}

//----------------------------------------------------------------------------------------------------
template<sInt4 BITS>
sInt4	Integer<BITS>::operator > (const Integer& a) const
{
	sInt4	cmp = DigitOp<NUM_DIGITS / 2>::Cmp ((uTwoDigit *) digit, (uTwoDigit *) a.digit);
	if (negative)
		if (a.negative)
			return (cmp < 0);
		else
			return FALSE;
	else
		if (a.negative)
			return TRUE;
		else
			return (cmp > 0);
}

//----------------------------------------------------------------------------------------------------
template<sInt4 BITS>
sInt4	Integer<BITS>::operator >= (const Integer& a) const
{
	sInt4	cmp = DigitOp<NUM_DIGITS / 2>::Cmp ((uTwoDigit *) digit, (uTwoDigit *) a.digit);
	if (negative)
		if (a.negative)
			return (cmp <= 0);
		else
			return FALSE;
	else
		if (a.negative)
			return TRUE;
		else
			return (cmp >= 0);
}

//----------------------------------------------------------------------------------------------------
template<sInt4 BITS>
Integer<BITS>::operator void* (void) const
{
	return DigitOp<NUM_DIGITS / 2>::Tst ((uTwoDigit *) digit) ? (void *) TRUE : (void *) FALSE;
}

//----------------------------------------------------------------------------------------------------
template<sInt4 BITS>
cstr	Integer<BITS>::ConvertToString (uDigit radix) const
{
	cstr		string = buffer + BITS + 3;
	if (overflow)
		*string-- = '?';
	uDigit	copy[NUM_DIGITS];
	DigitOp<NUM_DIGITS / 2>::Cop ((uTwoDigit *) copy, (uTwoDigit *) digit);
	do
	{
		uDigit	remainder = DigitOp<NUM_DIGITS>::Div_Digit (copy, gRadixMax[radix], copy, 0);
		string += DigitToStr (remainder, string, radix);
	}
	while (DigitOp<NUM_DIGITS / 2>::Tst ((uTwoDigit *) copy));
	while ((string[1] == '0') && string[2])
		string++;
	switch (radix)
	{
		case 2:
			*string-- = 'b';
			*string-- = '0';
			break;
		case 8:
			*string-- = '0';
			break;
		case 16:
			*string-- = 'x';
			*string-- = '0';
			break;
	}
	*string = negative ? '-' : '+';
	return string;
}

//----------------------------------------------------------------------------------------------------
template<sInt4 BITS>
void	Integer<BITS>::ConvertFromString (cstr string, uDigit radix)
{
	DigitOp<NUM_DIGITS / 2>::Set ((uTwoDigit *) digit, 0);
	overflow = FALSE;
	if (!isalnum (*string))
		negative = (*string++ == '-');
	else
		negative = FALSE;
	while (isalnum (*string))
	{
		uDigit	dig,
						mult;
		string += StrToDigit (string, radix, dig, mult);
		if (DigitOp<NUM_DIGITS>::Mul_Digit (digit, mult, digit, 0))
			overflow = TRUE;
		if (DigitOp<NUM_DIGITS>::Add_Digit (digit, dig, digit, 0))
			overflow = TRUE;
	}
	if (DigitOp<NUM_DIGITS / 2>::Tst ((uTwoDigit *) digit) == 0)
		negative = FALSE;
}

//----------------------------------------------------------------------------------------------------
template<sInt4 BITS>
Integer<BITS>	Integer<BITS>::PowerMod (Integer a, const Integer& b, const Integer& n)
{
	sInt4		top = DigitOp<NUM_DIGITS>::Top (b.digit),
					bits = (top * DIGIT_BITS) - DigitBits (b.digit[top - 1]);
	Integer	result = 1;
	for (sInt4 i = 0; i < bits; i++)
	{
		if ((b.digit[i >> DIGIT_SHIFT] >> (i & (DIGIT_BITS - 1))) & 0x01)
			result = (result * a) % n;
		a = (a * a) % n;
	}
	return result;
}

//----------------------------------------------------------------------------------------------------
template<sInt4 BITS>
sInt4	Integer<BITS>::IsComposite (const Integer& a, const Integer& n)
{
	Integer	b = n - 1,
					d = 1;
	sInt4		top = DigitOp<NUM_DIGITS>::Top (b.digit),
					bits = (top * DIGIT_BITS) - DigitBits (b.digit[top - 1]);
	for (sInt4 i = bits - 1; i >= 0; i--)
	{
		Integer	x = d;
		d = (d * d) % n;
		if ((d == 1) && (x != 1) && (x != b))
			return TRUE;
		if ((b.digit[i >> DIGIT_SHIFT] >> (i & (DIGIT_BITS - 1))) & 0x01)
			d = (d * a) % n;
	}
	if (d != 1)
		return TRUE;
	return FALSE;
}

//----------------------------------------------------------------------------------------------------
template<sInt4 BITS>
Integer<BITS>::Integer (uDigit *array, uDigit neg, uDigit over)
{
	DigitOp<NUM_DIGITS / 2>::Cop ((uTwoDigit *) digit, (uTwoDigit *) array);
	negative = neg;
	overflow = over;
}

//----------------------------------------------------------------------------------------------------
#undef	NUM_DIGITS

//----------------------------------------------------------------------------------------------------
template<class T>
inline	T	operator + (sDigit a, const T &b)
{
	return b + a;
}

//----------------------------------------------------------------------------------------------------
template<class T>
inline	T	operator - (sDigit a, const T &b)
{
	return T (a) - b;
}

//----------------------------------------------------------------------------------------------------
template<class T>
inline	T	operator * (sDigit a, const T &b)
{
	return b * a;
}

//----------------------------------------------------------------------------------------------------
template<class T>
inline	T	operator / (sDigit a, const T &b)
{
	return T (a) / b;
}

//----------------------------------------------------------------------------------------------------
template<class T>
inline	T	operator % (sDigit a, const T &b)
{
	return T (a) % b;
}

//----------------------------------------------------------------------------------------------------
#define	MAKE_INTEGER_TYPE(size, default_print_radix)													\
typedef	Integer<size>	sInt ## size;																	\
char	sInt ## size::buffer[size + 5];																	\
std::ostream	&operator << (std::ostream &s, const Integer<size> &a)									\
{																										\
	return s << (a.ConvertToString (default_print_radix));												\
}

//----------------------------------------------------------------------------------------------------

#endif	// _INTEGER_TEMPLATE_
