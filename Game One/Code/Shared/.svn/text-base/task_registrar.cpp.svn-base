//-----------------------------------------------------------------------------
// Copyright (C) 1997-2002 Bretton Wade
// All rights reserved.
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// include files
//-----------------------------------------------------------------------------
#include    "precompile.h"
#include    "task_registrar.h"

//-----------------------------------------------------------------------------
// force loaded modules
//-----------------------------------------------------------------------------
void
ForceLoadModules (void)
{
    DECLARE_FORCED_LOAD (Main);
    DECLARE_FORCED_LOAD (COM);
    DECLARE_FORCED_LOAD (Window);
    DECLARE_FORCED_LOAD (Direct3D);
    DECLARE_FORCED_LOAD (DirectMusic);
}

//-----------------------------------------------------------------------------
// class methods
//-----------------------------------------------------------------------------
/* void */
TaskRegistrar::~TaskRegistrar (void)
{
    // tell everybody we are going away
    SendMessageReverse (Task::SHUTDOWN);
}

//-----------------------------------------------------------------------------
void
TaskRegistrar::RegisterTask (Task* pTask)
{
    m_tasks.AddNode (pTask->GetName (), pTask);
}

//-----------------------------------------------------------------------------
void
TaskRegistrar::RegisterDependency (const Text& task, const Text& dependency)
{
    m_tasks.AddEdge (dependency, task);
}

//-----------------------------------------------------------------------------
bool
TaskRegistrar::SendMessage (Task::Message msg) const
{
    // fill the list of dependencies
    TextList    list;
    m_tasks.FindDependencies (list);
        
    // walk the list calling setup on all the tasks
    TextList::iterator  listIter = list.begin ();
    while (listIter != list.end ())
    {
        Text    name = *listIter++;
        //std::cerr << "Send (" << msg << ") to: " << name << std::endl;
        DebugPrint ("Send (%d) to %s\n", msg, cString (name));
        Task*   task = m_tasks.GetNode (name);
        if (not task->HandleMessage (msg))
        {
            DebugPrint ("Terminating message processing due to task failure: %s\n", (cString) name);
            return false;
        }
    }
    
    // report success
    DebugPrint ("message processing complete\n");
    return true;
}

//-----------------------------------------------------------------------------
bool
TaskRegistrar::SendMessageReverse (Task::Message msg) const
{
    // fill the list of dependencies
    TextList    list;
    m_tasks.FindDependencies (list);
        
    // walk the list in reverse order sending the message to all the tasks
    TextList::reverse_iterator  listIter = list.rbegin ();
    while (listIter != list.rend ())
    {
        Text    name = *listIter++;
        //std::cerr << "Send Reverse (" << msg << ") to: " << name << std::endl;
        DebugPrint ("Send (%d) to %s\n", msg, cString (name));
        Task*   task = m_tasks.GetNode (name);
        if (not task->HandleMessage (msg))
        {
            DebugPrint ("Terminating message processing due to task failure: %s\n", (cString) name);
            return false;
        }
    }
    
    // report success
    DebugPrint ("message processing complete\n");
    return true;
}

//-----------------------------------------------------------------------------
TaskRegistrar*
TaskRegistrar::GetSingleton (void)
{
    if (not IsValid ())
    {
        NEW_BLOCK_TYPE (_CRT_BLOCK);
        NewCall TaskRegistrar;
    }
    return Singleton<TaskRegistrar>::GetSingleton ();
}

//-----------------------------------------------------------------------------
