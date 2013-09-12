//------------------------------------------------------------------------------
//	Copyright (C) 1997-2000 Bretton Wade
//	All Rights Reserved
//------------------------------------------------------------------------------

#ifndef		_BW_TEXT_SCANNER_ACTION_H_
#define		_BW_TEXT_SCANNER_ACTION_H_

//------------------------------------------------------------------------------
//	includes
//------------------------------------------------------------------------------
#ifndef		_BW_SCANNER_ACTION_H_
#include	"scanner_action.h"
#endif	//	_BW_SCANNER_ACTION_H_

//------------------------------------------------------------------------------
//	class definitions
//------------------------------------------------------------------------------
class	TextScannerAction : public ScannerAction
{
	public:
virtual	~TextScannerAction (void);
virtual	void					Action (cString buffer, uInt length, uInt2 action, cString nextBuffer, uInt2 nextState);
	public:
		const enum
		{
			OUTPUT_IDENTIFIER = 1,					// output an identifier
			OUTPUT_WHITESPACE,						// output whitespace
			OUTPUT_NEWLINE,							// output a newline
			LAST_TEXT_SCANNER_ACTION
		};
};

//------------------------------------------------------------------------------

#endif	//	_BW_TEXT_SCANNER_ACTION_H_
