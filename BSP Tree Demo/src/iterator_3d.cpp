//------------------------------------------------------------------------------
//	File:					iterator.cp
//	Date:					9/26/94
//	Author:				Bretton Wade
//
//	Description:	this file contains the class methods for a list iterator
//
//------------------------------------------------------------------------------

#include "precompile.h"
#include "iterator_3d.h"

//------------------------------------------------------------------------------
//	constructor
//------------------------------------------------------------------------------
iterator::iterator (const listptr &l)																						//	normal constructor
{																																								//	begin
	list = l;																																			//	copy the list pointer
	Reset ();																																			//	start at the head of the list
}																																								//	end

//------------------------------------------------------------------------------
//	destructor
//------------------------------------------------------------------------------
iterator::~iterator (void)																											//	destructor
{																																								//	begin
}																																								//	end

//------------------------------------------------------------------------------
//	start over
//------------------------------------------------------------------------------
void		iterator::Reset (void)																									//	start at the head of the list
{																																								//	begin
	current = list->head;																													//	set the current node to the head of the list
}																																								//	end

//------------------------------------------------------------------------------
