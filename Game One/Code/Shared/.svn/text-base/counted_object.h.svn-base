//-----------------------------------------------------------------------------
// Copyright (C) 1997-2006 Bretton Wade
// All rights reserved.
//-----------------------------------------------------------------------------

#ifndef     _COUNTED_OBJECT_H_
#define     _COUNTED_OBJECT_H_

//-----------------------------------------------------------------------------
// class definitions
//-----------------------------------------------------------------------------
class    CountedObject
{
    public:
        /* void */      CountedObject (void);
        /* void */      CountedObject (const CountedObject& object);
virtual /* void */      ~CountedObject (void);

        void            AddRef (void);
        uInt4           RemoveRef (void);
        uInt4           GetCount (void) const;
        
    private:
        uInt4           m_count;
        
#ifndef NDEBUG
        uInt4           m_ID;
static  uInt4           s_ID;
#endif
};

//-----------------------------------------------------------------------------
// inlines
//-----------------------------------------------------------------------------
#ifndef     _COUNTED_OBJECT_INL_
#include    "counted_object.inl"
#endif  //  _COUNTED_OBJECT_INL_

//-----------------------------------------------------------------------------

#endif  //  _COUNTED_OBJECT_H_
