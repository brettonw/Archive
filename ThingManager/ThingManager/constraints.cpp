//-----------------------------------------------------------------------------
// Copyright (C) 2006 Bretton Wade and Firaxis Games, All Rights Reserved
//-----------------------------------------------------------------------------

#include    "precompile.h"
#include    "constraints.h"
#include    "point.h"
#include    "line.h"

//-----------------------------------------------------------------------------
// globals
//-----------------------------------------------------------------------------
int     g_num_corrections = 0;

//-----------------------------------------------------------------------------
// line length constraints
//-----------------------------------------------------------------------------
void
ApplyLineLengthConstraint (Thing& ptA, Thing& ptB, float constraint)
{
    // this function takes two points and a length between them that should be enforced
    // and it applies forces to make the length constraint true
	// calculate the distance between the two points
	D3DXVECTOR2     delta = ptB.pt - ptA.pt;
	float           distance = D3DXVec2Length (&delta);
	if ((fabsf (constraint - distance) / constraint) > c_fError)
	{
	    D3DXVec2Scale (&delta, &delta, 1.0f / distance);

	    float   move = c_tension * (constraint - distance);
    	
        // compute a scale factor depending on the relative mass of the two objects
        float   massA = ptA.radius * ptA.radius;
        float   massB = ptB.radius * ptB.radius;
        float   mass_total = massA + massB;
	    if (ptA.object_type bit_and OT_LOCKED)
	        massA = mass_total;
	    if (ptB.object_type bit_and OT_LOCKED)
	        massB = mass_total;
	    if (not (ptA.object_type bit_and OT_LOCKED))
	    {
	        ptA.pt = ptA.pt - (((massB / mass_total) * move) * delta);
	        ++g_num_corrections;
	    }
	    if (not (ptB.object_type bit_and OT_LOCKED))
	    {
	        ptB.pt = ptB.pt + (((massA / mass_total) * move) * delta);
	        ++g_num_corrections;
	    }
	}
}

//-----------------------------------------------------------------------------
// thing to thing distance constraints
//-----------------------------------------------------------------------------
void
ApplyCircleDistanceToCircleConstraint (Thing& ptA, Thing& ptB)
{
	// calculate the distance between the two points
	D3DXVECTOR2     delta = ptB.pt - ptA.pt;
	float           distance = D3DXVec2Length (&delta);
	float           constraint = ptA.radius + ptB.radius;
	if ((distance < constraint) and ((fabsf (constraint - distance) / constraint) > c_fError))
	{
	    D3DXVec2Scale (&delta, &delta, 1.0f / distance); 
	    float   move = c_tension * (constraint - distance);
    	
    	// compute a scale factor depending on the relative mass of the two objects
    	float   massA = ptA.radius * ptA.radius;
    	float   massB = ptB.radius * ptB.radius;
    	float   mass_total = massA + massB;
	    if (ptA.object_type bit_and OT_LOCKED)
	        massA = mass_total;
	    if (ptB.object_type bit_and OT_LOCKED)
	        massB = mass_total;
	    if (not (ptA.object_type bit_and OT_LOCKED))
	    {
	        ptA.pt = ptA.pt - (((massB / mass_total) * move) * delta);
	        ++g_num_corrections;
	    }
	    if (not (ptB.object_type bit_and OT_LOCKED))
	    {
	        ptB.pt = ptB.pt + (((massA / mass_total) * move) * delta);
	        ++g_num_corrections;
	    }
	}
}

//-----------------------------------------------------------------------------
bool
ApplyBoxLineLengthConstraint (D3DXVECTOR2& ptA, D3DXVECTOR2& ptB, float constraint)
{
	D3DXVECTOR2     delta = ptB - ptA;
	float           distance = D3DXVec2Length (&delta);
	if ((fabsf (constraint - distance) / constraint) > c_fError)
	{
	    D3DXVec2Scale (&delta, &delta, 1.0f / distance);
	    float   move = c_tension * (constraint - distance);
	    ptA = ptA - (move * delta);
	    ptB = ptB + (move * delta);
	    return true;
    }
    return false;
}

