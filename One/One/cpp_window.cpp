//------------------------------------------------------------------------------
//	Copyright (C) 1997-2000 Bretton Wade
//	All Rights Reserved
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//	includes
//------------------------------------------------------------------------------
#include	"precompile.h"
#include	"cpp_window.h"
#include	"cpp_scanner.h"
#include	"cpp_scanner_action.h"
#include	"task.h"
#include	"resources.h"

//------------------------------------------------------------------------------
//	static members
//------------------------------------------------------------------------------
HandleTo<TextScanner>	CPPWindow::myCPPScanner;

//--------------------------------------------------------------------------
//	tasks
//--------------------------------------------------------------------------
DEFINE_TASK (CPPWindow)
{
    DO_AFTER (TextWindow);
}

//--------------------------------------------------------------------------
ON_STARTUP (CPPWindow)
{
	CPPWindow::Startup ();
}

//--------------------------------------------------------------------------
ON_SHUTDOWN (CPPWindow)
{
}

//--------------------------------------------------------------------------
//	class methods
//------------------------------------------------------------------------------
CPPWindow::CPPWindow
(
	const Text&			filename,
	uInt				width,
	uInt				height
) : TextWindow (myCPPScanner, filename, width, height)
{
	// clear the check on ENGLISH and set the check on LANG_C
	HMENU	hMenu = GetMenu (myWindow);
	CheckMenuItem (hMenu, MENU_LANG_ENGLISH, MF_UNCHECKED);
	CheckMenuItem (hMenu, MENU_LANG_C, MF_CHECKED);
}

//------------------------------------------------------------------------------
CPPWindow::CPPWindow
(
	const TextWindow&	textWindow
) : TextWindow (myCPPScanner, textWindow)
{
	// clear the check on ENGLISH and set the check on LANG_C
	HMENU	hMenu = GetMenu (myWindow);
	CheckMenuItem (hMenu, MENU_LANG_ENGLISH, MF_UNCHECKED);
	CheckMenuItem (hMenu, MENU_LANG_C, MF_CHECKED);
}

//------------------------------------------------------------------------------
CPPWindow::~CPPWindow
(
	void
)
{
}

//------------------------------------------------------------------------------
void
CPPWindow::DrawAction
(
	cString				buffer,
	uInt				length,
	uInt2				action,
	cString				nextBuffer,
	uInt2				nextState
)
{
	switch (action)
	{
		case CPPScannerAction::OUTPUT_IDENTIFIER:
			SetTextColor (myDC, PALETTERGB (0, 0, 0));
			break;
		case CPPScannerAction::OUTPUT_PREPROCESSOR:
			SetTextColor (myDC, PALETTERGB (0, 64, 0));
			break;
		case CPPScannerAction::OUTPUT_HEADER_NAME:
			SetTextColor (myDC, PALETTERGB (128, 0, 128));
			break;
		case CPPScannerAction::OUTPUT_KEYWORD:
			SetTextColor (myDC, PALETTERGB (0, 0, 128));
			break;
		case CPPScannerAction::OUTPUT_INTEGER_LITERAL:
			SetTextColor (myDC, PALETTERGB (0, 64, 0));
			break;
		case CPPScannerAction::OUTPUT_CHARACTER_LITERAL:
			SetTextColor (myDC, PALETTERGB (128, 128, 128));
			break;
		case CPPScannerAction::OUTPUT_FLOATING_LITERAL:
			SetTextColor (myDC, PALETTERGB (128, 128, 0));
			break;
		case CPPScannerAction::OUTPUT_STRING_LITERAL:
			SetTextColor (myDC, PALETTERGB (128, 0, 0));
			break;
		case CPPScannerAction::OUTPUT_BOOLEAN_LITERAL:
			SetTextColor (myDC, PALETTERGB (0, 0, 128));
			break;
		case CPPScannerAction::OUTPUT_OPERATOR:
			SetTextColor (myDC, PALETTERGB (0, 0, 0));
			break;
		case CPPScannerAction::OUTPUT_ALTERNATE:
			SetTextColor (myDC, PALETTERGB (0, 0, 255));
			break;
		case CPPScannerAction::OUTPUT_COMMENT:
			SetTextColor (myDC, PALETTERGB (128, 128, 128));
			break;
		/*
		case CPPScannerAction::OUTPUT_INVALID_CHAR:
			SetTextColor (myDC, PALETTERGB (255, 0, 0));
			break;
		case CPPScannerAction::OUTPUT_BAD_INTEGER_LITERAL:
			SetTextColor (myDC, PALETTERGB (64, 0, 64));
			break;
		case CPPScannerAction::OUTPUT_UNTERMINATED_ERROR:
			SetTextColor (myDC, PALETTERGB (128, 128, 0));
			break;
		case CPPScannerAction::OUTPUT_BAD_ESCAPE:
			SetTextColor (myDC, PALETTERGB (255, 0, 255));
			break;
		*/
		default:
			SetTextColor (myDC, PALETTERGB (255, 0, 0));
			break;
	}
	TextWindow::DrawAction (buffer, length, action, nextBuffer, nextState);
}

//------------------------------------------------------------------------------
void
CPPWindow::Startup
(
	void
)
{
	myCPPScanner = new CPPScanner;
}

//------------------------------------------------------------------------------
