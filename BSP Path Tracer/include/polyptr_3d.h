//------------------------------------------------------------------------------
//	File:					polyptr.h
//	Date:					9/24/94
//	Author:				Bretton Wade
//
//	Description:	this file contains the class definition for a reference
//								counted polygon pointer.
//
//------------------------------------------------------------------------------

#include "rcpolygon_3d.h"

#ifndef	POLYPTR
#define	POLYPTR

//------------------------------------------------------------------------------
//	classes
//------------------------------------------------------------------------------
class	polyptr																																		//	reference counted polygon pointer class
{																																								//	begin polygon class definition
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
				rcpolygon	*ptr;																													//	the pointer to the reference counted polygon
	public:																																				//	members available externally
				polyptr (void);																													//	normal constructor
				polyptr (point_3d *buffer, int count, ...);															//	normal constructor
				polyptr (rcpolygon *ptr);																								//	normal constructor
				polyptr (const polyptr &ptr);																						//	copy constructor
				~polyptr (void);																												//	destructor
				rcpolygon	*operator -> (void) const;																		//	pointer dereference operator
				rcpolygon	*operator () (void) const;																		//	function call operator
				polyptr		&operator = (rcpolygon *ptr);																	//	assignment operator
				polyptr		&operator = (const polyptr &ptr);															//	assignment operator
};																																							//	end polygon class definition

//------------------------------------------------------------------------------
//	inlines
//------------------------------------------------------------------------------
inline	rcpolygon	*polyptr::operator -> (void) const														//	pointer dereference operator
{																																								//	begin
	return ptr;																																		//	return the pointer value
}																																								//	end

inline	rcpolygon	*polyptr::operator () (void) const														//	function call operator
{																																								//	begin
	return ptr;																																		//	return the pointer value
}																																								//	end

//------------------------------------------------------------------------------

#endif	//POLYPTR
