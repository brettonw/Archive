//------------------------------------------------------------------------------
//	File:					vector_2d.h
//	Date:					1/21/94
//	Author:				Bretton Wade
//
//	Description:	this file contains the class definition for a 2d vector_2d
//
//------------------------------------------------------------------------------

#include "tuple_2d.h"

#ifndef	VECTOR_2D
#define	VECTOR_2D

//------------------------------------------------------------------------------
//	classes
//------------------------------------------------------------------------------
#ifndef	POINT_2D
class	point_2d;																																	//	forward declaration
#endif

//------------------------------------------------------------------------------
class	vector_2d : public tuple_2d																								//	2 dimensional vector_2d class
{																																								//	begin vector_2d class definition
	protected:																																		//	members internal to this class hierarchy
	public:																																				//	public interface
		vector_2d (void) {}																													//	default constructor
		vector_2d (real x, real y);																									//	constructor from 2 values
		vector_2d (const vector_2d &v);																							//	copy constructor
		vector_2d (const point_2d &p);																							//	constructor from a point
		vector_2d	&operator = (const vector_2d &v);																	//	assignment operator
		vector_2d	operator * (real s) const;																				//	scalar multiplication
		vector_2d	operator / (real s) const;																				//	scalar division
		vector_2d	operator + (const vector_2d &v) const;														//	addition operator
		vector_2d	operator - (const vector_2d &v) const;														//	subtraction operator
		real			Norm (void) const;																								//	compute the length of the vector_2d
		vector_2d	&Normalize (void);																								//	reduce the vector_2d to length 1.0
};																																							//	end vector_2d class definition

//------------------------------------------------------------------------------

#endif	//VECTOR_2D
