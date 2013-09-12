//------------------------------------------------------------------------------
//	File:					bspnode.cp
//	Date:					04/07/97
//	Author:				Bretton Wade
//
//	Description:	this file contains the methods for a bsp tree node
//
//------------------------------------------------------------------------------

#include "precompile.h"
#include "utility.h"
#include "bspnode_3d.h"
#include "iterator_3d.h"

//------------------------------------------------------------------------------
//	constructor
//------------------------------------------------------------------------------
bspnode::bspnode (const polyptr &poly)																					//	normal constructor
{																																								//	begin
	plane = poly->Plane ();																												//	copy the plane_3d equation
	on->AddToList (poly);																													//	install the polygon in the 'on' list
}																																								//	end

//------------------------------------------------------------------------------
//	destructor
//------------------------------------------------------------------------------
bspnode::~bspnode (void)																												//	destructor
{																																								//	begin
}																																								//	end

//------------------------------------------------------------------------------
//	insert a list of faces
//------------------------------------------------------------------------------
void	bspnode::Insert (listptr list, hclass keep)																//	insert a list of polygons into the tree
{																																								//	begin
	listptr	inside, outside;																											//	lists corresponding to partitions
	polyptr		poly, inp, outp;																										//	source polygon and partitions
	for (poly = list->Pop (); poly (); poly = list->Pop ())												//	loop over the polygons in the list one ata a time
	{																																							//	begin
		hclass	sgn = Split (poly, plane, inp, outp);																//	clip the polygon by the partitioning plane_3d
		if (sgn == HC_ON)																														//	if the source polygon lies within the partitioning plane_3d
			on->AddToList (poly);																											//	insert it into the coincident faces list
		else																																				//	otherwise
		{																																						//	begin
			if ((sgn == HC_IN) || (sgn == HC_SPANNING))																//	if some part of the result is inside
				inside->AddToList (inp);																								//	send it through the inside children
			if ((sgn == HC_OUT) || (sgn == HC_SPANNING))															//	if some part of the result is outside
				outside->AddToList (outp);																							//	send it through the outside children
		}																																						//	end
	}																																							//	end
	if (!inside->Empty ())																												//	if the inside list is not empty
		in.Insert (inside, keep, HC_IN);																						//	insert the inside list into the inside children
	if (!outside->Empty ())																												//	if the outside list is not empty
		out.Insert (outside, keep, HC_OUT);																					//	insert the outside list into the outside children
}																																								//	end

//------------------------------------------------------------------------------
//	push a single face through the tree
//------------------------------------------------------------------------------
void	bspnode::Push (polyptr poly, listptr result, hclass keep)									//	push a list of polygons through the tree
{																																								//	begin
	listptr	inside, outside;																											//	lists corresponding to partitions
	polyptr	inp, outp;																														//	source polygon and its partitions
	hclass	sgn = Split (poly, plane, inp, outp);																	//	clip the polygon by the partitioning plane_3d
	if (sgn == HC_ON)																															//	if the source polygon lies within the partitioning plane_3d
		result->AddToList (poly);																										//	insert it into the result list
	else																																					//	otherwise
	{																																							//	begin
		if ((sgn == HC_IN) || (sgn == HC_SPANNING))																	//	if some part of the result is inside
			in.Push (inp, result, keep, HC_IN);																				//	push it through the in node
		if ((sgn == HC_OUT) || (sgn == HC_SPANNING))																//	if some part of the result is outside
			out.Push (outp, result, keep, HC_OUT);																		//	push it through the out node
	}																																							//	end
}																																								//	end

//------------------------------------------------------------------------------
//	push a list of faces through the tree
//------------------------------------------------------------------------------
void	bspnode::Push (listptr list, listptr result, hclass keep)									//	push a list of polygons through the tree
{																																								//	begin
	listptr	inside, outside;																											//	lists corresponding to partitions
	polyptr		poly, inp, outp;																										//	source polygon and its partitions
	for (poly = list->Pop (); poly (); poly = list->Pop ())												//	loop through all of the polygons in the list
	{																																							//	begin
		hclass	sgn = Split (poly, plane, inp, outp);																//	clip the polygon by the partitioning plane_3d
		if (sgn == HC_ON)																														//	if the source polygon lies within the partitioning plane_3d
			result->AddToList (poly);																									//	insert it into the result list
		else																																				//	otherwise
		{																																						//	begin
			if ((sgn == HC_IN) || (sgn == HC_SPANNING))																//	if some part of the result is inside
				inside->AddToList (inp);																								//	send it through the inside children
			if ((sgn == HC_OUT) || (sgn == HC_SPANNING))															//	if some part of the result is outside
				outside->AddToList (outp);																							//	send it through the outside children
		}																																						//	end
	}																																							//	end
	if (!inside->Empty ())																												//	if the inside list is not empty
		in.Push (inside, result, keep, HC_IN);																			//	push the inside list through the inside children
	if (!outside->Empty ())																												//	if the outside list is not empty
		out.Push (outside, result, keep, HC_OUT);																		//	push the outside list through the outside children
}																																								//	end

