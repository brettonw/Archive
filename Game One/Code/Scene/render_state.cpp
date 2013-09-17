//-----------------------------------------------------------------------------
// Copyright (C) 1997-2006 Bretton Wade
// All Rights Reserved
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// include files
//-----------------------------------------------------------------------------
#include    "precompile.h"
#include    "render_state.h"
#include    "direct_3d.h"

//-----------------------------------------------------------------------------
// static class members
//-----------------------------------------------------------------------------
DEFINE_FACTORY (RenderState);
D3DXMATRIX	    RenderState::s_projectionTransform;
D3DXMATRIX	    RenderState::s_viewTransform;

//-----------------------------------------------------------------------------
// class methods
//-----------------------------------------------------------------------------
void
RenderState::Render (const Direct3D& pDirect3D, const RenderPass& renderPass) const
{
    // save the current effect so we can restore it
    PtrToEffect     pPreviousEffect = Effect::GetCurrent ();

    // set the effect to the effect we want
    PtrToEffect pEffect = Effect::Find (m_effectName);
    Assert (pEffect);
    Effect::Select (pEffect);
    PtrToD3DXEffect pCurrentEffect = pEffect->GetD3DXEffect ();
    
    // set  the effect parameters for lighting
    D3D_REQUIRE (pCurrentEffect->SetValue (m_lightDirParameterHandle, renderPass.m_lightDirection, sizeof (renderPass.m_lightDirection)));
    D3D_REQUIRE (pCurrentEffect->SetValue (m_lightColorParameterHandle, renderPass.m_lightColor, sizeof (renderPass.m_lightColor)));

    // set the effect parameters for material and texture
    if (m_diffuseTexture)
        D3D_REQUIRE (pCurrentEffect->SetTexture (m_textureParameterHandle, m_diffuseTexture));
    D3D_REQUIRE (pCurrentEffect->SetValue (m_colorParameterHandle, m_color, sizeof (m_color)));

    // set the effect parameters for view transform matrices
    D3D_REQUIRE (pCurrentEffect->SetMatrix (m_projectionTransformParameterHandle, &s_projectionTransform));
    D3D_REQUIRE (pCurrentEffect->SetMatrix (m_viewTransformParameterHandle, &s_viewTransform));

    // when rendering with the effect state, note that the effect framework 
    // saves all the current graphics state and sets only those values specified 
    // in the effect so a "parent" effect can be employed for inherited 
    // behaviors. A side effect of the begin call is that we figure out how 
    // many passes are in the effect, but we don't use that
    UINT            numPasses;
    D3D_REQUIRE (pCurrentEffect->Begin (&numPasses, 0));
    D3D_REQUIRE (pCurrentEffect->BeginPass (renderPass.m_iPass));

    // do the parental thang
    SceneGraphNode::Render (pDirect3D, renderPass);
    
    // end the current pass and the effect. This restores the state that was
    // saved in the call to Begin
    D3D_REQUIRE (pCurrentEffect->EndPass ());
    D3D_REQUIRE (pCurrentEffect->End ());
    
    // restore the previous effect
    Effect::Select (pPreviousEffect);
}

//-----------------------------------------------------------------------------
/* void */
RenderState::RenderState (const PtrToXMLNode& pXMLNode) : 
    SceneGraphNode (pXMLNode)
{
    // fetch the direct 3d object
    Direct3D&           pDirect3D = *(Direct3D::GetSingleton ());
     
    // read the texture name and create a texture
    Text                textureName = pXMLNode->GetAttribute ("texture");
    if (textureName.Length () > 0)
    {
        textureName = "Texture\\" + textureName;
        D3D_REQUIRE (D3DXCreateTextureFromFile (*pDirect3D, textureName, &m_diffuseTexture));
    }
    
    // read the effect name and load the effect
    m_effectName = pXMLNode->GetAttribute ("effect");
    PtrToEffect         pEffect = Effect::Find (m_effectName);
    PtrToD3DXEffect     pD3DXEffect = pEffect->GetD3DXEffect ();
    
    // get the diffuse color for the material
    m_color.w = 1.0f;
    sscanf (pXMLNode->GetAttribute ("rgb"), "(%f, %f, %f)", &m_color.x, &m_color.y, &m_color.z);

    // cache the parameter handles so we can efficiently set the dynamic effect
    // parameters during rendering, these are going to be standard in all of our .fx files
    m_lightDirParameterHandle = pD3DXEffect->GetParameterByName (0, "g_lightDir");
    Assert (m_lightDirParameterHandle);
    m_lightColorParameterHandle = pD3DXEffect->GetParameterByName (0, "g_lightColor");
    Assert (m_lightColorParameterHandle);
    m_colorParameterHandle = pD3DXEffect->GetParameterByName (0, "g_color");
    Assert (m_colorParameterHandle);
    m_textureParameterHandle = pD3DXEffect->GetParameterByName (0, "g_diffuseTexture");
    Assert (m_textureParameterHandle);
    m_projectionTransformParameterHandle = pD3DXEffect->GetParameterByName (0, "g_projectionTransform");
    Assert (m_projectionTransformParameterHandle);
    m_viewTransformParameterHandle = pD3DXEffect->GetParameterByName (0, "g_viewTransform");
    Assert (m_viewTransformParameterHandle);
}

//-----------------------------------------------------------------------------
/* void */
RenderState::~RenderState (void)
{
}

//-----------------------------------------------------------------------------
