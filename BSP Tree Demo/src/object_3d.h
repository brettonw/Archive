//------------------------------------------------------------------------------
//	File:					object_3d.h
//	Date:					9/22/94
//	Author:				Bretton Wade
//
//	Description:	this file contains the class definition for an object_3d
//
//------------------------------------------------------------------------------

#include "ray_3d.h"

#ifndef	OBJECT_3D
#define	OBJECT_3D

//------------------------------------------------------------------------------
//	classes
//------------------------------------------------------------------------------
class	object_3d																																		//	object_3d class
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
	public:																																				//	members available externally
				object_3d (void) {}																												//	constructor
virtual	~object_3d (void);																													//	destructor
virtual	real		RayIntersection (const ray& r) const = 0;												//	return the distance along the ray at which the intersection occurs
};																																							//	end

//------------------------------------------------------------------------------

#endif	//OBJECT_3D
