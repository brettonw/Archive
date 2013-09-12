//------------------------------------------------------------------------------
//	Copyright (C) 1997-2000 Bretton Wade
//	All Rights Reserved
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//	includes
//------------------------------------------------------------------------------
#include	"precompile.h"
#include	"scrolling_window.h"

//------------------------------------------------------------------------------
//	class methods
//------------------------------------------------------------------------------
ScrollingWindow::ScrollingWindow
(
	const Text&			className,
	const Text&			windowName,
	uInt				scrollType,
	DWORD				windowStyle,
	DWORD				extendedWindowStyle,
	sInt				x,
	sInt				y,
	sInt				width,
	sInt				height
) : Window (className, windowName, windowStyle bitor ((scrollType bitand SCROLL_HORIZONTAL) ? WS_HSCROLL : 0) bitor ((scrollType bitand SCROLL_VERTICAL) ? WS_VSCROLL : 0), extendedWindowStyle, x, y, width, height)
{
	myScrollType = scrollType;
	myHScrollSize = myVScrollSize = 1;
	if (myScrollType bitand SCROLL_HORIZONTAL)
	{
		myHScroll.cbSize = sizeof (SCROLLINFO);
		myHScroll.fMask = SIF_ALL bitor SIF_DISABLENOSCROLL;
		myHScroll.nMin = 0;
		myHScroll.nMax = 0;
		myHScroll.nPage = 0;
		myHScroll.nPos = 0;
		SetScrollInfo (myWindow, SB_HORZ, &myHScroll, true);
	}
	if (myScrollType bitand SCROLL_VERTICAL)
	{
		myVScroll.cbSize = sizeof (SCROLLINFO);
		myVScroll.fMask = SIF_ALL bitor SIF_DISABLENOSCROLL;
		myVScroll.nMin = 0;
		myVScroll.nMax = 0;
		myVScroll.nPage = 0;
		myVScroll.nPos = 0;
		SetScrollInfo (myWindow, SB_VERT, &myVScroll, true);
	}
}

//------------------------------------------------------------------------------
ScrollingWindow::~ScrollingWindow
(
	void
)
{
}

//------------------------------------------------------------------------------
void
ScrollingWindow::SetScrollSize
(
	sInt				x,
	sInt				y
)
{
	myHScrollSize = x;
	myVScrollSize = y;
}

//------------------------------------------------------------------------------
void
ScrollingWindow::SetScrollRange
(
	sInt				width,
	sInt				height,
	sInt				maxWidth,
	sInt				maxHeight
)
{
	if (myScrollType bitand SCROLL_HORIZONTAL)
	{
		myHScroll.fMask = SIF_PAGE bitor SIF_RANGE bitor SIF_DISABLENOSCROLL;
		myHScroll.nPage = width;
		myHScroll.nMax = maxWidth;
		SetScrollInfo (myWindow, SB_HORZ, &myHScroll, true);
	}
	if (myScrollType bitand SCROLL_VERTICAL)
	{
		myVScroll.fMask = SIF_PAGE bitor SIF_RANGE bitor SIF_DISABLENOSCROLL;
		myVScroll.nPage = height;
		myVScroll.nMax = maxHeight;
		SetScrollInfo (myWindow, SB_VERT, &myVScroll, true);
	}
}

//------------------------------------------------------------------------------
bool
ScrollingWindow::OnHScroll
(
	sInt				code,
	sInt				value
)
{
	sInt	newPos = myHScroll.nPos,
			maxPos = myHScroll.nMax - max (myHScroll.nPage - 1, 0);

	// if the window has more data than can be seen on one window
	if (maxPos > 0)
	{
		// figure out what scrolling to do
		switch (code)
		{
			case SB_BOTTOM:
				newPos = myHScroll.nMax;
				break;
			case SB_ENDSCROLL:
				break;
			case SB_LINELEFT:
				newPos--;
				newPos = (newPos >= myHScroll.nMin) ? newPos : myHScroll.nMin;
				break;
			case SB_LINERIGHT:
				newPos++;
				newPos = (newPos <= maxPos) ? newPos : maxPos;
				break;
			case SB_PAGELEFT:
				newPos -= myHScroll.nPage;
				newPos = (newPos >= myHScroll.nMin) ? newPos : myHScroll.nMin;
				break;
			case SB_PAGERIGHT:
				newPos += myHScroll.nPage;
				newPos = (newPos <= maxPos) ? newPos : maxPos;
				break;
			case SB_THUMBPOSITION:
			case SB_THUMBTRACK:
				newPos = value;
				break;
			case SB_TOP:
				newPos = myHScroll.nMin;
				break;
		}

		// now do the scrolling
		sInt	delta = myHScroll.nPos - newPos;
		if (delta != 0)
		{
			myHScroll.fMask = SIF_POS bitor SIF_DISABLENOSCROLL;
			myHScroll.nPos = newPos;
			SetScrollInfo (myWindow, SB_HORZ, &myHScroll, true);
			ScrollWindow (myWindow, delta * myHScrollSize, 0, 0, 0);
		}
	}
	return true;
}

