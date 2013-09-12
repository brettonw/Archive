//------------------------------------------------------------------------------
//	Copyright (C) 1997-2000 Bretton Wade
//	All Rights Reserved
//------------------------------------------------------------------------------

#ifndef		_BW_CMD_LINE_SCANNER_H_
#define		_BW_CMD_LINE_SCANNER_H_

//------------------------------------------------------------------------------
//	includes
//------------------------------------------------------------------------------
#ifndef		_BW_SCANNER_H_
#include	"scanner.h"
#endif	//	_BW_SCANNER_H_

//------------------------------------------------------------------------------
//	class definitions
//------------------------------------------------------------------------------
class	CmdLineScanner : public Scanner
{
	public:
		CmdLineScanner (void);
		CmdLineScanner (const Text& filename);
virtual	~CmdLineScanner (void);
virtual	void					BuildScanner (void);
	public:
		const enum
		{
			DEFAULT_STATE = ScannerState::DEFAULT_STATE,
			IDENTIFIER_STATE,
			WHITESPACE_STATE,
			QUOTE_STATE,
			CLOSE_QUOTE_STATE
		};
};

//------------------------------------------------------------------------------

#endif	//	_BW_CMD_LINE_SCANNER_H_
