//------------------------------------------------------------------------------
//	Copyright (C) 1997-2000 Bretton Wade
//	All Rights Reserved
//------------------------------------------------------------------------------

//--------------------------------------------------------------------------
//	includes
//--------------------------------------------------------------------------
#include	"precompile.h"
#include	"bits.h"
#include	"task.h"
#include	"test.h"

//--------------------------------------------------------------------------
//	global variables
//--------------------------------------------------------------------------
sPointer	gBitCount = 0;

//--------------------------------------------------------------------------
//	tasks
//--------------------------------------------------------------------------
DEFINE_TASK (BitCountTable)
{
    DO_AFTER (StdErr);
}

//--------------------------------------------------------------------------
ON_STARTUP (BitCountTable)
{
	gBitCount = new sInt1[256];
	for (sInt i = 0; i < 256; i++)
	{
		gBitCount[i] = 0;
		for (sInt bits = i; bits != 0; gBitCount[i]++)
			bits &= bits - 1;
	}
}

//--------------------------------------------------------------------------
ON_SHUTDOWN (BitCountTable)
{
	delete[] gBitCount;
}

//--------------------------------------------------------------------------
//	functions
//--------------------------------------------------------------------------
sInt
CountBits
(
	sInt4				value
)
{
	assert (gBitCount);
	uInt1*	ptr = (uInt1*) &value;
	return gBitCount[ptr[0]] + gBitCount[ptr[1]] + gBitCount[ptr[2]] + gBitCount[ptr[3]];
}

//--------------------------------------------------------------------------
sInt
HighestBitPosition
(
	sInt4				value
)
{
	sInt	position = 0;
	while (value)
	{
		value >>= 1;
		position++;
	}
	return position;
}

//--------------------------------------------------------------------------
sInt4
HighestBit
(
	sInt4				value
)
{
	return 1 << (HighestBitPosition (value) - 1);
}

//--------------------------------------------------------------------------
sInt
LowestBitPosition
(
	sInt4				value
)
{
	return HighestBitPosition (LowestBit (value));
}

//--------------------------------------------------------------------------
sInt4
LowestBit
(
	sInt4				value
)
{
	return value bitand -value;
}

//--------------------------------------------------------------------------
bool
IsPowerOf2
(
	sInt4				value
)
{
	return bool (value == LowestBit (value));
}

//--------------------------------------------------------------------------
void
PrintBits
(
	uInt4				value
)
{
	for (uInt j = 0; j < 3; j++)
	{
		for (uInt i = 0; i < 8; i++)
		{
			cerr << ((value bitand 0x80000000) ? "1" : "0");
			value <<= 1;
		}
		cerr << ".";
	}
	for (uInt i = 0; i < 8; i++)
	{
		cerr << ((value bitand 0x80000000) ? "1" : "0");
		value <<= 1;
	}
}

//--------------------------------------------------------------------------
//	tests
//--------------------------------------------------------------------------
#ifdef	_RUN_TESTS
DEFINE_TEST (Bits)
{
}

//--------------------------------------------------------------------------
ON_TEST (Bits)
{
	DO_TEST_REPORT;
	TEST_CONDITION (CountBits (0) == 0);
	for (sInt i = 0; i < 32; i++)
		TEST_CONDITION (CountBits (1 << i) == 1);

	for (i = 0; i < 20; i++)
		TEST_CONDITION (CountBits (7 << i) == 3);

	for (i = 0; i < 32; i++)
		TEST_CONDITION (CountBits ((1 << i) - 1) == i);

	for (i = 0; i < 28; i++)
		TEST_CONDITION (HighestBitPosition (7 << i) == (i + 3));

	for (i = 0; i < 28; i++)
		TEST_CONDITION (HighestBit (7 << i) == (4 << i));

	for (i = 0; i < 28; i++)
		TEST_CONDITION (LowestBitPosition (7 << i) == (i + 1));

	for (i = 0; i < 28; i++)
		TEST_CONDITION (LowestBit (7 << i) == (1 << i));

	for (i = 0; i < 28; i++)
		TEST_CONDITION (not IsPowerOf2 (7 << i));

	for (i = 0; i < 28; i++)
		TEST_CONDITION (IsPowerOf2 (1 << i));
}
#endif

//--------------------------------------------------------------------------
