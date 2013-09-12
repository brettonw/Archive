//------------------------------------------------------------------------------
//	Copyright (C) 1997-2000 Bretton Wade
//	All Rights Reserved
//------------------------------------------------------------------------------

#ifndef		_BW_TEXT_SCANNER_H_
#define		_BW_TEXT_SCANNER_H_

//------------------------------------------------------------------------------
//	includes
//------------------------------------------------------------------------------
#ifndef		_BW_SCANNER_H_
#include	"scanner.h"
#endif	//	_BW_SCANNER_H_

//------------------------------------------------------------------------------
//	class definitions
//------------------------------------------------------------------------------
class	TextScanner : public Scanner
{
	public:
		TextScanner (void);
		TextScanner (const Text& filename);
virtual	~TextScanner (void);
virtual	void					BuildScanner (void);
	public:
		const enum
		{
			DEFAULT_STATE = ScannerState::DEFAULT_STATE,
			IDENTIFIER_STATE,
			WHITESPACE_STATE,
			NEWLINE_STATE_1,
			NEWLINE_STATE_2
		};
};

//------------------------------------------------------------------------------

#endif	//	_BW_TEXT_SCANNER_H_
