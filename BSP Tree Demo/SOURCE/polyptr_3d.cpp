//------------------------------------------------------------------------------
//	File:					polyptr.cp
//	Date:					9/24/94
//	Author:				Bretton Wade
//
//	Description:	this file contains the class methods for a reference
//								counted polygon pointer.
//
//------------------------------------------------------------------------------

#include "precompile.h"
#include "polyptr_3d.h"

//------------------------------------------------------------------------------
//	constructor
//------------------------------------------------------------------------------
polyptr::polyptr (void)																													//	normal constructor
{																																								//	begin
	ptr = 0;																																			//	empty polygon
}																																								//	end

//------------------------------------------------------------------------------
//	constructor
//------------------------------------------------------------------------------
polyptr::polyptr (point_3d *buffer, int count, ...)															//	normal constructor
{																																								//	begin
	va_list	pts;																																	//	standard arg parameter list
	va_start (pts, count);																												//	set up the variable parameters
	ptr = new rcpolygon (buffer, count, pts);																			//	allocate the new polygon
	va_end (pts);																																	//	finish up the paramter list
	ptr->ref_count++;																															//	increment the reference count
}																																								//	end

//------------------------------------------------------------------------------
//	constructor
//------------------------------------------------------------------------------
polyptr::polyptr (rcpolygon *poly)																							//	normal constructor
{																																								//	begin
	ptr = poly;																																		//	copy the polygon pointer
	if (ptr)																																			//	if it's valid
		ptr->ref_count++;																														//	increment the reference count
}																																								//	end

//------------------------------------------------------------------------------
//	constructor
//------------------------------------------------------------------------------
polyptr::polyptr (const polyptr &poly)																					//	copy constructor
{																																								//	begin
	ptr = poly.ptr;																																//	copy the pointer from the polyptr
	if (ptr)																																			//	if it's valid
		ptr->ref_count++;																														//	increment the reference count
}																																								//	end

//------------------------------------------------------------------------------
//	destructor
//------------------------------------------------------------------------------
polyptr::~polyptr (void)																												//	destructor
{																																								//	begin
	if (ptr)																																			//	if the representation is valid
		if (--(ptr->ref_count) == 0)																								//	if the decremented reference is zero
			delete ptr;																																//	delete the representation
}																																								//	end

//------------------------------------------------------------------------------
//	assignment
//------------------------------------------------------------------------------
polyptr		&polyptr::operator = (rcpolygon *poly)																//	assignment operator
{																																								//	begin
	if (poly)																																			//	if this is a valid rep
		poly->ref_count++;																													//	increment its reference count
	if (ptr)																																			//	if the current representation is valid
		if (--(ptr->ref_count) == 0)																								//	if the decremented reference is zero
			delete ptr;																																//	delete the representation
	ptr = poly;																																		//	copy the new representation pointer
	return *this;																																	//	return the reference to this
}																																								//	end

//------------------------------------------------------------------------------
//	assignment
//------------------------------------------------------------------------------
polyptr		&polyptr::operator = (const polyptr &poly)														//	assignment operator
{																																								//	begin
	if (poly.ptr)																																	//	if this is a valid rep
		poly.ptr->ref_count++;																											//	increment its reference count
	if (ptr)																																			//	if the current representation is valid
		if (--ptr->ref_count == 0)																									//	if the decremented reference is zero
			delete ptr;																																//	delete the representation
	ptr = poly.ptr;																																//	copy the new representation pointer
	return *this;																																	//	return the reference to this
}																																								//	end

//------------------------------------------------------------------------------
