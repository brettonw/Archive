//------------------------------------------------------------------------------
//	Copyright (C) 1997-2000 Bretton Wade
//	All Rights Reserved
//------------------------------------------------------------------------------

//--------------------------------------------------------------------------
//	includes
//--------------------------------------------------------------------------
#include	"precompile.h"
#include	"stack_of.h"
#include	"test.h"

//--------------------------------------------------------------------------
//	tests
//--------------------------------------------------------------------------
#ifdef	_RUN_TESTS
DEFINE_TEST (StackOf)
{
	DO_AFTER (ResizableVectorOf);
}

//--------------------------------------------------------------------------
ON_TEST (StackOf)
{
	DO_TEST_REPORT;

	StackOf<uInt>	stack;

	TEST_CONDITION (stack.GetSize () == 0);
	stack.Push (1);
	TEST_CONDITION (stack.GetSize () == 1);
	stack.Push (2);
	TEST_CONDITION (stack.GetSize () == 2);
	stack.Push (3);
	TEST_CONDITION (stack.GetSize () == 3);
	stack.Push (4);
	TEST_CONDITION (stack.GetSize () == 4);
	stack.Push (5);
	TEST_CONDITION (stack.GetSize () == 5);

	TEST_CONDITION (stack.Get (0) == 5);
	TEST_CONDITION (stack.GetSize () == 5);
	TEST_CONDITION (stack.Get (1) == 4);
	TEST_CONDITION (stack.GetSize () == 5);
	TEST_CONDITION (stack.Get (2) == 3);
	TEST_CONDITION (stack.GetSize () == 5);
	TEST_CONDITION (stack.Get (3) == 2);
	TEST_CONDITION (stack.GetSize () == 5);
	TEST_CONDITION (stack.Get (4) == 1);
	TEST_CONDITION (stack.GetSize () == 5);
	try
	{
		TEST_CONDITION (stack.Get (5) == 42);
	}
	catch (...)
	{
		TEST_CONDITION ("Catch expected exception");
	}

	TEST_CONDITION (stack.Pop () == 5);
	TEST_CONDITION (stack.GetSize () == 4);
	TEST_CONDITION (stack.Pop () == 4);
	TEST_CONDITION (stack.GetSize () == 3);
	TEST_CONDITION (stack.Pop () == 3);
	TEST_CONDITION (stack.GetSize () == 2);
	TEST_CONDITION (stack.Pop () == 2);
	TEST_CONDITION (stack.GetSize () == 1);
	TEST_CONDITION (stack.Pop () == 1);
	TEST_CONDITION (stack.GetSize () == 0);
	try
	{
		TEST_CONDITION (stack.Pop () == 42);
	}
	catch (...)
	{
		TEST_CONDITION ("Catch expected exception");
	}

}
#endif

//--------------------------------------------------------------------------
