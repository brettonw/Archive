//------------------------------------------------------------------------------
//	Copyright (C) 1997-2000 Bretton Wade
//	All Rights Reserved
//------------------------------------------------------------------------------

#ifndef		_BW_BITS_H_
#define		_BW_BITS_H_

//--------------------------------------------------------------------------
//	function declarations
//--------------------------------------------------------------------------
sInt	CountBits (sInt4 value);
sInt	HighestBitPosition (sInt4 value);
sInt4	HighestBit (sInt4 value);
sInt	LowestBitPosition (sInt4 value);
sInt4	LowestBit (sInt4 value);
bool	IsPowerOf2 (sInt4 value);
void	PrintBits (uInt4 value);

//--------------------------------------------------------------------------

#endif	//	_BW_BITS_H_
