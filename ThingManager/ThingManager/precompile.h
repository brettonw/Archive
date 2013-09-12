// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once


#define WIN32_LEAN_AND_MEAN		            // Exclude rarely-used stuff from Windows headers
#define _WIN32_WINNT        0x0500          // get recent windows features
#define _WIN32_WINDOWS      _WIN32_WINNT    // get recent windows features
#define _CRT_SECURE_NO_DEPRECATE            // shut up about standard programming practices
#define	_USE_MATH_DEFINES					// we want M_PI in particular

// Windows Header Files
#include    <windows.h>
#include    <windowsx.h>

// C RunTime Header Files
#include    <stdlib.h>
#include    <stdio.h>
#include    <malloc.h>
#include    <memory.h>
#include	<math.h>

//------------------------------------------------------------------------------
// project includes
//------------------------------------------------------------------------------
#include "assert.h"
#include "debug.h"

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
// some types to be generic across platforms
//------------------------------------------------------------------------------
typedef unsigned char   Byte; 
typedef unsigned short  Half; 
typedef unsigned int    Word; 