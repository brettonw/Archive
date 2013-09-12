//------------------------------------------------------------------------------
//	File:					draw.cp
//	Date:					9/30/96
//	Author:				Bretton Wade
//
//	Description:	this file contains the interface functions for some basic
//								drawing operations
//
//------------------------------------------------------------------------------

#include "precompile.h"
#include "view.h"
#include "draw.h"

//------------------------------------------------------------------------------
//	functions
//------------------------------------------------------------------------------
void		MoveTo (const point_2d &pt)																							//	move to a point_3d
{																																								//	begin
	if (view::current)																														//	check for a valid view
		view::current->MoveToPt (pt);																								//	move the pen
}																																								//	end

//------------------------------------------------------------------------------
//	functions
//------------------------------------------------------------------------------
void		LineTo (const point_2d &pt)																							//	draw a line to a point_3d
{																																								//	begin
	if (view::current)																														//	check for a valid view
		view::current->LineToPt (pt);																								//	draw the line
}																																								//	end

//------------------------------------------------------------------------------
//	functions
//------------------------------------------------------------------------------
void		Circle (const point_2d &a, const point_2d &b)														//	draw a circle defined by a rectangle
{																																								//	begin
	if (view::current)																														//	check for a valid view
		view::current->Circle (a, b);																								//	draw the circle
}																																								//	end

//------------------------------------------------------------------------------
//	functions
//------------------------------------------------------------------------------
void		CrossHair (const point_2d &pt)																					//	draw a crosshair at a point_3d
{																																								//	begin
	if (view::current)																														//	check for a valid view
		view::current->CrossHair (pt);																							//	draw the crosshair
}																																								//	end

//------------------------------------------------------------------------------
//	functions
//------------------------------------------------------------------------------
void		DrawPolygon (polyptr poly)																							//	draw a polygon
{																																								//	begin
	if (view::current)																														//	check for a valid view
		view::current->DrawPolygon (poly);																					//	draw the polygon
}																																								//	end


//------------------------------------------------------------------------------
//	functions
//------------------------------------------------------------------------------
HDC		GetCurrentDC (void)																												//	return a drawing context
{																																								//	begin
	if (view::current)																														//	check for a valid view
		return view::current->hdc;																									//	return the current dc
	return 0;																																			//	return null
}																																								//	end


//------------------------------------------------------------------------------
