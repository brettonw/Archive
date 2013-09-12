//------------------------------------------------------------------------------
//	Copyright (C) 1997-2000 Bretton Wade
//	All Rights Reserved
//------------------------------------------------------------------------------

#ifndef		_BW_COMMAND_H_
#define		_BW_COMMAND_H_

//--------------------------------------------------------------------------
//	includes
//--------------------------------------------------------------------------
#ifndef		_BW_TEXT_H_
#include	"text.h"
#endif	//	_BW_TEXT_H_

//------------------------------------------------------------------------------
//	class definitions
//------------------------------------------------------------------------------
class Command : public CountedObject
{
	public:
	    /* void */      Command (const Text& name);
virtual /* void */      ~Command (void);
virtual bool            Do (void) = 0;
virtual bool            Undo (void) = 0;
        Text            GetName (void) const;
        
    protected:
        Text            myName;
};

//------------------------------------------------------------------------------

#endif	//	_BW_COMMAND_H_
