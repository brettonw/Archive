//------------------------------------------------------------------------------
//	File:					cube.cp
//	Date:					05/02/97
//	Author:				Bretton Wade
//
//	Description:	this file contains the function for cube generation
//
//------------------------------------------------------------------------------

#include "precompile.h"
#include "cube.h"
#include "iterator_3d.h"

//------------------------------------------------------------------------------
//	dicing parameters
//------------------------------------------------------------------------------
#define	DICE_RES	8
#define	POLY_RES	(DICE_RES - 1)
#define	OVER_POLY_RES	(R(1.0) / real (POLY_RES))

//------------------------------------------------------------------------------
//	generate a diced rectangle
//------------------------------------------------------------------------------
listptr	Face (point_3d *pts, int a, int b, int c)																//	return a list of polygons in a mesh
{																																								//	begin
	vector_3d	bc = (pts[c] - pts[b]) * OVER_POLY_RES,
						ab = (pts[b] - pts[a]) * OVER_POLY_RES;
	point_3d	array[DICE_RES][DICE_RES]; 
	listptr		list;
	for (int y = 0; y < DICE_RES; y++)
		for (int x = 0; x < DICE_RES; x++)
			array[x][y] = pts[a] + (bc * real (x)) + (ab * real (y));
	for (int j = 0; j < POLY_RES; j++)
		for (int i = 0; i < POLY_RES; i++)
		{
			point_3d	buffer[4];
			buffer[0] = array[i][j];
			buffer[1] = array[i][j + 1];
			buffer[2] = array[i + 1][j + 1];
			buffer[3] = array[i + 1][j];
			list->AddToList (polyptr (buffer, 4, 0, 1, 2, 3));
		}
	return list;
}																																								//	end

//------------------------------------------------------------------------------
//	generate a cube
//------------------------------------------------------------------------------
listptr	Cube (matrix_3d transformation, real emit)															//	compute a cube given a transformation
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
	listptr	cube_polys,
					tmpList;
	tmpList = Face (cube_points, 0, 1, 2);
	cube_polys->Append (tmpList);
	tmpList = Face (cube_points, 7, 6, 5);
	cube_polys->Append (tmpList);
	tmpList = Face (cube_points, 0, 3, 7);
	cube_polys->Append (tmpList);
	tmpList = Face (cube_points, 0, 4, 5);
	cube_polys->Append (tmpList);
	tmpList = Face (cube_points, 5, 6, 2);
	cube_polys->Append (tmpList);
	tmpList = Face (cube_points, 3, 2, 6);
	cube_polys->Append (tmpList);
	//	set the emissivity of all the polygons in the list
	iterator	entry (cube_polys);																									//	list iterator
	for (polyptr poly = entry (); poly (); poly = entry ())												//	do for all the entries in the list
		poly->SetEmissivity (emit);
	//	return the result
	return cube_polys;
}																																								//	end

//------------------------------------------------------------------------------
