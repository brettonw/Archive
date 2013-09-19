//------------------------------------------------------------------------------
//	File:					split.cp
//	Date:					04/29/97
//	Author:				Bretton Wade
//
//	Description:	this file contains the method for splitting a polygon
//								with a plane.
//
//------------------------------------------------------------------------------

#include "precompile.h"
#include <iostream>
#include "utility.h"
#include "split.h"

//------------------------------------------------------------------------------
static	point_3d	gOutpts[64],																									//	an array of points for the out polygon
					gInpts[64];																									//	an array of points for the in polygon

//------------------------------------------------------------------------------
//	plane the polygon with a plane_3d
//------------------------------------------------------------------------------
hclass	Split (const polyptr &poly, const plane_3d &plane, polyptr &in, polyptr &out)//	split the polygon with a plane_3d
{																																								//	begin
	int				out_c = 0, in_c = 0;																								//	set the point_3d counts to 0
	point_3d	ptA, ptB;																														//	two points to step through
	real			sideA, sideB;																												//	the classification of ptA and ptB respectively
	hclass		poly_class = HC_ON;																									//	assume plane_3d and polygon coincident for starters
	ptA = poly->Vertex (poly->Count () - 1);																			//	start with the last point_3d
	sideA = ptA | plane;																													//	classify it relative to the plane_3d
	for (short i = -1; ++i < poly->Count ();)																			//	loop on the points
	{																																							//	begin
		ptB = poly->Vertex (i);																											//	get the current point_3d
		sideB = ptB | plane;																												//	classify it relative to the plane_3d
		if (sideB > EPSILON)																												//	if the current point_3d is on the positive side
		{																																						//	begin
			if (poly_class == HC_ON)																									//	if the polygon classification is on
				poly_class = HC_OUT;																										//	classify the polygon as out
			else if (poly_class != HC_OUT)																						//	else if the polygon classification is not out
				poly_class = HC_SPANNING;																								//	set the polygon classification to spanning
			if (sideA < -EPSILON)																											//	if the previous point_3d was on the opposite side of the plane_3d
			{																																					//	begin
				vector_3d	v = ptB - ptA;																								//	compute the vector_3d between the points
				gOutpts[out_c++] = gInpts[in_c++] = 																			//	add the newly computed point_3d to the partitions
					ptA + (v * (-(ptA | plane) / (v | plane)));														//	add the newly computed point_3d to the partitions
				poly_class = HC_SPANNING;																								//	set the poly_class appropriately
			}																																					//	end
			gOutpts[out_c++] = ptB;																										//	add the current point_3d to the positive partition
		}																																						//	end
		else if (sideB < -EPSILON)																									//	the current point_3d is on the negative side
		{																																						//	begin
			if (poly_class == HC_ON)																									//	if the polygon classification is on
				poly_class = HC_IN;																											//	classify the polygon as in
			else if (poly_class != HC_IN)																							//	else if the polygon classification is not in
				poly_class = HC_SPANNING;																								//	set the polygon classification to spanning
			if (sideA > EPSILON)																											//	if the previous point_3d was on the opposite side of the plane_3d
			{																																					//	begin
				vector_3d	v = ptB - ptA;																								//	compute the vector_3d between the points
				gOutpts[out_c++] = gInpts[in_c++] = 																			//	add the newly computed point_3d to the partitions
					ptA + (v * (-(ptA | plane) / (v | plane)));														//	add the newly computed point_3d to the partitions
				poly_class = HC_SPANNING;																								//	set the poly_class appropriately
			}																																					//	end
			gInpts[in_c++] = ptB;																											//	add the current point_3d to the negative partition
		}																																						//	end
		else																																				//	the current point_3d is on the plane_3d
			gOutpts[out_c++] = gInpts[in_c++] = ptB;																		//	add the current point_3d to the partitions
		ptA = ptB;																																	//	copy the current point_3d to the last point_3d
		sideA = sideB;																															//	copy the current point_3d's side information...
	}																																							//	end
	switch (poly_class)																														//	perform the appropriate action based on the classification
	{																																							//	begin
		case HC_OUT:																																//	if the polygon is entirely positive
			out = poly;																																//	make the positive partition
			break;																																		//	end positive
		case HC_IN:																																	//	if the polygon is entirely negative
			in = poly;																																//	make the negative partition
			break;																																		//	end negative
		case HC_SPANNING:																														//	if the polygon was plane
			out = new rcpolygon (gOutpts, out_c);																			//	make the positive partition
			out->SetEmissivity (poly->Emissivity ());																	//	copy the emissivity of the original polygon
			in = new rcpolygon (gInpts, in_c);																					//	make the negative partition
			in->SetEmissivity (poly->Emissivity ());																	//	copy the emissivity of the original polygon
			break;																																		//	end spanning
	}																																							//	end
	return poly_class;																														//	return the classification
}																																								//	end

