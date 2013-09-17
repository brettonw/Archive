//-----------------------------------------------------------------------------
// Copyright (C) 2006 Bretton Wade and Firaxis Games, All Rights Reserved
//-----------------------------------------------------------------------------

#include    "precompile.h"
#include    "line.h"
#include    "point.h"

//-----------------------------------------------------------------------------
// globals
//-----------------------------------------------------------------------------
vector<Line>    g_line_list;
int             g_line_bar = 0;

//-----------------------------------------------------------------------------
void
Line::CalculateTangentNormals (D3DXVECTOR2& normal_left, D3DXVECTOR2& normal_right) const
{
    // compute the vector between the points and the angle of that line
	Thing&      ptA = g_thing_list[iA];
	Thing&      ptB = g_thing_list[iB];
	D3DXVECTOR2 delta = ptB.pt - ptA.pt;
	float       phi = atan2f (delta.y, delta.x);
	
	// compute the distance between the two points, and use that to compute
	// the angle to the tangent line contact point
	float       d = D3DXVec2Length (&delta);
	float       theta = acosf ((ptA.radius - ptB.radius) / d);
	
	// calculate the tangent line normal vectors
	normal_left = D3DXVECTOR2 (cosf (phi + theta), sinf (phi + theta));
	normal_right = D3DXVECTOR2 (cosf (phi - theta), sinf (phi - theta));
}

//-----------------------------------------------------------------------------
