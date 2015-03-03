//-----------------------------------------------------------------------------
// Copyright (C) 2006 Bretton Wade, All Rights Reserved
//-----------------------------------------------------------------------------

#ifndef     _COLLIDER_H_
#define     _COLLIDER_H_

//-----------------------------------------------------------------------------
// includes
//-----------------------------------------------------------------------------
#ifndef     _MASH_H_
#include    "mash.h"
#endif  //  _MASH_H_

#ifndef     _UPPER_HALF_H_
#include    "upper_half.h"
#endif  //  _UPPER_HALF_H_

//-----------------------------------------------------------------------------
// classes
//-----------------------------------------------------------------------------
class Collider 
{
    public: //  XXX should be private
        float               m_maxSize[2];
        Word                m_boundsCount;
        UpperHalf           m_bounds[2][Thing::N * 2];

    private:
        // methods for dealing with the bounds
        void                AddToBounds (Word axis, const Thing* pThing, float buffer);
        void                RemoveFromBounds (Word axis, const Thing* pThing);
        void                SortBounds (Word axis, Word size);
        
        // methods for dealing with the "open" list
        void                AddToOpen (const Thing* pA, const Thing* pB);
        void                RemoveFromOpen (const Thing* pA, const Thing* pB);

    public:
        enum
        {
            // open list size, fit the collider in 128KB please
            TARGET_MEM  = USHRT_MAX * 2,
            OPEN_SIZE   = (TARGET_MEM - (sizeof (Word) * 2) - sizeof (ListNode<Thing>) - (sizeof (float) * 2) - (sizeof (UpperHalf) * 4 * Thing::N)) / sizeof (Mash)
        };
        
    public:
        Word                m_thingCount;
        ListNode<Thing>     m_sentinel;
        
        Word                m_openCount;
        Mash                m_open[OPEN_SIZE];
        
    public:
        /* void*/           Collider (void);
        
        // methods for dealing with things
        void                AddThing (Thing* pThing, float buffer = 0);
        void                RemoveThing (Thing* pThing);
        Word                GetThingcount (void) const;
        Thing*              FindThing (const Vector2& pt) const;

        // update the collider state
        void                Update (float buffer = 0);
        
        // debugging aid
        bool                ValidateOpen (void) const;
};

//-----------------------------------------------------------------------------

#endif  //  _COLLIDER_H_
