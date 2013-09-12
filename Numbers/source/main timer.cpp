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
#include "digit template.h"
#include "digit.h"
#include "global.h"
#include "rsaref.h"
#include "nn.h"
#include "timer.h"

//----------------------------------------------------------------------------------------------------
void	main (void)
{
	Integer<128>	i_a ("ffffffffffff", 16),
								i_b ("1", 16),
								i_c = i_a + i_b,
								i_d = i_a - i_b,
								i_e = i_a * 2,
								i_f = i_a / 2,
								i_g = i_a * i_b,
								i_h = i_a / i_b;
	i_a.Print (16);
	i_b.Print (16);
	i_c.Print (16);
	i_d.Print (16);
	i_e.Print (16);
	i_f.Print (16);
	i_g.Print (16);
	i_h.Print (16);
	
	NN_DIGIT			nn_a[2] = {0xffffffff, 0x0000ffff},
								nn_b[2] = {0x00000001, 0x00000000},
								nn_c[2],
								nn_d[2],
								nn_g[2],
								nn_h[2],
								nn_i[2];
	NN_Add (nn_c, nn_a, nn_b, 2);
	NN_Sub (nn_d, nn_a, nn_b, 2);
	NN_Mult (nn_g, nn_a, nn_b, 2);
	NN_Div (nn_h, nn_i, nn_a, 2, nn_b, 2);
	printf ("NNADD %08x%08x\n", nn_c[1], nn_c[0]);
	printf ("NNSUB %08x%08x\n", nn_d[1], nn_d[0]);
	printf ("NNMUL %08x%08x\n", nn_g[1], nn_g[0]);
	printf ("NNDIV %08x%08x\n", nn_h[1], nn_h[0]);
	printf ("NNMOD %08x%08x\n", nn_i[1], nn_i[0]);

	Digit					a[4] = {0xffff, 0xffff, 0xffff, 0x0000},
								b[4] = {0x0001, 0x0000, 0x0000};
	DigitArray<4>	A (a),
								B (b),
								C = A + B,
								D = A - B,
								E = A * 2,
								F = A / 2,
								G = B * A,
								H = A / B;
	A.PrintHex ();
	B.PrintHex ();
	C.PrintHex ();
	D.PrintHex ();
	E.PrintHex ();
	F.PrintHex ();
	G.PrintHex ();
	H.PrintHex ();

	Timer	timer;
	timer.Start ();
	for (sInt4 i = 0; i < 100000; i++)
	{
		A + B;
		A - B;
		A * B;
//		A / B;
	}
	double	elapsed = timer.Elapsed ();
	printf ("(digit) elapsed time: %f seconds\nops/second: %f\n\n", elapsed, 100000 / elapsed);

	timer.Start ();
	for (i = 0; i < 100000; i++)
	{
		i_a + i_b;
		i_a - i_b;
		i_a * i_b;
//		i_a / i_b;
	}
	elapsed = timer.Elapsed ();
	printf ("(int64) elapsed time: %f seconds\nops/second: %f\n\n", elapsed, 100000 / elapsed);

	timer.Start ();
	for (i = 0; i < 100000; i++)
	{
		NN_Add (nn_c, nn_a, nn_b, 2);
		NN_Sub (nn_d, nn_a, nn_b, 2);
		NN_Mult (nn_g, nn_a, nn_b, 2);
//		NN_Div (nn_h, nn_i, nn_a, 2, nn_b, 2);
	}
	elapsed = timer.Elapsed ();
	printf ("(NN2) elapsed time: %f seconds\nops/second: %f\n", elapsed, 100000 / elapsed);
}

//----------------------------------------------------------------------------------------------------
