//------------------------------------------------------------------------------
//	File:					arcball.cp
//	Date:					9/30/96
//	Author:				Bretton Wade
//
//	Description:	this file contains the class methods for Ken Shoemake's
//								ArcBall rotation controller
//								
//								This class presumes the existence of a number of drawing
//								routines, and a graphics system that uses Virtual Device
//								Coordinates ranging from -1 to 1 in both x and y. The
//								ArcBall itself and the feedback are projected orthographically
//								onto the screen.
//								
//------------------------------------------------------------------------------

#include "precompile.h"
#include "draw.h"
#include "arcball.h"
#include "quaternion_3d.h"

//------------------------------------------------------------------------------
//	constructor
//------------------------------------------------------------------------------
arcball::arcball (const point_2d &cent, real rad)																//	normal constructor
{																																								//	begin
	center = vector_2d (cent);																										//	store the center point_2d of the arcball
	radius = rad;																																	//	store the radius of the arcball
}																																								//	end

//------------------------------------------------------------------------------
//	take begin of drag
//------------------------------------------------------------------------------
void	arcball::Click (const point_2d &pt)																				//	the first point_3d in the arc
{																																								//	begin
	start_vec = MapToSphere (pt - center);																				//	map the point_3d to the arcball sphere
	start_pt = point_2d (start_vec[X], start_vec[Y]) + center;										//	save the mapped vector_3d as a point_3d too
}																																								//	end

//------------------------------------------------------------------------------
//	complete drag
//------------------------------------------------------------------------------
matrix_3d	arcball::Drag (const point_2d &pt)																		//	the second point_3d in the arc
{																																								//	begin
	end_vec = MapToSphere (pt - center);																					//	map the point_3d to the arcball sphere
	end_pt = point_2d (end_vec[X], end_vec[Y]) + center;													//	save the mapped vector_3d as a point_3d too
	return Quaternion (start_vec, end_vec);																				//	return the matrix_3d equivalent to the quaternion
}																																								//	end

//------------------------------------------------------------------------------
//	draw the sphere of the arcball
//------------------------------------------------------------------------------
void	arcball::DrawBackground (void) const																			//	draw the sphere
{																																								//	begin
	HDC		hdc = GetCurrentDC ();																									//	get teh current drawing context
	HPEN		pen = CreatePen (PS_SOLID, 0, 0x00555555);														//	make a pen for the polygon outlines
	HBRUSH	brush = CreateSolidBrush (0x00333333);																//	make a brush for the polygon fill
	HGDIOBJ	oldpen = SelectObject (hdc, pen);																			//	select the pen
	HGDIOBJ	oldbrush = SelectObject (hdc, brush);																	//	select teh brush
	point_2d	A = point_2d (-radius, -radius) + center,														//	compute the bottom left point_3d of a rectangle
						B = point_2d (radius, radius) + center;															//	compute the top right of a rectangle
	Circle (A, B);																																//	draw a circle inside the rectangle in the view
	SelectObject (hdc, oldbrush);																									//	restore the old brush
	SelectObject (hdc, oldpen);																										//	restore the old pen
	DeleteObject (pen);																														//	free up the pen
	DeleteObject (brush);																													//	free up the pen
}																																								//	end

//------------------------------------------------------------------------------
//	draw all the arcball feedback
//------------------------------------------------------------------------------
void	arcball::DrawForeground (void) const																			//	draw the arcball interface
{																																								//	begin
	HDC		hdc = GetCurrentDC ();																									//	get teh current drawing context
	HPEN	pen = CreatePen (PS_SOLID, 0, 0x00ff0000);															//	make a pen for the arc
	HGDIOBJ	oldpen = SelectObject (hdc, pen);																			//	select the pen
	DrawArc (5);																																	//	draw the great arc on the arcball sphere
	SelectObject (hdc, oldpen);																										//	restore the old pen
	DeleteObject (pen);																														//	free up the pen
	pen = CreatePen (PS_SOLID, 0, 0x000000ff);																		//	make a pen for the crosshair
	oldpen = SelectObject (hdc, pen);																							//	select the pen
	CrossHair (end_pt);																														//	draw a crosshair at the end point_3d
	SelectObject (hdc, oldpen);																										//	restore the old pen
	DeleteObject (pen);																														//	free up the pen
	pen = CreatePen (PS_SOLID, 0, 0x00ffffff);																		//	make a pen for the crosshair
	oldpen = SelectObject (hdc, pen);																							//	select the pen
	CrossHair (start_pt);																													//	draw a crosshair at the atrt point_3d
	SelectObject (hdc, oldpen);																										//	restore the old pen
	DeleteObject (pen);																														//	free up the pen
}																																								//	end

//------------------------------------------------------------------------------
//	compute the segments on the feedback arc
//------------------------------------------------------------------------------
void	arcball::ComputeArc (short segs, const vector_3d &a, const vector_3d &b) const	//	compute the arc between two points with the given number of segments
{																																								//	begin
	if (segs)																																			//	if more segments need to be computed
	{																																							//	begin
		vector_3d	bisector = (a + b).Normalize () * radius;													//	compute the vector_3d that bisects the angle between 'a' and 'b'
		ComputeArc (segs - 1, a, bisector);																					//	compute the sub arc from 'a' to the 'bisector'
		ComputeArc (segs - 1, bisector, b);																					//	compute the sub arc from the 'bisector' to 'b'
	}																																							//	end
	else																																					//	otherwise
		LineTo (point_2d (b[X], b[Y]) + center);																		//	draw a line to the segment endpoint
}																																								//	end

//------------------------------------------------------------------------------
//	draw the feedback arc
//------------------------------------------------------------------------------
void	arcball::DrawArc (short numsegs) const																		//	draw the arc between the two points
{																																								//	begin
	MoveTo (start_pt);																														//	move the pen to the start_vec point_3d
	ComputeArc (numsegs, start_vec, end_vec);																			//	compute the arc and draw each segment
}																																								//	end

//------------------------------------------------------------------------------
//	map some click point_3d to the arcball sphere
//------------------------------------------------------------------------------
vector_3d	arcball::MapToSphere (const point_2d &pt)															//	compute the intersection pt of the arcball with a line to the point_3d
{																																								//	begin
	real	r = (pt[X] * pt[X]) + (pt[Y] * pt[Y]),																	//	compute the square of the length of the vector_3d to the point_3d from the center
				radsq = radius * radius;																								//	compute the square of the radius of the arcball sphere
	if (r > radsq)																																//	if the point_3d is mapped outside of the arcball sphere
	{																																							//	begin
		real	s = radius / SQRT (r);																								//	compute a normalizing factor
		return vector_3d (pt[X] * s, pt[Y] * s, R(0.0));														//	return the "normalized" vector_3d, a point_3d on the sphere
	}																																							//	end
	else																																					//	alse
		return vector_3d (pt[X], pt[Y], SQRT (radsq - r));													//	return a vector_3d to a point_3d mapped inside the sphere
}																																								//	end

//------------------------------------------------------------------------------
