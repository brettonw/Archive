//-----------------------------------------------------------------------------
// Copyupper (C) 2006 Bretton Wade, All Rights Reserved
//-----------------------------------------------------------------------------

#include    "precompile.h"
#include    "collider.h"
#include    "constants.h"
#include    "array.h"

//-----------------------------------------------------------------------------
// class methods
//-----------------------------------------------------------------------------
void
Collider::AddToBounds (Word axis, const Thing* pThing, float buffer)
{
    // NOTE that the size of the bounds array is set by the number of things
    // allocated, so it's not possible to run out of space here. Therefore, 
    // there is no need to check for this in the release runtime
    
    // set up a pointer to the bounds array we want to work with
    UpperHalf*  bounds = &m_bounds[axis][0];
    
    // find the place in the bounds where we should put this thing to insure 
    // that it correctly relates to all objects it could possibly interact with
    // we do this by putting it just below the lowest bound of the largest
    // thing in the array that is near the correct position
    float   value = pThing->m_bound[axis][LOWER_BOUND] - (m_maxSize[axis] + buffer);
    Word    index = FindInSortedArray (bounds, value, m_boundsCount);
    
    // if this bound doesn't go at the end of the array, move the rest of the 
    // array entries out of the way so we can insert the new bounds where we want
    if (index < m_boundsCount)
        memmove (&bounds[index + 2], &bounds[index], (m_boundsCount - index) * sizeof (UpperHalf));
    
    // add the bounds at our desired location, and force a sort to make sure
    // that the sorted condition is in place for future additions
    bounds[index++] = UpperHalf (pThing, axis, false);
    bounds[index] = UpperHalf (pThing, axis, true);
    SortBounds (axis, m_boundsCount + 2);

    // compare ourselves to the largest object bound and update that bound...
    float   size = pThing->m_bound.GetSize (axis);
    m_maxSize[axis] = (size > m_maxSize[axis]) ? size : m_maxSize[axis];
}

//-----------------------------------------------------------------------------
void
Collider::RemoveFromBounds (Word axis, const Thing* pThing)
{
    // set up a pointer to the bounds array we want to work with
    UpperHalf*  bounds = &m_bounds[axis][0];
    
    // get the two bound indices we need to eliminate. this is assuming that 
    // the thing was actually part of the collider, and so some bounds must 
    // be present in the array. since we are removing two bounds, and we
    // know one is higher than the other, we search for the lower one first
    // and then use that as the minimum bound for the higher, giving a slight
    // performance boost to the second search
    Word    lower = FindInSortedArray (bounds, pThing->m_bound[axis][LOWER_BOUND], m_boundsCount);
    Word    upper = FindInSortedArray (bounds, pThing->m_bound[axis][UPPER_BOUND], m_boundsCount, lower + 1);
    //Assert ((lower < size) and (upper < size));
    Assert (bounds[lower].GetThing () == pThing);
    Assert (bounds[upper].GetThing () == pThing);
    
    // check to see if there are any bounds between the two positions
    Word    to = lower;
    Word    from = lower + 1;
    Word    count = upper - from;
    
    // pull the bounds between the two removed bounds down by 1 if needed
    if (count > 0)
        memmove (&bounds[to], &bounds[from], count * sizeof (UpperHalf));
    
    // pull all the remaining bounds down by 2 in the bound array
    to = upper - 1;
    from = upper + 1;
    count = m_boundsCount - from;
    memmove (&bounds[to], &bounds[from], count * sizeof (UpperHalf));
}

