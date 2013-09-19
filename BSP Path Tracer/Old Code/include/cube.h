//------------------------------------------------------------------------------
//	File:					cube.h
//	Date:					04/29/97
//	Author:				Bretton Wade
//
//	Description:	this file contains the function for cube generation
//
//------------------------------------------------------------------------------

#include	"matrix_3d.h"
#include	"listptr_3d.h"

#ifndef	CUBE
#define	CUBE

//------------------------------------------------------------------------------
//	functions
//------------------------------------------------------------------------------
listptr	Cube (matrix_3d transformation, real emit = R(0.0));										//	compute a cube given a transformation

//------------------------------------------------------------------------------

#endif	//CUBE
