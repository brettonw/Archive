//-----------------------------------------------------------------------------
// Copyright (C) 2002 Microsoft, All rights reserved
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// includes
//-----------------------------------------------------------------------------
#include    "tgeom.h"
#include    "debug.h"
#include    <stdarg.h>
#include    <crtdbg.h>

#if DEBUG_GEOMETRY
//-----------------------------------------------------------------------------
// global variables
//-----------------------------------------------------------------------------
char    g_dbgBuffer[2048];
bool    g_bDebugSpew = false;

//-----------------------------------------------------------------------------
// functions
//-----------------------------------------------------------------------------
void
DebugPrint (const char* format, ...)
{
    va_list argList;
    va_start (argList, format);
    vsprintf (g_dbgBuffer, format, argList);
    OutputDebugString (g_dbgBuffer);
    va_end (argList);
}

//-----------------------------------------------------------------------------
#else
//-----------------------------------------------------------------------------
// functions
//-----------------------------------------------------------------------------
void
DebugPrint (const char* format, ...)
{
}

//-----------------------------------------------------------------------------
#endif
