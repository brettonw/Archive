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
// class methods
//-----------------------------------------------------------------------------
/* void */
Task::Task (const Text& name) : 
    m_name (name)
{
    TaskRegistrar::GetSingleton ()->RegisterTask (this);
}

//-----------------------------------------------------------------------------
/* void */
Task::~Task (void)
{
    // destruction is in random order, do something in the destructor
    // only if it does not depend on anything else
}

//-----------------------------------------------------------------------------
/* void */
void
Task::DoAfter (const Text& dependency) const
{
    TaskRegistrar::GetSingleton ()->RegisterDependency (m_name, dependency);
}

//-----------------------------------------------------------------------------
