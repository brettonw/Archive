//------------------------------------------------------------------------------
//	File:					plane_3d.cp
//	Date:					8/26/94
//	Author:				Bretton Wade
//
//	Description:	this file contains the methods for a plane_3d
//
//------------------------------------------------------------------------------

#include "precompile.h"
#include "utility.h"
#include "plane_3d.h"

//------------------------------------------------------------------------------
//	constructor
//------------------------------------------------------------------------------
plane_3d::plane_3d (const vector_3d& v, const point_3d& p)											//	normal constructor
: tuple_3d (v[X], v[Y], v[Z], - (v | p))																				//	superclass constructor
{																																								//	begin
}																																								//	end

//------------------------------------------------------------------------------
//	constructor
//------------------------------------------------------------------------------
plane_3d::plane_3d (const point_3d& a, const point_3d& b, const point_3d& c)		//	normal constructor
{																																								//	begin
	vector_3d	v = ((b - a) ^ (c - b)).Normalize ();																//	compute the normal vector_3d
	tuple_3d::operator () (v[X], v[Y], v[Z], - (v | a));													//	set up the plane_3d values
}																																								//	end

//------------------------------------------------------------------------------
//	constructor
//------------------------------------------------------------------------------
plane_3d::plane_3d (real a, real b, real c, real d) : tuple_3d (a, b, c, d)			//	component constructor
{																																								//	begin
}																																								//	end

//------------------------------------------------------------------------------
//	constructor
//------------------------------------------------------------------------------
plane_3d::plane_3d (const plane_3d& p) : tuple_3d (p)														//	copy constructor
{																																								//	begin
}																																								//	end

//------------------------------------------------------------------------------
//	constructor
//------------------------------------------------------------------------------
plane_3d::plane_3d (const tuple_3d& t) : tuple_3d (t)														//	constructor from a tuple_3d
{																																								//	begin
}																																								//	end

//------------------------------------------------------------------------------
//	destructor
//------------------------------------------------------------------------------
plane_3d::~plane_3d (void)																											//	destructor
{																																								//	begin
}																																								//	end

//------------------------------------------------------------------------------
//	assignment
//------------------------------------------------------------------------------
plane_3d	&plane_3d::operator = (const plane_3d& p)															//	assignment operator
{																																								//	begin
	tuple_3d::operator () (p[X], p[Y], p[Z], p[W]);																//	copy the plane_3d values
	return *this;																																	//	return the reference to this
}																																								//	end

//------------------------------------------------------------------------------
//	assignment
//------------------------------------------------------------------------------
plane_3d	&plane_3d::operator = (const tuple_3d& t)															//	assignment operator
{																																								//	begin
	tuple_3d::operator = (t);																											//	copy the plane_3d values
	return *this;																																	//	return the reference to this
}																																								//	end

//------------------------------------------------------------------------------
//	compute the plane_3d equation
//------------------------------------------------------------------------------
void	plane_3d::Define (const vector_3d& v, const point_3d& p)									//	compute the plane_3d equation
{																																								//	begin
	tuple_3d::operator () (v[X], v[Y], v[Z], - (v | p));													//	set the tuple_3d values
}																																								//	end

//------------------------------------------------------------------------------
//	compute the plane_3d equation
//------------------------------------------------------------------------------
void	plane_3d::Define (const point_3d& a, const point_3d& b, const point_3d& c)//	compute the plane_3d equation
{																																								//	begin
	vector_3d	v = (b - a) ^ (c - b);																							//	compute the normal vector_3d
	tuple_3d::operator () (v[X], v[Y], v[Z], - (v | a));													//	set up the plane_3d values
}																																								//	end

//------------------------------------------------------------------------------
//	invert the plane_3d
//------------------------------------------------------------------------------
void	plane_3d::Invert (void)																										//	invert the values of the plane_3d equation
{																																								//	begin
	xyz[X] *= R(-1.0); xyz[Y] *= R(-1.0); xyz[Z] *= R(-1.0); xyz[W] *= R(-1.0);		//	reverse the tuple_3d values
}																																								//	end

//------------------------------------------------------------------------------
//	compute the distance at which the ray intersects the plane_3d
//------------------------------------------------------------------------------
real		plane_3d::RayIntersection (const ray& r) const													//	return the distance along the ray at which the intersection occurs
{																																								//	begin
	real costheta = -(r.Direction () | *this);																		//	compute the cosine of the angle between the ray and the plane_3d normal
	if (FABS (costheta) < EPSILON) return R(-1.0);																	//	return a -1 for plane_3d and ray parallel
	return (r.Origin () | *this) / costheta;																			//	compute the distance from the ray origin to the plane_3d, and divide by the cosine 
}																																								//	end

//------------------------------------------------------------------------------
