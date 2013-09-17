//-----------------------------------------------------------------------------
// Copyright (C) 1997-2006 Bretton Wade
// All Rights Reserved
//-----------------------------------------------------------------------------

#ifndef     _WINDOW_H_
#define     _WINDOW_H_

//-----------------------------------------------------------------------------
// include files
//-----------------------------------------------------------------------------
#ifndef     _TEXT_H_
#include    "text.h"
#endif  //  _TEXT_H_

//-----------------------------------------------------------------------------
// globals
//-----------------------------------------------------------------------------

// there is only ever one window for a game application. This is the handle to
// it should you ever need access to that handle for anything. The window is 
// created at a default size, and resized by the DirectX device creation
extern HWND     g_windowHandle;

// this value is set by the activation and deactivation of the main window, 
// when the window is dropped to the background, this value is set to true and
// heartbeat processing is halted. when the application is brought back to the
// foreground, this value is set to false and heartbeat processing resumes
extern bool     g_bApplicationPaused;

//-----------------------------------------------------------------------------

#endif  //  _WINDOW_H_
