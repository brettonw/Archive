//------------------------------------------------------------------------------
//	Copyright (C) 1997-2000 Bretton Wade
//	All Rights Reserved
//------------------------------------------------------------------------------

//--------------------------------------------------------------------------
//	includes
//--------------------------------------------------------------------------
#include	"precompile.h"
#include	"handle_to.h"
#include	"test.h"

//--------------------------------------------------------------------------
//	tests
//--------------------------------------------------------------------------
#ifdef	_RUN_TESTS
DEFINE_TEST (HandleTo)
{
	DO_AFTER (Handle);
}

//--------------------------------------------------------------------------
ON_TEST (HandleTo)
{
	DO_TEST_REPORT;
	CountedObject	object;
	TEST_CONDITION (object.GetCount () == 0);
	object.AddRef ();
	object.AddRef ();
	object.AddRef ();
	TEST_CONDITION (object.GetCount () == 3);
	TEST_CONDITION (object.RemoveRef () == 2);
	TEST_CONDITION (object.RemoveRef () == 1);
	TEST_CONDITION (object.RemoveRef () == 0);
	TEST_CONDITION (object.GetCount () == 0);
}
#endif

//--------------------------------------------------------------------------
