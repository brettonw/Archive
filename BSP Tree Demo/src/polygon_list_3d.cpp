#include    "precompile.h"
#include    "polygon_list_3d.h"
#include    "view.h"

PolygonList_3d::PolygonList_3d (void)																												//	normal constructor
{																																								//	begin
}																																								//	end

PolygonList_3d::~PolygonList_3d (void)																											//	destructor
{																																								//	begin
}																																								//	end

void PolygonList_3d::addToList (const PtrToPolygon_3d& poly)																//	add a polygon to the list
{																																								//	begin
    polygons.push_back (poly);
	for (uint i = 0; i < poly->getCount (); i++)																			//	take each vertex of the polygon
		box += poly->getVertex (i);																										//	and add it into the bounding box for the list
}																																								//	end

void PolygonList_3d::append (const PtrToPolygonList_3d& list)																				//	append the contents of list to this list
{																																								//	begin
    polygons.insert (polygons.end (), list->polygons.begin (), list->polygons.end ());
}																																								//	end

PtrToPolygon_3d	PolygonList_3d::pop (void)																										//	pop the head from the list
{			
    if (polygons.size () > 0) {
        PtrToPolygon_3d polygon = polygons.back ();
        polygons.pop_back ();
        return polygon;
    }
    return PtrToPolygon_3d ();
}																																								//	end

bool		PolygonList_3d::empty (void) const																						//	return whether or not the list is empty
{																																								//	begin
    return (polygons.size () == 0);
}																																								//	end

void		PolygonList_3d::draw (void) const																							//	draw the polygons in the list
{																																								//	begin
    for (std::vector<PtrToPolygon_3d>::const_iterator it = polygons.begin (); it != polygons.end (); ++it) {
        gView->drawPolygon (*it);
    }
}																																								//	end
