//------------------------------------------------------------------------------
//	File:					vector_3d.h
//	Date:					8/26/94
//	Author:				Bretton Wade
//
//	Description:	this file contains the class definition for a vector_3d
//
//------------------------------------------------------------------------------

#include "tuple_3d.h"

#ifndef	VECTOR_3D
#define	VECTOR_3D

//------------------------------------------------------------------------------
//	classes
//------------------------------------------------------------------------------
#ifndef	POINT_3D
class	point_3d;																																	//	forward declaration
#endif

//------------------------------------------------------------------------------
class	vector_3d : public tuple_3d																								//	4 dimensional vector_3d class
{																																								//	begin vector_3d class definition
	public:																																				//	public interface
		vector_3d (void) {}																													//	default constructor
		vector_3d (real x, real y, real z, real w = R(0.0));												//	constructor from 4 values
		vector_3d (const vector_3d &v);																							//	copy constructor
		vector_3d (const point_3d &p);																							//	constructor from a point_3d
		vector_3d (const tuple_3d &t);																							//	constructor from a tuple_3d
		vector_3d	&operator = (const vector_3d &v);																	//	assignment operator
		vector_3d	&operator = (const tuple_3d &t);																	//	assignment operator
		void			operator () (real x, real y, real z, real w = R(0.0));						//	function call operator
		vector_3d operator * (real s) const;																				//	scalar multiplication
		vector_3d operator / (real s) const;																				//	scalar division
		vector_3d	operator ^ (const vector_3d &v) const;														//	cross product
		vector_3d	operator + (const vector_3d &v) const;														//	addition operator
		vector_3d	&operator += (const vector_3d &v);																//	self addition operator
		vector_3d	operator - (const vector_3d &v) const;														//	subtraction operator
		vector_3d	operator - (void) const;																					//	unary minus operator
		real			Norm (void) const;																								//	compute the length of the vector_3d
		vector_3d	&Normalize (void);																								//	reduce the vector_3d to length 1.0
};																																							//	end vector_3d class definition

//------------------------------------------------------------------------------
//	global operators
//------------------------------------------------------------------------------
inline
vector_3d   operator * (real s, const vector_3d& v)
{
    return v * s;
}

//------------------------------------------------------------------------------
//	global variables
//------------------------------------------------------------------------------
extern	vector_3d	ZERO_VECTOR;																									//	a zero value vector_3d

//------------------------------------------------------------------------------

#endif	//VECTOR_3D
