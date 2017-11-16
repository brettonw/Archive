//------------------------------------------------------------------------------
//	File:					point_3d.h
//	Date:					8/26/94
//	Author:				Bretton Wade
//
//	Description:	this file contains the class definition for a point_3d
//
//------------------------------------------------------------------------------

#include "tuple_3d.h"

#ifndef	POINT_3D
#define	POINT_3D

//------------------------------------------------------------------------------
//	classes
//------------------------------------------------------------------------------
#ifndef	VECTOR_3D
class	vector_3d;																																//	forward declaration
#endif

//------------------------------------------------------------------------------
class	point_3d : public tuple_3d																								//	4 dimensional point_3d class
{																																								//	begin point_3d class definition
	public:																																				//	public interface
		point_3d (void) {}																													//	default constructor
		point_3d (real x, real y, real z, real w = R(1.0));													//	constructor from 4 values
		point_3d (const point_3d &p);																								//	copy constructor
		point_3d (const vector_3d &v);																							//	constructor from a vector_3d
		point_3d (const tuple_3d &t);																								//	constructor from a tuple_3d
		point_3d	&operator = (const point_3d &p);																	//	assignment operator
		point_3d	&operator = (const tuple_3d &t);																	//	assignment operator
		void			operator () (real x, real y, real z, real w = R(1.0));						//	function call operator
		vector_3d	operator - (const point_3d &p) const;															//	subtraction operator
		point_3d	operator + (const vector_3d &v) const;														//	addition of a vector_3d to a point_3d
};																																							//	end point_3d class definition

//------------------------------------------------------------------------------
//	global variables
//------------------------------------------------------------------------------
extern	point_3d	ORIGIN_3D;																										//	the origin of the coordinate system

//------------------------------------------------------------------------------

#endif	//POINT_3D
