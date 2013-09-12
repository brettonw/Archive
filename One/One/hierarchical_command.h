//------------------------------------------------------------------------------
//	Copyright (C) 1997-2000 Bretton Wade
//	All Rights Reserved
//------------------------------------------------------------------------------

#ifndef		_BW_HIERARCHICAL_COMMAND_H_
#define		_BW_HIERARCHICAL_COMMAND_H_

//--------------------------------------------------------------------------
//	includes
//--------------------------------------------------------------------------
#ifndef		_BW_COMMAND_H_
#include	"command.h"
#endif	//	_BW_COMMAND_H_

#ifndef		_BW_LIST_OF_H_
#include	"list_of.h"
#endif	//	_BW_LIST_OF_H_

#ifndef		_BW_PTR_TO_H_
#include	"ptr_to.h"
#endif	//	_BW_PTR_TO_H_

//------------------------------------------------------------------------------
//	class definitions
//------------------------------------------------------------------------------
class HierarchicalCommand : public Command
{
	public:
        /* void */              HierarchicalCommand (const Text& name);
virtual /* void */              ~HierarchicalCommand (void);
virtual bool                    Do (void);
virtual bool                    Undo (void);
        void                    AddCommand (const PtrTo<Command>& command);

    private:
        ListOf<PtrTo<Command> > myListOfCommands;
};

//------------------------------------------------------------------------------

#endif	//	_BW_HIERARCHICAL_COMMAND_H_
