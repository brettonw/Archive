//------------------------------------------------------------------------------
//	Copyright (C) 1997-2018 Bretton Wade
//	All Rights Reserved
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//	includes
//------------------------------------------------------------------------------
#include	"precompile.h"
#include	"cmd_line_scanner.h"
#include	"cmd_line_scanner_action.h"
#include	"directory.h"

//------------------------------------------------------------------------------
//	class methods
//------------------------------------------------------------------------------
CmdLineScanner::CmdLineScanner(void) : Scanner(GetDataFileName("Scanners", "CmdLineScanner.stb")) {
    if (not myTableWasLoaded)
        BuildScanner();
}

//------------------------------------------------------------------------------
CmdLineScanner::CmdLineScanner(const Text& filename) : Scanner(filename) {}

//------------------------------------------------------------------------------
CmdLineScanner::~CmdLineScanner(void) {}

//------------------------------------------------------------------------------
void CmdLineScanner::BuildScanner(void) {
    ResetScanner();
    OnInput(DEFAULT_STATE, "\0", DEFAULT_STATE, DONT_STORE_INPUT, CmdLineScannerAction::OUTPUT_IDENTIFIER);
    OnInput(DEFAULT_STATE, "\t ", WHITESPACE_STATE, STORE_INPUT_BEFORE, CmdLineScannerAction::NO_ACTION);
    OnInput(DEFAULT_STATE, "\"", QUOTE_STATE, STORE_INPUT_BEFORE, CmdLineScannerAction::OUTPUT_OPEN_QUOTE);
    OnAnyInput(DEFAULT_STATE, IDENTIFIER_STATE, STORE_INPUT_BEFORE, CmdLineScannerAction::NO_ACTION);

    OnInput(IDENTIFIER_STATE, "\0", DEFAULT_STATE, DONT_STORE_INPUT, CmdLineScannerAction::OUTPUT_IDENTIFIER);
    OnInput(IDENTIFIER_STATE, "\t ", DEFAULT_STATE, STORE_INPUT_AFTER, CmdLineScannerAction::OUTPUT_IDENTIFIER);
    OnAnyInput(IDENTIFIER_STATE, IDENTIFIER_STATE, STORE_INPUT_BEFORE, CmdLineScannerAction::NO_ACTION);

    OnInput(WHITESPACE_STATE, "\t ", WHITESPACE_STATE, STORE_INPUT_BEFORE, CmdLineScannerAction::NO_ACTION);
    OnAnyInput(WHITESPACE_STATE, DEFAULT_STATE, STORE_INPUT_AFTER, CmdLineScannerAction::OUTPUT_WHITESPACE);

    OnInput(QUOTE_STATE, "\0", DEFAULT_STATE, DONT_STORE_INPUT, CmdLineScannerAction::OUTPUT_UNCLOSED_QUOTED_IDENTIFER);
    OnInput(QUOTE_STATE, "\"", CLOSE_QUOTE_STATE, STORE_INPUT_AFTER, CmdLineScannerAction::OUTPUT_QUOTED_IDENTIFIER);
    OnAnyInput(QUOTE_STATE, QUOTE_STATE, STORE_INPUT_BEFORE, CmdLineScannerAction::NO_ACTION);

    OnAnyInput(CLOSE_QUOTE_STATE, DEFAULT_STATE, STORE_INPUT_BEFORE, CmdLineScannerAction::OUTPUT_CLOSE_QUOTE);

    SaveStateTable(GetDataFileName("Scanners", "CmdLineScanner.stb"));
}

//------------------------------------------------------------------------------
