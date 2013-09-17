//-----------------------------------------------------------------------------
// Copyright (C) 1997-2006 Bretton Wade
// All Rights Reserved
//-----------------------------------------------------------------------------

#ifndef     _RENDER_PASS_H_
#define     _RENDER_PASS_H_

//-----------------------------------------------------------------------------
// enums
//-----------------------------------------------------------------------------
enum
{
    AMBIENT_PASS            = 0,
    SHADOW_STENCIL_PASS		= 1,
    LIGHTING_PASS           = 2
};

//-----------------------------------------------------------------------------
// structures
//-----------------------------------------------------------------------------
struct RenderPass
{
    public:
        uInt            m_iPass;
        D3DLIGHTTYPE    m_lightType;
        D3DXVECTOR3     m_lightPosition;
        D3DXVECTOR3     m_lightDirection;
        D3DXVECTOR4     m_lightColor;
        
    public:
        /* void */      RenderPass (uInt iPass, D3DLIGHTTYPE lightType, const D3DXVECTOR3& lightPos, const D3DXVECTOR3& lightDir, const D3DXVECTOR4& lightCol) :
                            m_iPass (iPass), m_lightType (lightType), m_lightPosition (lightPos), m_lightDirection (lightDir), m_lightColor (lightCol) {}

mutable D3DXVECTOR3     m_cachePosition;
mutable D3DXVECTOR3     m_cacheDirection;
};

//-----------------------------------------------------------------------------

#endif  //  _RENDER_PASS_H_
