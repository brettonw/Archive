//-----------------------------------------------------------------------------
// Copyright (C) 1997-2006 Bretton Wade
// All rights reserved.
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// include files
//-----------------------------------------------------------------------------
#include    "precompile.h"
#include    "debug.h"

//-----------------------------------------------------------------------------
// global variables
//-----------------------------------------------------------------------------
char            g_dbgBuffer[2048];

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

#ifndef     NDEBUG

//-----------------------------------------------------------------------------
// global variables
//-----------------------------------------------------------------------------
int             g_iNewCallBlockType = _NORMAL_BLOCK;

//-----------------------------------------------------------------------------
// class methods
//-----------------------------------------------------------------------------
/* void */
MemorySnitch::MemorySnitch (void)
{
    _CrtMemCheckpoint (&m_memState);
}

//-----------------------------------------------------------------------------
/* void */
MemorySnitch::~MemorySnitch (void)
{
    _CrtMemState    newMemState;
    _CrtMemState    diffMemState;
    _CrtMemCheckpoint (&newMemState);
    if (_CrtMemDifference (&diffMemState, &m_memState, &newMemState))
    {



        Assert ("Memory states don't match" and false);
        _CrtMemDumpAllObjectsSince (&m_memState);
    }
}

//-----------------------------------------------------------------------------

#endif  //  NDEBUG
