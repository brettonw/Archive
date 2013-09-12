//------------------------------------------------------------------------------
//	Copyright (C) 1997-2000 Bretton Wade
//	All Rights Reserved
//------------------------------------------------------------------------------

#ifndef		_BW_SCANNER_H_
#define		_BW_SCANNER_H_

//------------------------------------------------------------------------------
//	includes
//------------------------------------------------------------------------------
#ifndef		_BW_TEXT_H_
#include	"text.h"
#endif	//	_BW_TEXT_H_

#ifndef		_BW_SCANNER_STATE_H_
#include	"scanner_state.h"
#endif	//	_BW_SCANNER_STATE_H_

#ifndef		_BW_SCANNER_ACTION_H_
#include	"scanner_action.h"
#endif	//	_BW_SCANNER_ACTION_H_

#ifndef		_BW_RESIZABLE_VECTOR_OF_H_
#include	"resizable_vector_of.h"
#endif	//	_BW_RESIZABLE_VECTOR_OF_H_

//------------------------------------------------------------------------------
//	class definitions
//------------------------------------------------------------------------------
class	Scanner
{
	public:
		Scanner (void);
		Scanner (const Text& filename);
virtual	~Scanner (void);
virtual	void					        BuildScanner (void);
		void					        ResetScanner (void);
		void					        OnInput (uInt2 state, cString input, uInt2 nextState, uInt2 storage, uInt2 action);
		void					        OnAnyInput (uInt2 state, uInt2 nextState, uInt2 storage, uInt2 action);
		void					        OnAlphabeticInput (uInt2 state, uInt2 nextState, uInt2 storage, uInt2 action);
		void					        OnNumericInput (uInt2 state, uInt2 nextState, uInt2 storage, uInt2 action);
		void					        OnHexadecimalInput (uInt2 state, uInt2 nextState, uInt2 storage, uInt2 action);
		void					        OnWhitespaceInput (uInt2 state, uInt2 nextState, uInt2 storage, uInt2 action);
		void					        SaveStateTable (const Text& filename);
		void					        LoadStateTable (const Text& filename);
		uInt2					        ScanText (const Text& text, ScannerAction* action, uInt2 startState);
		uInt2					        ScanText (cString text, uInt length, ScannerAction* action, uInt2 startState);
		uInt2					        ScanCharacter (cString& text, ScannerAction* action, uInt2 state);
		uInt2					        EndScan (cString text, ScannerAction* action, uInt2 state);
	public:
		const enum
		{
			//	input control codes
			DONT_STORE_INPUT = 0,		//	do not store the input
			STORE_INPUT_BEFORE = 1,		//	store the input into the buffer before performing the action
			STORE_INPUT_AFTER = 2		//	store the input into the buffer after performing the action
		};
	protected:
		ResizableVectorOf<ScannerState> myStateTable;
		cString					        myStart;
		cString					        myStop;
		bool					        myTableWasLoaded;
};

//------------------------------------------------------------------------------

#endif	//	_BW_SCANNER_H_
