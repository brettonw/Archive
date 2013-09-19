//------------------------------------------------------------------------------
//	File:					bsptree.cp
//	Date:					05/02/97
//	Author:				Bretton Wade
//
//	Description:	this file contains the methods for a bsp tree
//
//------------------------------------------------------------------------------

#include "precompile.h"
#include "bspnode_3d.h"

//------------------------------------------------------------------------------
//	constructor
//------------------------------------------------------------------------------
bsptree::bsptree (void)																													//	constructor
{																																								//	begin
	node = 0;																																			//	no dangling pointers
}																																								//	end

//------------------------------------------------------------------------------
//	destructor
//------------------------------------------------------------------------------
bsptree::~bsptree (void)																												//	destructor
{																																								//	begin
	if (node)																																			//	if this is a leaf in the tree
		delete node;																																//	delete the structure data
}																																								//	end

//------------------------------------------------------------------------------
//	insert a list of faces into the tree
//------------------------------------------------------------------------------
void	bsptree::Insert (listptr list, hclass keep, hclass cur)										//	insert a list of polygons into the tree
{																																								//	begin
	if (list->Empty ()) return;																										//	don't do anything if the list is empty
	if (node)																																			//	if the tree is valid
		node->Insert (list, keep);																									//	insert the polyptr list
	else																																					//	otherwise
		if ((cur == keep) || (keep == HC_SPANNING))																	//	if the current node is the kind we want
		{																																						//	begin
			node = new bspnode (list->Pop ());																				//	create the leaf representation with first polyptr in the list
			if (!list->Empty ())																											//	if the list is not empty now
				node->Insert (list, HC_SPANNING);																				//	insert the remaining polyptr list
		}																																						//	end
}																																								//	end

//------------------------------------------------------------------------------
//	push a face through the tree
//------------------------------------------------------------------------------
void	bsptree::Push (polyptr poly, listptr result, hclass keep, hclass cur)			//	push a polyptr through the tree
{																																								//	begin
	if (node)																																			//	if the tree is valid
		node->Push (poly, result, keep);																						//	push the polyptr
	else																																					//	otherwise
		if (cur == keep)																														//	if the current node is the kind we want
			result->AddToList (poly);																									//	add the polyptr to the list
}																																								//	end

//------------------------------------------------------------------------------
//	push a list of faces through the tree
//------------------------------------------------------------------------------
void	bsptree::Push (listptr list, listptr result, hclass keep, hclass cur)			//	push a list of polygons through the tree
{																																								//	begin
	if (list->Empty ()) return;																										//	don't do anything if the list is empty
	if (node)																																			//	if the tree is valid
		node->Push (list, result, keep);																						//	push the polyptr list
	else																																					//	otherwise
		if (cur == keep)																														//	if the current node is the kind we want
			result->Append (list);																										//	append the list to the results
}																																								//	end

//------------------------------------------------------------------------------
//	reduce to boundary
//------------------------------------------------------------------------------
void	bsptree::Reduce (void)																										//	reduce the tree to only boundary polygons
{																																								//	begin
	if (node)																																			//	if the tree is valid
		node->Reduce ();																														//	compute the boundary representation
}																																								//	end

//------------------------------------------------------------------------------
//	draw
//------------------------------------------------------------------------------
void	bsptree::Draw (const point_3d &eye) const																	//	draw the bsp
{																																								//	begin
	if (node)																																			//	if the tree is valid
		node->Draw (eye);																														//	draw it
}																																								//	end

//------------------------------------------------------------------------------
//	intersect with ray
//------------------------------------------------------------------------------
bool	bsptree::RayIntersection (const ray &r, polyptr &poly_hit, point_3d &ipt) const	//	compute the polygon intersected by a ray
{																																								//	begin
	if (node)																																			//	if the tree is valid
		return node->RayIntersection (r, poly_hit, ipt);														//	trace it
	return FALSE;																																	//	return a no hit
}																																								//	end

//------------------------------------------------------------------------------
//	convert the tree into an unordered list
//------------------------------------------------------------------------------
listptr	bsptree::TreeToList (void)																							//	return a list of all the polygons in the tree
{																																								//	begin
	listptr	myList;																																//	place to get a list
	if (node)																																			//	if the tree is valid
		myList = node->TreeToList ();																								//	make it into a list
	return myList;																																//	return the list
}																																								//	end

//------------------------------------------------------------------------------
