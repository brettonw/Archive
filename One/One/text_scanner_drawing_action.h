//------------------------------------------------------------------------------
//	Copyright (C) 1997-2000 Bretton Wade
//	All Rights Reserved
//------------------------------------------------------------------------------

#ifndef		_BW_TEXT_SCANNER_DRAWING_ACTION_H_
#define		_BW_TEXT_SCANNER_DRAWING_ACTION_H_

//------------------------------------------------------------------------------
//	includes
//------------------------------------------------------------------------------
#ifndef		_BW_TEXT_SCANNER_ACTION_H_
#include	"text_scanner_action.h"
#endif	//	_BW_TEXT_SCANNER_ACTION_H_

#ifndef		_BW_TEXT_WINDOW_H_
#include	"text_window.h"
#endif	//	_BW_TEXT_WINDOW_H_

//------------------------------------------------------------------------------
//	forward definitions
//------------------------------------------------------------------------------
class	TextWindow;

//------------------------------------------------------------------------------
//	class definitions
//------------------------------------------------------------------------------
class	TextScannerDrawingAction : public TextScannerAction
{
	public:
		TextScannerDrawingAction (TextWindow* window);
virtual	~TextScannerDrawingAction (void);
virtual	void					Action (cString buffer, uInt length, uInt2 action, cString nextBuffer, uInt2 nextState);
	protected:
		TextWindow*				myWindow;
};

//------------------------------------------------------------------------------

#endif	//	_BW_TEXT_SCANNER_DRAWING_ACTION_H_
