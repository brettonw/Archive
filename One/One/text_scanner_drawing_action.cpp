//------------------------------------------------------------------------------
//	Copyright (C) 1997-2000 Bretton Wade
//	All Rights Reserved
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//	includes
//------------------------------------------------------------------------------
#include	"precompile.h"
#include	"text_scanner_drawing_action.h"

//------------------------------------------------------------------------------
//	class methods
//------------------------------------------------------------------------------
TextScannerDrawingAction::TextScannerDrawingAction
(
	TextWindow*				window
)
{
	myWindow = window;
}

//------------------------------------------------------------------------------
TextScannerDrawingAction::~TextScannerDrawingAction
(
	void
)
{
}

//------------------------------------------------------------------------------
void
TextScannerDrawingAction::Action
(
	cString				buffer,
	uInt				length,
	uInt2				action,
	cString				nextBuffer,
	uInt2				nextState
)
{
	myWindow->DrawAction (buffer, length, action, nextBuffer, nextState);
}

//------------------------------------------------------------------------------
