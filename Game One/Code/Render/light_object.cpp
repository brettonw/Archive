//-----------------------------------------------------------------------------
// Copyright (C) 1997-2006 Bretton Wade
// All Rights Reserved
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// include files
//-----------------------------------------------------------------------------
#include    "precompile.h"
#include    "light_object.h"

//-----------------------------------------------------------------------------
// static class members
//-----------------------------------------------------------------------------
DEFINE_FACTORY (LightObject);

//-----------------------------------------------------------------------------
// class methods
//-----------------------------------------------------------------------------
/* void */
LightObject::LightObject (const PtrToXMLNode& pXMLNode) :
    IndexedObject<LightObject> (pXMLNode)
{
    // the XML looks like this
    //<LightObject name="lightA" light="AMBIENT"     rgb="(0.2, 0.15, 0.4)"/>
    //<LightObject name="lightB" light="DIRECTIONAL" rgb="(1.0, 1.0, 0.8)" direction="(-5.0, -4.0, -1.0)"/>
    //<LightObject name="lightC" light="POINT"       rgb="(1.0, 1.0, 0.8)" position="(-5.0, -4.0, -1.0)"/>
    //<LightObject name="lightD" light="SPOT"        rgb="(1.0, 1.0, 0.8)" position="(-5.0, -4.0, -1.0)" direction="(-5.0, -4.0, -1.0)"/>

    // get the light color, position, and direction
    m_color.w = 1.0f;
    sscanf (pXMLNode->GetAttribute ("rgb"), "(%f, %f, %f)", &m_color.x, &m_color.y, &m_color.z);
    sscanf (pXMLNode->GetAttribute ("position"), "(%f, %f, %f)", &m_position.x, &m_position.y, &m_position.z);
    sscanf (pXMLNode->GetAttribute ("direction"), "(%f, %f, %f)", &m_direction.x, &m_direction.y, &m_direction.z);
    
    // normalize the light direction
    D3DXVec3Normalize (&m_direction, &m_direction);

    // get the light type
    Text    light = pXMLNode->GetAttribute ("light");
    if (light == "AMBIENT")
        m_type = static_cast<D3DLIGHTTYPE> (0);
    else if (light == "DIRECTIONAL")
        m_type = D3DLIGHT_DIRECTIONAL;
    else if (light == "POINT")
        m_type = D3DLIGHT_POINT;
    else if (light == "SPOT")
        m_type = D3DLIGHT_SPOT;
}

//-----------------------------------------------------------------------------
/* void */
LightObject::~LightObject (void)
{
}

//-----------------------------------------------------------------------------
