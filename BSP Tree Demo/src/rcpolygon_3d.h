//------------------------------------------------------------------------------
//	File:					rcpolygon.h
//	Date:					9/23/94
//	Author:				Bretton Wade
//
//	Description:	this file contains the class definition for a reference
//								counted polygon.
//
//------------------------------------------------------------------------------

#include "polygon_3d.h"

#ifndef	RCPOLYGON
#define	RCPOLYGON

//------------------------------------------------------------------------------
//	classes
//------------------------------------------------------------------------------
class	rcpolygon : public polygon																								//	3 dimensional reference counted polygon class
{																																								//	begin polygon class definition
	friend	class	polyptr;																												//	polyptr is my friend
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
				uchar		ref_count;																											//	the number of references to this object
	public:																																				//	members available externally
				rcpolygon (point_3d *buffer, uchar count, va_list pts);									//	normal constructor
				rcpolygon (point_3d *buffer, uchar count);															//	normal constructor
virtual	~rcpolygon (void);																											//	destructor
};																																							//	end polygon class definition

//------------------------------------------------------------------------------

#endif	//RCPOLYGON
