//-----------------------------------------------------------------------------
// Copyright (C) 2006 Bretton Wade and Firaxis Games, All Rights Reserved
//-----------------------------------------------------------------------------

#include    "precompile.h"
#include    "point.h"

//-----------------------------------------------------------------------------
// globals and statics
//-----------------------------------------------------------------------------
vector<Thing>   g_thing_list;
int             g_thing_bar = 0;
short           Thing::s_objectID = 0;

//-----------------------------------------------------------------------------
// functions
//-----------------------------------------------------------------------------
int
PointInList (const D3DXVECTOR2& vec, float radius)
{
    int                         index = 0;
    
	// make sure this point isn't already in the list
	vector<Thing>::iterator     iter = g_thing_list.begin ();
	vector<Thing>::iterator     end = g_thing_list.end ();
	while (iter != end)
	{
	    Thing&      pt = *iter++;
	    D3DXVECTOR2 delta = pt.pt - vec;
	    float       test_radius = (radius == -1) ? pt.radius : radius;
	    if (D3DXVec2Length (&delta) < test_radius)
	        return index;
	    ++index;
	}
	return -1;
}

//-----------------------------------------------------------------------------
