//------------------------------------------------------------------------------
//	Copyright (C) 1997-2000 Bretton Wade
//	All Rights Reserved
//------------------------------------------------------------------------------

//--------------------------------------------------------------------------
//	includes
//--------------------------------------------------------------------------
#include	"precompile.h"
#include	"task_registrar.h"
#include	"iterator_on_graph_of.h"

//--------------------------------------------------------------------------
//	static members
//--------------------------------------------------------------------------
GraphOf<Task*>*	TaskRegistrar::myTasks = 0;

//--------------------------------------------------------------------------
//	class methods
//--------------------------------------------------------------------------
void
TaskRegistrar::RegisterTask
(
	Task*			task
)
{
	if (!myTasks)
		myTasks = new GraphOf<Task*>;
	myTasks->AddNode (task->GetName (), task);
}

//--------------------------------------------------------------------------
void
TaskRegistrar::RunStartupTasks
(
	void
)
{
	if (myTasks)
	{
		IteratorOnGraphOf<Task*>	iterator (myTasks);
		
		// loop over all the nodes in the graph building the edges
		while (not iterator.IsDone ())
		{
		    try
		    {
			    iterator.GetCurrentItem ()->Setup (myTasks);
			}
			catch (NodeNotFound)
			{
			    // nothing is initialized yet, including the log files. There's little we
			    // can do except crash and use the debugger to figure out why.
			    // HINT: iterator.GetCurrentItem ()->GetName () has the name of the node being
			    // evaluated, and nodeNotFound has the name of the dependent node not found
			    MAKEITCRASH;
			}
			iterator.GotoNextItem ();
		}
		
		// try to walk the topology
		try
		{
		    iterator.WalkTopology ();
		}
		catch (CycleAtNode)
		{
		    // nothing is initialized yet, including the log files. There's little we can do
		    // except crash and use the debugger to figure out where the cycle is.
		    // HINT: nodeWithCycle has the name of the node currently being evaluated. The
		    // cycle must have some relationship to that node.
		    MAKEITCRASH;
		}
		
		// proceed with the tasks in dependency order
		while (not iterator.IsDone ())
		{
			iterator.GetCurrentItem ()->Startup ();
			iterator.GotoNextItem ();
		}
	}
}

//--------------------------------------------------------------------------
void
TaskRegistrar::RunShutdownTasks
(
	void
)
{
	if (myTasks)
	{
		IteratorOnGraphOf<Task*>	iterator (myTasks);
		iterator.GotoTail ();
		while (not iterator.IsDone ())
		{
			iterator.GetCurrentItem ()->Shutdown ();
			iterator.GotoPreviousItem ();
		}
		delete myTasks;
		myTasks = 0;
	}
}

//--------------------------------------------------------------------------
