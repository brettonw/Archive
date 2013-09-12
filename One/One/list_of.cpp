//------------------------------------------------------------------------------
//	Copyright (C) 1997-2000 Bretton Wade
//	All Rights Reserved
//------------------------------------------------------------------------------

//--------------------------------------------------------------------------
//	includes
//--------------------------------------------------------------------------
#include	"precompile.h"
#include	"list_of.h"
#include	"test.h"

//--------------------------------------------------------------------------
//	tests
//--------------------------------------------------------------------------
#ifdef	_RUN_TESTS
DEFINE_TEST (ListOf)
{
}

//--------------------------------------------------------------------------
ON_TEST (ListOf)
{
	DO_TEST_REPORT;
	String					strings[] =	{
											"test1",
											"a test",
											"something",
											"minty breath",
											"five"
						  				};
	sInt4					i;
	ListOf<String>			list;
	ClientForListOf<char*>	iterator (list);
	
	TEST_CONDITION (list.GetSize () == 0);
	TEST_CONDITION (iterator.IsDone ());
		

	for (i = 0; i < 5; i++)
		list.AddToTail (strings[i]);
	TEST_CONDITION (list.GetSize () == 5);

	for (i = 0; i < 5; i++)
		list.RemoveFromHead ();
	TEST_CONDITION (list.GetSize () == 0);

	for (i = 0; i < 5; i++)
		list.AddToHead (strings[i]);
	TEST_CONDITION (list.GetSize () == 5);

	for (i = 0; i < 5; i++)
		list.RemoveFromTail ();
	TEST_CONDITION (list.GetSize () == 0);

	for (i = 0; i < 5; i++)
		list.AddToTail (strings[i]);

	iterator.GotoHead ();
	for (i = 0; i < 5; i++)
	{
		char*	string = iterator.GetCurrentItem ();
		TEST_CONDITION (string == strings[i]);
		iterator.GotoNextItem ();
	}

	iterator.GotoTail ();
	for (i = 4; i>= 0; i--)
	{
		char*	string = iterator.GetCurrentItem ();
		TEST_CONDITION (string == strings[i]);
		iterator.GotoPreviousItem ();
	}

	iterator.GotoHead ();
	iterator.GotoNextItem ();
	iterator.GotoNextItem ();
	iterator.RemoveCurrentItem ();
	TEST_CONDITION (list.GetSize () == 4);
	TEST_CONDITION (iterator.GetCurrentItem () == strings[0]);

	iterator.RemoveCurrentItem ();
	TEST_CONDITION (list.GetSize () == 3);
	TEST_CONDITION (iterator.GetCurrentItem () == strings[1]);

	iterator.RemoveCurrentItem ();
	TEST_CONDITION (list.GetSize () == 2);
	TEST_CONDITION (iterator.GetCurrentItem () == strings[3]);

	iterator.RemoveCurrentItem ();
	TEST_CONDITION (list.GetSize () == 1);
	TEST_CONDITION (iterator.GetCurrentItem () == strings[4]);

	iterator.RemoveCurrentItem ();
	TEST_CONDITION (list.GetSize () == 0);

	iterator.InsertAfterCurrentItem (strings[0]);
	TEST_CONDITION (iterator.GetCurrentItem () == strings[0]);

	iterator.InsertAfterCurrentItem (strings[1]);
	TEST_CONDITION (iterator.GetCurrentItem () == strings[1]);

	iterator.InsertBeforeCurrentItem (strings[2]);
	TEST_CONDITION (iterator.GetCurrentItem () == strings[2]);
	TEST_CONDITION (list.GetSize () == 3);

	iterator.InsertBeforeCurrentItem (strings[3]);
	TEST_CONDITION (iterator.GetCurrentItem () == strings[3]);
	TEST_CONDITION (list.GetSize () == 4);

	iterator.GotoPreviousItem ();
	TEST_CONDITION (iterator.GetCurrentItem () == strings[0]);

	iterator.InsertBeforeCurrentItem (strings[4]);
	TEST_CONDITION (iterator.GetCurrentItem () == strings[4]);

	iterator.GotoPreviousItem ();
	TEST_CONDITION (iterator.IsDone ());
}
#endif

//--------------------------------------------------------------------------
