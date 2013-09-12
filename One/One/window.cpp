//------------------------------------------------------------------------------
//	Copyright (C) 1997-2000 Bretton Wade
//	All Rights Reserved
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//	includes
//------------------------------------------------------------------------------
#include	"precompile.h"
#include	"application.h"
#include	"application_body.h"
#include	"window.h"
#include	"error_handler.h"

//------------------------------------------------------------------------------
//	class methods
//------------------------------------------------------------------------------
Window::Window
(
	const Text&			className,
	const Text&			windowName,
	DWORD				windowStyle,
	DWORD				extendedWindowStyle,
	sInt				x,
	sInt				y,
	sInt				width,
	sInt				height,
	Window*				parent,
	uInt				id
)
{
	assert (Application::IsValid ());
	myMouseIsDown = 0;
	WNDCLASSEX	windowClass;
	HINSTANCE	instance = Application::GetInstance ();
	RECT		rect;
	windowClass.cbSize = sizeof(WNDCLASSEX);
	TestFailZero (GetClassInfoEx (instance, className, &windowClass));
	TestFailZero (SetRect (&rect, 0, 0, width, height));
	TestFailZero (AdjustWindowRectEx (&rect, windowStyle, windowClass.lpszMenuName ? true : false, extendedWindowStyle));
	myWidthDelta = ((rect.right - rect.left) - width) + ((windowStyle bitand WS_VSCROLL) ? GetSystemMetrics (SM_CXVSCROLL) : 0);
	myHeightDelta = ((rect.bottom - rect.top) - height) + ((windowStyle bitand WS_HSCROLL) ?  GetSystemMetrics (SM_CYHSCROLL) : 0);
    TestFailZero (myWindow = CreateWindowEx (extendedWindowStyle, className, windowName.GetText (), windowStyle, x, y, width + myWidthDelta, height + myHeightDelta, parent ? parent->myWindow : 0, HMENU (id), Application::GetInstance (), 0));
	TestFailZero (SetWindowLong (myWindow, GWL_USERDATA, int (this)));
}

//------------------------------------------------------------------------------
Window::~Window
(
	void
)
{
	//	note that window destruction actually occurs through the messaging process
	//	and the close window method
}

//------------------------------------------------------------------------------
bool
Window::OnCloseWindow
(
	void
)
{
	assert (ApplicationBody::IsValid ());
	HWND	child = GetWindow (myWindow, GW_CHILD);
	while (child)
	{
		Window	*window = (Window *) GetWindowLong (myWindow, GWL_USERDATA);
		if (window and (not window->OnCloseWindow ()))
			return false;
		child = GetNextWindow (child, GW_HWNDNEXT);
	}
	TestFailZero (SetWindowLong (myWindow, GWL_USERDATA, 0));
	return true;
}

//------------------------------------------------------------------------------
void
Window::Pulse
(
	void
)
{
}

//------------------------------------------------------------------------------
bool
Window::OnPaintWindow
(
	void
)
{
	bool		handled = false;
	if (GetUpdateRect (myWindow, 0, false))
	{
		PAINTSTRUCT	paint;
		TestFailZero (BeginPaint (myWindow, &paint));
		handled = OnPaintWindow (paint);
		EndPaint (myWindow, &paint);
	}
	return handled;
}

//------------------------------------------------------------------------------
bool
Window::OnEraseWindow
(
	HDC					dc
)
{
	WNDCLASSEX	windowClass;
	HINSTANCE	instance = Application::GetInstance ();
	windowClass.cbSize = sizeof(WNDCLASSEX);
	TestFailZero (GetClassInfoEx (instance, GetWindowClassName (), &windowClass));
	if (windowClass.hbrBackground)
	{
		RECT		clientRect;
		TestFailZero (GetClientRect (myWindow, &clientRect));
		TestFailZero (FillRect (dc, &clientRect, windowClass.hbrBackground));
	}
	return true;
}

//------------------------------------------------------------------------------
bool
Window::OnKeyDown
(
	LPMSG				message
)
{
	if (OnKeyDown (message->wParam, message->lParam))
		return true;
	HACCEL	accelerators = GetWindowAccelerators ();
	if (accelerators)
		if (TranslateAccelerator (myWindow, accelerators, message))
			return true;
	if (TranslateMessage (message))
		return true;
	return false;
}

