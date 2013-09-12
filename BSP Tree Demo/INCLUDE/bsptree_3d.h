//------------------------------------------------------------------------------
//	File:					bsptree.h
//	Date:					9/25/94
//	Author:				Bretton Wade
//
//	Description:	this file contains the class definition for a bsp tree
//
//------------------------------------------------------------------------------

#include	"split.h"
#include	"listptr_3d.h"

#ifndef BSPTREE
#define BSPTREE

//------------------------------------------------------------------------------
//	classes
//------------------------------------------------------------------------------
class	bspnode;																																	//	forward declaration

//------------------------------------------------------------------------------
class	bsptree																																		//	binary space partition tree class
{																																								//	begin class definition
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
				bspnode	*node;																													//	pointer to the data for this tree
	public:																																				//	members available externally
				bsptree (void);																													//	default constructor
				~bsptree (void);																												//	destructor
				void		Insert (listptr list, hclass keep, hclass cur);									//	insert a polylist list into the tree
				void		Push (polyptr poly, listptr result, hclass keep, hclass cur);		//	push a polygon through the tree
				void		Push (listptr list, listptr result, hclass keep, hclass cur);		//	push a polygon list through the tree
				void		Reduce (void);																									//	reduce the tree to only boundary polygons
				void		Draw (const point_3d	&eye) const;															//	draw the tree to the view
				bool		RayIntersection (const ray &r, polyptr &poly_hit, point_3d &ipt) const;	//	compute the polygon intersected by a ray
};																																							//	endclass definition

//------------------------------------------------------------------------------

#endif //BSPTREE