//------------------------------------------------------------------------------
bool
ScrollingWindow::OnVScroll
(
	sInt				code,
	sInt				value
)
{
	sInt	newPos = myVScroll.nPos,
			maxPos = myVScroll.nMax - max (myVScroll.nPage - 1, 0);

	// if the window has more data than can be seen on one window
	if (maxPos > 0)
	{
		// figure out what scrolling to do
		switch (code)
		{
			case SB_BOTTOM:
				newPos = myVScroll.nMax;
				break;
			case SB_ENDSCROLL:
				break;
			case SB_LINEDOWN:
				newPos++;
				newPos = (newPos <= maxPos) ? newPos : maxPos;
				break;
			case SB_LINEUP:
				newPos--;
				newPos = (newPos >= myVScroll.nMin) ? newPos : myVScroll.nMin;
				break;
			case SB_PAGEDOWN:
				newPos += myVScroll.nPage;
				newPos = (newPos <= maxPos) ? newPos : maxPos;
				break;
			case SB_PAGEUP:
				newPos -= myVScroll.nPage;
				newPos = (newPos >= myVScroll.nMin) ? newPos : myVScroll.nMin;
				break;
			case SB_THUMBPOSITION:
			case SB_THUMBTRACK:
				newPos = value;
				break;
			case SB_TOP:
				newPos = myVScroll.nMin;
				break;
		}

		// now do the scrolling
		sInt	delta = myVScroll.nPos - newPos;
		if (delta != 0)
		{
			myVScroll.fMask = SIF_POS bitor SIF_DISABLENOSCROLL;
			myVScroll.nPos = newPos;
			SetScrollInfo (myWindow, SB_VERT, &myVScroll, true);
			ScrollWindow (myWindow, 0, delta * myVScrollSize, 0, 0);
		}
	}
	return true;
}

//------------------------------------------------------------------------------
void
ScrollingWindow::HScrollTo
(
	uInt				value
)
{
	// if the place we want is before the start of the display
	if ((value < uInt (myHScroll.nPos)) or (value >= (myHScroll.nPos + myHScroll.nPage)))
	{
		// compute where we want the display to be
		uInt	partPage = myHScroll.nPage / 2;
		uInt	displayValue = (value > partPage) ? value - partPage : 0;

		// scroll the window to put the desired place in the middle
		OnHScroll (SB_THUMBPOSITION, displayValue);
	}
}

//------------------------------------------------------------------------------
void
ScrollingWindow::VScrollTo
(
	uInt				value
)
{
	// if the place we want is before the start of the display
	if ((value < uInt (myVScroll.nPos)) or (value >= (myVScroll.nPos + myVScroll.nPage)))
	{
		// compute where we want the display to be
		uInt	partPage = myVScroll.nPage / 2;
		uInt	displayValue = (value > partPage) ? value - partPage : 0;

		// scroll the window to put the desired place in the middle
		OnVScroll (SB_THUMBPOSITION, displayValue);
	}
}

//------------------------------------------------------------------------------
LRESULT CALLBACK
ScrollingWindow::WindowProc
(
	HWND				window,
	UINT				message,
	WPARAM				wParam,
	LPARAM				lParam
)
{
	ScrollingWindow	*theWindow = (ScrollingWindow *) GetWindowLong (window, GWL_USERDATA);
	if (theWindow)
		switch (message)
		{
			case WM_HSCROLL:
				if (theWindow->OnHScroll (sInt (LOWORD (wParam)), sInt (HIWORD (wParam))))
					return 0;
				break;
			case WM_VSCROLL:
				if (theWindow->OnVScroll (sInt (LOWORD (wParam)), sInt (HIWORD (wParam))))
					return 0;
				break;
		}
	return Window::WindowProc (window, message, wParam, lParam);
}

//------------------------------------------------------------------------------
ScrollingWindow::ScrollingWindow
(
	const ScrollingWindow&		window
) : Window (window)
{
	myScrollType = window.myScrollType;
	myHScrollSize = window.myHScrollSize;
	myVScrollSize = window.myVScrollSize;
	myHScroll = window.myHScroll;
	myVScroll = window.myVScroll;
}

//------------------------------------------------------------------------------
