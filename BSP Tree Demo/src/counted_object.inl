//-----------------------------------------------------------------------------
// Copyright (C) 1997-2006 Bretton Wade
// All rights reserved.
//-----------------------------------------------------------------------------

#ifndef     _COUNTED_OBJECT_INL_
#define     _COUNTED_OBJECT_INL_

#ifndef     _COUNTED_OBJECT_H_
#include    "counted_object.h"
#endif  //  _COUNTED_OBJECT_H_

inline
CountedObject::CountedObject (void) {
    count = 0;
#ifndef NDEBUG
    id = ++s_id;
#endif
}

inline
CountedObject::CountedObject (const CountedObject& /* object */) {
    count = 0;
}

inline
void CountedObject::addRef (void) {
    count++;
}

inline
uint CountedObject::removeRef (void) {
    return --count;
}

inline
uint CountedObject::getCount (void) const {
    return count;
}

#endif  //  _COUNTED_OBJECT_INL_
