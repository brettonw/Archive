//-----------------------------------------------------------------------------
// Copyright (C) 2006 Bretton Wade, All Rights Reserved
//-----------------------------------------------------------------------------

#ifndef     _MASH_H_
#define     _MASH_H_

//-----------------------------------------------------------------------------
// includes
//-----------------------------------------------------------------------------
#ifndef     _THING_H_
#include    "thing.h"
#endif  //  _THING_H_

//-----------------------------------------------------------------------------
// classes
//-----------------------------------------------------------------------------
class Mash
{
    private:
        union
        {
            Word        m_word;
            struct
            {
                Half    m_IDB;
                Half    m_IDA;
            };
        };
        
    private:
        void            Assign (Word IDA, Word IDB);
        
    public:    
        // constructors and destructor
        /* void */      Mash (void) {}
        /* void */      Mash (Word IDA, Word IDB);
        /* void */      Mash (const Thing* pThingA, const Thing* pThingB);
        /* void */      Mash (const Mash& mash);
        /* void */      ~Mash (void) {}
                            
        // core accessors
        Half            GetIDA (void) const;
        Half            GetIDB (void) const;
        Thing*          GetThingA (void) const;
        Thing*          GetThingB (void) const;
        
        // assignment operator
        Mash&           operator = (const Mash& mash);
        
        // comparison operators
        bool            operator < (const Mash& mash) const;
        bool            operator == (const Mash& mash) const;
        bool            operator != (const Mash& mash) const;
};

//-----------------------------------------------------------------------------
// inlines
//-----------------------------------------------------------------------------
#ifndef     _MASH_INL_
#include    "mash.inl"
#endif  //  _MASH_INL_

//-----------------------------------------------------------------------------

#endif  //  _MASH_H_
