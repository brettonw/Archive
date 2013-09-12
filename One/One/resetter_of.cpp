//------------------------------------------------------------------------------
//	Copyright (C) 1997-2000 Bretton Wade
//	All Rights Reserved
//------------------------------------------------------------------------------

//--------------------------------------------------------------------------
//	includes
//--------------------------------------------------------------------------
#include	"precompile.h"
#include	"resetter_of.h"
#include	"test.h"

//--------------------------------------------------------------------------
//	tests
//--------------------------------------------------------------------------
#ifdef	_RUN_TESTS
DEFINE_TEST (ResetterOf)
{
}

//--------------------------------------------------------------------------
ON_TEST (ResetterOf)
{
	DO_TEST_REPORT;
	cString				originalText = "Hello World\n",
						newText = "Goodbye Cruel World\n";
	Text				text (originalText);
	{
		ResetterOf<Text>	junk (text);
		TEST_CONDITION (text == originalText);
		text = newText;
		TEST_CONDITION (text == newText);
	}
	TEST_CONDITION (text == originalText);
	{
		ResetterOf<Text>	junk (text);
		TEST_CONDITION (text == originalText);
		text = newText;
		TEST_CONDITION (text == newText);
		junk.Cancel ();
	}
	TEST_CONDITION (text == newText);
}
#endif

//--------------------------------------------------------------------------
