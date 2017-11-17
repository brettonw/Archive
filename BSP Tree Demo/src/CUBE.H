//------------------------------------------------------------------------------
//	File:					cube.h
//	Date:					11/02/94
//	Author:				Bretton Wade
//
//	Description:	this file contains the function for cube generation
//
//------------------------------------------------------------------------------

#include	"matrix_3d.h"

#ifndef	    _POLYGON_LIST_
#include    "polygon_list_3d.h"
#endif  //  _POLYGON_LIST_

#ifndef	CUBE
#define	CUBE

//------------------------------------------------------------------------------
//	functions
//------------------------------------------------------------------------------
PtrToPolygonList_3d	Cube (matrix_3d transformation);																						//	compute a cube given a transformation

//------------------------------------------------------------------------------

#endif	//CUBE
