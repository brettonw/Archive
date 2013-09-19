//------------------------------------------------------------------------------
//	File:					polygon.cp
//	Date:					05/02/97
//	Author:				Bretton Wade
//
//	Description:	this file contains the methods for a polygon
//
//------------------------------------------------------------------------------

#include "precompile.h"
#include "polygon_3d.h"

//------------------------------------------------------------------------------
//	constructor
//------------------------------------------------------------------------------
polygon::polygon (point_3d *buffer, uchar cnt, va_list pts)											//	normal constructor
{																																								//	begin
	count = cnt;																																	//	copy the point_3d count
	points = new point_3d[count];																									//	allocate the array of points
	for (short i = 0; i < count; i++)																							//	loop for every point_3d
		points[i] = buffer[va_arg(pts, int)];																				//	copying it into the points array of the polygon
	plane.Define (Normal (), points[0]);																					//	compute the plane_3d equation
	ComputeArea ();																																//	compute the polygon area
	emit = R(0.0);																																//	no emissivity
	power = R(0.0);																																//	no impinging power
}																																								//	end

//------------------------------------------------------------------------------
//	constructor
//------------------------------------------------------------------------------
polygon::polygon (point_3d *buffer, uchar cnt)																	//	normal constructor
{																																								//	begin
	count = cnt;																																	//	copy the point_3d count
	points = new point_3d[count];																									//	allocate the array of points
	for (short i = 0; i < count; i++)																							//	loop for every point_3d
		points[i] = buffer[i];																											//	copying it into the points array of the polygon
	plane.Define (Normal (), points[0]);																					//	compute the plane_3d equation
	ComputeArea ();																																//	compute the polygon area
	emit = R(0.0);																																//	no emissivity
	power = R(0.0);																																//	no impinging power
}																																								//	end

//------------------------------------------------------------------------------
//	destructor
//------------------------------------------------------------------------------
polygon::~polygon (void)																												//	destructor
{																																								//	begin
	delete[] points;																															//	delete the point_3d array
}																																								//	end

//------------------------------------------------------------------------------
//	compute the polygon normal
//------------------------------------------------------------------------------
vector_3d	polygon::Normal (void) const																					//	compute the plane_3d normal vector_3d using Newell's method
{																																								//	begin
	//	assumes counter-clockwise point_3d order
	vector_3d	sum (ZERO_VECTOR);																									//	start with a zero vector_3d
	for (short i = 0, last = count - 1; i < count; last = i, i++)									//	loop through all of the points
	{																																							//	begin
		point_3d	A = points[last],																									//	a point_3d
							B = points[i];																										//	the following point_3d
		sum[X] += (A[Y] - B[Y]) * (A[Z] + B[Z]);																		//	add this edge's contribution to the average vector_3d
		sum[Y] += (A[Z] - B[Z]) * (A[X] + B[X]);																		//	add this edge's contribution to the average vector_3d
		sum[Z] += (A[X] - B[X]) * (A[Y] + B[Y]);																		//	add this edge's contribution to the average vector_3d
	}																																							//	end
	return sum.Normalize ();																											//	return the unit length normal vector_3d
}																																								//	end

//------------------------------------------------------------------------------
//	check polygon for containment of a coplanar point_3d
//------------------------------------------------------------------------------
bool	polygon::Contains (const point_3d &pt) const															//	test the point_3d to see if it is inside the polygon
{																																								//	begin
	//	based on code by Eric Haines from Graphics Gems IV
	coord	x, y;																																		//	indexing values
	switch (plane.MajorAxis ())																										//	switch on the major axis of the plane_3d
	{																																							//	begin
		case X:	x = Y; y = Z; break;																								//	throw away the x coordinate
		case Y:	x = Z; y = X; break;																								//	throw away the y coordinate
		case Z:	x = X; y = Y; break;																								//	throw away the z coordinate
	}																																							//	end
	real	tx = pt[x], ty = pt[y];																									//	temporary values
	point_3d	*p1 = &points[count - 1], *p2 = points;															//	pointers to the points composing the edge being tested
	int		yflag0 = ((*p1)[y] >= ty),																							//	check to see which side of the test point_3d the first point_3d is on
				inside = FALSE;																													//	start with inside false
  for (int i = count; i--;)																											//	loop over all of the points
	{																																							//	begin
		int	yflag1 = ((*p2)[y] >= ty);																							//	check which side of the test point_3d the subsequent point_3d lies on
		if (yflag0 != yflag1)																												//	if the points aren't on the same side
		{																																						//	begin
	  	int	xflag0 = ((*p1)[x] >= tx);																						//	check which side of the test coordinate the start point_3d is on
	    if (xflag0 == ((*p2)[x] >= tx))																						//	if the edge is all the way to one side of the test point_3d
			{																																					//	begin
				if (xflag0)																															//	if the start point_3d is greater than the test point_3d
					inside = !inside;																											//	increment the crossing count
			}																																					//	end
	   	else																																			//	otherwise, the edge spans the test point_3d on both axes
			{																																					//	begin
				if (((*p2)[x] - ((*p2)[y] - ty) * ((*p1)[x] - (*p2)[x]) / ((*p1)[y] - (*p2)[y])) >= tx)	//	if the intersection of the tx axis is on the right side of the test point_3d
		    	inside = !inside;																											//	increment the crossing count
			}																																					//	end
		}																																						//	end
		yflag0 = yflag1;																														//	save the point_3d classification
		p1 = p2;																																		//	skip to the subsequent point_3d
		p2 ++;																																			//	advance the subsequent point_3d
   }																																						//	end
   return bool (inside);																												//	return the result
}																																								//	end

//------------------------------------------------------------------------------
//	compute the area of the polygon
//------------------------------------------------------------------------------
void	polygon::ComputeArea (void)																								//	return the area of the polygon
{																																								//	begin
	vector_3d	sum = ZERO_VECTOR;																									//	start with 0
	for (int i = 0, last = count - 1; i < count; last = i, i++)										//	loop over all of the vertices in the polygon
		sum += (vector_3d (points[last])) ^ (vector_3d (points[i]));								//	add in the cross product of the two vectors between the current pair of points
	area = R(0.5) * sum.Norm ();																									//	divide the total vector lewngth by two to get the area
}																																								//	end

//------------------------------------------------------------------------------
//	reverse the orientation of the polygon
//------------------------------------------------------------------------------
void	polygon::Invert (void)																										//	completely reverse the orientation of the polygon
{																																								//	begin
	point_3d	*pts = new point_3d[count];																					//	allocate a new block of points
	short	reverse = count - 1;																										//	presubtract the count to find the reverse addition value
	for (short i = 0; i < count; i++)																							//	loop over all the points again
		pts[i] = points[reverse - i];																								//	copying them in reverse order
	delete[] points;																															//	delete the old points array
	points = pts;																																	//	set the points array
	plane.Invert ();																															//	reverse the plane_3d equation
}																																								//	end

//------------------------------------------------------------------------------
//	compute the distance at which the ray intersects the polygon
//------------------------------------------------------------------------------
real		polygon::RayIntersection (const ray &r) const														//	return the distance along the ray at which the intersection occurs
{																																								//	begin
	return	plane.RayIntersection (r);																						//	return the distance along the ray to the plane_3d
}																																								//	end

//------------------------------------------------------------------------------
