//------------------------------------------------------------------------------
//	Copyright (C) 1997-2000 Bretton Wade
//	All Rights Reserved
//------------------------------------------------------------------------------

//--------------------------------------------------------------------------
//	includes
//--------------------------------------------------------------------------
#include	"precompile.h"
#include	"iterator_on_graph_of.h"
#include	"test.h"

//--------------------------------------------------------------------------
//	tests
//--------------------------------------------------------------------------
#ifdef	_RUN_TESTS
DEFINE_TEST (GraphOf)
{
	DO_AFTER (HashTableOf);
	DO_AFTER (Text);
}

//--------------------------------------------------------------------------
ON_TEST (GraphOf)
{
	DO_TEST_REPORT;
	String			strings[] =	{
								"test1",
								"a test",
								"something",
								"minty breath",
								"five"
						  		};
	uInt			i;
	GraphOf<uInt>	graph;
	for (i = 0; i < 5; i++)
		graph.AddNode (strings[i], i);
	for (i = 0; i < 4; i++)
		graph.AddEdge (strings[i], strings[i + 1]);
	IteratorOnGraphOf<uInt>	iterator (graph);
	i = 0;
	while (not iterator.IsDone ())
	{
		TEST_CONDITION (iterator.GetCurrentItem () == i++);
		iterator.GotoNextItem ();
	}
}
#endif

//--------------------------------------------------------------------------
