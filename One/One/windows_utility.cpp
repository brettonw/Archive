//------------------------------------------------------------------------------
//	Copyright (C) 1997-2000 Bretton Wade
//	All Rights Reserved
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//	includes
//------------------------------------------------------------------------------
#include	"precompile.h"
#include	"windows_utility.h"

//------------------------------------------------------------------------------
//	callback functions
//------------------------------------------------------------------------------
BOOL CALLBACK
MyCountWindowsProc
(
	HWND			window,
	LPARAM			lParam
)
{
	uInt4*	windowCount = (uInt4*) lParam;
	(*windowCount)++;
	return true;
}

//------------------------------------------------------------------------------
BOOL CALLBACK
MyCloseWindowsProc
(
	HWND			window,
	LPARAM			lParam
)
{
	SendMessage (window, WM_CLOSE, 0, 0);
	if (IsWindow (window))
		return false;
	return true;
}

//------------------------------------------------------------------------------
//	functions
//------------------------------------------------------------------------------
uInt4
MyCountWindows
(
	void
)
{
	uInt4	windowCount = 0;
	DWORD	thread = GetCurrentThreadId ();
	EnumThreadWindows (thread, WNDENUMPROC (MyCountWindowsProc), LPARAM (&windowCount));
	return windowCount;
}

//------------------------------------------------------------------------------
void
MyCloseAllWindows
(
	void
)
{
	DWORD	thread = GetCurrentThreadId ();
	EnumThreadWindows (thread, WNDENUMPROC (MyCloseWindowsProc), 0);
}

//------------------------------------------------------------------------------
