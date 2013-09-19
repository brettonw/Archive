//------------------------------------------------------------------------------
//	File:					tuple_2d.cp
//	Date:					8/26/94
//	Author:				Bretton Wade
//
//	Description:	this file contains the methods for a tuple_2d
//
//------------------------------------------------------------------------------

#include "precompile.h"
#include "utility.h"
#include "tuple_2d.h"

//------------------------------------------------------------------------------
//	constructor
//------------------------------------------------------------------------------
tuple_2d::tuple_2d (const tuple_2d &t)																					//	copy constructor
{																																								//	begin
	xy[X] = t[X]; xy[Y] = t[Y];																										//	copy the values into the tuple_2d
}																																								//	end

//------------------------------------------------------------------------------
//	constructor
//------------------------------------------------------------------------------
tuple_2d::tuple_2d (real x, real y)																							//	constructor from 2 values
{																																								//	begin
	xy[X] = x; xy[Y] = y;																													//	copy the values into the tuple_2d
}																																								//	end

//------------------------------------------------------------------------------
//	assignment operator
//------------------------------------------------------------------------------
void	tuple_2d::operator = (const tuple_2d &t)																	//	assignment operator
{																																								//	begin
	xy[X] = t[X]; xy[Y] = t[Y];																										//	copy the values into the tuple_2d
}																																								//	end

//------------------------------------------------------------------------------
//	equality test
//------------------------------------------------------------------------------
bool	tuple_2d::operator == (const tuple_2d &t) const														//	equality operator
{																																								//	begin
	return	bool ((abs (xy[X] - t[X]) < EPSILON) && 															//	compare the x coordinates
								(abs (xy[Y] - t[Y]) < EPSILON));																//	compare the w coordinates			
}																																								//	end

//------------------------------------------------------------------------------
//	inequality test
//------------------------------------------------------------------------------
bool	tuple_2d::operator != (const tuple_2d &t) const														//	inequality operator
{																																								//	begin
	return	bool ((abs (xy[X] - t[X]) > EPSILON) || 															//	compare the x coordinates
								(abs (xy[Y] - t[Y]) > EPSILON));																//	compare the w coordinates			
}																																								//	end

//------------------------------------------------------------------------------
//	assignment
//------------------------------------------------------------------------------
void	tuple_2d::operator () (real x, real y)																		//	function call operator
{																																								//	begin
	xy[X] = x; xy[Y] = y;																													//	copy the values into the tuple_2d
}																																								//	end

//------------------------------------------------------------------------------
//	dot product
//------------------------------------------------------------------------------
real	tuple_2d::operator | (const tuple_2d &t) const														//	inner product operator
{																																								//	begin
	return 	(xy[X] * t[X]) + (xy[Y] * t[Y]);																			//	coordinate multiply
}																																								//	end

//------------------------------------------------------------------------------
