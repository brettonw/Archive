//-----------------------------------------------------------------------------
// Copyright (C) 1997-2002 Bretton Wade
// All rights reserved.
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// includes
//-----------------------------------------------------------------------------
#include    "stdafx.h"
#include    "debug.h"
#include    "assert.h"

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
// special handling for the debug memory handler. This will run during static
// initialization, to enable memory dumps at shutdown.
//-----------------------------------------------------------------------------
struct MemoryDumpStarter
{
    MemoryDumpStarter (void)
    {
        // Get current flag, Turn on leak-checking and boundary checking, and
        // set the flag for those options
        int tmpFlag = _CrtSetDbgFlag (_CRTDBG_REPORT_FLAG);
        tmpFlag |= _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF;
        _CrtSetDbgFlag (tmpFlag);
        
        // Talk about what we just did
        OutputDebugString ("Activated leak checking and validation!\n");
    }
};

MemoryDumpStarter   g_MemoryDumpStarter;

//-----------------------------------------------------------------------------

#endif  //  NDEBUG
