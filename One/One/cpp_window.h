//------------------------------------------------------------------------------
//	Copyright (C) 1997-2000 Bretton Wade
//	All Rights Reserved
//------------------------------------------------------------------------------

#ifndef		_BW_CPP_WINDOW_H_
#define		_BW_CPP_WINDOW_H_

//------------------------------------------------------------------------------
//	includes
//------------------------------------------------------------------------------
#ifndef		_BW_TEXT_WINDOW_H_
#include	"text_window.h"
#endif	//	_BW_TEXT_WINDOW_H_

//------------------------------------------------------------------------------
//	class definitions
//------------------------------------------------------------------------------
class	CPPWindow : public TextWindow
{
	public:
		CPPWindow (const Text& filename, uInt width, uInt height);
		CPPWindow (const TextWindow& textWindow);
virtual	~CPPWindow (void);
virtual	void					DrawAction (cString buffer, uInt length, uInt2 action, cString nextBuffer, uInt2 nextState);
static	void					Startup (void);
	protected:
static	HandleTo<TextScanner>	myCPPScanner;
};

//------------------------------------------------------------------------------

#endif	//	_BW_CPP_WINDOW_H_
