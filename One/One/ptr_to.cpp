//------------------------------------------------------------------------------
//	Copyright (C) 1997-2000 Bretton Wade
//	All Rights Reserved
//------------------------------------------------------------------------------

//--------------------------------------------------------------------------
//	includes
//--------------------------------------------------------------------------
#include	"precompile.h"
#include	"ptr_to.h"
#include	"test.h"

//--------------------------------------------------------------------------
//	tests
//--------------------------------------------------------------------------
#ifdef	_RUN_TESTS
DEFINE_TEST (PtrTo)
{
	DO_AFTER (CountedObject);
}

//--------------------------------------------------------------------------
ON_TEST (PtrTo)
{
	DO_TEST_REPORT;
	CountedObject*			object = new CountedObject;
	PtrTo<CountedObject>	ptr (object);
	TEST_CONDITION (ptr != 0);
	TEST_CONDITION (not (ptr == 0));
	TEST_CONDITION ((*ptr).GetCount () == 1);
	TEST_CONDITION (ptr->GetCount () == 1);
	TEST_CONDITION (ptr.IsUnique ());
	{
		PtrTo<CountedObject>	ptr2 (object);
		TEST_CONDITION (ptr->GetCount () == 2);
	}
	TEST_CONDITION (ptr->GetCount () == 1);
	{
		PtrTo<CountedObject>	ptr2 (ptr);
		TEST_CONDITION (ptr->GetCount () == 2);
	}
	TEST_CONDITION (ptr->GetCount () == 1);
	{
		PtrTo<CountedObject>	ptr2 (ptr);
		TEST_CONDITION (ptr2->GetCount () == 2);
		ptr2.MakeUnique ();
		TEST_CONDITION (ptr->GetCount () == 1);
		TEST_CONDITION (ptr.IsUnique ());
		TEST_CONDITION (ptr2->GetCount () == 1);
		TEST_CONDITION (ptr2.IsUnique ());
	}
	TEST_CONDITION (ptr->GetCount () == 1);
}
#endif

//--------------------------------------------------------------------------
