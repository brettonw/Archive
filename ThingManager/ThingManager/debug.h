//-----------------------------------------------------------------------------
// Copyright (C) 1997-2006 Bretton Wade and Firaxis Games
// All Rights Reserved
//-----------------------------------------------------------------------------

#ifndef     _DEBUG_H_
#define     _DEBUG_H_

//-----------------------------------------------------------------------------
// functions
//-----------------------------------------------------------------------------

// This function acts like printf, but it WON'T be compiled out in a 
// release build
void    DebugPrint (const char* format, ...);

//-----------------------------------------------------------------------------

#endif  //  _DEBUG_H_
