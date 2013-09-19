//------------------------------------------------------------------------------
//	File:					bound_3d.cp
//	Date:					9/26/94
//	Author:				Bretton Wade
//
//	Description:	this file contains the class methods for a bounding box
//
//------------------------------------------------------------------------------

#include "precompile.h"
#include "bound_3d.h"

//------------------------------------------------------------------------------
//	constructor
//------------------------------------------------------------------------------
bound_3d::bound_3d (void)																												//	constructor
{																																								//	begin
	max (-INFINITY, -INFINITY, -INFINITY);																				//	start max at -inf
	min (INFINITY, INFINITY, INFINITY);																						//	start min at inf
}																																								//	end

//------------------------------------------------------------------------------
//	self addition
//------------------------------------------------------------------------------
void		bound_3d::operator += (const bound_3d &box)															//	add a bounding box into this one
{																																								//	begin
	if (box.max[X] > max[X]) max[X] = box.max[X] + EPSILON;												//	compare the coordinate value, and replace if the magintude is appropriate
	if (box.max[Y] > max[Y]) max[Y] = box.max[Y] + EPSILON;												//	compare the coordinate value, and replace if the magintude is appropriate
	if (box.max[Z] > max[Z]) max[Z] = box.max[Z] + EPSILON;												//	compare the coordinate value, and replace if the magintude is appropriate
	if (box.min[X] < min[X]) min[X] = box.min[X] - EPSILON;												//	compare the coordinate value, and replace if the magintude is appropriate
	if (box.min[Y] < min[Y]) min[Y] = box.min[Y] - EPSILON;												//	compare the coordinate value, and replace if the magintude is appropriate
	if (box.min[Z] < min[Z]) min[Z] = box.min[Z] - EPSILON;												//	compare the coordinate value, and replace if the magintude is appropriate
}																																								//	end

//------------------------------------------------------------------------------
//	self addition
//------------------------------------------------------------------------------
void		bound_3d::operator += (const point_3d &pt)															//	add a point_3d into the bounding box
{																																								//	begin
	if (pt[X] > max[X]) max[X] = pt[X] + EPSILON;																	//	compare the coordinate value, and replace if the magintude is appropriate
	if (pt[Y] > max[Y]) max[Y] = pt[Y] + EPSILON;																	//	compare the coordinate value, and replace if the magintude is appropriate
	if (pt[Z] > max[Z]) max[Z] = pt[Z] + EPSILON;																	//	compare the coordinate value, and replace if the magintude is appropriate
	if (pt[X] < min[X]) min[X] = pt[X] - EPSILON;																	//	compare the coordinate value, and replace if the magintude is appropriate
	if (pt[Y] < min[Y]) min[Y] = pt[Y] - EPSILON;																	//	compare the coordinate value, and replace if the magintude is appropriate
	if (pt[Z] < min[Z]) min[Z] = pt[Z] - EPSILON;																	//	compare the coordinate value, and replace if the magintude is appropriate
}																																								//	end

//------------------------------------------------------------------------------
//	containment test
//------------------------------------------------------------------------------
bool		bound_3d::Contains (const point_3d &pt) const														//	return whether or not a point_3d is inside the bounding box
{																																								//	begin
	return	bool ((pt[X] <= max[X]) && (pt[X] >= min[X]) &&												//	if the x component is in
								(pt[Y] <= max[Y]) && (pt[Y] >= min[Y]) &&												//	and the y component is on
								(pt[Z] <= max[Z]) && (pt[Z] >= min[Z]));												//	and the z component is in...
}																																								//	end

//------------------------------------------------------------------------------
