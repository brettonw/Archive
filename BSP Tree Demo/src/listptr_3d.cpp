//------------------------------------------------------------------------------
//	File:					listptr.cp
//	Date:					9/26/94
//	Author:				Bretton Wade
//
//	Description:	this file contains the class methods for a list pointer
//
//------------------------------------------------------------------------------

#include "precompile.h"
#include "listptr_3d.h"

//------------------------------------------------------------------------------
//	constructor
//------------------------------------------------------------------------------
listptr::listptr (void)																													//	normal constructor
{																																								//	begin
	list = new polylist;																													//	create the new list
	list->ref_count++;																														//	increment the reference count
}																																								//	end

//------------------------------------------------------------------------------
//	constructor
//------------------------------------------------------------------------------
listptr::listptr (const listptr& l)																							//	normal constructor
{																																								//	begin
	list = l.list;																																//	copy the list pointer
	list->ref_count++;																														//	increment the reference count
}																																								//	end

//------------------------------------------------------------------------------
//	destructor
//------------------------------------------------------------------------------
listptr::~listptr (void)																												//	destructor
{																																								//	begin
	if (--list->ref_count == 0)																										//	decrement the reference count, if it is 0
		delete list;																																//	delete the list
}																																								//	end

//------------------------------------------------------------------------------
//	assignment
//------------------------------------------------------------------------------
listptr		&listptr::operator = (const listptr& l)																//	assignment operator
{																																								//	begin
	l.list->ref_count++;																													//	increment the reference count for the other list
	if (--list->ref_count == 0)																										//	decrement the reference count, if it is 0
		delete list;																																//	delete the list
	list = l.list;																																//	copy the list value from the other list
	return *this;																																	//	return a reference to this
}																																								//	end

//------------------------------------------------------------------------------