//-----------------------------------------------------------------------------
void
ApplyBoxShapeConstraints (D3DXVECTOR2& ptA, D3DXVECTOR2& ptB, D3DXVECTOR2& ptC, D3DXVECTOR2& ptD, float width, float height, int iterations = 5)
{
    // the input is for the construction vectors, it needs to be scaled to be correct
    width *= 2.0f;
    height *= 2.0f;

    // AB is always the X axis, and AC is always the Y axis
    // the box is considered homogeneous in composition, so all 4 points have the same mass
    bool    corrections_made;
    do
    {
        // clear the corrections flag, else we loop forever
        corrections_made = false;
        
        // edge length constraints
        corrections_made = corrections_made or ApplyBoxLineLengthConstraint (ptA, ptB, width);
        corrections_made = corrections_made or ApplyBoxLineLengthConstraint (ptC, ptD, width);
        corrections_made = corrections_made or ApplyBoxLineLengthConstraint (ptB, ptD, height);
        corrections_made = corrections_made or ApplyBoxLineLengthConstraint (ptA, ptC, height);

        // diagonal constraints
        float   diagonal = sqrtf ((width * width) + (height * height));
        corrections_made = corrections_made or ApplyBoxLineLengthConstraint (ptA, ptD, diagonal);
        corrections_made = corrections_made or ApplyBoxLineLengthConstraint (ptB, ptC, diagonal);
    }
    while ((iterations > 0) and corrections_made);
}

//-----------------------------------------------------------------------------
void    
MoveBoxPoints (float dp, float length, float m_scale, float move, D3DXVECTOR2& ptA, D3DXVECTOR2& ptB, D3DXVECTOR2& axis)
{
    // map the dot product (dp) to a linear weight of the force distance, 0 is 
    // closer to point A, 1 is closer to point B
    float       b_scale = dp / length;
    float       a_scale = 1.0f - b_scale;
    ptA = ptA - ((a_scale * m_scale * move) * axis);
    ptB = ptB - ((b_scale * m_scale * move) * axis);
}

//-----------------------------------------------------------------------------
void
CalculateBoxParameters (Thing& pt, D3DXVECTOR2& ptA, D3DXVECTOR2& ptB, D3DXVECTOR2& ptC, D3DXVECTOR2& ptD, float xsign, float ysign)
{
    ApplyBoxShapeConstraints (ptA, ptB, ptC, ptD, pt.width, pt.height);
    D3DXVECTOR2 AB = ptA - ptB;
    //D3DXVec2Normalize (&AB, &AB);
    D3DXVec2Scale (&AB, &AB, 1 / D3DXVec2Length(&AB));
    D3DXVECTOR2 xaxis = AB * xsign;
    D3DXVECTOR2 yaxis = D3DXVECTOR2 (xaxis.y, -xaxis.x) * ysign;
    pt.xaxis = xaxis;
    pt.pt = (ptA - (AB* pt.width)) + (yaxis * pt.height);
	++g_num_corrections;
}

