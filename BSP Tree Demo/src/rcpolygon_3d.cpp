//------------------------------------------------------------------------------
//	File:					rcpolygon.cp
//	Date:					9/23/94
//	Author:				Bretton Wade
//
//	Description:	this file contains the class methods for a reference
//								counted polygon.
//
//------------------------------------------------------------------------------

#include "precompile.h"
#include "rcpolygon_3d.h"

//------------------------------------------------------------------------------
//	constructor
//------------------------------------------------------------------------------
rcpolygon::rcpolygon (point_3d *buffer, uchar count, va_list pts) :							//	normal constructor
polygon (buffer, count, pts)																										//	parent class constructor
{																																								//	begin
	ref_count = 0;																																//	initialize the reference count
}																																								//	end

//------------------------------------------------------------------------------
//	constructor
//------------------------------------------------------------------------------
rcpolygon::rcpolygon (point_3d *buffer, uchar count) :													//	normal constructor
polygon (buffer, count)																													//	parent class constructor
{																																								//	begin
	ref_count = 0;																																//	initialize the reference count
}																																								//	end

//------------------------------------------------------------------------------
//	destructor
//------------------------------------------------------------------------------
rcpolygon::~rcpolygon (void)																										//	destructor
{																																								//	begin
}																																								//	end

//------------------------------------------------------------------------------
