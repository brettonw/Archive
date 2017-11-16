//------------------------------------------------------------------------------
//	File:					point_2d.h
//	Date:					11/07/94
//	Author:				Bretton Wade
//
//	Description:	this file contains the class definition for a 2d point
//
//------------------------------------------------------------------------------

#include "tuple_2d.h"

#ifndef	POINT_2D
#define	POINT_2D

//------------------------------------------------------------------------------
//	classes
//------------------------------------------------------------------------------
#ifndef	VECTOR_2D
class	Vector_2d;																																//	forward declaration
#endif

//------------------------------------------------------------------------------
class	point_2d : public Tuple_2d																								//	point_2d class
{																																								//	begin
	protected:																																		//	members internal to this class hierarchy
	public:																																				//	members available externally
				point_2d (void) {}																											//	default constructor
				point_2d (real x, real y);																							//	normal constructor
				point_2d (const point_2d &p);																						//	copy constructor
				point_2d (const Vector_2d &v);																					//	constructor from a 2d vector_3d
				point_2d	&operator = (const point_2d &p);															//	assignment operator
				point_2d	operator + (const Vector_2d &v) const;												//	add a vector_3d to a point
				Vector_2d	operator - (const point_2d &p) const;													//	subtract two points
};																																							//	end

//------------------------------------------------------------------------------
//	global variables
//------------------------------------------------------------------------------
extern	point_2d	ORIGIN_2D;																										//	the origin of the coordinate system

//------------------------------------------------------------------------------

#endif	//POINT_2D
