//----------------------------------------------------------------------------------------------------

//	Copyright ©1996 qualia, inc. All rights reserved.

//

//----------------------------------------------------------------------------------------------------



//----------------------------------------------------------------------------------------------------

#ifdef	_MSC_VER

#include "precompile.h"

#endif



//----------------------------------------------------------------------------------------------------

#include "integer template.h"

#include "timer.h"



#define	intbits	512

typedef	Integer<intbits>	Int;

//----------------------------------------------------------------------------------------------------

Int	Factorial (int n)

{

	Int		factorial (1);

	for (; n > 1; n--)

		factorial = factorial * n;

	return factorial;

}



//----------------------------------------------------------------------------------------------------

Int		Euclid (const Int &a, const Int &b)

{

	if (b == 0)

		return a;

	return Euclid (b, a % b);

}



//----------------------------------------------------------------------------------------------------

Int		ExtendedEuclid (const Int &a, const Int &b, Int &x, Int &y)

{

	if (b == 0)

	{

		x = 1;

		y = 0;

		return a;

	}

	Int		gcd = ExtendedEuclid (b, a % b, y, x);

	y = y - ((a / b) * x);

	return gcd;

}



//----------------------------------------------------------------------------------------------------

Int		ModularSolver (const Int &a, const Int &b, const Int &n)

{

	Int		x, y,

				d = ExtendedEuclid (a, n, x, y);

	if (b % d == 0)

		return (x * (b / d)) % n;

	return Int (0);

}



//----------------------------------------------------------------------------------------------------

Int		ModularExponentiation (const Int &a, const Int &b, const Int &n)

{

	Int		d (1);

	for (int i = intbits - 1; i >= 0; i--)

	{

		d = (d * d) % n;

		if ((b[i >> DIGIT_SHIFT] >> (i & DIGIT_MASK)) & 0x01)

			d = (d * a) % n;

	}

	return d;

}



//----------------------------------------------------------------------------------------------------

Bool	PrimeTest (int n)

{

	if ((n & 0x01) == 0)

		return FALSE;

	int		max = int (sqrt (double (n))) + 1;

	for (int i = 3; i < max; i++)

		if ((n % i) == 0)

			return FALSE;

	return TRUE;

}



//----------------------------------------------------------------------------------------------------

Digit	primes[64];

void	BuildPrimes (void)

{

	primes[0] = 2;

	for (sInt2 i = 1; i < 64; i++)

	{

		primes[i] = primes[i - 1] + 1;

		while (!PrimeTest (primes[i]))

			primes[i]++;

	}

}



//----------------------------------------------------------------------------------------------------

Bool	Witness (const Int &a, const Int &n)

{

	Int		d (1),

				b (n - 1);

	for (int i = intbits - 1; i >= 0; i--)

	{

		Int		x = d;

		d = (d * d) % n;

		if ((d == 1) && (x != 1) && (x != b))

			return TRUE;

		if ((b[i >> DIGIT_SHIFT] >> (i & DIGIT_MASK)) & 0x01)

			d = (d * a) % n;

	}

	if (d != 1)

		return TRUE;

	return FALSE;

}



//----------------------------------------------------------------------------------------------------

Bool	MillerRabin (const Int &n, int s)

{

	putchar ('.');

	for (int i = 0; i < s; i++)

	{

		if (Witness (primes[i], n))

			return FALSE;

		putchar ('+');

	}

	return TRUE;

}



//----------------------------------------------------------------------------------------------------

void	main (void)

{

	BuildPrimes ();

	Int		p ("1157920892373161954235709850086887", 10),

				q ("9846656405640394575840079131296439", 10);

	while (!MillerRabin (p, 10))

		p = p + 2;

	putchar ('\n');

	while (!MillerRabin (q, 10))

		q = q + 2;

	putchar ('\n');



	Int		n = p * q,

				phi = (p - 1) * (q - 1),

				e = phi % (p + q),	//	e randomly chosen..., just don't pick e = (phi / 2) - 1

				d,

				unused;

	if ((e & 0x01) == 0)

	{

		putchar ('*');

		e = e - 1;

	}

	for (; e > 0; e = e - 2)

	{

		putchar ('.');

		if (ExtendedEuclid (e, phi, d, unused) == 1)

		{

			putchar ('+');

			if (d > 0)

				break;

		}

	}

	printf ("\np =       "); p.Print ();

	printf ("q =       "); q.Print ();

	printf ("\nn =       "); n.Print ();

	printf ("e =       "); e.Print ();

	printf ("d =       "); d.Print ();



	printf ("\ntesting message\n");

	Int		message ("12345678901234567890123456789012345", 10),

				crypt  = ModularExponentiation (message, e, n),

				decrypt = ModularExponentiation (crypt, d, n);

	printf ("message -> "); message.Print (); 

	printf ("crypt   -> "); crypt.Print ();

	printf ("decrypt -> "); decrypt.Print ();

	if (message == decrypt)

		printf ("SUCCESS\n");

	else

		printf ("FAILURE\n");

}



//----------------------------------------------------------------------------------------------------

