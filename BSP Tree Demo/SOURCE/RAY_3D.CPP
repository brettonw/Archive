//------------------------------------------------------------------------------
//	File:					ray.cp
//	Date:					9/22/94
//	Author:				Bretton Wade
//
//	Description:	this file contains the class methods for a ray
//
//------------------------------------------------------------------------------

#include "precompile.h"
#include "ray_3d.h"

//------------------------------------------------------------------------------
//	constructor
//------------------------------------------------------------------------------
ray::ray (const point_3d &orig, const vector_3d &dir)														//	constructor
{																																								//	begin
	origin = orig; direction = dir;																								//	copy the parameters
	direction.Normalize ();																												//	make sure the direction vector_3d has length 1
}																																								//	end

//------------------------------------------------------------------------------
//	compute the intersection point_3d
//------------------------------------------------------------------------------
point_3d		ray::IntersectionPoint (real distance) const												//	compute the point_3d at which the intersection occurs
{																																								//	begin
	return origin + (direction * distance);																				//	return the point_3d
}																																								//	end
//------------------------------------------------------------------------------