//-----------------------------------------------------------------------------
void
ApplyBoxDistanceToCircleConstraint (Thing& box, Thing& circle)
{
    // a quick rejection test, assuming that the box width + box height is a
    // radius much bigger than a circumscribed circle on the box
    float           rough_r = circle.radius + box.width + box.height;
    D3DXVECTOR2     delta = circle.pt - box.pt;
    if (D3DXVec2LengthSq (&delta) > (rough_r * rough_r))
        return;
    
    // calculate the mass of the objects to be tested
    float           mass_box = box.width * box.height * 4.0f;
    float           mass_circle = circle.radius * circle.radius * D3DX_PI;
    float           mass_total = mass_box + mass_circle;
    if (circle.object_type bit_and OT_LOCKED)
    	mass_circle = mass_total;
    if (box.object_type bit_and OT_LOCKED)
    	mass_box = mass_total;
    float           mass_box_scale = mass_circle / mass_total;
    float           mass_circle_scale = mass_box / mass_total;

    // four lines and four circles of radius c_fSpacing, only two lines and
    // one circle actually need to be tested
    D3DXVECTOR2     xaxis = box.xaxis;
    D3DXVECTOR2     yaxis (-xaxis.y, xaxis.x);
    float           xsign = (D3DXVec2Dot (&delta, &xaxis) > 0.0f) ? 1.0f : -1.0f;
    float           ysign = (D3DXVec2Dot (&delta, &yaxis) > 0.0f) ? 1.0f : -1.0f;
    D3DXVec2Scale (&xaxis, &xaxis, xsign);
    D3DXVec2Scale (&yaxis, &yaxis, ysign);
    
    // calculate the box points, the convention here is that AB is the X axis, AC is the y axis
    D3DXVECTOR2     ptA = box.pt + (xaxis * box.width) + (yaxis * box.height);
    D3DXVECTOR2     ptB = box.pt - (xaxis * box.width) + (yaxis * box.height);
    D3DXVECTOR2     ptC = box.pt + (xaxis * box.width) - (yaxis * box.height);
    D3DXVECTOR2     ptD = box.pt - (xaxis * box.width) - (yaxis * box.height);
    
    // AB and AC are the segments we need to test against
    float           constraint = circle.radius;
	float           distance_AB = D3DXVec2Dot (&yaxis, &circle.pt) - D3DXVec2Dot (&ptA, &yaxis);
	float           distance_AC = D3DXVec2Dot (&xaxis, &circle.pt) - D3DXVec2Dot (&ptA, &xaxis);
	
	// in order to choose which axis to work against, we look at the distances from the edges to the test
	// point. if both are negative, the test point is inside the box and we want to work with the one closest
	// to its edge, e.g. the highest value (negative numbers, so the absolute value of the largest one is 
	// smaller). if both are positive, we want to respond only to the vertex pt. if either is positive and 
	// the other is negative, we want to work with the positive one, e.g. the highest value.
	if ((distance_AB < 0) or (distance_AC < 0))
	{
        D3DXVECTOR2     AX = circle.pt - ptA;
	    if (distance_AB > distance_AC)
	    {
	        // working with segment AB, the x axis
	        if ((distance_AB < constraint) and ((fabsf (constraint - distance_AB) / constraint) > c_fError))
            {
	            // check to see if the point is within range of the segment
	            float           dp = -D3DXVec2Dot (&xaxis, &AX);
	            float           AB_length = 2.0f * box.width;
	            if ((dp > 0) and (dp < AB_length))
	            {
	                // it is, so proceed with the constraint solver
	                float       move = c_tension * (constraint - distance_AB);
        	        
    	            // move the point away from the line
	                if (not (circle.object_type bit_and OT_LOCKED))
	                {
	                    circle.pt = circle.pt + ((mass_circle_scale * move) * yaxis);
	                    ++g_num_corrections;
	                }
        	            
	                if (not (box.object_type bit_and OT_LOCKED))
	                {
	                    MoveBoxPoints (dp, AB_length, mass_box_scale, move, ptA, ptB, yaxis);
	                    CalculateBoxParameters (box, ptA, ptB, ptC, ptD, xsign, ysign);
	                }
	            }
            }
	    }
	    else
	    {
	        // working with segment AC, the y axis
	        if ((distance_AC < constraint) and ((fabsf (constraint - distance_AC) / constraint) > c_fError))
            {
	            // check to see if the point is within range of the segment
	            float           dp = -D3DXVec2Dot (&yaxis, &AX);
	            float           AC_length = (2.0f * box.height);
	            if ((dp > 0) and (dp < AC_length))
	            {
	                // it is, so proceed with the constraint solver
	                float       move = c_tension * (constraint - distance_AC);
        	        
    	            // move the point away from the line
	                if (not (circle.object_type bit_and OT_LOCKED))
	                {
	                    circle.pt = circle.pt + ((mass_circle_scale * move) * xaxis);
    	                ++g_num_corrections;
	                }
        	            
	                if (not (box.object_type bit_and OT_LOCKED))
	                {
	                    MoveBoxPoints (dp, AC_length, mass_box_scale, move, ptA, ptC, xaxis);
	                    CalculateBoxParameters (box, ptA, ptB, ptC, ptD, xsign, ysign);
	                }
	            }
            }
	    }
	}
    else
    {
        // the circle point is in the region where it only affects the corner of the box, in this 
        // case we want to move the vertex, and then look at the delta vector to determine if there 
        // is any rotation implied by the constraint
        delta = circle.pt - ptA;
	    float           distance = D3DXVec2Length (&delta);
	    if ((distance < constraint) and ((fabsf (constraint - distance) / constraint) > c_fError))
	    {
	        D3DXVec2Scale (&delta, &delta, 1.0f / distance); 
	        float   move = c_tension * (constraint - distance);
        	
    	    // compute a scale factor depending on the relative mass of the two objects
	        if (not (circle.object_type bit_and OT_LOCKED))
	        {
	            circle.pt = circle.pt + (((mass_box / mass_total) * move) * delta);
	            ++g_num_corrections;
	        }
	        if (not (box.object_type bit_and OT_LOCKED))
	        {
	            ptA = ptA - ((mass_circle_scale * move) * delta);
	            CalculateBoxParameters (box, ptA, ptB, ptC, ptD, xsign, ysign);
	        }
	    }
	}
}

