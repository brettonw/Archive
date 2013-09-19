//------------------------------------------------------------------------------
//	File:					plane_3d.h
//	Date:					04/06/97
//	Author:				Bretton Wade
//
//	Description:	this file contains the class definition for a plane_3d
//
//------------------------------------------------------------------------------

#include "object_3d.h"

#ifndef	PLANE_3D
#define	PLANE_3D

//------------------------------------------------------------------------------
//	classes
//------------------------------------------------------------------------------
class	plane_3d : public tuple_3d, public object_3d 															//	3 dimensional plane_3d class
{																																								//	begin plane_3d class definition
	public:																																				//	public interface
				plane_3d (void) {}																											//	default constructor
				plane_3d (const vector_3d &v, const point_3d &p);												//	normal constructor
				plane_3d (const point_3d &a, const point_3d &b, const point_3d &c);			//	normal constructor
				plane_3d (real a, real b, real c, real d);															//	component constructor
				plane_3d (const plane_3d &p);																						//	copy constructor
				plane_3d (const tuple_3d &t);																						//	constructor from a tuple_3d
virtual	~plane_3d	(void);																												//	destructor
				plane_3d	&operator = (const plane_3d &p);															//	assignment operator
				plane_3d	&operator = (const tuple_3d &t);															//	assignment operator
				void			Define (const vector_3d &v, const point_3d &p);								//	compute the plane_3d equation
				void			Define (const point_3d &a, const point_3d &b, const point_3d &c);//	compute the plane_3d equation
				void			Invert (void);																								//	invert the values of the plane_3d equation
virtual	real			RayIntersection (const ray &r) const;													//	return the distance along the ray at which the intersection occurs
};																																							//	end plane_3d class definition

//------------------------------------------------------------------------------

#endif	//PLANE_3D
