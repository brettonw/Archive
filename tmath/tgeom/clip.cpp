//-----------------------------------------------------------------------------
// Copyright (C) 2002-2005 Bretton Wade, All rights reserved
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// includes
//-----------------------------------------------------------------------------
#include    "tgeom.h"
#include    "clip.h"

//-----------------------------------------------------------------------------
// intersection case table - the line intersection code will test three points
// against the intersecting line, and use those three values to build a code
// that is looked up to find the sequence of operations that should be done
// to handle that case. Here, we define the table, and a macro to support 
// construction of the table
//-----------------------------------------------------------------------------
stdext::hash_map<int, int>  g_IntersectionCode;

//-----------------------------------------------------------------------------
#define SET_INTERSECTION_CODE(last, current, next, code)                        \
    g_IntersectionCode[MAKE_INTERSECTION_CODE (last, current, next)] = code;
    
//-----------------------------------------------------------------------------
// static table builder - the following class uses a static member to force
// initialization of the intersection code table at static startup. The table
// enumerates all the possible code combinations (3^3=27) and the action for
// each code.
//-----------------------------------------------------------------------------
class   IntersectionCodeBuilder
{
    IntersectionCodeBuilder (void)
    {
        sInt    LEFT = TURNS_LEFT;
        sInt    RIGHT = TURNS_RIGHT;
        sInt    CO = COLINEAR;
        
        //                     LAST     CURRENT   NEXT      CODE
        SET_INTERSECTION_CODE (RIGHT,   RIGHT,    RIGHT,    DO_NOTHING);
        SET_INTERSECTION_CODE (LEFT,    RIGHT,    RIGHT,    DO_NOTHING);
        SET_INTERSECTION_CODE (CO,      RIGHT,    RIGHT,    DO_NOTHING);
        SET_INTERSECTION_CODE (RIGHT,   RIGHT,    LEFT,     NEW_CHAIN bit_or POST_SECT);
        SET_INTERSECTION_CODE (LEFT,    RIGHT,    LEFT,     NEW_CHAIN bit_or POST_SECT);
        SET_INTERSECTION_CODE (CO,      RIGHT,    LEFT,     NEW_CHAIN bit_or POST_SECT);
        SET_INTERSECTION_CODE (RIGHT,   RIGHT,    CO,       DO_NOTHING);
        SET_INTERSECTION_CODE (LEFT,    RIGHT,    CO,       DO_NOTHING);
        SET_INTERSECTION_CODE (CO,      RIGHT,    CO,       DO_NOTHING);

        SET_INTERSECTION_CODE (RIGHT,   LEFT,     RIGHT,    PRE_EMIT bit_or PRE_SECT);
        SET_INTERSECTION_CODE (LEFT,    LEFT,     RIGHT,    PRE_EMIT bit_or PRE_SECT);
        SET_INTERSECTION_CODE (CO,      LEFT,     RIGHT,    PRE_EMIT bit_or PRE_SECT);
        SET_INTERSECTION_CODE (RIGHT,   LEFT,     LEFT,     PRE_EMIT);
        SET_INTERSECTION_CODE (LEFT,    LEFT,     LEFT,     PRE_EMIT);
        SET_INTERSECTION_CODE (CO,      LEFT,     LEFT,     PRE_EMIT);
        SET_INTERSECTION_CODE (RIGHT,   LEFT,     CO,       PRE_EMIT);
        SET_INTERSECTION_CODE (LEFT,    LEFT,     CO,       PRE_EMIT);
        SET_INTERSECTION_CODE (CO,      LEFT,     CO,       PRE_EMIT);

        SET_INTERSECTION_CODE (RIGHT,   CO,       RIGHT,    DO_NOTHING);
        SET_INTERSECTION_CODE (LEFT,    CO,       RIGHT,    PRE_EMIT);
        SET_INTERSECTION_CODE (CO,      CO,       RIGHT,    DO_NOTHING);
        SET_INTERSECTION_CODE (RIGHT,   CO,       LEFT,     NEW_CHAIN bit_or POST_EMIT);
        SET_INTERSECTION_CODE (LEFT,    CO,       LEFT,     SPECIAL_CASE);
        SET_INTERSECTION_CODE (CO,      CO,       LEFT,     NEW_CHAIN bit_or POST_EMIT);
        SET_INTERSECTION_CODE (RIGHT,   CO,       CO,       DO_NOTHING);
        SET_INTERSECTION_CODE (LEFT,    CO,       CO,       PRE_EMIT);
        SET_INTERSECTION_CODE (CO,      CO,       CO,       DO_NOTHING);
    }
static  IntersectionCodeBuilder s_intersectionCodebuilder;
};
IntersectionCodeBuilder IntersectionCodeBuilder::s_intersectionCodebuilder;

//-----------------------------------------------------------------------------
// STL helpers - instantiation
//-----------------------------------------------------------------------------
template bool EndPointCmp<double> (EndPoint<double>& lhs, EndPoint<double>& rhs);
template bool EndPointCmp<float> (EndPoint<float>& lhs, EndPoint<float>& rhs);

//-----------------------------------------------------------------------------
