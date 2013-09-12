//------------------------------------------------------------------------------
//	Copyright (C) 1997-2000 Bretton Wade
//	All Rights Reserved
//------------------------------------------------------------------------------

//--------------------------------------------------------------------------
//	includes
//--------------------------------------------------------------------------
#include	"precompile.h"
#include	"hash_table_of.h"
#include	"test.h"

//--------------------------------------------------------------------------
//	tests
//--------------------------------------------------------------------------
#ifdef	_RUN_TESTS
DEFINE_TEST (HashTableOf)
{
	DO_AFTER (Bits);
	DO_AFTER (ListOf);
}

//--------------------------------------------------------------------------
ON_TEST (HashTableOf)
{
	DO_TEST_REPORT;
	String			strings[] =	{
								"test1",
								"a test",
								"something",
								"minty breath",
								"five"
						  		};
	try
	{
		HashTableOf<Text, String>	table;
		table.PutItem (strings[0], strings[0]);
		TEST_CONDITION (strcmp (strings[0], table.GetItem (strings[0])) == 0);

		table.PutItem (strings[1], strings[1]);
		TEST_CONDITION (strcmp (strings[0], table.GetItem (strings[0])) == 0);
		TEST_CONDITION (strcmp (strings[1], table.GetItem (strings[1])) == 0);

		table.PutItem (strings[2], strings[2]);
		TEST_CONDITION (strcmp (strings[0], table.GetItem (strings[0])) == 0);
		TEST_CONDITION (strcmp (strings[1], table.GetItem (strings[1])) == 0);
		TEST_CONDITION (strcmp (strings[2], table.GetItem (strings[2])) == 0);

		table.PutItem (strings[3], strings[3]);
		TEST_CONDITION (strcmp (strings[0], table.GetItem (strings[0])) == 0);
		TEST_CONDITION (strcmp (strings[1], table.GetItem (strings[1])) == 0);
		TEST_CONDITION (strcmp (strings[2], table.GetItem (strings[2])) == 0);
		TEST_CONDITION (strcmp (strings[3], table.GetItem (strings[3])) == 0);

		table.PutItem (strings[4], strings[4]);
		TEST_CONDITION (strcmp (strings[0], table.GetItem (strings[0])) == 0);
		TEST_CONDITION (strcmp (strings[1], table.GetItem (strings[1])) == 0);
		TEST_CONDITION (strcmp (strings[2], table.GetItem (strings[2])) == 0);
		TEST_CONDITION (strcmp (strings[3], table.GetItem (strings[3])) == 0);
		TEST_CONDITION (strcmp (strings[4], table.GetItem (strings[4])) == 0);
	}
	catch (...)
	{
		TEST_CONDITION (false and "Catch unexpected exception");
	}
}
#endif

//--------------------------------------------------------------------------
