//------------------------------------------------------------------------------
//	File:					quaternion.h
//	Date:					9/18/94
//	Author:				Bretton Wade
//
//	Description:	this file contains the methods for a quaternion
//
//------------------------------------------------------------------------------

#include "vector_3d.h"
#include "matrix_3d.h"

#ifndef QUATERNION
#define QUATERNION

//------------------------------------------------------------------------------
//	functions
//------------------------------------------------------------------------------
matrix_3d	Quaternion (const vector_3d& begin, const vector_3d& end);						//	function to map a quaternion to a transformation matrix_3d

//------------------------------------------------------------------------------

#endif //QUATERNION