//-----------------------------------------------------------------------------
void
ApplyBoxDistanceToBoxConstraint (Thing& ptA, Thing& ptB)
{
}

//-----------------------------------------------------------------------------
void
ApplyThingDistanceToThingConstraint (Thing& ptA, Thing& ptB)
{
    // possible combinations:
    // circle and circle
    // box and box
    // circle and box
    // box and circle
    int     shapeA = ptA.object_type bit_and OT_SHAPE;
    int     shapeB = ptB.object_type bit_and OT_SHAPE;
    if (shapeA == OT_CIRCLE)
    {
        if (shapeB == OT_CIRCLE)
            ApplyCircleDistanceToCircleConstraint (ptA, ptB);
        else if (shapeB == OT_BOX)
            ApplyBoxDistanceToCircleConstraint (ptB, ptA);
    }
    else if (shapeA == OT_BOX)
    {
        if (shapeB == OT_CIRCLE)
            ApplyBoxDistanceToCircleConstraint (ptA, ptB);
        else if (shapeB == OT_BOX)
            ApplyBoxDistanceToBoxConstraint (ptA, ptB);
    }
}

//-----------------------------------------------------------------------------
// line to thing distance constraints
//-----------------------------------------------------------------------------
void
ApplyLineDistanceToCircleConstraint (Thing& pt, Line& ln, float distance, const D3DXVECTOR2& n, float c, const D3DXVECTOR2& ptA, const D3DXVECTOR2& delta, float delta_len, float mass_ln)
{
    float           constraint = pt.radius;
	if ((distance < constraint) and ((fabsf (constraint - distance) / constraint) > c_fError))
    {
	    // check to see if the point is within range of the segment
	    D3DXVECTOR2 AC = pt.pt - ptA;
	    float       dp = D3DXVec2Dot (&delta, &AC);
	    if ((dp > 0) and (dp < delta_len))
	    {
	        // it is, so proceed with the constraint solver
	        float   move = c_tension * (constraint - distance);
	        
	        // compute a scale factor depending on the relative mass of the two objects
    	    float   mass_pt = pt.radius * pt.radius * D3DX_PI;
    	    float   mass_sum = mass_pt + mass_ln;
    	    
    	    // move the point away from the line
    	    if (pt.object_type bit_and OT_LOCKED)
    	        mass_pt = mass_sum;
	        if (not (pt.object_type bit_and OT_LOCKED))
	            pt.pt = pt.pt + (((mass_ln / mass_sum) * move) * n);
	            
	        // map dp to a linear weight of the force distance, 0 is closer to 
	        // point A, 1 is closer to point B
	        float   b_scale = dp / delta_len;
	        float   a_scale = 1.0f - b_scale;
	        float   m_scale = mass_pt / mass_sum;
	        Thing&  ptA = g_thing_list[ln.iA];
	        if (not (ptA.object_type bit_and OT_LOCKED))
	        {
	            ptA.pt = ptA.pt - ((a_scale * m_scale * move) * n);
	            ++g_num_corrections;
	        }
	        Thing&  ptB = g_thing_list[ln.iB];
	        if (not (ptB.object_type bit_and OT_LOCKED))
	        {
	            ptB.pt = ptB.pt - ((b_scale * m_scale * move) * n);
	            ++g_num_corrections;
	        }
	    }
    }
}

