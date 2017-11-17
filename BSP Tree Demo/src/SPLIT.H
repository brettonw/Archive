//------------------------------------------------------------------------------
//	File:					split.h
//	Date:					04/07/97
//	Author:				Bretton Wade
//
//	Description:	this file contains the method for splitting a polygon
//								with a plane.
//
//------------------------------------------------------------------------------

#ifndef     _POLYGON_
#include	"polygon_3d.h"
#endif  //  _POLYGON_

#ifndef	SPLIT
#define	SPLIT

//------------------------------------------------------------------------------
//	enumerations
//------------------------------------------------------------------------------
enum	hclass {HC_IN = -1, HC_ON = 0, HC_OUT = 1, HC_SPANNING = 2};							//	classification type for plane_3d comparisons

//------------------------------------------------------------------------------
//	functions
//------------------------------------------------------------------------------
hclass	Split (const PtrToPolygon_3d&, const plane_3d&, PtrToPolygon_3d&, PtrToPolygon_3d&);						//	split the polygon with a plane_3d

//------------------------------------------------------------------------------

#endif	//SPLIT
