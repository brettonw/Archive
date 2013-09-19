//------------------------------------------------------------------------------
//	File:					polygon.cp
//	Date:					05/02/97
//	Author:				Bretton Wade
//
//	Description:	this file contains the methods for a polygon
//
//------------------------------------------------------------------------------

#include <iostream>

#include "precompile.h"
#include "polygon_3d.h"
#include	"transform_3d.h"
#include	"view.h"

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
	xform = VectorMatrix (plane);
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
	xform = VectorMatrix (plane);
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
//	compute triangle area
//------------------------------------------------------------------------------
real	ComputeTriangleArea (const point_3d& a, const point_3d& b, const point_3d& c)
{
	vector_3d	A (a),
				B (b),
				C (c),
				sum = (A ^ B) + (B ^ C) + (C ^ A);
	return R(0.5) * sum.Norm ();																									//	divide the total vector lewngth by two to get the area
}

//------------------------------------------------------------------------------
//	compute the area of the polygon
//------------------------------------------------------------------------------
void	polygon::ComputeArea (void)																								//	return the area of the polygon
{																																								//	begin
	int	    triangle_count = count - 2;
	vector_3d   last = points[1] - points[0];
	little_area = new real[triangle_count];
	area = R(0.0);
	for (int i = 2, area_i = 0; i < count; i++, area_i++)										//	loop over all of the vertices in the polygon
	{
		little_area[area_i] = ComputeTriangleArea (points[0], points[i - 1], points[i]);
		area += little_area[area_i];								//	add in the cross product of the two vectors between the current pair of points
	}
	//cerr << "Polygon Area: " << area << endl << "  Made of: ";
	for (int i = 0; i < triangle_count; i++)
	{
	    //cerr << little_area[i] << " ";
	    little_area[i] /= area;
	}
	//cerr << endl;
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
//	return a parameterized sample point on the polygon
//------------------------------------------------------------------------------
point_3d	polygon::SamplePoint (void)																					//	return a sample point on the polygon
{
	real	s = sDist.Sample (),
			t = tDist.Sample (),
    		sum = R(0.0);
	int		triangle_count = count - 2,
			i;
	if (s >= R(1.0))
		i = triangle_count - 1;
	else
		for (i = 0; s > (sum + little_area[i]); i++)
			sum += little_area[i];
	s = (s - sum) / little_area[i];
    t = SQRT (t);
    real	a = R(1.0) - t,
		b = (R(1.0) - s) * t,
		c = s * t;
    vector_3d	A (points[0]),
		B (points[++i]),
		C (points[++i]);
	return ((a * A) + (b * B) + (c * C));
}

//------------------------------------------------------------------------------
vector_3d		polygon::SampleVector (void)
{
	real	theta = thetaDist.Sample (),
			phi = phiDist.Sample ();
	return view::Phong (theta, phi) * xform;
}

//------------------------------------------------------------------------------
//	compute the distance at which the ray intersects the polygon
//------------------------------------------------------------------------------
real		polygon::RayIntersection (const ray &r) const														//	return the distance along the ray at which the intersection occurs
{																																								//	begin
	return	plane.RayIntersection (r);																						//	return the distance along the ray to the plane_3d
}																																								//	end

//------------------------------------------------------------------------------
