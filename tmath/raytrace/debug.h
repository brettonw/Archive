//-----------------------------------------------------------------------------
// Copyright (C) 2003 Bretton Wade, All rights reserved
//-----------------------------------------------------------------------------

#ifndef     _DEBUG_H_
#define     _DEBUG_H_

//-----------------------------------------------------------------------------
// functions
//-----------------------------------------------------------------------------
void    DebugPrint (const char* format, ...);

//-----------------------------------------------------------------------------
// stream class
//-----------------------------------------------------------------------------
class DebugStream : public std::ostream
{
    public:
        /* void */  DebugStream (void);
        /* void */  ~DebugStream (void);
};

//-----------------------------------------------------------------------------
extern  DebugStream g_dbgout;

//-----------------------------------------------------------------------------
// macros
//-----------------------------------------------------------------------------
#ifdef      _USE_DEBUG_SPEW
extern  bool    g_bDebugSpew;
struct DebugSpewScope
{
    bool        bOldDebugSpew;
    /* void */  DebugSpewScope (bool bDebugSpew)
    {
        bOldDebugSpew = g_bDebugSpew;
        g_bDebugSpew = bDebugSpew;
    }
    /* void */  ~DebugSpewScope (void)
    {
        g_bDebugSpew = bOldDebugSpew;
    }
};
#define     DEBUG_SPEW_ON           g_bDebugSpew = true
#define     DEBUG_SPEW_OFF          g_bDebugSpew = false
#define     DEBUG_SPEW_SCOPE(on)    DebugSpewScope dbgSpewScope (on)
#else   //  _USE_DEBUG_SPEW
#define     DEBUG_SPEW_ON           NOOP
#define     DEBUG_SPEW_OFF          NOOP
#define     DEBUG_SPEW_SCOPE(on)    NOOP
#endif  //  _USE_DEBUG_SPEW

//-----------------------------------------------------------------------------

#endif  //  _DEBUG_H_
