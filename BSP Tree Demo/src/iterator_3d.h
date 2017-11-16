//------------------------------------------------------------------------------
//	File:					iterator.h
//	Date:					9/26/94
//	Author:				Bretton Wade
//
//	Description:	this file contains the class definition for a list iterator
//
//------------------------------------------------------------------------------

#include "listptr_3d.h"

#ifndef	ITERATOR
#define	ITERATOR

//------------------------------------------------------------------------------
//	classes
//------------------------------------------------------------------------------
class	iterator																																	//	polygon list iterator class
{																																								//	begin class definition
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
				listptr	list;																														//	pointer to the list in use
				polylist::node	*current;																								//	pointer to the node the iterator is examining
	public:																																				//	members available externally
				iterator (const listptr& list);																					//	normal constructor
				~iterator (void);																												//	destructor
				polyptr	operator () (void);																							//	function call operator
				void		Reset (void);																										//	start at the head of the list
};																																							//	end polygon class definition

//------------------------------------------------------------------------------
//	inlines
//------------------------------------------------------------------------------
inline	polyptr	iterator::operator () (void)																		//	function call operator
{																																								//	begin
	if (current)
	{
		polylist::node	*hold = current;
		current = current->next;
		return hold->ptr;
	}
	else
		return polyptr (0);
}																																								//	end

//------------------------------------------------------------------------------

#endif	//ITERATOR
