//------------------------------------------------------------------------------
//	Copyright (C) 1997-2000 Bretton Wade
//	All Rights Reserved
//------------------------------------------------------------------------------

#ifndef		_BW_WINDOW_H_
#define		_BW_WINDOW_H_

//------------------------------------------------------------------------------
//	includes
//------------------------------------------------------------------------------
#ifndef		_BW_TEXT_H_
#include	"text.h"
#endif	//	_BW_TEXT_H_

//------------------------------------------------------------------------------
//	class definitions
//------------------------------------------------------------------------------
class	Window
{
	public:
		Window (const Text& className, const Text& windowName, DWORD windowStyle, DWORD extendedWindowStyle = 0, sInt x = CW_USEDEFAULT, sInt y = 0, sInt width = CW_USEDEFAULT, sInt height = 0, Window* parent = 0, uInt id = 0);
virtual	~Window (void);
virtual	bool					OnCloseWindow (void);
virtual	void					Pulse (void);
		bool					OnPaintWindow (void);
virtual	bool					OnPaintWindow (PAINTSTRUCT& paint) = 0;
virtual	bool					OnEraseWindow (HDC dc);
		bool					OnKeyDown (LPMSG message);
virtual	bool					OnKeyDown (sInt virtualKeyCode, uInt data);
virtual	bool					OnCharDown (uInt keyChar, sInt data);
virtual	bool					OnCommand (sInt command);
virtual	bool					OnMouseDown (uInt which, const POINTS& point, uInt flags);
virtual	bool					OnMouseMove (const POINTS& point, uInt flags);
virtual	bool					OnMouseUp (uInt which, const POINTS& point, uInt flags);
virtual	bool					OnActivate (void);
virtual	bool					OnDeactivate (void);
virtual	bool					OnSizingWindow (sInt code, RECT* rect);
virtual	void					SizeWindow (sInt width, sInt height);
		void					SizeToChildren (RECT* rect);
virtual	void					SetCursor (void);
		void					ShowWindow (bool show = true);
		void					SetName (const Text& windowName);
virtual	Text					GetWindowClassName (void) = 0;
virtual	HACCEL					GetWindowAccelerators (void) = 0;
static	void					RegisterWindowClass (DWORD classStyle, WNDPROC windowProc, const Text& className, uInt menuID = 0);
static	LRESULT CALLBACK		WindowProc (HWND window, UINT message, WPARAM wParam, LPARAM lParam);
	public:
		const enum
		{
			LEFT_BUTTON = 1,
			RIGHT_BUTTON = 2,
			MIDDLE_BUTTON = 4
		};
	protected:
		Window (const Window& window);
	protected:
		HWND					myWindow;
		sInt					myWidthDelta;
		sInt					myHeightDelta;
		uInt					myMouseIsDown;
		POINTS					myMouseDownPoint;
};

//------------------------------------------------------------------------------

#endif	//	_BW_WINDOW_H_
