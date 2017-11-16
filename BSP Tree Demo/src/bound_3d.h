//------------------------------------------------------------------------------
//	File:					bound_3d.h
//	Date:					9/26/94
//	Author:				Bretton Wade
//
//	Description:	this file contains the class definition for a bounding box
//
//------------------------------------------------------------------------------

#include "point_3d.h"

#ifndef	BOUND_3D
#define	BOUND_3D

//------------------------------------------------------------------------------
//	classes
//------------------------------------------------------------------------------
class	bound_3d																																	//	bounding box class
{																																								//	begin
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
				point_3d	max,																													//	the minimum bound point_3d
									min;																													//	the maximum bound point_3d
	public:																																				//	members available externally
				bound_3d (void);																												//	constructor
				void			operator += (const bound_3d& box);														//	add a bounding box into this one
				void			operator += (const point_3d& pt);															//	add a point_3d into the bounding box
				bool			Contains (const point_3d& pt) const;													//	return whether or not a point_3d is inside the bounding box
};																																							//	end

//------------------------------------------------------------------------------

#endif	//BOUND_3D
