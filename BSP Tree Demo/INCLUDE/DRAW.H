//------------------------------------------------------------------------------
//	File:					draw.h
//	Date:					9/30/96
//	Author:				Bretton Wade
//
//	Description:	this file contains the interface functions for some basic
//								drawing operations
//
//------------------------------------------------------------------------------

#include	"point_2d.h"
#include	"polyptr_3d.h"

#ifndef	DRAW
#define	DRAW

//------------------------------------------------------------------------------
//	functions
//------------------------------------------------------------------------------
void		MoveTo (const point_2d &pt);																						//	move to a point_3d
void		LineTo (const point_2d &pt);																						//	draw a line to a point_3d
void		Circle (const point_2d &a, const point_2d &b);													//	draw a circle defined by a rectangle
void		CrossHair (const point_2d &pt);																					//	draw a crosshair at a point_3d
void		DrawPolygon (polyptr poly);																							//	draw a polygon
HDC			GetCurrentDC (void);																										//	return a drawing context

//------------------------------------------------------------------------------

#endif	//DRAW
