//-----------------------------------------------------------------------------
// Copyright (C) 1997-2006 Bretton Wade
// All rights reserved.
//-----------------------------------------------------------------------------

#ifndef     _COUNTED_OBJECT_H_
#define     _COUNTED_OBJECT_H_

class    CountedObject {
    public:
    CountedObject (void);
    CountedObject (const CountedObject& object);
    virtual ~CountedObject (void);

    void addRef (void);
    uint removeRef (void);
    uint getCount (void) const;

    private:
    uint count;

#ifndef NDEBUG
    uint id;
    static  uint s_id;
#endif
};

#ifndef     _COUNTED_OBJECT_INL_
#include    "counted_object.inl"
#endif  //  _COUNTED_OBJECT_INL_

#endif  //  _COUNTED_OBJECT_H_
