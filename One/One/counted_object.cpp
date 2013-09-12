//------------------------------------------------------------------------------
//	Copyright (C) 1997-2000 Bretton Wade
//	All Rights Reserved
//------------------------------------------------------------------------------

//--------------------------------------------------------------------------
//	includes
//--------------------------------------------------------------------------
#include	"precompile.h"
#include	"counted_object.h"
#include	"test.h"

//--------------------------------------------------------------------------
//	class methods
//--------------------------------------------------------------------------
CountedObject::CountedObject
(
	void
)
{
	myCount = 0;
}

//--------------------------------------------------------------------------
CountedObject::CountedObject
(
	const CountedObject&	object
)
{
	myCount = 0;
}

//--------------------------------------------------------------------------
CountedObject::~CountedObject
(
	void
)
{
	assert (myCount == 0);
}

//--------------------------------------------------------------------------
void
CountedObject::AddRef
(
	void
)
{
	myCount++;
}

//--------------------------------------------------------------------------
uInt4
CountedObject::RemoveRef
(
	void
)
{
	return --myCount;
}

//--------------------------------------------------------------------------
uInt4
CountedObject::GetCount
(
	void
) const
{
	return myCount;
}

//--------------------------------------------------------------------------
//	tests
//--------------------------------------------------------------------------
#ifdef	_RUN_TESTS
DEFINE_TEST (CountedObject)
{
}

//--------------------------------------------------------------------------
ON_TEST (CountedObject)
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