//-----------------------------------------------------------------------------
void
Collider::SortBounds (Word axis, Word size)
{
    // The algorithm is to walk the sorted lists looking for items that are out of 
    // order. if one is found, it needs to be moved to its proper position.
    // presumably the proper position is just one step away, so we do this by simply
    // swapping with the next one.  For further information see "gnome sort".
    // NOTE that size is a parameter rather than calculating it here, so that it
    // can be used from places that don't have the same size
    UpperHalf*  bounds = &m_bounds[axis][0];
    Word        last = size - 1;
    Word        skip = 0;
    for (Word i = 0; i < last;)
    {
        // advance the skip counter
        if (i == skip)
            skip = i + 1;
        Half    j = i + 1;
        
        // compare the bounds to see if they are out of order
        if (bounds[i] > bounds[j])
        {
            // Swapping means that a pair is either added to the open list, or removed.
            // We need to understand what to do about a swap, there are 4 cases as follows:
            // 1. i is upper, j is upper:   2 upper bounds are swapping
            // 2. i is lower, j is lower:   2 lower bounds are swapping
            // 3. i is upper, j is lower:   i's upper bound is swapping with j's lower bound
            // 4. i is lower, j is upper:   i's lower bound is swapping with j's upper bound
            // And our responses to these cases are as follows:
            // 1. pair remains in open list, no action
            // 2. pair remains in open list, no action
            // 3. pair is added to open list
            // 4. pair is removed from open list
            // First we check to see if this is case 3 or 4, then which of the two it is
            if (bounds[i].IsUpper () != bounds[j].IsUpper ())
            {
                if (bounds[i].IsUpper ())
                {
                    // case 3
                    // Check to see if the pair is overlapping in the alternative axis too
                    // and do the add if it does
                    Thing*  pThingI = bounds[i].GetThing ();
                    Thing*  pThingJ = bounds[j].GetThing ();
                    Word    altAxis = (axis == X_AXIS) ? Y_AXIS : X_AXIS;
                    if (Bound::OverlapAxis (altAxis, pThingI->m_bound, pThingJ->m_bound))
                        AddToOpen (pThingI, pThingJ);
                }
                else
                {
                    // case 4
                    RemoveFromOpen (bounds[i].GetThing (), bounds[j].GetThing ());
                }
            }
            
            // the bounds are out of order, so we need to swap them
            Half    tmp = bounds[j].m_half;
            bounds[j].m_half = bounds[i].m_half;
            bounds[i].m_half = tmp;
            
            // back up one so we can propagate a change backwards if we have to, but only
            // if this isn't already the first item in the list
            if (i > 0)
                --i;
        }
        else
        {
            // no swap is necessary, because this pair is in order, move to the
            // next unsorted item in the list
            i = skip;
        }
    }
}

//-----------------------------------------------------------------------------
void
Collider::AddToOpen (const Thing* pThingA, const Thing* pThingB)
{
    // if we're out of space in the open array, just punt - this allows the 
    // system to keep running, i.e. fail by degrading appearance
    if (m_openCount < OPEN_SIZE)
    {
        // check the constraint masks on the two objects to see if they should
        // be interacting. This is, for example, two fixed position things
        // that shouldn't have a link
        Word    maskA = pThingA->m_objectType bit_and OT_CONSTRAINT_MASK;
        Word    maskB = pThingB->m_objectType bit_and OT_CONSTRAINT_MASK;
        if (not (maskA bit_and maskB))
        {
            // build the mash, and try to find it
            Mash    mash (pThingA, pThingB);
            Word    index = FindInSortedArray (m_open, mash, m_openCount);
            if (index == m_openCount)
            {
                // the new entry belongs at the end of the array. simply increment 
                // the open count and add the item
                ++m_openCount;
                m_open[index] = mash;
            }
            else if (m_open[index] != mash)
            {
                // the new entry belongs in the middle of the array somewhere, 
                // we have to move the old stuff out of the way, then increment
                // the open count and add the item
                memmove (&m_open[index + 1], &m_open[index], (m_openCount - index) * sizeof (Mash));
                ++m_openCount;
                m_open[index] = mash;
            }
            else
            {
                // the entry is already in the list in the upper place, do nothing
            }
        }
    }
    else
    {
        // silently fail?
        Assert (false);
    }
}

//-----------------------------------------------------------------------------
void
Collider::RemoveFromOpen (const Thing* pThingA, const Thing* pThingB)
{
    // build the mash, and try to find it in the array
    Mash    mash (pThingA, pThingB);
    Word    index = FindInSortedArray (m_open, mash, m_openCount);
    if ((index < m_openCount) and (m_open[index] == mash))
    {
        // decrement the open count and collapse the rest of the array
        --m_openCount;
        memmove (&m_open[index], &m_open[index + 1], (m_openCount - index) * sizeof (Mash));
    }
    else
    {
        // There are three cases that could result in us getting here:
        // 1. If we never added the pair to the open list in the first place, 
        //    such as might happen if it failed the alternate axis bound test 
        //    prior to adding. 
        // 2. We had a crossing in one axis that resulted in removing the link 
        //    already. 
        // 3. We ran out of room in the open list when we were adding this pair
    }
}

