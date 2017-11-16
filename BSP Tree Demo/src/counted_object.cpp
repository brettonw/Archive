//-----------------------------------------------------------------------------
// Copyright (C) 1997-2006 Bretton Wade
// All rights reserved.
//-----------------------------------------------------------------------------

#include    "precompile.h"
#include    "counted_object.h"

#ifndef NDEBUG
uint   CountedObject::s_id = 0;
#endif

CountedObject::~CountedObject (void) {
    //Assert (m_count == 0);
}

