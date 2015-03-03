//-----------------------------------------------------------------------------
// Copyright (C) 2006 Bretton Wade, All Rights Reserved
//-----------------------------------------------------------------------------

#ifndef     _THING_H_
#define     _THING_H_

//-----------------------------------------------------------------------------
// includes
//-----------------------------------------------------------------------------
#ifndef     _OBJECT_TYPE_H_
#include    "object_type.h"
#endif  //  _OBJECT_TYPE_H_

#ifndef     _LIST_NODE_H_
#include    "list_node.h"
#endif  //  _LIST_NODE_H_

#ifndef     _BOUND_H_
#include    "bound.h"
#endif  //  _BOUND_H_

//-----------------------------------------------------------------------------
// classes
//-----------------------------------------------------------------------------
class Thing : public ListNode<Thing>
{
    public:
        enum
        {
            // this is the maximum number of Things allowed. they will be
            // created all at once on the first use. make sure UpperHalf 
            // m_id has enough resolution to index all N Things if you
            // change this value
            N =         4000
        };
        
    public:
        Vector2     m_center;
        Bound           m_bound;
        union
        {
            float       m_radius;
            struct
            {
                float   m_width;
                float   m_height;
                Half    m_axis;
            };
        };
        Half            m_objectType;
        
    public:    
        // constructors and destructor
        /* void */      Thing (void) 
                            {}
        /* void */      Thing (const Vector2& center, float radius, short type = OT_FREE) : 
                            m_center (center),
                            m_radius (radius),
                            m_objectType (type bit_or OT_CIRCLE)
                            {}
        /* void */      Thing (const Vector2& center, float width, float height, Half axis, short type = OT_FREE) : 
                            m_center (center),
                            m_width (width),
                            m_height (height),
                            m_axis (axis),
                            m_objectType (type bit_or OT_BOX)
                            {}
        /* void */      ~Thing (void)
                            {}
                            
        // general utility methods
        void            UpdateBounds (float buffer = 0);  // XXX eventually include dt as a parm
        bool            ContainsPoint (const Vector2& pt) const;
        
        // ID and conversion routines
        bool            IsValid (void) const;
        Word            GetID (void) const;
static  Thing*          GetThing (Word id);

        // new and delete operators
        void*           operator new (size_t size);
        void            operator delete (void* ptr);

    private:
static  Thing*          s_pFreeList;
static  Thing*          s_pThingBase;
};

//-----------------------------------------------------------------------------
// inlines
//-----------------------------------------------------------------------------
#ifndef     _THING_INL_
#include    "thing.inl"
#endif  //  _THING_INL_

//-----------------------------------------------------------------------------

#endif  //  _THING_H_
