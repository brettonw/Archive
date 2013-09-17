//-----------------------------------------------------------------------------
// Copyright (C) 1997-2006 Bretton Wade
// All Rights Reserved
//-----------------------------------------------------------------------------

#ifndef     PRECOMPILE_H
#define     PRECOMPILE_H

//-----------------------------------------------------------------------------
// system include files
//-----------------------------------------------------------------------------

#define     WIN32_LEAN_AND_MEAN
#define		_CRT_SECURE_NO_DEPRECATE
#include    <windows.h>

/*
#if _MSC_VER < 1300
// if somebody happens to have an old platform SDK, or a slightly older version
// of DirectX 8, they might be missing the definition of a ULONG_PTR;
typedef unsigned long ULONG_PTR;
typedef ULONG_PTR DWORD_PTR;
#endif  //  _BASETSD_H_
*/

#include    <malloc.h>
#include	<d3dx9.h>
//-----------------------------------------------------------------------------
// c++ library include files
//-----------------------------------------------------------------------------
#include    <limits>
#include    <vector>
#include    <list>
#include    <map>

//-----------------------------------------------------------------------------
// local include files
//-----------------------------------------------------------------------------
#include    "definitions.h"
#include    "debug.h"
#include    "assert.h"

//-----------------------------------------------------------------------------

#endif  //  PRECOMPILE_H
