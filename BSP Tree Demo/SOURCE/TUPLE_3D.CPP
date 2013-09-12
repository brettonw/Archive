//------------------------------------------------------------------------------
//	File:					tuple_3d.cp
//	Date:					8/26/94
//	Author:				Bretton Wade
//
//	Description:	this file contains the methods for a tuple_3d
//
//------------------------------------------------------------------------------

#include "precompile.h"
#include "utility.h"
#include "tuple_3d.h"

//------------------------------------------------------------------------------
//	constructor
//------------------------------------------------------------------------------
tuple_3d::tuple_3d (const tuple_3d &t)																					//	copy constructor
{																																								//	begin
	xyz[X] = t[X]; xyz[Y] = t[Y]; xyz[Z] = t[Z]; xyz[W] = t[W];										//	copy the values into the tuple_3d
}																																								//	end

//------------------------------------------------------------------------------
//	constructor
//------------------------------------------------------------------------------
tuple_3d::tuple_3d (real x, real y, real z, real w)															//	constructor from 4 values
{																																								//	begin
	xyz[X] = x; xyz[Y] = y; xyz[Z] = z; xyz[W] = w;																//	copy the values into the tuple_3d
}																																								//	end

//------------------------------------------------------------------------------
//	assignment operator
//------------------------------------------------------------------------------
void	tuple_3d::operator = (const tuple_3d &t)																	//	assignment operator
{																																								//	begin
	xyz[X] = t[X]; xyz[Y] = t[Y]; xyz[Z] = t[Z]; xyz[W] = t[W];										//	copy the values into the tuple_3d
}																																								//	end

//------------------------------------------------------------------------------
//	equality test
//------------------------------------------------------------------------------
bool	tuple_3d::operator == (const tuple_3d &t) const														//	equality operator
{																																								//	begin
	return	bool ((abs (xyz[X] - t[X]) < EPSILON) && 															//	compare the x coordinates
								(abs (xyz[Y] - t[Y]) < EPSILON) &&															//	compare the y coordinates
								(abs (xyz[Z] - t[Z]) < EPSILON) && 															//	compare the z coordinates
								(abs (xyz[W] - t[W]) < EPSILON));																//	compare the w coordinates			
}																																								//	end

//------------------------------------------------------------------------------
//	inequality test
//------------------------------------------------------------------------------
bool	tuple_3d::operator != (const tuple_3d &t) const														//	inequality operator
{																																								//	begin
	return	bool ((abs (xyz[X] - t[X]) > EPSILON) || 															//	compare the x coordinates
								(abs (xyz[Y] - t[Y]) > EPSILON) ||															//	compare the y coordinates
								(abs (xyz[Z] - t[Z]) > EPSILON) || 															//	compare the z coordinates
								(abs (xyz[W] - t[W]) > EPSILON));																//	compare the w coordinates			
}																																								//	end

//------------------------------------------------------------------------------
//	assignment
//------------------------------------------------------------------------------
void	tuple_3d::operator () (real x, real y, real z, real w)										//	function call operator
{																																								//	begin
	xyz[X] = x; xyz[Y] = y; xyz[Z] = z; xyz[W] = w;																//	copy the values into the tuple_3d
}																																								//	end

//------------------------------------------------------------------------------
//	dot product
//------------------------------------------------------------------------------
real	tuple_3d::operator | (const tuple_3d &t) const														//	inner product operator
{																																								//	begin
	return 	(xyz[X] * t[X]) + 																										//	return a scalar, x coordinate multiply
					(xyz[Y] * t[Y]) +																											//	y coordinate multiply
					(xyz[Z] * t[Z]) +																											//	z coordinate multiply
					(xyz[W] * t[W]);																											//	w coordinate multiply
}																																								//	end

//------------------------------------------------------------------------------
//	compute the major axis
//------------------------------------------------------------------------------
coord	tuple_3d::MajorAxis (void) const																					//	return the major axis of the tuple_3d
{																																								//	begin
	coord	axis = X;																																//	the major axis of the tuple_3d, start with x
	if (abs (xyz[Y]) > abs (xyz[X]))																							//	if the y component of the tuple_3d is greater than x
		axis = Y;																																		//	y is the major axis
	if (abs (xyz[Z]) > abs (xyz[axis]))																						//	if the Z component of the tuple_3d is greater
		axis = Z;																																		//	z is the major axis
	return axis;																																	//	return the axis
}																																								//	end

//------------------------------------------------------------------------------
//	compute the minor axis
//------------------------------------------------------------------------------
coord	tuple_3d::MinorAxis (void) const																					//	return the minor axis of the tuple_3d
{																																								//	begin
	coord	axis = X;																																//	the major axis of the tuple_3d, start with x
	if (abs (xyz[Y]) <= abs (xyz[X]))																							//	if the y component of the tuple_3d is greater than x
		axis = Y;																																		//	y is the major axis
	if (abs (xyz[Z]) <= abs (xyz[axis]))																					//	if the Z component of the tuple_3d is greater
		axis = Z;																																		//	z is the major axis
	return axis;																																	//	return the axis
}																																								//	end

//------------------------------------------------------------------------------
