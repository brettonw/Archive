//------------------------------------------------------------------------------
//	File:					transform.h
//	Date:					9/4/94
//	Author:				Bretton Wade
//
//	Description:	this file contains methods for the transformations matrices
//
//------------------------------------------------------------------------------

#include "vector_3d.h"
#include "point_3d.h"
#include "matrix_3d.h"

#ifndef	TRANSFORM
#define	TRANSFORM

//------------------------------------------------------------------------------
//	Functions
//------------------------------------------------------------------------------
matrix_3d	RotateX (real angle);																										//	build a transformation matrix_3d
matrix_3d	RotateY (real angle);																										//	build a transformation matrix_3d
matrix_3d	RotateZ (real angle);																										//	build a transformation matrix_3d
matrix_3d	Translate (real x, real y, real z);																			//	build a transformation matrix_3d
matrix_3d	Scale (real x, real y, real z);																					//	build a transformation matrix_3d
matrix_3d	Perspective (real distance);																						//	build a transformation matrix_3d
matrix_3d	VectorMatrix (tuple_3d& n);																							//	build a vector_3d rotation matrix_3d
matrix_3d	ViewMatrix (const vector_3d& u, const vector_3d& v, const vector_3d& n, const point_3d& r);	//	build a viewing matrix_3d from a vector_3d set

//------------------------------------------------------------------------------

#endif	//TRANSFORM