//-----------------------------------------------------------------------------
/* void*/
Collider::Collider (void) :
    m_boundsCount (0),
    m_openCount (0)
{
    // initialize the sentinel
    m_sentinel.InitSentinel ();
    
    // clear the open list to zero
    memset (m_open, 0, sizeof (m_open));
    
    // check some of the memory conditions
    Assert (UpperHalf::UPPER_HALF_MAX > Thing::N);
    DebugPrint ("Thing Size: %dB (%d = %dB)\n", sizeof(Thing), Thing::N, sizeof(Thing) * Thing::N);
}

//-----------------------------------------------------------------------------
void
Collider::AddThing (Thing* pThing, float buffer)
{
    Assert (pThing and (not pThing->IsValid ()));

    // first, we need to update the bounds on the thing so that they are
    // correct when we search for their position, then add the bounds to
    // the sorting arrays
    pThing->UpdateBounds (buffer);
    AddToBounds (X_AXIS, pThing, buffer);
    AddToBounds (Y_AXIS, pThing, buffer);
    m_boundsCount += 2;

    // increment the thing count, and add it to the list
    ++m_thingCount;
    pThing->AddToList (m_sentinel.GetPrev ());
}

//-----------------------------------------------------------------------------
void
Collider::RemoveThing (Thing* pThing)
{
    Assert (pThing and pThing->IsValid ());

    // remove all references to the thing from the open list, and adjust the 
    // open count accordingly. Note that the open list is sorted such that 
    // once we hit the target id in IDA of the current entry, there will never
    // be another reference to the target
    // XXX this is a brute force removal, linear on the open list size I
    // XXX wonder if there is a more efficient way
    Word    id = pThing->GetID ();
    Word    stop = id + 1;
    Word    src = 0;
    Word    dst = 0;
    while ((src < m_openCount) and (m_open[src].GetIDA () < stop))
    {
        if ((m_open[src].GetIDA () == id) or (m_open[src].GetIDB () == id))
            ++src;
        else
            m_open[dst++] = m_open[src++];
    }
    while (src < m_openCount)
        m_open[dst++] = m_open[src++];
    m_openCount = dst;
    
    // remove the thing from all the bounds lists
    RemoveFromBounds (X_AXIS, pThing);
    RemoveFromBounds (Y_AXIS, pThing);
    m_boundsCount -= 2;

    // decrement the thing count, and remove it from the list
    --m_thingCount;
    pThing->RemoveFromList ();
}


//-----------------------------------------------------------------------------
Thing*
Collider::FindThing (const Vector2& pt) const
{
    // only do this if there is something to find
    if (m_boundsCount)
    {
        // compute the bounds of the collider
        Word                last = m_boundsCount - 1;
        Bound               bound;
        bound.Set (m_bounds[X_AXIS][0].GetValue (), m_bounds[X_AXIS][last].GetValue (), m_bounds[Y_AXIS][0].GetValue (), m_bounds[Y_AXIS][last].GetValue ());
        
        // choose the axis with the smallest search region based on the fraction of
        // the largest size to the overall bound, and construct the search range 
        // keeping in mind that the widest object in the collider defines how far 
        // we have to look up and down. 
        float               width = bound.GetSize (X_AXIS);
        float               height = bound.GetSize (Y_AXIS);
        Word                axis = ((m_maxSize[X_AXIS] / width) < (m_maxSize[Y_AXIS] / height)) ? X_AXIS : Y_AXIS;
        const UpperHalf*    bounds = &m_bounds[axis][0];
        float               range = m_maxSize[axis];
        float               value = pt[axis];
        
        // find the index in the bound where the pt's value should be
        Word                index = FindInSortedArray (&m_bounds[axis][0], value, m_boundsCount);
        
        // if the pt is completely outside the search range, the returned index
        // will be either 0 or the size of the search array. We use a buffer on
        // the bounds, so a return value of 0 can't actually be inside an object
        if ((index != 0) and (index != m_boundsCount))
        {
        
            // look at the surrounding entries in the bounding list, up to the
            // range, for candidate objects. 
            Word    lo = index;
            while ((lo > 0) and ((value - bounds[lo - 1].GetValue ()) <= range))
                --lo;
            Word    hi = index;
            Word    last = m_boundsCount - 1;
            while ((hi < last) and ((bounds[hi + 1].GetValue () - value) <= range))
                ++hi;
                
            // look at the objects found in the range. If we are inside a thing, its 
            // lower bound will be on one side of the found index, and the upper bound 
            // will be on the other (keeping in mind that the found index might be 
            // one of those bounds), and both will be inside the search range. For 
            // this reason, we can ignore upper bounds, and thus avoid testing the 
            // same object twice
            Word    altAxis = (axis == X_AXIS) ? Y_AXIS : X_AXIS;
            index = lo;
            while (index <= hi)
            {
                // check to see if this is a lower bound
                if (not bounds[index].IsUpper ())
                {
                    // check to see if the test point is inside the bound of this thing
                    Thing*  pThing = bounds[index].GetThing ();
                    if (pThing->m_bound.ContainsPt (pt))
                    {
                        // test the object for actual containment and return it if
                        // the test returns true
                        if (pThing->ContainsPoint (pt))
                            return pThing;
                    }
                }
                
                // move to the next candidate
                ++index;
            }
        }
    }
    
    // no containing object was found
    return 0;
}

