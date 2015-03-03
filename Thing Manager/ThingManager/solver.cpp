//-----------------------------------------------------------------------------
// Copyright (C) 2006 Bretton Wade, All Rights Reserved
//-----------------------------------------------------------------------------

#include    "precompile.h"
#include    "solver.h"
#include    "constants.h"

//-----------------------------------------------------------------------------
// globals
//-----------------------------------------------------------------------------
Solver          g_solver;

//-----------------------------------------------------------------------------
// constants
//-----------------------------------------------------------------------------
const float     c_fError = 1.0e-2f;     // 1 percent
const float     c_tension = 0.925f;     // 1.0 is exact, but may appear too stiff
const float     c_fBuffer = 1.0e-2f;    // one hundredth

//-----------------------------------------------------------------------------
// class methods
//-----------------------------------------------------------------------------
/* void*/
Solver::Solver (void)
{
}
   
//-----------------------------------------------------------------------------
void
Solver::AddThing (Thing* pThing)
{
    if (pThing)
    {
        // add it to the collider
        m_collider.AddThing (pThing, c_fBuffer);
    }
}
   
//-----------------------------------------------------------------------------
void
Solver::RemoveThing (Thing* pThing)
{
    if (pThing)
    {
        // remove it from the collider
        m_collider.RemoveThing (pThing);
    }
}
   
//-----------------------------------------------------------------------------
void
Solver::RelaxConstraints (Word iterations)     
{
    // loop as many times as we need to    
    do
    {
        // update the bounds, and the collider
        m_collider.Update (c_fBuffer);
        
        // clear the correction count
        m_corrections = 0;

        // loop over the open list
        for (Word i = 0; i < m_collider.m_openCount; ++i)
        {
            // de-mash the stored key and apply the distance constraint
            Thing*  pThingA = m_collider.m_open[i].GetThingA ();
            Thing*  pThingB = m_collider.m_open[i].GetThingB ();
            ApplyCircleDistanceToCircleConstraint (pThingA, pThingB);
        }
    }
	while (--iterations and (m_corrections > 0));
}
   
//-----------------------------------------------------------------------------
void
Solver::ApplyCircleDistanceToCircleConstraint (Thing* pThingA, Thing* pThingB)
{
	// calculate the distance between the two points
	Vector2			delta = pThingB->m_center - pThingA->m_center;
	float           distance = delta.Length ();
	float           constraint = pThingA->m_radius + pThingB->m_radius;
	if ((distance < constraint) and ((fabsf (constraint - distance) / constraint) > c_fError))
	{
		delta = delta * (1.0f / distance);
	    float   move = c_tension * (constraint - distance);
    	
    	// compute a scale factor depending on the relative mass of the two objects
    	float   massA = pThingA->m_radius * pThingA->m_radius;
    	float   massB = pThingB->m_radius * pThingB->m_radius;
    	float   mass_total = massA + massB;
	    if (pThingA->m_objectType bit_and OT_FIXED_POSITION)
	        massA = mass_total;
	    if (pThingB->m_objectType bit_and OT_FIXED_POSITION)
	        massB = mass_total;
	    if (not (pThingA->m_objectType bit_and OT_FIXED_POSITION))
	    {
	        pThingA->m_center = pThingA->m_center - (((massB / mass_total) * move) * delta);
	        ++m_corrections;
	    }
	    if (not (pThingB->m_objectType bit_and OT_FIXED_POSITION))
	    {
	        pThingB->m_center = pThingB->m_center + (((massA / mass_total) * move) * delta);
	        ++m_corrections;
	    }
	}
}

//-----------------------------------------------------------------------------
