//-----------------------------------------------------------------------------
// Copyright (C) 1997-2006 Bretton Wade
// All Rights Reserved
//-----------------------------------------------------------------------------

#ifndef     _LIGHT_OBJECT_H_
#define     _LIGHT_OBJECT_H_

//-----------------------------------------------------------------------------
// include files
//-----------------------------------------------------------------------------
#ifndef     _INDEXED_OBJECT_H_
#include    "indexed_object.h"
#endif  //  _INDEXED_OBJECT_H_

//-----------------------------------------------------------------------------
// useful type definitions
//-----------------------------------------------------------------------------
MAKE_PTR_TO (LightObject);

//-----------------------------------------------------------------------------
// class definitions
//-----------------------------------------------------------------------------
class LightObject : public IndexedObject<LightObject>
{
    public:
        D3DLIGHTTYPE        m_type;
        D3DXVECTOR3         m_direction;
        D3DXVECTOR3         m_position;
		D3DXVECTOR4			m_color;
        
    public:
    DECLARE_FACTORY (LightObject, LightObject);
};

//-----------------------------------------------------------------------------

#endif  //  _LIGHT_OBJECT_H_
