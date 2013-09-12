//------------------------------------------------------------------------------
//	Copyright (C) 1997-2000 Bretton Wade
//	All Rights Reserved
//------------------------------------------------------------------------------

#ifndef		_BW_SCROLLING_WINDOW_H_
#define		_BW_SCROLLING_WINDOW_H_

//------------------------------------------------------------------------------
//	includes
//------------------------------------------------------------------------------
#ifndef		_BW_WINDOW_H_
#include	"window.h"
#endif	//	_BW_WINDOW_H_

//------------------------------------------------------------------------------
//	class definitions
//------------------------------------------------------------------------------
class	ScrollingWindow : public Window
{
	public:
		ScrollingWindow (const Text& className, const Text& windowName, uInt scrollType, DWORD windowStyle, DWORD extendedWindowStyle, sInt x, sInt y, sInt width, sInt height);
virtual	~ScrollingWindow (void);
		void					SetScrollSize (sInt x, sInt y);
		void					SetScrollRange (sInt width, sInt height, sInt maxWidth, sInt maxHeight);
		bool					OnHScroll (sInt code, sInt value = 0);
		bool					OnVScroll (sInt code, sInt value = 0);
		void					HScrollTo (uInt value);
		void					VScrollTo (uInt value);
static	LRESULT CALLBACK		WindowProc (HWND window, UINT message, WPARAM wParam, LPARAM lParam);
	public:
		const enum
		{
			SCROLL_HORIZONTAL = 1,
			SCROLL_VERTICAL = 2,
			SCROLL_BOTH = SCROLL_HORIZONTAL + SCROLL_VERTICAL
		};
	protected:
		ScrollingWindow (const ScrollingWindow& window);
	protected:
		uInt					myScrollType;
		sInt					myHScrollSize;
		sInt					myVScrollSize;
		SCROLLINFO				myHScroll;
		SCROLLINFO				myVScroll;
};

//------------------------------------------------------------------------------

#endif	//	_BW_SCROLLING_WINDOW_H_
