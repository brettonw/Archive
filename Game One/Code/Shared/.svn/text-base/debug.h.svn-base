//-----------------------------------------------------------------------------
// Copyright (C) 1997-2006 Bretton Wade
// All Rights Reserved
//-----------------------------------------------------------------------------

#ifndef     _DEBUG_H_
#define     _DEBUG_H_

#ifndef     NDEBUG

//-----------------------------------------------------------------------------
// include files
//-----------------------------------------------------------------------------
#ifndef     _INC_CRTDBG
#include    <crtdbg.h>
#endif  //  _INC_CRTDBG

#ifndef     _RESETTER_H_
#include    "resetter.h"
#endif  //  _RESETTER_H_

//-----------------------------------------------------------------------------
// macros to enable debug memory handler
//-----------------------------------------------------------------------------
#define     DEBUG_NEW_BLOCK         new (g_iNewCallBlockType, __FILE__, __LINE__)
#define     NewCall                 DEBUG_NEW_BLOCK
#define     NEW_BLOCK_TYPE(type)    Resetter<int> __MACRO__NewBlockType (g_iNewCallBlockType, type)
#define     DEBUG_NEW_PARAMETERS    , int iBlockType, const char* szFileName, int iLineNumber
#define     SNITCH                  MemorySnitch __MACRO__MemorySnitch

//-----------------------------------------------------------------------------
// global variables
//-----------------------------------------------------------------------------
extern int  g_iNewCallBlockType;

//-----------------------------------------------------------------------------
// class definitions
//-----------------------------------------------------------------------------
class MemorySnitch
{
    public:
        /* void */      MemorySnitch (void);
        /* void */      ~MemorySnitch (void);
        
    private:
        _CrtMemState    m_memState;
};

//-----------------------------------------------------------------------------

#else  //  NDEBUG

//-----------------------------------------------------------------------------
// macros to hide debug memory handling
//-----------------------------------------------------------------------------
#define     NewCall                 new
#define     NEW_BLOCK_TYPE(type)
#define     DEBUG_NEW_PARAMETERS
#define     SNITCH

//-----------------------------------------------------------------------------

#endif  //  NDEBUG

//-----------------------------------------------------------------------------
// functions
//-----------------------------------------------------------------------------

// This function acts like printf, but it WON'T be compiled out in a 
// release build
void    DebugPrint (const char* format, ...);

//-----------------------------------------------------------------------------

#endif  //  _DEBUG_H_
