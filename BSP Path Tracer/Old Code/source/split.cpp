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
#include "split.h"

//------------------------------------------------------------------------------
//	plane the polygon with a plane_3d
//------------------------------------------------------------------------------
hclass	Split (const polyptr &poly, const plane_3d &plane, polyptr &in, polyptr &out)//	split the polygon with a plane_3d
{																																								//	begin
	static	point_3d	outpts[64],																									//	an array of points for the out polygon
										inpts[64];																									//	an array of points for the in polygon
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
				outpts[out_c++] = inpts[in_c++] = 																			//	add the newly computed point_3d to the partitions
					ptA + (v * (-(ptA | plane) / (v | plane)));														//	add the newly computed point_3d to the partitions
				poly_class = HC_SPANNING;																								//	set the poly_class appropriately
			}																																					//	end
			outpts[out_c++] = ptB;																										//	add the current point_3d to the positive partition
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
				outpts[out_c++] = inpts[in_c++] = 																			//	add the newly computed point_3d to the partitions
					ptA + (v * (-(ptA | plane) / (v | plane)));														//	add the newly computed point_3d to the partitions
				poly_class = HC_SPANNING;																								//	set the poly_class appropriately
			}																																					//	end
			inpts[in_c++] = ptB;																											//	add the current point_3d to the negative partition
		}																																						//	end
		else																																				//	the current point_3d is on the plane_3d
			outpts[out_c++] = inpts[in_c++] = ptB;																		//	add the current point_3d to the partitions
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
			out = new rcpolygon (outpts, out_c);																			//	make the positive partition
			out->SetEmissivity (poly->Emissivity ());																	//	copy the emissivity of the original polygon
			in = new rcpolygon (inpts, in_c);																					//	make the negative partition
			in->SetEmissivity (poly->Emissivity ());																	//	copy the emissivity of the original polygon
			break;																																		//	end spanning
	}																																							//	end
	return poly_class;																														//	return the classification
}																																								//	end

//------------------------------------------------------------------------------
