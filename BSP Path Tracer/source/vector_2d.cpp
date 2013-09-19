//------------------------------------------------------------------------------
//	File:					vector_2d.cp
//	Date:					1/21/95
//	Author:				Bretton Wade
//
//	Description:	this file contains the methods for a 2d vector_3d
//
//------------------------------------------------------------------------------

#include "precompile.h"
#include "point_2d.h"
#include "vector_2d.h"

//------------------------------------------------------------------------------
//	constructor
//------------------------------------------------------------------------------
vector_2d::vector_2d (real x, real y) : tuple_2d (x, y)													//	constructor from 2 values
{
}

//------------------------------------------------------------------------------
//	constructor
//------------------------------------------------------------------------------
vector_2d::vector_2d (const vector_2d &v) : tuple_2d (v)												//	copy constructor
{
}

//------------------------------------------------------------------------------
//	constructor
//------------------------------------------------------------------------------
vector_2d::vector_2d (const point_2d &p) : tuple_2d (p)													//	constructor from a point
{
}
//------------------------------------------------------------------------------
//	assignment operator
//------------------------------------------------------------------------------
vector_2d	&vector_2d::operator = (const vector_2d &v)														//	assignment operator
{
	tuple_2d::operator = (v);
	return *this;
}

//------------------------------------------------------------------------------
//	multiply a vector_3d by a scalar
//------------------------------------------------------------------------------
vector_2d	vector_2d::operator * (real s) const																	//	scalar multiplication
{
	return vector_2d (xy[X] * s, xy[Y] * s);
}

//------------------------------------------------------------------------------
//	divide a vector_3d by a scalar
//------------------------------------------------------------------------------
vector_2d	vector_2d::operator / (real s) const																	//	scalar division
{
	return vector_2d (xy[X] * s, xy[Y] * s);
}

//------------------------------------------------------------------------------
//	add two vectors
//------------------------------------------------------------------------------
vector_2d	vector_2d::operator + (const vector_2d &v) const											//	addition operator
{
	return vector_2d (xy[X] + v[X], xy[Y] + v[Y]);
}

//------------------------------------------------------------------------------
//	subtract two vectors
//------------------------------------------------------------------------------
vector_2d	vector_2d::operator - (const vector_2d &v) const											//	subtraction operator
{
	return vector_2d (xy[X] - v[X], xy[Y] - v[Y]);
}

//------------------------------------------------------------------------------
//	compute the length of the vector_3d
//------------------------------------------------------------------------------
real			vector_2d::Norm (void) const																					//	compute the length of the vector_2d
{
	return SQRT ((xy[X] * xy[X]) + (xy[Y] * xy[Y]));
}

//------------------------------------------------------------------------------
//	normalize the vector_3d
//------------------------------------------------------------------------------
vector_2d	&vector_2d::Normalize (void)																					//	reduce the vector_2d to length 1.0
{
	real	length = Norm ();
	xy[X] /= length; xy[Y] /= length;
	return *this;
}

//------------------------------------------------------------------------------
