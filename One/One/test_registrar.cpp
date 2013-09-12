//------------------------------------------------------------------------------
//	Copyright (C) 1997-2000 Bretton Wade
//	All Rights Reserved
//------------------------------------------------------------------------------

//--------------------------------------------------------------------------
//	includes
//--------------------------------------------------------------------------
#include	"precompile.h"
#include	"test_registrar.h"
#include	"iterator_on_graph_of.h"

//--------------------------------------------------------------------------
//	static members
//--------------------------------------------------------------------------
GraphOf<Test*>*	TestRegistrar::myTests = 0;

//--------------------------------------------------------------------------
//	class methods
//--------------------------------------------------------------------------
void
TestRegistrar::RegisterTest
(
	Test*			test
)
{
	if (!myTests)
		myTests = new GraphOf<Test*>;
	myTests->AddNode (test->GetName (), test);
}

//--------------------------------------------------------------------------
void
TestRegistrar::RunTests
(
	void
)
{
	if (myTests)
	{
		IteratorOnGraphOf<Test*>	iterator (myTests);
		
		// loop over all of the nodes in the graph building the edges
		while (not iterator.IsDone ())
		{
		    try
		    {
			    iterator.GetCurrentItem ()->Setup (myTests);
			}
			catch (NodeNotFound)
			{
			    throw InvalidTestDependency (iterator.GetCurrentItem ()->GetName ());
			}
			iterator.GotoNextItem ();
		}
		
		// try to walk the topology, this can fail if there is
		// a cycle in the test dependencies, but the log should
		// provide necessary clues
	    iterator.WalkTopology ();
		
		// proceed with the tests in dependency order
		while (not iterator.IsDone ())
		{
			iterator.GetCurrentItem ()->DoTest ();
			iterator.GotoNextItem ();
		}
		delete myTests;
		myTests = 0;
	}
}

//--------------------------------------------------------------------------
