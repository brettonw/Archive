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
Scanner::Scanner
(
	void
) : myStateTable (4)
{
	myTableWasLoaded = false;
}

//------------------------------------------------------------------------------
Scanner::Scanner
(
	const Text&				filename
) : myStateTable (16)
{
	LoadStateTable (filename);
}

//------------------------------------------------------------------------------
Scanner::~Scanner
(
	void
)
{
}

//------------------------------------------------------------------------------
void
Scanner::BuildScanner
(
	void
)
{
}

//------------------------------------------------------------------------------
void
Scanner::ResetScanner
(
	void
)
{
    uInt    size = myStateTable.GetSize ();
    for (uInt i = 0; i < size; i++)
	myStateTable[i].SetAll (ScannerState::INVALID_STATE, DONT_STORE_INPUT, ScannerAction::NO_ACTION);
}

//------------------------------------------------------------------------------
void
Scanner::OnInput
(
	uInt2					state,
	cString					input,
	uInt2					nextState,
	uInt2					storage,
	uInt2					action
)
{
	assert (not ((nextState == state) and (storage == STORE_INPUT_AFTER)));
	if (strlen (input) > 0)
		while (*input)
			myStateTable[state].Set (*input++, nextState, storage, action);
	else
		myStateTable[state].Set (0, nextState, storage, action);
}

//------------------------------------------------------------------------------
void
Scanner::OnAnyInput
(
	uInt2					state,
	uInt2					nextState,
	uInt2					storage,
	uInt2					action
)
{
	assert (not ((nextState == state) and (storage == STORE_INPUT_AFTER)));
	myStateTable[state].SetAll (nextState, storage, action);
}

//------------------------------------------------------------------------------
void
Scanner::OnAlphabeticInput
(
	uInt2					state,
	uInt2					nextState,
	uInt2					storage,
	uInt2					action
)
{
	assert (not ((nextState == state) and (storage == STORE_INPUT_AFTER)));
	for (uInt2 i = 0; i < ScannerState::STATE_COUNT; i++)
		if (isalnum (i))
			myStateTable[state].Set (i, nextState, storage, action);
}

//------------------------------------------------------------------------------
void
Scanner::OnNumericInput
(
	uInt2					state,
	uInt2					nextState,
	uInt2					storage,
	uInt2					action
)
{
	assert (not ((nextState == state) and (storage == STORE_INPUT_AFTER)));
	for (uInt2 i = 0; i < ScannerState::STATE_COUNT; i++)
		if (isdigit (i))
			myStateTable[state].Set (i, nextState, storage, action);
}

//------------------------------------------------------------------------------
void
Scanner::OnHexadecimalInput
(
	uInt2					state,
	uInt2					nextState,
	uInt2					storage,
	uInt2					action
)
{
	assert (not ((nextState == state) and (storage == STORE_INPUT_AFTER)));
	for (uInt2 i = 0; i < ScannerState::STATE_COUNT; i++)
		if (isxdigit (i))
			myStateTable[state].Set (i, nextState, storage, action);
}

//------------------------------------------------------------------------------
void
Scanner::OnWhitespaceInput
(
	uInt2					state,
	uInt2					nextState,
	uInt2					storage,
	uInt2					action
)
{
	assert (not ((nextState == state) and (storage == STORE_INPUT_AFTER)));
	for (uInt2 i = 0; i < ScannerState::STATE_COUNT; i++)
		if (isspace (i))
			myStateTable[state].Set (i, nextState, storage, action);
}

//------------------------------------------------------------------------------
void
Scanner::SaveStateTable
(
	const Text&				filename
)
{
	ofstream	stateTableFile (filename, ios::binary);
	if (stateTableFile.good ())
	{
		uInt	tableSize = myStateTable.GetSize ();
		stateTableFile.write (sPointer (&tableSize), sizeof (uInt));
		for (uInt i = 0; i < tableSize; i++)
			myStateTable[i].SaveScannerState (stateTableFile);
	}
}

//------------------------------------------------------------------------------
void
Scanner::LoadStateTable
(
	const Text&				filename
)
{
	ifstream	stateTableFile (filename, ios::binary);
	if (stateTableFile.good ())
	{
		uInt	tableSize;
		stateTableFile.read (sPointer (&tableSize), sizeof (uInt));
		for (uInt i = 0; i < tableSize; i++)
			myStateTable[i].LoadScannerState (stateTableFile);
		myTableWasLoaded = true;
	}
	else
	{
		myTableWasLoaded = false;
	}
}

//------------------------------------------------------------------------------
uInt2
Scanner::ScanText
(
	const Text&				text,
	ScannerAction*			action,
	uInt2					state
)
{
	return ScanText (text, text.Length (), action, state);
}

//------------------------------------------------------------------------------
uInt2
Scanner::ScanText
(
	cString					text,
	uInt					length,
	ScannerAction*			action,
	uInt2					state
)
{
	myStart = myStop = text;
	cString		stop = text + length;
	while (text != stop)
		state = ScanCharacter (text, action, state);
	return EndScan (text, action, state);
}

//------------------------------------------------------------------------------
uInt2
Scanner::ScanCharacter
(
	cString&				text,
	ScannerAction*			action,
	uInt2					state
)
{
	const	ScannerStateInfo&	info = myStateTable[state][*text];
	assert (info.myNextState != ScannerState::INVALID_STATE);
	state = info.myNextState;
	switch (info.myStorage)
	{
		case DONT_STORE_INPUT:
			text++;
			break;
		case STORE_INPUT_BEFORE:
			assert (myStop == text);
			text++;
			myStop++;
			break;
		case STORE_INPUT_AFTER:
			break;
	}
	if (info.myAction != ScannerAction::NO_ACTION)
	{
		action->Action (myStart, myStop - myStart, info.myAction, text, state);
		myStart = myStop = text;
	}
	return state;
}

//------------------------------------------------------------------------------
uInt2
Scanner::EndScan
(
	cString					text,
	ScannerAction*			action,
	uInt2					state
)
{
	const	ScannerStateInfo*	info = &myStateTable[state][0];
	assert (info->myNextState != ScannerState::INVALID_STATE);

	while (myStart != text)
	{
		if (info->myAction != ScannerAction::NO_ACTION)
		{
			action->Action (myStart, myStop - myStart, info->myAction, text, info->myNextState);
			myStart = myStop = text;
		}
		else
		{
			state = info->myNextState;
			info = &myStateTable[state][0];
		}
	}
	return info->myNextState;
}

//------------------------------------------------------------------------------