//-----------------------------------------------------------------------------
void
Collider::Update (float buffer)
{
    if (m_boundsCount)
    {
        // clear the max size data
        m_maxSize[X_AXIS] = m_maxSize[Y_AXIS] = 0;

        // update all the thing bounds
        Thing*  pThing = m_sentinel.GetNext ();
        Thing*  pEnd = s_cast<Thing*> (&m_sentinel);
        while (pThing != pEnd)
        {
            // things move around, so we have to update their bounds on a
            // regular basis to insure that we are sorting correctly
            // this also serves to pull all the active things into cache
            pThing->UpdateBounds (buffer);
            
            // calculate the size of the bounds on the thing, and compare them
            // to the recorded bounds so we are storing the largest size found
            // these sizes could change with velocity, for instance
            float   width = pThing->m_bound.GetSize (X_AXIS);
            float   height = pThing->m_bound.GetSize (Y_AXIS);
            m_maxSize[X_AXIS] = (width > m_maxSize[X_AXIS]) ? width : m_maxSize[X_AXIS];
            m_maxSize[Y_AXIS] = (height > m_maxSize[Y_AXIS]) ? height : m_maxSize[Y_AXIS];
            
            // advance to the next thing in the list
            pThing = pThing->GetNext ();
        }

        // sort the bounds lists
        SortBounds (X_AXIS, m_boundsCount);
        SortBounds (Y_AXIS, m_boundsCount);
    }
}

//-----------------------------------------------------------------------------
bool
Collider::ValidateOpen (void) const
{
    if (m_boundsCount > 1)
    {
        //DebugPrint ("* Validation Started\n");
        // loop over all of the objects possibly colliding (n^2), and if we find
        // a collision, validate that it's in the open list
        const Thing*    pThing = m_sentinel.GetNext ();
        const Thing*    pEnd = r_cast<const Thing*> (&m_sentinel);
        while (pThing != pEnd)
        {
            // loop from here on
            const Thing*    pNext = pThing->GetNext ();
            while (pNext != pEnd)
            {
                //DebugPrint ("Compare %d against %d\n", pThing->GetID (), pNext->GetID ());
                if (Bound::Overlap (pThing->m_bound, pNext->m_bound))
                {
                    // check the constraint masks on the two objects to see if they should
                    // be interacting. This is, for example, two fixed position things
                    // that shouldn't have a link
                    Word    mask = pThing->m_objectType bit_and OT_CONSTRAINT_MASK;
                    Word    maskNext = pNext->m_objectType bit_and OT_CONSTRAINT_MASK;
                    if (not (mask bit_and maskNext))
                    {
                        Mash    mash (pThing, pNext);
                        Word    index = FindInSortedArray (m_open, mash, m_openCount);
                        if ((index == m_openCount) or (m_open[index] != mash))
                        {
                            DebugPrint ("ERROR: Missing Collision Pair (%03d, %03d)\n", pThing->GetID (), pNext->GetID ());
                            Assert (false);
                            return false;
                        }
                    }
                }
                pNext = pNext->GetNext ();
            };
            pThing = pThing->GetNext ();
        }
        
        // loop over all the entries in the open list and validate that all of 
        // them are potential intersections
        for (Word i = 0; i < m_openCount; ++i)
        {
            Thing*  pThingA = m_open[i].GetThingA ();
            Thing*  pThingB = m_open[i].GetThingB ();
            if (not Bound::Overlap (pThingA->m_bound, pThingB->m_bound))
            {
                DebugPrint ("ERROR: Erroneous Collision Pair (%03d, %03d)\n", pThingA->GetID (), pThingB->GetID ());
                Assert (false);
                return false;
            }
        }
    }
    DebugPrint ("* Validation Succeeded\n");
    return true;
}

//-----------------------------------------------------------------------------
