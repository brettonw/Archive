//------------------------------------------------------------------------------
//	File:					point_3d.cp
//	Date:					8/26/94
//	Author:				Bretton Wade
//
//	Description:	this file contains the methods for a point_3d
//
//------------------------------------------------------------------------------

#include "precompile.h"
#include "point_3d.h"
#include "vector_3d.h"

//------------------------------------------------------------------------------
//	global variables
//------------------------------------------------------------------------------
point_3d	ORIGIN_3D (R(0.0), R(0.0), R(0.0));																		//	the origin of the coordinate system

//------------------------------------------------------------------------------
//	constructor
//------------------------------------------------------------------------------
point_3d::point_3d (real x, real y, real z, real w) : tuple_3d (x, y, z, w)			//	constructor from three values
{																																								//	begin
	if (w != R(1.0))																															//	if the homogenous coordinate is not a valid projection
	{																																							//	begin
		xyz[X] /= w; xyz[Y] /= w; xyz[Z] /= w; xyz[W] = R(1.0);											//	divide the coordinates by w
	}																																							//	end
}																																								//	end

//------------------------------------------------------------------------------
//	constructor
//------------------------------------------------------------------------------
point_3d::point_3d (const point_3d& p) : tuple_3d (p)														//	copy constructor
{																																								//	begin
}																																								//	end

//------------------------------------------------------------------------------
//	constructor
//------------------------------------------------------------------------------
point_3d::point_3d (const vector_3d& v) : tuple_3d (v[X], v[Y], v[Z], R(1.0))		//	constructor from a vector_3d
{																																								//	begin
}																																								//	end

//------------------------------------------------------------------------------
//	constructor
//------------------------------------------------------------------------------
point_3d::point_3d (const tuple_3d& t)																					//	constructor from a tuple_3d
{																																								//	begin
	real	w = t[W];																																//	get the homogenous coordinate
	if (w != R(1.0))																															//	if the homogenous ccordinate needs to be divided
	{																																							//	begin
		xyz[X] = t[X] / w; xyz[Y] = t[Y] / w;																				//	copy the values into the tuple_3d
		xyz[Z] = t[Z] / w; xyz[W] = R(1.0);																					//	copy the values into the tuple_3d
	}																																							//	end
	else																																					//	otherwise
	{																																							//	begin
		xyz[X] = t[X]; xyz[Y] = t[Y];																								//	copy the values into the tuple_3d
		xyz[Z] = t[Z]; xyz[W] = t[W];																								//	copy the values into the tuple_3d
	}																																							//	end
}																																								//	end

//------------------------------------------------------------------------------
//	assignment
//------------------------------------------------------------------------------
point_3d	&point_3d::operator = (const point_3d& p)															//	assignment operator
{																																								//	begin
	tuple_3d::operator () (p[X], p[Y], p[Z], p[W]);																//	copy the values
	return *this;																																	//	return the reference to this
}																																								//	end

//------------------------------------------------------------------------------
//	assignment
//------------------------------------------------------------------------------
point_3d	&point_3d::operator = (const tuple_3d& t)															//	assignment operator
{																																								//	begin
	real	w = t[W];																																//	get the homogenous coordinate
	if (w != R(1.0))																															//	if the homogenous ccordinate needs to be divided
		tuple_3d::operator () (t[X] / w, t[Y] / w, t[Z] / w, R(1.0));								//	copy the values, dividing by the homogenous coordinate
	else																																					//	otherwise
		tuple_3d::operator = (t);																										//	copy the values
	return *this;																																	//	return the reference to this
}																																								//	end

//------------------------------------------------------------------------------
//	assignment by values
//------------------------------------------------------------------------------
void	point_3d::operator () (real x, real y, real z, real w)										//	function call operator
{																																								//	begin
	tuple_3d::operator () (x, y, z, w);																						//	assign the values
}																																								//	end

//------------------------------------------------------------------------------
//	subtraction
//------------------------------------------------------------------------------
vector_3d	point_3d::operator - (const point_3d& p) const												//	subtraction operator
{																																								//	begin
	return vector_3d (	xyz[X] - p[X], 																						//	return a vector_3d, x subtraction
									xyz[Y] - p[Y], 																								//	y subtraction
									xyz[Z] - p[Z]);																								//	z subtraction
}																																								//	end

//------------------------------------------------------------------------------
//	addition of a vector_3d
//------------------------------------------------------------------------------
point_3d	point_3d::operator + (const vector_3d& v) const												//	addition of a vector_3d to a point_3d
{																																								//	begin
	return point_3d (xyz[X] + v[X], 																							//	return a point_3d, x addition
								xyz[Y] + v[Y], 																									//	y addition
								xyz[Z] + v[Z]);																									//	z addition
}																																								//	end

//------------------------------------------------------------------------------
