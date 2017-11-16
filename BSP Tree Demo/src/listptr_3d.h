//------------------------------------------------------------------------------
//	File:					listptr.h
//	Date:					9/26/94
//	Author:				Bretton Wade
//
//	Description:	this file contains the class definition for a list pointer
//
//------------------------------------------------------------------------------

#include	"polylist_3d.h"

#ifndef	LISTPTR
#define	LISTPTR

//------------------------------------------------------------------------------
//	classes
//------------------------------------------------------------------------------
class	listptr																																		//	polygon list pointer class
{																																								//	begin class definition
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
				polylist	*list;																												//	the pointer to the list
	public:																																				//	members available externally
				listptr (void);																													//	normal constructor
				listptr (const listptr& list);																					//	normal constructor
				~listptr (void);																												//	destructor
				listptr		&operator = (const listptr& list);														//	assignment operator
				polylist	*operator -> (void) const;																		//	pointer dereference operator
};																																							//	end polygon class definition

//------------------------------------------------------------------------------
//	inlines
//------------------------------------------------------------------------------
inline	polylist	*listptr::operator -> (void) const														//	pointer dereference operator
{																																								//	begin
	return list;																																	//	return the pointer to the list
}																																								//	end

//------------------------------------------------------------------------------

#endif	//LISTPTR
