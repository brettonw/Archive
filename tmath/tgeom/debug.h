//-----------------------------------------------------------------------------
// Copyright (C) 2002-2005 Bretton Wade, All rights reserved
//-----------------------------------------------------------------------------

#ifndef     _DEBUG_H_
#define     _DEBUG_H_

//-----------------------------------------------------------------------------
// functions
//-----------------------------------------------------------------------------
void    DebugPrint (const char* format, ...);

//-----------------------------------------------------------------------------
// macros
//-----------------------------------------------------------------------------
#ifdef      _DEBUG
#define     DEBUG_GEOMETRY  1
#if         DEBUG_GEOMETRY
extern  bool    g_bDebugSpew;
#define     DEBUG_PRINT(object) if (g_bDebugSpew) object.DebugPrint ()
#define     DEBUG_PRINT_ON      g_bDebugSpew = true
#define     DEBUG_PRINT_OFF     g_bDebugSpew = false
#define     DEBUG_PRINT_SCOPE   DebugPrintScope dbgPrintScope
#endif
#else
#define     DEBUG_PRINT(object) NOOP
#define     DEBUG_PRINT_ON      NOOP
#define     DEBUG_PRINT_OFF     NOOP
#define     DEBUG_PRINT_SCOPE   NOOP
#endif

//-----------------------------------------------------------------------------
// structures
//-----------------------------------------------------------------------------
#if         DEBUG_GEOMETRY
struct DebugPrintScope
{
    bool        bOldDebugSpew;
    /* void */  DebugPrintScope (void)
    {
        bOldDebugSpew = g_bDebugSpew;
    }
    /* void */  ~DebugPrintScope (void)
    {
        g_bDebugSpew = bOldDebugSpew;
    }
};
#endif

//-----------------------------------------------------------------------------

#endif  //  _DEBUG_H_
