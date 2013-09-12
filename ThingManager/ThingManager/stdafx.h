// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once


#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#define _WIN32_WINNT  0x0500  // get recent windows features
#define _WIN32_WINDOWS  _WIN32_WINNT  // get recent windows features
// Windows Header Files:
#include <windows.h>
#include <windowsx.h>
// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <d3dx9.h>
#include <vector>
using namespace std;

// TODO: reference additional headers your program requires here
//------------------------------------------------------------------------------
// ... because msvc is not standard C++, with my own twist on the naming
//------------------------------------------------------------------------------
#define bit_or          |
#define bit_and         &
#define exclusive_or    ^
#define or              ||
#define and             &&
#define not             !

