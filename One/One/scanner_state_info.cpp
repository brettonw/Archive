//------------------------------------------------------------------------------
//	Copyright (C) 1997-2000 Bretton Wade
//	All Rights Reserved
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//	includes
//------------------------------------------------------------------------------
#include	"precompile.h"
#include	"scanner.h"

//------------------------------------------------------------------------------
//	class methods
//------------------------------------------------------------------------------
ScannerStateInfo::ScannerStateInfo
(
	void
)
{
	myNextState = ScannerState::INVALID_STATE;
}

//------------------------------------------------------------------------------
ScannerStateInfo::ScannerStateInfo
(
	uInt2					nextState,
	uInt2					storage,
	uInt2					action
)
{
	myNextState = nextState;
	myStorage = storage;
	myAction = action;
}

//------------------------------------------------------------------------------
void
ScannerStateInfo::operator ()
(
	uInt2					nextState,
	uInt2					storage,
	uInt2					action
)
{
	myNextState = nextState;
	myStorage = storage;
	myAction = action;
}

//------------------------------------------------------------------------------
