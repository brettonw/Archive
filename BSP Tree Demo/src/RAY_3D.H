//------------------------------------------------------------------------------
//	File:					ray.h
//	Date:					9/22/94
//	Author:				Bretton Wade
//
//	Description:	this file contains the class definition for a ray
//
//------------------------------------------------------------------------------

#include "point_3d.h"
#include "vector_3d.h"

#ifndef	RAY
#define	RAY

//------------------------------------------------------------------------------
//	classes
//------------------------------------------------------------------------------
class	ray																																				//	ray class
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
				point_3d		origin;																											//	the originating point_3d of the ray
				vector_3d	direction;																										//	the direction vector_3d of the ray
	public:																																				//	members available externally
				ray (const point_3d &origin, const vector_3d &direction);								//	constructor
const 	point_3d 	&Origin (void) const;																					//	return a reference to the origin point_3d of the ray
const 	vector_3d	&Direction (void) const;																			//	return a reference to the direction vector_3d of the ray
				point_3d	IntersectionPoint (real distance) const;											//	compute the point_3d at which the intersection occurs
};																																							//	end

//------------------------------------------------------------------------------
//	inlines
//------------------------------------------------------------------------------
inline	const 	point_3d 	&ray::Origin (void) const															//	return a reference to the origin point_3d of the ray
{																																								//	begin
	return origin;																																//	return the origin
}																																								//	end

inline	const 	vector_3d 	&ray::Direction (void) const												//	return a reference to the direction vector_3d of the ray
{																																								//	begin
	return direction;																															//	return the direction
}																																								//	end

//------------------------------------------------------------------------------

#endif	//RAY