//------------------------------------------------------------------------------
bool
Window::OnKeyDown
(
	sInt				virtualKey,
	uInt				data
)
{
	return false;
}

//------------------------------------------------------------------------------
bool
Window::OnCharDown
(
	uInt				keyChar,
	sInt				data
)
{
	return false;
}

//------------------------------------------------------------------------------
bool
Window::OnCommand
(
	sInt				command
)
{
	return false;
}

//------------------------------------------------------------------------------
bool
Window::OnMouseDown
(
	uInt				which,
	const POINTS&		point,
	uInt				flags
)
{
	myMouseIsDown |= which;
	myMouseDownPoint = point;
	SetCapture (myWindow);
	return true;
}

//------------------------------------------------------------------------------
bool
Window::OnMouseMove
(
	const POINTS&		point,
	uInt				flags
)
{
    return true;
}

//------------------------------------------------------------------------------
bool
Window::OnMouseUp
(
	uInt				which,
	const POINTS&		point,
	uInt				flags
)
{
	TestFailZero (ReleaseCapture ());
	if (myMouseIsDown bitand which)
		myMouseIsDown ^= which;
	return true;
}

//------------------------------------------------------------------------------
bool
Window::OnActivate
(
	void
)
{
	return false;
}

//------------------------------------------------------------------------------
bool
Window::OnDeactivate
(
	void
)
{
	return false;
}

//------------------------------------------------------------------------------
bool
Window::OnSizingWindow
(
	sInt				side,
	RECT*				rect
)
{
	RECT	clientRect;
	clientRect.left = clientRect.top = 0;
	clientRect.right = (rect->right - rect->left) - myWidthDelta;
	clientRect.bottom = (rect->bottom - rect->top) - myHeightDelta;
	HWND	child = GetWindow (myWindow, GW_CHILD);
	while (child)
	{
		Window	*window = (Window *) GetWindowLong (myWindow, GWL_USERDATA);
		if (window)
		{
			RECT	windowRect;
			GetWindowRect (child, &windowRect);
			ScreenToClient (myWindow, LPPOINT (&windowRect.left));
			ScreenToClient (myWindow, LPPOINT (&windowRect.right));
			//	XXX possibly a different algorithm depending on the 'side'
			//	XXX needs flags to describe positioning algorithm...
			if (windowRect.right > clientRect.right)
				windowRect.right = clientRect.right;
			if (windowRect.bottom > clientRect.bottom)
				windowRect.bottom = clientRect.bottom;
			window->OnSizingWindow (side, &windowRect);
		}
		child = GetNextWindow (child, GW_HWNDNEXT);
	}
	SizeToChildren (&clientRect);
	return true;
}

//------------------------------------------------------------------------------
void
Window::SizeWindow
(
	sInt				width,
	sInt				height
)
{
	TestFailZero (SetWindowPos (myWindow, 0, 0, 0, width + myWidthDelta, height + myHeightDelta, SWP_NOACTIVATE bitor SWP_NOZORDER bitor SWP_NOMOVE bitor SWP_NOSENDCHANGING));
}

//------------------------------------------------------------------------------
void
Window::SizeToChildren
(
	RECT*				rect
)
{
}

//------------------------------------------------------------------------------
void
Window::SetCursor
(
	void
)
{
	::SetCursor (LoadCursor (0, IDC_CROSS));
}

//------------------------------------------------------------------------------
void
Window::ShowWindow
(
	bool				show
)
{
	::ShowWindow (myWindow, show ? SW_SHOWNORMAL : SW_HIDE);
}

//------------------------------------------------------------------------------
void
Window::SetName
(
	const Text&			windowName
)
{
	::SetWindowText (myWindow, windowName);
}

