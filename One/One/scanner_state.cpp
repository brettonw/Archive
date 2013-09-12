//------------------------------------------------------------------------------
//	Copyright (C) 1997-2000 Bretton Wade
//	All Rights Reserved
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//	includes
//------------------------------------------------------------------------------
#include	"precompile.h"
#include	"scanner_state.h"

//------------------------------------------------------------------------------
//	class methods
//------------------------------------------------------------------------------
ScannerState::ScannerState
(
	void
)
{
}

//------------------------------------------------------------------------------
ScannerState::ScannerState
(
	const ScannerState&		scannerState
)
{
	memcpy (myStateInfo, scannerState.myStateInfo, STATE_COUNT * sizeof(ScannerStateInfo));
}

//------------------------------------------------------------------------------
ScannerState&
ScannerState::operator =
(
	const ScannerState&		scannerState
)
{
	memcpy (myStateInfo, scannerState.myStateInfo, STATE_COUNT * sizeof(ScannerStateInfo));
	return *this;
}

//------------------------------------------------------------------------------
const ScannerStateInfo&
ScannerState::operator []
(
	uInt					index
)
{
	return myStateInfo[index];
}

//------------------------------------------------------------------------------
void
ScannerState::SaveScannerState
(
	ofstream&				stream
)
{
	stream.write (sPointer (myStateInfo), STATE_COUNT * sizeof(ScannerStateInfo));
}

//------------------------------------------------------------------------------
void
ScannerState::LoadScannerState
(
	ifstream&				stream
)
{
	stream.read (sPointer (myStateInfo), STATE_COUNT * sizeof(ScannerStateInfo));
}

//------------------------------------------------------------------------------
bool
ScannerState::Set
(
	uInt2					input,
	uInt2					nextState,
	uInt2					storage,
	uInt2					action
)
{
	if (myStateInfo[input].myNextState == INVALID_STATE)
	{
		myStateInfo[input] (nextState, storage, action);
		return true;
	}
	return false;
}

//------------------------------------------------------------------------------
void
ScannerState::SetAll
(
	uInt2					nextState,
	uInt2					storage,
	uInt2					action
)
{
	for (uInt2 i = 0; i < STATE_COUNT; i++)
		Set (i, nextState, storage, action);;
}

//------------------------------------------------------------------------------
