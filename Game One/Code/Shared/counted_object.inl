//-----------------------------------------------------------------------------
// Copyright (C) 1997-2006 Bretton Wade
// All rights reserved.
//-----------------------------------------------------------------------------

#ifndef     _COUNTED_OBJECT_INL_
#define     _COUNTED_OBJECT_INL_

//-----------------------------------------------------------------------------
// include files
//-----------------------------------------------------------------------------
#ifndef     _COUNTED_OBJECT_H_
#include    "counted_object.h"
#endif  //  _COUNTED_OBJECT_H_

//-----------------------------------------------------------------------------
// class methods
//-----------------------------------------------------------------------------
inline
/* void */
CountedObject::CountedObject (void)
{
    m_count = 0;
#ifndef NDEBUG
    m_ID = ++s_ID;
#endif
}

//-----------------------------------------------------------------------------
inline
/* void */
CountedObject::CountedObject (const CountedObject& /* object */)
{
    m_count = 0;
}

//-----------------------------------------------------------------------------
inline
void
CountedObject::AddRef (void)
{
    m_count++;
}

//-----------------------------------------------------------------------------
inline
uInt4
CountedObject::RemoveRef (void)
{
    return --m_count;
}

//-----------------------------------------------------------------------------
inline
uInt4
CountedObject::GetCount (void) const
{
    return m_count;
}

//-----------------------------------------------------------------------------

#endif  //  _COUNTED_OBJECT_INL_
