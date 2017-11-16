//------------------------------------------------------------------------------
//	File:					vector_3d.cp
//	Date:					8/26/94
//	Author:				Bretton Wade
//
//	Description:	this file contains the methods for a vector_3d
//
//------------------------------------------------------------------------------

#include "precompile.h"
#include "point_3d.h"
#include "vector_3d.h"

//------------------------------------------------------------------------------
//	global variables
//------------------------------------------------------------------------------
vector_3d	ZERO_VECTOR (R(0.0), R(0.0), R(0.0));																	//	a zero value vector_3d

//------------------------------------------------------------------------------
//	constructor
//------------------------------------------------------------------------------
vector_3d::vector_3d (real x, real y, real z, real w) : tuple_3d (x, y, z, w)		//	constructor from three values
{																																								//	begin
}																																								//	end

//------------------------------------------------------------------------------
//	constructor
//------------------------------------------------------------------------------
vector_3d::vector_3d (const vector_3d& v) : tuple_3d (v)												//	copy constructor
{																																								//	begin
}																																								//	end

//------------------------------------------------------------------------------
//	constructor
//------------------------------------------------------------------------------
vector_3d::vector_3d (const point_3d& p) : tuple_3d (p[X], p[Y], p[Z], R(0.0))	//	constructor from a point_3d
{																																								//	begin
}																																								//	end

//------------------------------------------------------------------------------
//	constructor
//------------------------------------------------------------------------------
vector_3d::vector_3d (const tuple_3d& t)																				//	constructor from a tuple_3d
{																																								//	begin
	xyz[X] = t[X]; xyz[Y] = t[Y]; xyz[Z] = t[Z]; xyz[W] = R(0.0);									//	copy the values into the tuple_3d
}																																								//	end

//------------------------------------------------------------------------------
//	assignment
//------------------------------------------------------------------------------
vector_3d	&vector_3d::operator = (const vector_3d& v)														//	assignment operator
{																																								//	begin
	tuple_3d::operator () (v[X], v[Y], v[Z], v[W]);																//	copy the values
	return *this;																																	//	return the reference to this
}																																								//	end

//------------------------------------------------------------------------------
//	assignment
//------------------------------------------------------------------------------
vector_3d	&vector_3d::operator = (const tuple_3d& t)														//	assignment operator
{																																								//	begin
	tuple_3d::operator () (t[X], t[Y], t[Z], R(0.0));															//	copy the values
	return *this;																																	//	return the reference to this
}																																								//	end

//------------------------------------------------------------------------------
//	assignment by values
//------------------------------------------------------------------------------
void	vector_3d::operator () (real x, real y, real z, real w)										//	function call operator
{																																								//	begin
	tuple_3d::operator () (x, y, z, w);																						//	assign the coordinates
}																																								//	end

//------------------------------------------------------------------------------
//	scalar multiplication
//------------------------------------------------------------------------------
vector_3d 	vector_3d::operator * (real s) const																//	scalar multiplication
{																																								//	begin
	return vector_3d (xyz[X] * s, xyz[Y] * s, xyz[Z] * s);												//	return the multiplied vector_3d
}																																								//	end

//------------------------------------------------------------------------------
//	division by a scalar
//------------------------------------------------------------------------------
vector_3d 	vector_3d::operator / (real s) const																//	scalar division
{																																								//	begin
	return vector_3d (xyz[X] / s, xyz[Y] / s, xyz[Z] / s);												//	return the divided vector_3d
}																																								//	end

//------------------------------------------------------------------------------
//	cross product
//------------------------------------------------------------------------------
vector_3d	vector_3d::operator ^ (const vector_3d& v) const											//	cross product
{																																								//	begin
	return vector_3d (	(xyz[Y] * v[Z]) - (xyz[Z] * v[Y]), 												//	return a vector_3d, x coordinate
									(xyz[Z] * v[X]) - (xyz[X] * v[Z]),														//	y coordinate
									(xyz[X] * v[Y]) - (xyz[Y] * v[X]));														//	z coordinate
}																																								//	end

//------------------------------------------------------------------------------
//	addition
//------------------------------------------------------------------------------
vector_3d	vector_3d::operator + (const vector_3d& v) const											//	addition operator
{																																								//	begin
	return vector_3d (	xyz[X] + v[X], 																						//	return a vector_3d, x coordinate
									xyz[Y] + v[Y],																								//	y coordinate
									xyz[Z] + v[Z]);																								//	z coordinate
}																																								//	end

//------------------------------------------------------------------------------
//	self addition
//------------------------------------------------------------------------------
vector_3d	&vector_3d::operator += (const vector_3d& v)													//	self addition operator
{																																								//	begin
	xyz[X] += v[X]; 																															//	x coordinate
	xyz[Y] += v[Y];																																//	y coordinate
	xyz[Z] += v[Z];																																//	z coordinate
	return *this;																																	//	return the vector_3d reference
}																																								//	end

//------------------------------------------------------------------------------
//	subtraction
//------------------------------------------------------------------------------
vector_3d	vector_3d::operator - (const vector_3d& v) const											//	subtraction operator
{																																								//	begin
	return vector_3d (	xyz[X] - v[X], 																						//	return a vector_3d, x coordinate
									xyz[Y] - v[Y],																								//	y coordinate
									xyz[Z] - v[Z]);																								//	z coordinate
}																																								//	end

//------------------------------------------------------------------------------
//	negation
//------------------------------------------------------------------------------
vector_3d	vector_3d::operator - (void) const																		//	unary minus operator
{																																								//	begin
	return vector_3d (-xyz[X], -xyz[Y], -xyz[Z]);																	//	z coordinate
}																																								//	end

//------------------------------------------------------------------------------
//	compute the length
//------------------------------------------------------------------------------
real	vector_3d::Norm (void) const																							//	compute the length of the vector_3d
{																																								//	begin
	return SQRT ((*this) | (*this));																							//	return the square root of the dot product
}																																								//	end

//------------------------------------------------------------------------------
//	reduce to length 1
//------------------------------------------------------------------------------
vector_3d	&vector_3d::Normalize (void)																					//	reduce the vector_3d to length 1.0
{																																								//	begin
	real	n = Norm ();																														//	compute the length of the vector_3d
	xyz[X] /= n; xyz[Y] /= n; xyz[Z] /= n;																				//	divide through by the length
	return *this;																																	//	return a referencre to this vector_3d
}																																								//	end

//------------------------------------------------------------------------------
