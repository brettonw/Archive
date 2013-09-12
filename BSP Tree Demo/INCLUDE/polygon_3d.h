//------------------------------------------------------------------------------
//	File:					polygon.h
//	Date:					04/07/97
//	Author:				Bretton Wade
//
//	Description:	this file contains the class definition for a polygon.
//
//------------------------------------------------------------------------------

#include "plane_3d.h"

#ifndef	POLYGON
#define	POLYGON

//------------------------------------------------------------------------------
//	classes
//------------------------------------------------------------------------------
class	polygon : public object_3d																								//	3 dimensional polygon class
{																																								//	begin polygon class definition
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
				plane_3d	plane;																												//	plane_3d equation for the polygon
				uchar			count;																												//	number of points in the polygon
				point_3d	*points;																											//	the array of points that compose the polygon
	public:																																				//	members available externally
				polygon (point_3d *buffer, uchar count, va_list pts);										//	normal constructor
				polygon (point_3d *buffer, uchar count);																//	normal constructor
virtual	~polygon (void);																												//	destructor
				uchar			Count (void) const;																						//	return the number of points in the polygon
const		point_3d	&Vertex (int i) const;																				//	return the ith point_3d of the polygon
const		plane_3d	&Plane (void) const;																					//	return a reference to the plane_3d of the polygon
				vector_3d	Normal (void) const;																					//	compute the plane_3d normal vector_3d Newell's method
				bool			Contains (const point_3d &pt) const;													//	test the point_3d to see if it is inside the polygon (crossings method)
				void			Invert (void);																								//	completely reverse the orientation of the polygon
virtual	real			RayIntersection (const ray &r) const;													//	return the distance along the ray at which the intersection occurs
};																																							//	end polygon class definition

//------------------------------------------------------------------------------
//	inlines
//------------------------------------------------------------------------------
inline	uchar	polygon::Count (void) const																				//	return the number of points in the polygon
{																																								//	begin
	return count;																																	//	return the count
}																																								//	end

inline	const	point_3d	&polygon::Vertex (int i) const													//	return the ith point_3d of the polygon
{																																								//	begin
	return points[i];																															//	return the point_3d
}																																								//	end

inline	const	plane_3d	&polygon::Plane (void) const														//	return a reference to the plane_3d of the polygon
{																																								//	begin
	return plane;																																	//	return the plane_3d associated with the polygon
}																																								//	end

//------------------------------------------------------------------------------

#endif	//POLYGON
