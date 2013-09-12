//------------------------------------------------------------------------------
//	Copyright (C) 1997-2000 Bretton Wade
//	All Rights Reserved
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//	includes
//------------------------------------------------------------------------------
#include	"precompile.h"
#include	"text_scanner_action.h"

//------------------------------------------------------------------------------
//	class methods
//------------------------------------------------------------------------------
TextScannerAction::~TextScannerAction
(
	void
)
{
}

//------------------------------------------------------------------------------
void
TextScannerAction::Action
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
			cerr.write (buffer, length);
			cerr << ")" << endl;
			break;
		case OUTPUT_WHITESPACE:
			cerr << "  OUTPUT_WHITESPACE_ACTION (";
			cerr.write (buffer, length);
			cerr << ")" << endl;
			break;
		case OUTPUT_NEWLINE:
			cerr << "  OUTPUT_NEWLINE_ACTION" << endl;
			break;
	}
}

//------------------------------------------------------------------------------
