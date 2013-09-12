//------------------------------------------------------------------------------
//	Copyright (C) 1997-2000 Bretton Wade
//	All Rights Reserved
//------------------------------------------------------------------------------

#ifndef		_BW_SCREEN_PARTITION_H_
#define		_BW_SCREEN_PARTITION_H_

//------------------------------------------------------------------------------
//	includes
//------------------------------------------------------------------------------
#ifndef		_BW_POLYGON_2D_H_
#include	"polygon2d.h"
#endif	//	_BW_POLYGON_2D_H_

#ifndef		_BW_BIT_MASK_H_
#include	"bit_mask.h"
#endif	//	_BW_BIT_MASK_H_

//------------------------------------------------------------------------------
//	class definitions
//------------------------------------------------------------------------------
class	ScreenPartition
{
	public:
		ScreenPartition (void);
		~ScreenPartition (void);
		void					RenderPolygon (const Polygon2d& polygon);
	private:
		String					myBuffer;
		uInt4					myBufferSize;
		ArrayOf<ScannerState>	myStateTable;
		ScannerActionFunction	myActionFunction;
};

//------------------------------------------------------------------------------

#endif	//	_BW_SCREEN_PARTITION_H_
