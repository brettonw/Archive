//------------------------------------------------------------------------------
//	Copyright (C) 1997-2000 Bretton Wade
//	All Rights Reserved
//------------------------------------------------------------------------------

#ifndef		_BW_CMD_LINE_SCANNER_ACTION_H_
#define		_BW_CMD_LINE_SCANNER_ACTION_H_

//------------------------------------------------------------------------------
//	includes
//------------------------------------------------------------------------------
#ifndef		_BW_SCANNER_ACTION_H_
#include	"scanner_action.h"
#endif	//	_BW_SCANNER_ACTION_H_

//------------------------------------------------------------------------------
//	class definitions
//------------------------------------------------------------------------------
class	CmdLineScannerAction : public ScannerAction
{
	public:
virtual	~CmdLineScannerAction (void);
virtual	void					Action (cString buffer, uInt length, uInt2 action, cString nextBuffer, uInt2 nextState);
	public:
		const enum
		{
			OUTPUT_IDENTIFIER = 1,					// output an identifier
			OUTPUT_WHITESPACE,						// output whitespace
			OUTPUT_OPEN_QUOTE,						// output an open quote character
			OUTPUT_CLOSE_QUOTE,						// output a close quote character
			OUTPUT_QUOTED_IDENTIFIER,				// output an identifier that was enclosed in quotes
			OUTPUT_UNCLOSED_QUOTED_IDENTIFER		// output an identifier that was in an unclosed quotation
		};
};

//------------------------------------------------------------------------------

#endif	//	_BW_CMD_LINE_SCANNER_ACTION_H_
