//------------------------------------------------------------------------------
//	File:					polylist.h
//	Date:					9/26/94
//	Author:				Bretton Wade
//
//	Description:	this file contains the class definition for a polygon list
//
//------------------------------------------------------------------------------

#include	"polyptr_3d.h"
#include	"bound_3d.h"

#ifndef	POLYLIST
#define	POLYLIST

//------------------------------------------------------------------------------
//	classes
//------------------------------------------------------------------------------
class	polylist																																	//	polygon list class
{																																								//	begin class definition
	friend	class	listptr;																												//	a list pointer can play with my private parts!
	friend	class	iterator;																												//	so can a list iterator
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
	struct	node																																	//	node structure definition
	{																																							//	begin
				polyptr		ptr;																													//	reference counted polygon pointer
				node			*next;																												//	pointer to the next node
	};																																						//	end
				node			*head;																												//	the pointer to the head of the list
				bound_3d	box;																													//	bounding box for the list
				uchar			ref_count;																										//	reference count
				polylist (void);																												//	normal constructor
				~polylist (void);																												//	destructor
	public:																																				//	members available externally
				void			AddToList (const polyptr& poly);															//	add a polygon to the list
				void			Append (listptr& list);																				//	append the contents of list to this list
				polyptr		Pop (void);																										//	pop the head from the list
				bool			Empty (void) const;																						//	return whether or not the list is empty
				void			draw (void) const;																						//	draw the polygons in the list
const		bound_3d	&BoundingBox (void) const;																		//	return a reference to the bounding box of the list
};																																							//	end polygon class definition

//------------------------------------------------------------------------------
//	inlines
//------------------------------------------------------------------------------
inline	const		bound_3d	&polylist::BoundingBox (void) const										//	return a reference to the bounding box of the list
{																																								//	begin
	return box;																																		//	return the bounding box
}																																								//	end

//------------------------------------------------------------------------------

#endif	//POLYLIST
