//-----------------------------------------------------------------------------
// Copyright (C) 2006 Bretton Wade and Firaxis Games, All Rights Reserved
//-----------------------------------------------------------------------------

#ifndef     _THING_H_
#define     _THING_H_

//-----------------------------------------------------------------------------
// includes
//-----------------------------------------------------------------------------
#ifndef     _CONSTANTS_H_
#include    "constants.h"
#endif  //  _CONSTANTS_H_

#ifndef     _OBJECT_TYPE_H_
#include    "object_type.h"
#endif  //  _OBJECT_TYPE_H_

//-----------------------------------------------------------------------------
// classes
//-----------------------------------------------------------------------------
class Thing 
{
    private:
static  short           s_objectID;

    public:
        D3DXVECTOR2     pt;
        short           object_id;
        short           object_type;
        union
        {
            float               radius;
            struct
            {
                float           width;
                float           height;
                D3DXVECTOR2     xaxis;
            };
        };
        
    public:    
        /* void */      Thing (const D3DXVECTOR2& _pt, float _rad, short _type = OT_FREE) : 
                            pt (_pt), 
                            radius (_rad), 
                            object_id (s_objectID++), 
                            object_type (_type bit_or OT_CIRCLE) {}
        /* void */      Thing (const D3DXVECTOR2& _pt, const D3DXVECTOR2& _axs, float _wid, float _hgt, short _type = OT_FREE) : 
                            pt (_pt), 
                            xaxis (_axs), 
                            width (_wid),
                            height (_hgt),
                            object_id (s_objectID++), 
                            object_type (_type bit_or OT_BOX) {}
};

//-----------------------------------------------------------------------------
// globals
//-----------------------------------------------------------------------------
extern vector<Thing>    g_thing_list;
extern int              g_thing_bar;

//-----------------------------------------------------------------------------
// functions
//-----------------------------------------------------------------------------
int     PointInList (const D3DXVECTOR2& vec, float radius = c_fMinWidth);

//-----------------------------------------------------------------------------

#endif  //  _THING_H_