//------------------------------------------------------------------------------
//	reduce to boundary polygons
//------------------------------------------------------------------------------
void	bspnode::Reduce (void)																										//	reduce the tree to only boundary polygons
{																																								//	begin
	listptr	results, boundary;																										//	results lists
	for (polyptr poly = on->Pop (); poly (); poly = on->Pop ())										//	loop through all of the polygons in the list
	{																																							//	begin
		if (abs (poly->Plane ()[W] + plane[W]) > EPSILON)														//	if the plane_3d is facing the same way as the polygon
		{																																						//	begin
			in.Push (poly, results, HC_IN, HC_IN);																		//	push the polygon down and keep in bits
			out.Push (results, boundary, HC_OUT, HC_OUT);															//	push the results down and keep the out bits
		}																																						//	end
		else																																				//	otherwise, the plane_3d and polygon are facing opposite directions
		{																																						//	begin
			out.Push (poly, results, HC_OUT, HC_OUT);																	//	push the polygon down and keep the out bits
			in.Push (results, boundary, HC_IN, HC_IN);																//	push the results down and keep in bits
		}																																						//	end
	}																																							//	end
	on = boundary;																																//	assign the new coincident faces list
	in.Reduce ();																																	//	tell the in child to compute its boundaries
	out.Reduce ();																																//	tell the out children to compute its boundaries
}																																								//	end

//------------------------------------------------------------------------------
//	draw
//------------------------------------------------------------------------------
void	bspnode::Draw (const point_3d &eye) const																	//	draw the tree
{																																								//	begin
	real	sgn = eye | plane;																											//	compute the distance from the eye to the plane_3d
	if (sgn < R(0.0))																															//	if the eye is on the negative side of the plane_3d
	{																																							//	begin
		out.Draw (eye);																															//	draw the positive side children
		on->Draw ();																																//	draw the polygons coincident with the dividing plane_3d
		in.Draw (eye);																															//	draw the negative side children
	}																																							//	end
	else																																					//	otherwise
	{																																							//	begin
		in.Draw (eye);																															//	draw the negative side children
		on->Draw ();																																//	draw the polygons coincident with the dividing plane_3d
		out.Draw (eye);																															//	draw the positive side children
	}																																							//	end
}																																								//	end

//------------------------------------------------------------------------------
//	trace a ray through the node
//------------------------------------------------------------------------------
bool	bspnode::RayIntersection (const ray &r, polyptr &poly_hit, point_3d &ipt) const	//	compute the polygon intersected by a ray
{																																								//	begin
	real	dist = r.Origin () | plane,																							//	compute the distance from the ray origin to the plane_3d
				costheta = r.Direction () | plane;																			//	compute the cosine of the angle between the ray direction and the plane_3d normal
	if (dist > EPSILON)																														//	if the ray originates on the positive side of the plane_3d
	{																																							//	begin
		if (out.RayIntersection (r, poly_hit, ipt)) return TRUE;										//	trace the ray into the positive child
		if (costheta < -EPSILON)																										//	if the ray direction is headed into the plane_3d
		{																																						//	begin
			if (PlaneSearch (r, poly_hit, ipt)) return TRUE;													//	check the intersection with the coincident list
			return in.RayIntersection (r, poly_hit, ipt);															//	trace the ray through the negative child
		}																																						//	end
	}																																							//	end
	else if (dist < -EPSILON)																											//	otherwise, if the ray originates on the negative side of the plane_3d
	{																																							//	begin
		if (in.RayIntersection (r, poly_hit, ipt)) return TRUE;											//	trace the ray through the negative child
		if (costheta > EPSILON)																											//	if the ray direction is headed into the plane_3d
		{																																						//	begin
			if (PlaneSearch (r, poly_hit, ipt)) return TRUE;													//	check the intersection with the coincident list
			return out.RayIntersection (r, poly_hit, ipt);														//	trace the ray into the positive child
		}																																						//	end
	}																																							//	end
	else																																					//	otehrwise, the ray originbates on the plane_3d
	{																																							//	begin
		if (costheta < -EPSILON)																										//	if the ray is headed in
			return in.RayIntersection (r, poly_hit, ipt);															//	trace the ray through the negative child
		else if (costheta > EPSILON)																								//	otherwise, if the ray is headed out
			return out.RayIntersection (r, poly_hit, ipt);														//	trace the ray through the positive child
	}																																							//	end
	return FALSE;																																	//	the ray did not intersect anything in this tree
}																																								//	end

//------------------------------------------------------------------------------
//	find a ray intersection with the coincident polygon list
//------------------------------------------------------------------------------
bool	bspnode::PlaneSearch (const ray &r, polyptr &poly_hit, point_3d &ipt) const	//	find the intersection of a ray and the plane_3d list
{																																								//	begin
	real	dist = plane.RayIntersection (r);																				//	compute the distance along the ray to the plane_3d
	if (dist > R(0.0))																														//	if it is positive
	{																																							//	begin
		ipt = r.IntersectionPoint (dist);																						//	compute the intersection point_3d
		if (on->BoundingBox ().Contains (ipt))																			//	check to see if it is in the bounding box for the list
		{																																						//	begin
			iterator	entry (on);																											//	list iterator
			for (polyptr poly = entry (); poly (); poly = entry ())										//	do for all the entries in th elist
				if (poly->Contains (ipt))																								//	if the point_3d is inside the polygon
				{																																				//	begin
					poly_hit = poly;																											//	set the hit polygon
					return TRUE;																													//	return the success
				}																																				//	end
		}																																						//	end
	}																																							//	end
	return FALSE;																																	//	no intersection, return FALSE
}																																								//	end

//------------------------------------------------------------------------------
