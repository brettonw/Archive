//------------------------------------------------------------------------------
//	Copyright (C) 1997-2000 Bretton Wade
//	All Rights Reserved
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//	includes
//------------------------------------------------------------------------------
#include	"precompile.h"
#include	"cmd_line_scanner_action.h"

//------------------------------------------------------------------------------
//	class methods
//------------------------------------------------------------------------------
CmdLineScannerAction::~CmdLineScannerAction
(
	void
)
{
}

//------------------------------------------------------------------------------
void
CmdLineScannerAction::Action
(
	cString				buffer,
	uInt				length,
	uInt2				action,
	cString				nextBuffer,
	uInt2				nextState
)
{
	switch (action)
	{
		case OUTPUT_IDENTIFIER:
			cerr << "  OUTPUT_IDENTIFIER_ACTION (";
			break;
		case OUTPUT_WHITESPACE:
			cerr << "  OUTPUT_WHITESPACE_ACTION (";
			break;
		case OUTPUT_OPEN_QUOTE:
			cerr << "  OUTPUT_OPEN_QUOTE_ACTION (";
			break;
		case OUTPUT_CLOSE_QUOTE:
			cerr << "  OUTPUT_CLOSE_QUOTE_ACTION (";
			break;
		case OUTPUT_QUOTED_IDENTIFIER:
			cerr << "  OUTPUT_QUOTED_IDENTIFIER_ACTION (";
			break;
		case OUTPUT_UNCLOSED_QUOTED_IDENTIFER:
			cerr << "  OUTPUT_UNCLOSED_QUOTED_IDENTIFER_ACTION (";
			break;
	}
	cerr.write (buffer, length);
	cerr << ")" << endl;
}

//------------------------------------------------------------------------------