//------------------------------------------------------------------------------
inline	int	MaxOf (real a, real b, real c)
{
	if (a > b)
	{
		if (a > c)
			// a is greatest
			return 0;
		else
			// c is greatest
			return 2;
	}
	else if (b > c)	// b is greater than a
		// b is greatest
		return 1;
	else
		//	c is greatest
		return 2;
}

//------------------------------------------------------------------------------
bool	Split (const polyptr &poly, real max_area, polyptr& a, polyptr& b)
{
	real	area = poly->Area ();
	if (area > max_area)
	{
		if (poly->Count () > 3)
		{
			// here we'll just chop one of the triangles of the polygon off
			for (int i = 0; i < poly->Count (); i++)
				gOutpts[i] = poly->Vertex (i);
			gInpts[0] = poly->Vertex (0);
			gInpts[1] = poly->Vertex (poly->Count () - 2);
			gInpts[2] = poly->Vertex (poly->Count () - 1);
			a = new rcpolygon (gOutpts, poly->Count () - 1);
			b = new rcpolygon (gInpts, 3);
		}
		else
		{
			//	here we'll subdivide a triangle by splitting the longest edge in half
			vector_3d	AB = poly->Vertex (1) - poly->Vertex (0),
						BC = poly->Vertex (2) - poly->Vertex (1),
						CA = poly->Vertex (0) - poly->Vertex (2);
			real		ABlength = AB | AB,
						BClength = BC | BC,
						CAlength = CA | CA,
						scale = R(0.5) + ((UnitRandom ()  - R(0.5)) * R(0.25));
			switch (MaxOf (ABlength, BClength, CAlength))
			{
				case 0:
					gOutpts[0] = poly->Vertex (0);
					gOutpts[1] = poly->Vertex (0) + (AB * scale);
					gOutpts[2] = poly->Vertex (2);

					gInpts[0] = gOutpts[1];
					gInpts[1] = poly->Vertex (1);
					gInpts[2] = poly->Vertex (2);
					break;
				case 1:
					gOutpts[0] = poly->Vertex (0);
					gOutpts[1] = poly->Vertex (1);
					gOutpts[2] = poly->Vertex (1) + (BC * scale);

					gInpts[0] = gOutpts[2];
					gInpts[1] = poly->Vertex (2);
					gInpts[2] = poly->Vertex (0);
					break;
				case 2:
					gOutpts[0] = poly->Vertex (0);
					gOutpts[1] = poly->Vertex (1);
					gOutpts[2] = poly->Vertex (2) + (CA * scale);

					gInpts[0] = gOutpts[2];
					gInpts[1] = poly->Vertex (1);
					gInpts[2] = poly->Vertex (2);
					break;
				default:
					std::cerr << "OHHH SHIT, case got lose" << std::endl;
					break;
			}
			a = new rcpolygon (gOutpts, 3);
			b = new rcpolygon (gInpts, 3);
		}
		return true;
	}
	return false;
}

//------------------------------------------------------------------------------
