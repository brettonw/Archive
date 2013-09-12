//------------------------------------------------------------------------------
//	File:					bspnode.h
//	Date:					9/26/94
//	Author:				Bretton Wade
//
//	Description:	this file contains the class definition for a bsp tree node
//
//------------------------------------------------------------------------------

#include "bsptree_3d.h"

#ifndef BSPNODE
#define BSPNODE

//------------------------------------------------------------------------------
class	bspnode																																		//	binary space partition tree node class
{																																								//	begin class definition
	friend	class	bsptree;																												//	bsptree can play with my private parts
	private:																																			//	members internal to this class only
	protected:																																		//	members internal to this class hierarchy
				plane_3d		plane;																											//	the plane_3d equation of this node
				listptr	on;																															//	list of faces on the plane_3d
				bsptree	in;																															//	subtree of the "in" space relative to the plane_3d
				bsptree	out;																														//	subtree of the "out" space relative to the plane_3d
				bspnode (const polyptr &poly);																					//	normal constructor
				~bspnode (void);																												//	destructor
	public:																																				//	members available externally
				void		Insert (listptr list, hclass keep);															//	selectively insert a polygon list into the tree
				void		Push (polyptr poly, listptr result, hclass keep);								//	push a polygon through the tree
				void		Push (listptr list, listptr result, hclass keep);								//	push a list through the tree
				void		Reduce (void);																									//	reduce the tree to only boundary polygons
				void		Draw (const point_3d	&eye) const;															//	draw the tree
				bool		RayIntersection (const ray &r, polyptr &poly_hit, point_3d &ipt) const;	//	compute the polygon intersected by a ray
				bool		PlaneSearch (const ray &r, polyptr &poly_hit, point_3d &ipt) const;//	find the intersection of a ray and the plane_3d list
};																																							//	end class definition

//------------------------------------------------------------------------------

#endif //BSPNODE
