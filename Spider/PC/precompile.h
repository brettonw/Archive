//------------------------------------------------------------------------------
// Copyright (C) 2006 Bretton Wade, All Rights Reserved
//------------------------------------------------------------------------------

#ifndef     STDAFX
#define     STDAFX

//------------------------------------------------------------------------------
// Modify the following defines if you have to target a platform prior to the 
// ones specified below. Refer to MSDN for the latest info on corresponding 
// values for different platforms.
//------------------------------------------------------------------------------

#ifndef     WINVER				        // features specific to Windows XP or later.
#define     WINVER          0x0501
#endif

#ifndef     _WIN32_WINNT		        // features specific to Windows XP or later.                   
#define     _WIN32_WINNT    0x0501
#endif						

#ifndef     _WIN32_WINDOWS		        // features specific to Windows 98 or later.
#define     _WIN32_WINDOWS  0x0501
#endif

#ifndef     _WIN32_IE			        // features specific to IE 6.0 or later.
#define     _WIN32_IE       0x0600
#endif

#define     WIN32_LEAN_AND_MEAN	        // exclude rarely-used stuff from Windows headers
#define     _CRT_RAND_S                 // include secure random number generation
#define     _CRT_SECURE_NO_DEPRECATE    // don't warn me about using standard C functions

//------------------------------------------------------------------------------
// Windows Header Files:
//------------------------------------------------------------------------------
#include    <windows.h>
#include    <windowsx.h>
#include    <mmsystem.h>

//------------------------------------------------------------------------------
// C and C++ RunTime Header Files
//------------------------------------------------------------------------------
#include    <stdlib.h>
#include    <stdio.h>
#include    <limits.h>
#include    <math.h>
#include    <new.h>
#include    <assert.h>

//------------------------------------------------------------------------------
// Application headers files
//------------------------------------------------------------------------------
#include    "resource.h"

//------------------------------------------------------------------------------
// ... because msvc is not standard C++, with my own twist on the naming
//------------------------------------------------------------------------------
#define bit_or          |
#define bit_and         &
#define xor             ^
#define or              ||
#define and             &&
#define not             !

//------------------------------------------------------------------------------
// because *_cast is just too damn long to be readable
//------------------------------------------------------------------------------
#define r_cast          reinterpret_cast
#define s_cast          static_cast
#define c_cast          const_cast

//------------------------------------------------------------------------------
// global defines
//------------------------------------------------------------------------------
#define USE_OFFSCREEN_RENDERING
enum
{
    SCALE_FACTOR    =   2
};

//------------------------------------------------------------------------------
// global variables
//------------------------------------------------------------------------------
extern  HINSTANCE       g_hInst;
extern  HWND            g_hWnd;
extern  HDC             g_offscreenDC;
extern  POINT           g_cursorPos;
extern  POINT           g_lastCursorPos;
extern  RECT            g_dirtyRect;
extern  bool            g_bUseBackground;
extern  bool            g_bUseCardback;
extern  bool            g_bPlaySound;

//------------------------------------------------------------------------------
#endif  //  STDAFX
