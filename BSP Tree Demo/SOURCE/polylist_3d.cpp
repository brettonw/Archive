//------------------------------------------------------------------------------
//	File:					polylist.cp
//	Date:					9/26/94
//	Author:				Bretton Wade
//
//	Description:	this file contains the class methods for a polygon list
//
//------------------------------------------------------------------------------

#include "precompile.h"
#include "listptr_3d.h"

//------------------------------------------------------------------------------
//	constructor
//------------------------------------------------------------------------------
polylist::polylist (void)																												//	normal constructor
{																																								//	begin
	head = 0;																																			//	the list is empty
	ref_count = 0;																																//	start out with no references
}																																								//	end

//------------------------------------------------------------------------------
//	destructor
//------------------------------------------------------------------------------
polylist::~polylist (void)																											//	destructor
{																																								//	begin
	while (head)																																	//	while the list is not empty
	{																																							//	begin
		node	*kill = head;																													//	save the head element for a moment
		head = head->next;																													//	advance the head element
		delete kill;																																//	kill the former head
	}																																							//	end
}																																								//	end

//------------------------------------------------------------------------------
//	add to list
//------------------------------------------------------------------------------
void		polylist::AddToList (const polyptr &poly)																//	add a polygon to the list
{																																								//	begin
	node	*add = new node;																												//	create a new node
	add->ptr = poly;																															//	copy the polygon pointer
	add->next = head;																															//	set the new node at the head of the list
	head = add;																																		//	install the new head
	for (int i = 0; i < poly->Count (); i++)																			//	take each vertex of the polygon
		box += poly->Vertex (i);																										//	and add it into the bounding box for the list
}																																								//	end

//------------------------------------------------------------------------------
//	add to list
//------------------------------------------------------------------------------
void		polylist::Append (listptr &list)																				//	append the contents of list to this list
{																																								//	begin
	while (list->head)																														//	while there are elements in in the appending list
		AddToList (list->Pop ());																										//	pop them off and add them to this list
}																																								//	end

//------------------------------------------------------------------------------
//	pop the head element
//------------------------------------------------------------------------------
polyptr	polylist::Pop (void)																										//	pop the head from the list
{																																								//	begin
	if (head)																																			//	if there is an entry in the list
	{																																							//	begin
		polyptr	poly = head->ptr;																										//	get the polygon pointer from the head element
		node		*kill = head;																												//	save the head element for a moment
		head = head->next;																													//	advance the head to the next node
		delete kill;																																//	kill the former head
		return poly;																																//	return the polygon pointer
	}																																							//	end
	else																																					//	otherwise. there are no entries
		return polyptr ();																													//	return a null polygon
}																																								//	end

//------------------------------------------------------------------------------
//	test the list for emptiness
//------------------------------------------------------------------------------
bool		polylist::Empty (void) const																						//	return whether or not the list is empty
{																																								//	begin
	return bool (head == 0);																											//	return whether the head is a valid pointer
}																																								//	end

//------------------------------------------------------------------------------
//	draw all the polygons in the list
//------------------------------------------------------------------------------
void		polylist::Draw (void) const																							//	draw the polygons in the list
{																																								//	begin
	node	*current = head;																												//	start at the top
	while (current)																																//	while there are more items in the list
	{																																							//	begin
		extern	void	DrawPolygon (polyptr);
		DrawPolygon (current->ptr);																									//	draw the polygon
		current = current->next;																										//	advance to the next list entry
	}																																							//	end
}																																								//	end

//------------------------------------------------------------------------------
