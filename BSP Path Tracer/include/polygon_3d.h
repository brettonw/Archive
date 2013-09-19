//------------------------------------------------------------------------------
//	File:					polygon.h
//	Date:					05/02/97
//	Author:				Bretton Wade
//
//	Description:	this file contains the class definition for a polygon.
//
//------------------------------------------------------------------------------

#include "plane_3d.h"
#include "matrix_3d.h"
#include "distributor.h"

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
				real			area;																													//	the area of the polygon
				real			emit;																													//	the emissivity of the polygon
				real			power;
				real*			little_area;																											//	the toal amount of power impinging on the surface
				distributor		sDist;
				distributor		tDist;
				distributor		thetaDist;
				distributor		phiDist;
				matrix_3d			xform;
	public:																																				//	members available externally
				real			little_power;	
	public:																																				//	members available externally
				polygon (point_3d *buffer, uchar count, va_list pts);										//	normal constructor
				polygon (point_3d *buffer, uchar count);																//	normal constructor
virtual	~polygon (void);																												//	destructor
				uchar			Count (void) const;																						//	return the number of points in the polygon
const		point_3d	&Vertex (int i) const;																				//	return the ith point_3d of the polygon
const		plane_3d	&Plane (void) const;																					//	return a reference to the plane_3d of the polygon
				vector_3d	Normal (void) const;																					//	compute the plane_3d normal vector_3d Newell's method
				bool			Contains (const point_3d &pt) const;													//	test the point_3d to see if it is inside the polygon (crossings method)
				void			ComputeArea (void);																						//	compute the area of the polygon
				real			Area (void) const;																						//	return the area of the polygon
				void			Invert (void);																								//	completely reverse the orientation of the polygon
				void			SetEmissivity (real emit);																		//	set the polygon emissivity
				real			Emissivity (void) const;																			//	get the emissivity of the polygon
				void			AddPower (real power);																				//	add to the polygon impinging power
				real			Power (void) const;	
				point_3d		SamplePoint (void);																					//	return the polygon impinging power
				vector_3d		SampleVector (void);
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

inline	real	polygon::Area (void) const																				//	return the area of the polygon
{																																								//	begin
	return area;																																	//	return the area
}																																								//	end

inline	void	polygon::SetEmissivity (real emissivity)													//	set the polygon emissivity
{																																								//	begin
	emit = emissivity;																														//	copy the new value
}																																								//	end

inline	real	polygon::Emissivity (void) const																	//	get the emissivity of the polygon
{																																								//	begin
	return emit;																																	//	return the emissivity
}																																								//	end

inline	void	polygon::AddPower (real p)																				//	add to the polygon impinging power
{																																								//	begin
	power += p;																																		//	add in the new value
}																																								//	end

inline	real	polygon::Power (void) const																				//	return the polygon impinging power
{																																								//	begin
	return power;																																	//	return the impinging power
}																																								//	end

//------------------------------------------------------------------------------

#endif	//POLYGON