//-----------------------------------------------------------------------------
void
ApplyLineDistanceToBoxConstraint (Thing& pt, Line& ln, float distance, const D3DXVECTOR2& n, float c, const D3DXVECTOR2& ptA, const D3DXVECTOR2& delta, float delta_len, float mass_ln)
{
    // figure out which of the 4 corners of the box has the most error
    // with respect to the line, compute the movement of that point, then
    // recalculate the box parameters
    
    
    /*
    // a quick rejection test, assuming that the box width + box height is a
    // radius much bigger than a circumscribed circle on the box
    float           rough_r = circle.radius + box.width + box.height;
    D3DXVECTOR2     delta = circle.pt - box.pt;
    if (D3DXVec2LengthSq (&delta) > (rough_r * rough_r))
        return;
    
    // calculate the mass of the objects to be tested
    float           mass_box = box.width * box.height * 4.0f;
    float           mass_circle = circle.radius * circle.radius * D3DX_PI;
    float           mass_total = mass_box + mass_circle;
    if (circle.object_type bit_and OT_LOCKED)
    	mass_circle = mass_total;
    if (box.object_type bit_and OT_LOCKED)
    	mass_box = mass_total;
    float           mass_box_scale = mass_circle / mass_total;
    float           mass_circle_scale = mass_box / mass_total;

    // four lines and four circles of radius c_fSpacing, only two lines and
    // one circle actually need to be tested
    D3DXVECTOR2     xaxis = box.xaxis;
    D3DXVECTOR2     yaxis (-xaxis.y, xaxis.x);
    float           xsign = (D3DXVec2Dot (&delta, &xaxis) > 0.0f) ? 1.0f : -1.0f;
    float           ysign = (D3DXVec2Dot (&delta, &yaxis) > 0.0f) ? 1.0f : -1.0f;
    D3DXVec2Scale (&xaxis, &xaxis, xsign);
    D3DXVec2Scale (&yaxis, &yaxis, ysign);
    
    // calculate the box points, the convention here is that AB is the X axis, AC is the y axis
    D3DXVECTOR2     ptA = box.pt + (xaxis * box.width) + (yaxis * box.height);
    D3DXVECTOR2     ptB = box.pt - (xaxis * box.width) + (yaxis * box.height);
    D3DXVECTOR2     ptC = box.pt + (xaxis * box.width) - (yaxis * box.height);
    D3DXVECTOR2     ptD = box.pt - (xaxis * box.width) - (yaxis * box.height);
    */
}

//-----------------------------------------------------------------------------
void
ApplyLineDistanceToThingConstraint (Thing& pt, Line& ln, float distance, const D3DXVECTOR2& n, float c, const D3DXVECTOR2& ptA, const D3DXVECTOR2& delta, float delta_len, float mass_ln)
{
    // multiple dispatch
    if ((pt.object_type bit_and OT_SHAPE) == OT_CIRCLE)
        ApplyLineDistanceToCircleConstraint (pt, ln, distance, n, c, ptA, delta, delta_len, mass_ln);
    else if ((pt.object_type bit_and OT_SHAPE) == OT_BOX)
        ApplyLineDistanceToBoxConstraint (pt, ln, distance, n, c, ptA, delta, delta_len, mass_ln);
}

