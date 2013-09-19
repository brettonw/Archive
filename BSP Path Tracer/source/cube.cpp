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
#include "split.h"
#include "control.h"

//------------------------------------------------------------------------------
//	generate a diced rectangle
//------------------------------------------------------------------------------
void	FaceRecursiveSubdivide (listptr& list, const polyptr& poly, real max_area)
{
	polyptr	a, b;
	if (Split (poly, max_area, a, b))
	{
		FaceRecursiveSubdivide (list, a, max_area);
		FaceRecursiveSubdivide (list, b, max_area);
	}
	else
		list->AddToList (poly);
}
																//	return a list of polygons in a mesh
//------------------------------------------------------------------------------
//	generate a diced rectangle
//------------------------------------------------------------------------------
listptr	Face (point_3d *pts, int a, int b, int c)																//	return a list of polygons in a mesh
{																																								//	begin
	listptr		list;
	point_3d	array[4];
	array[0] = pts[a];
	array[1] = pts[b];
	array[2] = pts[c];
	array[3] = pts[a] + (pts[c] - pts[b]);
	polyptr		poly (array, 4, 0, 1, 2, 3);
	FaceRecursiveSubdivide (list, poly, MAX_POLY_AREA);
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
