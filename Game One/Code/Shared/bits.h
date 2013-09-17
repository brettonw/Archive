//-----------------------------------------------------------------------------
// Copyright (C) 1997-2006 Bretton Wade
// All rights reserved.
//-----------------------------------------------------------------------------

#ifndef     _BITS_H_
#define     _BITS_H_

//-----------------------------------------------------------------------------
// function declarations
//-----------------------------------------------------------------------------
sInt        HighestBitPosition (sInt4 value);
sInt4       HighestBit (sInt4 value);
sInt        LowestBitPosition (sInt4 value);
sInt4       LowestBit (sInt4 value);
bool        IsPowerOf2 (sInt4 value);

//-----------------------------------------------------------------------------

#endif  //  _BITS_H_
