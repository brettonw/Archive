//------------------------------------------------------------------------------
//	Copyright (C) 1997-2000 Bretton Wade
//	All Rights Reserved
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//	includes
//------------------------------------------------------------------------------
#include	"precompile.h"
#include	"text_scanner.h"
#include	"text_scanner_action.h"
#include	"directory.h"

//------------------------------------------------------------------------------
//	class methods
//------------------------------------------------------------------------------
TextScanner::TextScanner
(
	void
) : Scanner (GetDataFileName ("Scanners", "TextScanner.stb"))
{
	if (not myTableWasLoaded)
		BuildScanner ();
}

//------------------------------------------------------------------------------
TextScanner::TextScanner
(
	const Text&				filename
) : Scanner (filename)
{
}

//------------------------------------------------------------------------------
TextScanner::~TextScanner
(
	void
)
{
}

//------------------------------------------------------------------------------
void
TextScanner::BuildScanner
(
	void
)
{
	//	XXX	this is not final!
	ResetScanner ();
	OnInput				(DEFAULT_STATE,				"\0",		DEFAULT_STATE,				DONT_STORE_INPUT,		TextScannerAction::OUTPUT_IDENTIFIER				);
	OnInput				(DEFAULT_STATE,				"\n",		NEWLINE_STATE_1,			DONT_STORE_INPUT,		TextScannerAction::NO_ACTION						);
	OnInput				(DEFAULT_STATE,				"\r",		NEWLINE_STATE_2,			DONT_STORE_INPUT,		TextScannerAction::NO_ACTION						);
	OnInput				(DEFAULT_STATE,				"\t ",		WHITESPACE_STATE,			STORE_INPUT_BEFORE,		TextScannerAction::NO_ACTION						);
	OnAnyInput			(DEFAULT_STATE,							IDENTIFIER_STATE,			STORE_INPUT_BEFORE,		TextScannerAction::NO_ACTION						);

	OnInput				(IDENTIFIER_STATE,			"\0",		DEFAULT_STATE,				DONT_STORE_INPUT,		TextScannerAction::OUTPUT_IDENTIFIER				);
	OnInput				(IDENTIFIER_STATE,			"\n\r\t ",	DEFAULT_STATE,				STORE_INPUT_AFTER,		TextScannerAction::OUTPUT_IDENTIFIER				);
	OnAnyInput			(IDENTIFIER_STATE,						IDENTIFIER_STATE,			STORE_INPUT_BEFORE,		TextScannerAction::NO_ACTION						);

	OnInput				(WHITESPACE_STATE,			"\t ",		WHITESPACE_STATE,			STORE_INPUT_BEFORE,		TextScannerAction::NO_ACTION						);
	OnAnyInput			(WHITESPACE_STATE,						DEFAULT_STATE,				STORE_INPUT_AFTER,		TextScannerAction::OUTPUT_WHITESPACE				);

	OnInput				(NEWLINE_STATE_1,			"\r",		DEFAULT_STATE,				DONT_STORE_INPUT,		TextScannerAction::OUTPUT_NEWLINE					);
	OnAnyInput			(NEWLINE_STATE_1,						DEFAULT_STATE,				STORE_INPUT_AFTER,		TextScannerAction::OUTPUT_NEWLINE					);

	OnInput				(NEWLINE_STATE_2,			"\n",		DEFAULT_STATE,				DONT_STORE_INPUT,		TextScannerAction::OUTPUT_NEWLINE					);
	OnAnyInput			(NEWLINE_STATE_2,						DEFAULT_STATE,				STORE_INPUT_AFTER,		TextScannerAction::OUTPUT_NEWLINE					);

	SaveStateTable (GetDataFileName ("Scanners", "TextScanner.stb"));
}

//------------------------------------------------------------------------------
