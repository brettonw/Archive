//-----------------------------------------------------------------------------
// Copyright (C) 2006 Bretton Wade, All Rights Reserved
//-----------------------------------------------------------------------------

#ifndef     _UPPER_HALF_H_
#define     _UPPER_HALF_H_

//-----------------------------------------------------------------------------
// includes
//-----------------------------------------------------------------------------
#ifndef     _THING_H_
#include    "thing.h"
#endif  //  _THING_H_

//-----------------------------------------------------------------------------
// classes
//-----------------------------------------------------------------------------
class UpperHalf
{
    public:
        enum
        {
            // this is the number of bits allocated to tracking Things in the
            // bound arrays used by the collider. If you change Thing::N, you
            // should ensure that UPPER_HALF_MAX > Thing::N
            BITS_ISUPPER =      1,
            BITS_AXIS =         1,
            BITS_ID =           16 - (BITS_AXIS + BITS_ISUPPER),
            UPPER_HALF_MAX =    (1 << BITS_ID) - 1
        };
        
    public:
        union
        {
            Half        m_half;
            struct
            {
                Half    m_ID        : BITS_ID;
                Half    m_axis      : BITS_AXIS;
                Half    m_isUpper   : BITS_ISUPPER;
            };
        };
        
    public:    
        // constructors and destructor
        /* void */      UpperHalf (void) {}
        /* void */      UpperHalf (const Thing* pThing, Word axis, bool bIsUpper);
        /* void */      UpperHalf (const UpperHalf& half);
        /* void */      ~UpperHalf (void) {}
                            
        // core accessors
        Word            GetID (void) const;
        Thing*          GetThing (void) const;
        Word            GetAxis (void) const;
        bool            IsUpper (void) const;
        
        // bound accessors
        float           GetValue (void) const;

        // operators
        UpperHalf&      operator = (const UpperHalf& half);
        bool            operator > (const UpperHalf& half) const;
};

//-----------------------------------------------------------------------------
// inlines
//-----------------------------------------------------------------------------
#ifndef     _UPPER_HALF_INL_
#include    "upper_half.inl"
#endif  //  _UPPER_HALF_INL_

//-----------------------------------------------------------------------------

#endif  //  _UPPER_HALF_H_
