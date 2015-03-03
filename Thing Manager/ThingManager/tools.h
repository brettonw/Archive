//-----------------------------------------------------------------------------
// Copyright (C) 2006 Bretton Wade, All Rights Reserved
//-----------------------------------------------------------------------------

#ifndef     _TOOLS_H_
#define     _TOOLS_H_

//-----------------------------------------------------------------------------
// includes
//-----------------------------------------------------------------------------
#ifndef     _THING_MANAGER_H_
#include    "thing_manager.h"
#endif  //  _THING_MANAGER_H_

//-----------------------------------------------------------------------------
// enumerations
//-----------------------------------------------------------------------------
enum ToolState
{
    TS_CIRCLE       = 0,
    TS_BOX          = 1,
    TS_LINE         = 2
};

//-----------------------------------------------------------------------------
// defines
//-----------------------------------------------------------------------------
#define TOOLBAR_SIZE        (CORE_TOOLBAR_SIZE * SCALE_FACTOR)

//-----------------------------------------------------------------------------
// globals
//-----------------------------------------------------------------------------
extern ToolState    g_toolState;
extern int          g_isDown;

//-----------------------------------------------------------------------------
// functions
//-----------------------------------------------------------------------------
void    DrawToolbar (void);
bool    ToolbarProc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
bool    DragViewProc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
bool    CircleToolProc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
bool    BoxToolProc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
bool    LineToolProc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

//-----------------------------------------------------------------------------

#endif  //  _TOOLS_H_
