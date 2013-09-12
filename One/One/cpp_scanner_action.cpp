//------------------------------------------------------------------------------
//	Copyright (C) 1997-2000 Bretton Wade
//	All Rights Reserved
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//	includes
//------------------------------------------------------------------------------
#include	"precompile.h"
#include	"cpp_scanner_action.h"

//------------------------------------------------------------------------------
//	class methods
//------------------------------------------------------------------------------
CPPScannerAction::~CPPScannerAction
(
	void
)
{
}

//------------------------------------------------------------------------------
void
CPPScannerAction::Action
(
	cString					buffer,
	uInt					length,
	uInt2					action,
	cString					nextBuffer,
	uInt2					nextState
)
{
	switch (action)
	{
		case OUTPUT_IDENTIFIER:
			cerr << "  OUTPUT_IDENTIFIER (";
			cerr.write (buffer, length);
			cerr << ")" << endl;
			break;
	}
}

//------------------------------------------------------------------------------
