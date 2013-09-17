//-----------------------------------------------------------------------------
// Copyright (C) 1997-2006 Bretton Wade
// All Rights Reserved
//-----------------------------------------------------------------------------

#ifndef     _GEOMETRY_CUBE_H_
#define     _GEOMETRY_CUBE_H_

//-----------------------------------------------------------------------------
// include files
//-----------------------------------------------------------------------------
#ifndef     _GEOMETRY_OBJECT_INDEXED_H_
#include    "geometry_object_indexed.h"
#endif  //  _GEOMETRY_OBJECT_INDEXED_H_

//-----------------------------------------------------------------------------
// class definitions
//-----------------------------------------------------------------------------
class Cube : public GeometryObjectIndexed
{
    DECLARE_FACTORY (GeometryObject, Cube);
};

//-----------------------------------------------------------------------------

#endif  //  _GEOMETRY_CUBE_H_
