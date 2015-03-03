//-----------------------------------------------------------------------------
// Copyright (C) 2006 Bretton Wade, All Rights Reserved
//-----------------------------------------------------------------------------

#ifndef     _THING_MANAGER_H_
#define     _THING_MANAGER_H_

//-----------------------------------------------------------------------------
// includes
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// defines
//-----------------------------------------------------------------------------
#define SCALE_FACTOR        1
#define CORE_WINDOW_X_SIZE  512
#define CORE_WINDOW_Y_SIZE  512
#define CORE_TOOLBAR_SIZE   32
#define WINDOW_X_SIZE       (CORE_WINDOW_X_SIZE * SCALE_FACTOR)
#define WINDOW_Y_SIZE       (CORE_WINDOW_Y_SIZE * SCALE_FACTOR)

//-----------------------------------------------------------------------------
// globals
//-----------------------------------------------------------------------------
extern HINSTANCE    hInst;
extern bool         g_bApplicationPaused ;
extern bool         g_bIsDrag;
extern bool         g_bIsDrawn;
extern HDC		    g_offscreenDC;
extern HWND         g_hwnd;

//-----------------------------------------------------------------------------
// functions
//-----------------------------------------------------------------------------
void        DrawEverything (void);

//-----------------------------------------------------------------------------

#endif  //  _THING_MANAGER_H_
