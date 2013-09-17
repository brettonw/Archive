//-----------------------------------------------------------------------------
// Copyright (C) 2003 Bretton Wade, All rights reserved
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// includes
//-----------------------------------------------------------------------------
#include    "stdafx.h"
#include    "debug.h"
//#include    <streambuf>
//#include    <ostream>
//#include    <stdarg.h>

#ifdef      _USE_DEBUG_SPEW
//-----------------------------------------------------------------------------
// global variables
//-----------------------------------------------------------------------------
char        g_dbgBuffer[2048];
bool        g_bDebugSpew = false;

//-----------------------------------------------------------------------------
// stream buffer class used by the debug stream
//-----------------------------------------------------------------------------
class DebugStreamBuf : public std::streambuf
{
    public:
        DebugStreamBuf () 
        { 
            setp (0,0); 
            setg (0,0,0); 
        }

    protected:
virtual int_type overflow(int_type _Meta)
        {
            if (g_bDebugSpew)
            {
                // this is a really stupid stream mechanism that outputs one character
                // at a time. I would *love* to have a better understanding of how to
                // get buffering for free in this derivation context
                char    buffer[2] = {0, 0};
                buffer[0] = _Meta;
                OutputDebugString (buffer);
            }
            return _Meta;
        }
};

//-----------------------------------------------------------------------------
// functions
//-----------------------------------------------------------------------------
void
DebugPrint (const char* format, ...)
{
    if (g_bDebugSpew)
    {
        va_list argList;
        va_start (argList, format);
        vsprintf (g_dbgBuffer, format, argList);
        OutputDebugString (g_dbgBuffer);
        va_end (argList);
    }
}

//-----------------------------------------------------------------------------
#else
//-----------------------------------------------------------------------------
// stream buffer class used by the debug stream
//-----------------------------------------------------------------------------
class DebugStreamBuf : public std::streambuf
{
    public:
        DebugStreamBuf () 
        { 
            setp (0,0); 
            setg (0,0,0); 
        }

    protected:
virtual int_type overflow(int_type _Meta)
        {
            return _Meta;
        }
};

//-----------------------------------------------------------------------------
// functions
//-----------------------------------------------------------------------------
void
DebugPrint (const char* format, ...)
{
}

//-----------------------------------------------------------------------------
#endif

//-----------------------------------------------------------------------------
// stream class
//-----------------------------------------------------------------------------
/* void */  
DebugStream::DebugStream (void) : 
    std::ostream (new DebugStreamBuf) 
{
}

//-----------------------------------------------------------------------------
/* void */  
DebugStream::~DebugStream (void) 
{ 
    delete rdbuf (); 
}

//-----------------------------------------------------------------------------
// global debug stream
//-----------------------------------------------------------------------------
DebugStream g_dbgout;

//-----------------------------------------------------------------------------