//------------------------------------------------------------------------------
void
Window::RegisterWindowClass
(
	DWORD				classStyle,
	WNDPROC				windowProc,
	const Text&			className,
	uInt				menuID
)
{
	WNDCLASSEX	windowClass;
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.style = classStyle;
	if (windowProc)
		windowClass.lpfnWndProc = windowProc;
	else
		windowClass.lpfnWndProc = WindowProc;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = sizeof (uInt);
	windowClass.hInstance = Application::GetInstance ();
	windowClass.hIcon = LoadIcon (0, IDI_APPLICATION);
	windowClass.hCursor = LoadCursor (0, IDC_ARROW);
	#ifdef      _DEBUG
	windowClass.hbrBackground = CreateSolidBrush (PALETTERGB (255, 0, 0));
	#else   //  _DEBUG
	windowClass.hbrBackground = CreateSolidBrush (PALETTERGB (240, 240, 240));
	#endif  //  _DEBUG
	windowClass.lpszMenuName = menuID ? MAKEINTRESOURCE (menuID) : LPTSTR (menuID);
	windowClass.lpszClassName = className;
	windowClass.hIconSm = 0;
	TestFailZero (RegisterClassEx (&windowClass));
}

//------------------------------------------------------------------------------
LRESULT CALLBACK
Window::WindowProc
(
	HWND				window,
	UINT				message,
	WPARAM				wParam,
	LPARAM				lParam
)
{
	Window	*theWindow = (Window *) GetWindowLong (window, GWL_USERDATA);
	if (theWindow)
		switch (message)
		{
			case WM_CLOSE:
				if (theWindow->OnCloseWindow () == true)
				{
					delete theWindow;
					TestFailZero (DestroyWindow (window));
				}
				return 0;
			case WM_ERASEBKGND:
				if (theWindow->OnEraseWindow (HDC (wParam)))
					return 1;
				break;
			case WM_PAINT:
				if (theWindow->OnPaintWindow ())
					return 0;
				break;
			case WM_KEYDOWN:
			case WM_KEYUP:
				assert (ApplicationBody::GetCurrentMessage ()->message == message);
				if (theWindow->OnKeyDown (ApplicationBody::GetCurrentMessage ()))
					return 0;
				break;
			case WM_CHAR:
				if (theWindow->OnCharDown (wParam, lParam))
					return 0;
				break;
			case WM_COMMAND:
				if (theWindow->OnCommand (LOWORD (wParam)))
					return 0;
				break;
			case WM_LBUTTONDOWN:
				if (theWindow->OnMouseDown (LEFT_BUTTON, MAKEPOINTS (lParam), wParam))
					return 0;
				break;
			case WM_MBUTTONDOWN:
				if (theWindow->OnMouseDown (MIDDLE_BUTTON, MAKEPOINTS (lParam), wParam))
					return 0;
				break;
			case WM_RBUTTONDOWN:
				if (theWindow->OnMouseDown (RIGHT_BUTTON, MAKEPOINTS (lParam), wParam))
					return 0;
				break;
			case WM_MOUSEMOVE:
				if (theWindow->OnMouseMove (MAKEPOINTS (lParam), wParam))
				    return 0;
				break;
			case WM_LBUTTONUP:
				if (theWindow->OnMouseUp (LEFT_BUTTON, MAKEPOINTS (lParam), wParam))
					return 0;
				break;
			case WM_MBUTTONUP:
				if (theWindow->OnMouseUp (MIDDLE_BUTTON, MAKEPOINTS (lParam), wParam))
					return 0;
				break;
			case WM_RBUTTONUP:
				if (theWindow->OnMouseUp (RIGHT_BUTTON, MAKEPOINTS (lParam), wParam))
					return 0;
				break;
			case WM_ACTIVATE:
				if (LOWORD (wParam) == WA_INACTIVE)
				{
					if (theWindow->OnDeactivate ())
						return 0;
				}
				else if (theWindow->OnActivate ())
					return 0;
				break;
			case WM_SIZING:
				if (theWindow->OnSizingWindow (sInt (wParam), (RECT*) lParam))
					return TRUE;
				break;
			case WM_SETCURSOR:
				if (LOWORD (lParam) == HTCLIENT)
				{
					theWindow->SetCursor ();
					return TRUE;
				}
				break;
		}
	return DefWindowProc (window, message, wParam, lParam);
}

//------------------------------------------------------------------------------
Window::Window
(
	const Window&		window
)
{
	myWindow = window.myWindow;
	myMouseIsDown = window.myMouseIsDown;
	myMouseDownPoint = window.myMouseDownPoint;
	myWidthDelta = window.myWidthDelta;
	myHeightDelta = window.myHeightDelta;
	TestFailZero (SetWindowLong (myWindow, GWL_USERDATA, int (this)));
}

//------------------------------------------------------------------------------
