//-----------------------------------------------------------------------------
// Copyright (C) 2006 Bretton Wade, All Rights Reserved
//-----------------------------------------------------------------------------

#include    "precompile.h"
#include    "thing.h"
#include    "solver.h"

//-----------------------------------------------------------------------------
// static variables
//-----------------------------------------------------------------------------
Thing*          Thing::s_pFreeList = 0;
Thing*          Thing::s_pThingBase = 0;

//-----------------------------------------------------------------------------
// class methods
//-----------------------------------------------------------------------------
void
Thing::UpdateBounds (float buffer)
{
    switch (m_objectType bit_and OT_SHAPE_MASK)
    {
        case OT_CIRCLE:
        {
            float   r = m_radius + buffer;
            m_bound.Set (m_center.x - r, m_center.x + r, m_center.y - r, m_center.y + r);
            break;
        }
        //case OT_BOX:
    }
}

//-----------------------------------------------------------------------------
bool
Thing::ContainsPoint (const Vector2& pt) const
{
    switch (m_objectType bit_and OT_SHAPE_MASK)
    {
        case OT_CIRCLE:
        {
            Vector2		delta = pt - m_center;
			float       lengthSquared = delta.LengthSq ();
            float       radiusSquared = m_radius * m_radius;
            return (lengthSquared < radiusSquared);
        }
        //case OT_BOX:
    }
    return false;
}

//-----------------------------------------------------------------------------
void*
Thing::operator new (size_t size)
{
  entrypoint:
    if (s_pFreeList)
    {
        Thing*  ptr = s_pFreeList;
        s_pFreeList = s_pFreeList->GetNext ();
        return ptr;
    }
    else
    {
        if (s_pThingBase)
        {
            // this situation is that the initial allocation has run out, which 
            // can't be handled simply by allocating more because then the id's
            // would be meaningless. There may be some possibility of relocating
            // the entire array, but that's probably not a great solution in the
            // context of game performance
            return 0;
        }
        else
        {
            // set up the free list from a block allocation of N objects
            s_pThingBase = r_cast<Thing*> (new Byte[sizeof (Thing) * N]);
            s_pFreeList = 0;
            for (Word i = N; i > 0; --i)
                delete &s_pThingBase[i - 1];
            goto entrypoint;
        }
    }
}
   
//-----------------------------------------------------------------------------
