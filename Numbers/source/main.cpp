//------------------------------------------------------------------------------
//	Copyright ©1996-2004 Bretton Wade, All rights reserved.
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifndef     _PRECOMPILE_
#include    "precompile.h"
#endif  //  _PRECOMPILE_

#include    "integer template.h"
#include    "timer.h"
#include    <iostream>

//------------------------------------------------------------------------------
MAKE_INTEGER_TYPE (2048, 10);

//------------------------------------------------------------------------------
template<class T>
void    Factors (const T &a)
{
    T   x (1);
    T   y = a / x;
    while (x <= y)
    {
        if ((x * y) == a)
            std::cout << a << " / " << x << " = " << y << '\n';
        x = x + 1;
        y = a / x;
    }
     
}

//------------------------------------------------------------------------------
template<class T>
T		ExtendedEuclid (const T &a, T &b, T &x, T &y)
{
	if (b == T (0))
	{
		x = 1;
		y = 0;
		return a;
	}
	T		tmp = a % b,
			gcd = ExtendedEuclid (b, tmp, y, x);
	y -= ((a / b) * x);
	return gcd;
}

//------------------------------------------------------------------------------
void	validate (void)
{
	FILE	*fp = fopen ("mathtest.txt", "r");
	if (fp)
	{
		int		c = fgetc (fp),
					op;
		while (c != EOF)
		{
			sDigit		multiplier = 1;
			sInt2048		A (0),
								B (0),
								C (0),
								D (0);
			if (c == '-')
			{
				multiplier = -1;
				c = fgetc (fp);
			}
			while (isdigit (c))
			{
				A = (A * 10) + sDigit (c - '0');
				c = fgetc (fp);
			}
			A = A * multiplier;
			multiplier = 1;
			op = c;
			c = fgetc (fp);
			if (c == '-')
			{
				multiplier = -1;
				c = fgetc (fp);
			}
			while (isdigit (c))
			{
				B = (B * 10) + sDigit (c - '0');
				c = fgetc (fp);
			}
			B = B * multiplier;
			multiplier = 1;
			c = fgetc (fp);
			if (c == '-')
			{
				multiplier = -1;
				c = fgetc (fp);
			}
			while (isdigit (c))
			{
				C = (C * 10) + sDigit (c - '0');
				c = fgetc (fp);
			}
			C = C * multiplier;
			c = fgetc (fp);
			switch (op)
			{
				case '+':
					D = A + B;
					break;
				case '-':
					D = A - B;
					break;
				case '*':
					D = A * B;
					break;
				case '/':
					D = A / B;
					break;
				case '%':
					if (((A / B) * B) + (A % B) != A)
						std::cout << "mod failed: "<< A << " % " << B << " = " << A % B << '\n';
					D = A % B;
					break;
			}
			if (D != C)
			{
				std::cout << "Failure: " << A << " " << char (op) << " " << B << " = " << C << ", not " << D << '\n';
				std::cout << "   [RETURN] to continue\n";
				char	buffer[10];
				//gets (buffer);
			}
			else
				std::cout << "OK: " << A << " " << char (op) << " " << B << " = " << D << '\n';
		}
		fclose (fp);
		std::cout << "Done\n\n";
	}
}

//------------------------------------------------------------------------------
sInt2048	Factorial (sInt4 value)
{
	if (value > 1)
		return Factorial (value - 1) * sDigit (value);
	return sInt2048 (1);
}

//------------------------------------------------------------------------------
sInt2048	Power (sInt4 base, sInt4 power)
{
	if (power > 1)
		return Power (base, power - 1) * sDigit (base);
	return base;
}

//------------------------------------------------------------------------------
void	speedtest (void)
{
	sInt2048	A = "0xffffffffffff",
					B = "0b11001100110011",
					C = A + B,
					D = A - B,
					E = B * A,
					F = A / B,
					G = A * A,
					H = ((B - 0x3334) * 10) * (B - 0x3335);
	std::cout << A << '\n';
	std::cout << B << '\n';
	std::cout << C << '\n';
	std::cout << D << '\n';
	std::cout << E << '\n';
	std::cout << F << '\n';
	std::cout << G << '\n';
	std::cout << H << '\n';

	Timer	timer;
	timer.Start ();
	for (sInt4 i = 0; i < 10000; i++)
	{
		A + B;
		A - B;
		A * B;
		A / B;
	}
	double	elapsed = timer.Elapsed ();
	std::cout << "\n(digit) elapsed time: " << elapsed << " seconds\nops/second: " << (10000 / elapsed) << '\n';
}

//------------------------------------------------------------------------------
int	main (void)
{
    Factors (sInt2048 (1323));
    //return 0;
/*
  sInt2048  rubik = (Factorial (8) * Factorial (12) * Power (2, 12) * Power (3, 8)) / 12;
  std::cout << "(8! * 12! * (2^12) * (3^8)) / 12 = " << rubik << "\n\n";
*/

//	sInt2048	p = "41876926418769269267492267",
//					  q = "876926426876926426749214909";
	sInt2048	p = (sInt2048 (1) << 511) + 3;
	sInt2048	q = (sInt2048 (1) << 510) + 3;
   	std::cerr << "p' = " << p << '\n';
	std::cerr << "q' = " << q << '\n';
	while (sInt2048::IsComposite (2, p))
	{
		std::cerr << '.';
		p += 2;
	}
	std::cerr << '\n';

	while (sInt2048::IsComposite (2, q))
	{
		std::cerr << '.';
		q += 2;
	}
	std::cerr << '\n';

	std::cout << "p = " << p << '\n';
	std::cout << "q = " << q << '\n';

	sInt2048	n = p * q;
	sInt2048	phi = (p - 1) * (q - 1);
	sInt2048	e = 65537;
	sInt2048	d;
	sInt2048	unused;
	sInt2048	zero (0);
	
	ExtendedEuclid (e, phi, d, unused);
	
	if (d < zero)
		d += phi;
		
	std::cout << "n = " << n << '\n';
	std::cout << "phi = " << phi << '\n';
	std::cout << "e = " << e << '\n';
	std::cout << "d = " << d << '\n';
	std::cout << "testing message\n";
	sInt2048	message = "123456789123456789",
					crypt,
					decrypt;
	std::cout << "message -> " << message << '\n';

	// pad the message for security (http://en.wikipedia.org/wiki/Optimal_Asymmetric_Encryption_Padding)

	std::cout << "crypt   -> " << (crypt  = sInt2048::PowerMod (message, e, n)) << '\n';
	std::cout << "decrypt -> " << (decrypt = sInt2048::PowerMod (crypt, d, n)) << '\n';
	if (message == decrypt)
		std::cout << "SUCCESS\n";
	else
		std::cout << "FAILURE\n";

	std::cout << "\n\n\n";
	return 0;
}

//------------------------------------------------------------------------------
