//------------------------------------------------------------------------------
//	File:					tuple_3d.h
//	Date:					8/26/94
//	Author:				Bretton Wade
//
//	Description:	this file contains the class definition for a tuple_3d
//
//------------------------------------------------------------------------------

#include "coord.h"

#ifndef	TUPLE
#define	TUPLE

//------------------------------------------------------------------------------
//	classes
//------------------------------------------------------------------------------
class	tuple_3d																																	//	4 dimensional tuple_3d class
{																																								//	begin tuple_3d class definition
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
				real	xyz[4];																														//	4 tuple_3d
	public:																																				//	members available externally
				tuple_3d (void) {}																											//	default constructor
				tuple_3d (const tuple_3d &t);																						//	copy constructor
				tuple_3d (real x, real y, real z, real w);															//	constructor from 4 values
				void	operator = (const tuple_3d &t);																		//	assignment operator
				bool	operator == (const tuple_3d &t) const;														//	equality operator
				bool	operator != (const tuple_3d &t) const;														//	inequality operator
				void	operator () (real x, real y, real z, real w);											//	function call operator
				real	operator | (const tuple_3d &t) const;															//	inner product operator
				real	operator [] (coord c) const;																			//	array reference operator
				real	&operator [] (coord c);																						//	array reference operator
				coord	MajorAxis (void) const;																						//	return the major axis of the tuple_3d
				coord	MinorAxis (void) const;																						//	return the minor axis of the tuple_3d
};																																							//	end tuple_3d class definition

//------------------------------------------------------------------------------
//	inlines
//------------------------------------------------------------------------------
inline	real	tuple_3d::operator [] (coord c) const															//	array reference operator
{																																								//	begin
	return xyz[c];																																//	return the appropriate coordinate of the tuple_3d
}																																								//	end

//------------------------------------------------------------------------------
inline	real	&tuple_3d::operator [] (coord c)																	//	array reference operator
{																																								//	begin
	return xyz[c];																																//	return the appropriate coordinate of the tuple_3d
}																																								//	end

//------------------------------------------------------------------------------

#endif	//TUPLE
