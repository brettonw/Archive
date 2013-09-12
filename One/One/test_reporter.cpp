//------------------------------------------------------------------------------
//	Copyright (C) 1997-2000 Bretton Wade
//	All Rights Reserved
//------------------------------------------------------------------------------

//--------------------------------------------------------------------------
//	includes
//--------------------------------------------------------------------------
#include	"precompile.h"
#include	"test_reporter.h"

//--------------------------------------------------------------------------
//	class methods
//--------------------------------------------------------------------------
TestReporter::TestReporter
(
	const Text&		name,
	bool			verbose
)
{
	myVerbose = verbose;
	cerr << "Testing " << name << " ... ";
}

//--------------------------------------------------------------------------
TestReporter::~TestReporter
(
	void
)
{
	if (myVerbose)
		cerr << endl;
	cerr << "OK." << endl;
	if (myVerbose)
		cerr << endl;
}

//--------------------------------------------------------------------------
void
TestReporter::Fail
(
	const Text&		test
)
{
	cerr << endl << "  Failed at (" << test << ").";
	#ifdef _DEBUG
	_CrtDbgBreak ();
	#endif
	exit (1);
}

//--------------------------------------------------------------------------
void
TestReporter::Succeed
(
	const Text&		test
)
{
	cerr << endl << "  Succeeded at (" << test << ").";
}

//--------------------------------------------------------------------------
