//------------------------------------------------------------------------------
//	File:					point_2d.cp
//	Date:					11/07/94
//	Author:				Bretton Wade
//
//	Description:	this file contains the class definition for a (u, v) point_2d
//
//------------------------------------------------------------------------------

#include "precompile.h"
#include "point_2d.h"
#include "vector_2d.h"

//------------------------------------------------------------------------------
//	global variables
//------------------------------------------------------------------------------
point_2d	ORIGIN_2D (R(0.0), R(0.0));																						//	the origin of the coordinate system

//------------------------------------------------------------------------------
//	constructor
//------------------------------------------------------------------------------
point_2d::point_2d (real x, real y) : tuple_2d (x, y)														//	normal constructor
{																																								//	begin
}																																								//	end

//------------------------------------------------------------------------------
//	constructor
//------------------------------------------------------------------------------
point_2d::point_2d (const point_2d &p) : tuple_2d (p)														//	copy constructor
{																																								//	begin
}																																								//	end

//------------------------------------------------------------------------------
//	constructor
//------------------------------------------------------------------------------
point_2d::point_2d (const vector_2d &v) : tuple_2d (v)														//	constructor from a 2d vector
{																																								//	begin
}																																								//	end

//------------------------------------------------------------------------------
//	assignment operator
//------------------------------------------------------------------------------
point_2d	&point_2d::operator = (const point_2d &p)															//	assignment operator
{																																								//	begin
	tuple_2d::operator = (p);
	return *this;
}																																								//	end

//------------------------------------------------------------------------------
//	vector addition operator
//------------------------------------------------------------------------------
point_2d	point_2d::operator + (const vector_2d &v) const												//	add a vector to a point
{																																								//	begin
	return point_2d (xy[X] + v[X], xy[Y] + v[Y]);
}																																								//	end

//------------------------------------------------------------------------------
//	subtraction operator
//------------------------------------------------------------------------------
vector_2d	point_2d::operator - (const point_2d &p) const												//	subtract two points
{																																								//	begin
	return vector_2d (xy[X] - p[X], xy[Y] - p[Y]);
}																																								//	end

//------------------------------------------------------------------------------
