//------------------------------------------------------------------------------
//	Copyright (C) 1997-2000 Bretton Wade
//	All Rights Reserved
//------------------------------------------------------------------------------

#ifndef		_BW_LINE_STATE_H_
#define		_BW_LINE_STATE_H_

//------------------------------------------------------------------------------
//	includes
//------------------------------------------------------------------------------
#ifndef		_BW_SCANNER_H_
#include	"scanner.h"
#endif	//	_BW_SCANNER_H_

//------------------------------------------------------------------------------
//	class definitions
//------------------------------------------------------------------------------
class	LineState
{
	public:
		LineState (void);
		uInt2				myState;
		uInt				myStartOffset;
		uInt				myLength;
		//cString				myString;
		//cString				myEnd;
		uInt				GetEnd (void);
};

//------------------------------------------------------------------------------
//	inlines
//------------------------------------------------------------------------------
inline
LineState::LineState (void)
{
	myState = ScannerState::INVALID_STATE;
	myStartOffset = myLength = 0;
}

//------------------------------------------------------------------------------
inline
uInt	LineState::GetEnd (void)
{
	return myStartOffset + myLength;
}

//------------------------------------------------------------------------------

#endif	//	_BW_LINE_STATE_H_
