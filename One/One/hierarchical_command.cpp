//------------------------------------------------------------------------------
//	Copyright (C) 1997-2000 Bretton Wade
//	All Rights Reserved
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//	includes
//------------------------------------------------------------------------------
#include	"precompile.h"
#include	"hierarchical_command.h"
#include	"iterator_on_list_of.h"

//------------------------------------------------------------------------------
//	class methods
//------------------------------------------------------------------------------
/* void */
HierarchicalCommand::HierarchicalCommand
(
    const Text&     name
) : Command (name)
{
}

//------------------------------------------------------------------------------
/* void */
HierarchicalCommand::~HierarchicalCommand
(
    void
)
{
}

//------------------------------------------------------------------------------
bool
HierarchicalCommand::Do
(
    void
)
{
    bool                                success = true;
    IteratorOnListOf<PtrTo<Command> >   iterator (myListOfCommands);
    
    // loop over the list of commands do-ing each one
    while (not iterator.IsDone () and success)
    {
        success = iterator.GetCurrentItem ()->Do ();
        iterator.GotoNextItem ();
    }
    
    // if a command failed for some reason, we need to unwind 
    // what we were trying to do
    if (not success)
    {
        iterator.GotoPreviousItem ();
        while (not iterator.IsDone ())
        {
            iterator.GetCurrentItem ()->Undo ();
            iterator.GotoPreviousItem ();
        }
    }
    
    // return whether or not the command was successfully executed
    return success;
}

//------------------------------------------------------------------------------
bool
HierarchicalCommand::Undo
(
    void
)
{
    bool                                success = true;
    IteratorOnListOf<PtrTo<Command> >   iterator (myListOfCommands);
    iterator.GotoTail ();

    // loop over the list of commands undo-ing each one
    while (not iterator.IsDone () and success)
    {
        success = iterator.GetCurrentItem ()->Undo ();
        iterator.GotoPreviousItem ();
    }

    // if a command failed for some reason, we need to unwind 
    // what we were trying to undo
    if (not success)
    {
        iterator.GotoNextItem ();
        while (not iterator.IsDone ())
        {
            iterator.GetCurrentItem ()->Do ();
            iterator.GotoNextItem ();
        }
    }
    
    // return whether or not the command was successfully executed
    return success;
}

//------------------------------------------------------------------------------
void
HierarchicalCommand::AddCommand
(
    const PtrTo<Command>&   command
)
{
    myListOfCommands.AddToTail (command);
}

//------------------------------------------------------------------------------
