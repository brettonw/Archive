//------------------------------------------------------------------------------
//	File:					tuple_2d.h
//	Date:					1/21/94
//	Author:				Bretton Wade
//
//	Description:	this file contains the class definition for a 2d tuple_3d
//
//------------------------------------------------------------------------------

#include "coord.h"

#ifndef	TUPLE_2D
#define	TUPLE_2D

//------------------------------------------------------------------------------
//	classes
//------------------------------------------------------------------------------
class	tuple_2d																																	//	2 dimensional tuple_2d class
{																																								//	begin tuple_2d class definition
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
				real	xy[2];																														//	2 tuple_2d
	public:																																				//	members available externally
				tuple_2d (void) {}																											//	default constructor
				tuple_2d (const tuple_2d &t);																						//	copy constructor
				tuple_2d (real x, real y);																							//	constructor from 2 values
				void	operator = (const tuple_2d &t);																		//	assignment operator
				bool	operator == (const tuple_2d &t) const;														//	equality operator
				bool	operator != (const tuple_2d &t) const;														//	inequality operator
				void	operator () (real x, real y);																			//	function call operator
				real	operator | (const tuple_2d &t) const;															//	inner product operator
				real	operator [] (coord c) const;																			//	array reference operator
				real	&operator [] (coord c);																						//	array reference operator
};																																							//	end tuple_2d class definition

//------------------------------------------------------------------------------
//	inlines
//------------------------------------------------------------------------------
inline	real	tuple_2d::operator [] (coord c) const															//	array reference operator
{																																								//	begin
	return xy[c];																																	//	return the appropriate coordinate of the tuple_2d
}																																								//	end

//------------------------------------------------------------------------------
inline	real	&tuple_2d::operator [] (coord c)																	//	array reference operator
{																																								//	begin
	return xy[c];																																	//	return the appropriate coordinate of the tuple_2d
}																																								//	end

//------------------------------------------------------------------------------

#endif	//TUPLE_2D
