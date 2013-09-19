//------------------------------------------------------------------------------
//	File:					split.h
//	Date:					04/07/97
//	Author:				Bretton Wade
//
//	Description:	this file contains the method for splitting a polygon
//								with a plane.
//
//------------------------------------------------------------------------------

#include "polyptr_3d.h"

#ifndef	SPLIT
#define	SPLIT

//------------------------------------------------------------------------------
//	enumerations
//------------------------------------------------------------------------------
enum	hclass {HC_IN = -1, HC_ON = 0, HC_OUT = 1, HC_SPANNING = 2};							//	classification type for plane_3d comparisons

//------------------------------------------------------------------------------
//	functions
//------------------------------------------------------------------------------
hclass	Split (const polyptr&, const plane_3d&, polyptr&, polyptr&);						//	split the polygon with a plane_3d
bool	Split (const polyptr&, real max_area, polyptr&, polyptr&);

//------------------------------------------------------------------------------

#endif	//SPLIT
