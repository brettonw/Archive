//------------------------------------------------------------------------------
//	File:					cube.cp
//	Date:					11/02/94
//	Author:				Bretton Wade
//
//	Description:	this file contains the function for cube generation
//
//------------------------------------------------------------------------------

#include "precompile.h"
#include "cube.h"

//------------------------------------------------------------------------------
//	generate a cube
//------------------------------------------------------------------------------
listptr	Cube (matrix_3d transformation)																					//	compute a cube given a transformation
{																																								//	begin
	point_3d		cube_points[8];																										//	eight vertices
	//	compute the vertices
	cube_points[0] = point_3d (R( 1.0), R( 1.0), R( 1.0)) * transformation;
	cube_points[1] = point_3d (R(-1.0), R( 1.0), R( 1.0)) * transformation;
	cube_points[2] = point_3d (R(-1.0), R(-1.0), R( 1.0)) * transformation;
	cube_points[3] = point_3d (R( 1.0), R(-1.0), R( 1.0)) * transformation;
	cube_points[4] = point_3d (R( 1.0), R( 1.0), R(-1.0)) * transformation;
	cube_points[5] = point_3d (R(-1.0), R( 1.0), R(-1.0)) * transformation;
	cube_points[6] = point_3d (R(-1.0), R(-1.0), R(-1.0)) * transformation;
	cube_points[7] = point_3d (R( 1.0), R(-1.0), R(-1.0)) * transformation;
	//	now compute the polygons
	listptr	cube_polys;
	cube_polys->AddToList (polyptr (cube_points, 4, 0, 1, 2, 3));
	cube_polys->AddToList (polyptr (cube_points, 4, 7, 6, 5, 4));
	cube_polys->AddToList (polyptr (cube_points, 4, 0, 3, 7, 4));
	cube_polys->AddToList (polyptr (cube_points, 4, 0, 4, 5, 1));
	cube_polys->AddToList (polyptr (cube_points, 4, 5, 6, 2, 1));
	cube_polys->AddToList (polyptr (cube_points, 4, 3, 2, 6, 7));
	//	return the result
	return cube_polys;
}																																								//	end

//------------------------------------------------------------------------------