//-----------------------------------------------------------------------------
// the constraint solver
//-----------------------------------------------------------------------------
void
RelaxPoints (int iterations)
{
    int i = iterations;
    do
    {
        g_num_corrections = 0;
        {
            // iterate over the line segments and enforce length constraints
            vector<Line>::iterator      ln_iter = g_line_list.begin ();
            vector<Line>::iterator      ln_end = g_line_list.end ();
            while (ln_iter != ln_end)
            {
                Line&       ln = *ln_iter++;
                if (ln.length > 0.0f)
                    ApplyLineLengthConstraint (g_thing_list[ln.iA], g_thing_list[ln.iB], ln.length);
	        }
	    }
    
        {
            // iterate over the points for each line segment
            vector<Line>::iterator      ln_iter = g_line_list.begin ();
            vector<Line>::iterator      ln_end = g_line_list.end ();
            while (ln_iter != ln_end)
            {
                Line&       ln = *ln_iter++;
                if (not (ln.object_type bit_and OT_DONT_APPLY_CONSTRAINT))
                {
	                Thing&      ptA = g_thing_list[ln.iA];
	                Thing&      ptB = g_thing_list[ln.iB];
        
                    // figure the mass of the line for applying forces
                    D3DXVECTOR2 delta = ptB.pt - ptA.pt;	        
        	        float   mass_ln = ((ptA.radius + ptB.radius) * D3DXVec2Length (&delta));

	                // calculate the tangent lines
	                D3DXVECTOR2 normal_left;
	                D3DXVECTOR2 normal_right;
	                ln.CalculateTangentNormals (normal_left, normal_right);
	                D3DXVECTOR2 ptA_left = ptA.pt + (normal_left * ptA.radius);
	                D3DXVECTOR2 ptB_left = ptB.pt + (normal_left * ptB.radius);
	                D3DXVECTOR2 delta_left = ptB_left - ptA_left;
	                float       delta_left_len = D3DXVec2Length (&delta_left);
	                D3DXVec2Scale (&delta_left, &delta_left, 1.0f / delta_left_len);
	                float       c_left = -D3DXVec2Dot (&normal_left, &ptA_left);
    	            
	                D3DXVECTOR2 ptA_right = ptA.pt + (normal_right * ptA.radius);
	                D3DXVECTOR2 ptB_right = ptB.pt + (normal_right * ptB.radius);
	                D3DXVECTOR2 delta_right = ptB_right - ptA_right;
	                float       delta_right_len = D3DXVec2Length (&delta_right);
	                D3DXVec2Scale (&delta_right, &delta_right, 1.0f / delta_right_len);
	                float       c_right = -D3DXVec2Dot (&normal_right, &ptA_right);
                    
                    // loop over all of the points
	                vector<Thing>::iterator     pt_iter = g_thing_list.begin ();
	                vector<Thing>::iterator     pt_end = g_thing_list.end ();
	                while (pt_iter != pt_end)
	                {
	                    Thing&          pt = *pt_iter++;
	                    if ((pt.object_id != ptA.object_id) and (pt.object_id != ptB.object_id))
	                    {
	                        // calculate the distance between the point and the line
	                        float           distance_left = D3DXVec2Dot (&normal_left, &pt.pt) + c_left;
	                        float           distance_right = D3DXVec2Dot (&normal_right, &pt.pt) + c_right;
	                        if (distance_left > distance_right)
	                            ApplyLineDistanceToThingConstraint (pt, ln, distance_left, normal_left, c_left, ptA_left, delta_left, delta_left_len, mass_ln);
	                        else
	                            ApplyLineDistanceToThingConstraint (pt, ln, distance_right, normal_right, c_right, ptA_right, delta_right, delta_right_len, mass_ln);
	                    }
	                }
	            }
            }
        }
        
        {
            // DUMB n^2 iteration through the things, a better scheme is needed
	        vector<Thing>::iterator     iter = g_thing_list.begin ();
	        vector<Thing>::iterator     end = g_thing_list.end ();
	        while (iter != end)
	        {
	            Thing&                  thing = *iter++;
	            vector<Thing>::iterator iter2 = iter;
	            while (iter2 != end)
	                ApplyThingDistanceToThingConstraint (thing, *iter2++);
	        }
	    }
	}
	while (--i and (g_num_corrections > 0));
}

//-----------------------------------------------------------------------------
