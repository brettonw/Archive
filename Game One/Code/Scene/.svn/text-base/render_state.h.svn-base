//-----------------------------------------------------------------------------
// Copyright (C) 1997-2006 Bretton Wade
// All Rights Reserved
//-----------------------------------------------------------------------------

#ifndef     _RENDER_STATE_H_
#define     _RENDER_STATE_H_

//-----------------------------------------------------------------------------
// include files
//-----------------------------------------------------------------------------
#ifndef     _SCENE_GRAPH_NODE_H_
#include    "scene_graph_node.h"
#endif  //  _SCENE_GRAPH_NODE_H_

//-----------------------------------------------------------------------------
// include files
//-----------------------------------------------------------------------------
#ifndef     _EFFECT_H_
#include    "effect.h"
#endif  //  _EFFECT_H_

//-----------------------------------------------------------------------------
// class definitions
//-----------------------------------------------------------------------------
class RenderState : public SceneGraphNode
{
    public:
virtual void                    Render (const Direct3D& pDirect3D, const RenderPass& renderPass) const;
static  void                    SetProjectionTransform (const D3DXMATRIX* pTransform) { s_projectionTransform = *pTransform; }
static  void                    SetViewingTransform (const D3DXMATRIX* pTransform) { s_viewTransform = *pTransform; }
    
    protected:
        Text                    m_effectName;
        PtrToD3DTexture         m_diffuseTexture;
        D3DXVECTOR4			    m_color;
        
        D3DXHANDLE              m_lightDirParameterHandle;
        D3DXHANDLE              m_lightColorParameterHandle;
        D3DXHANDLE              m_colorParameterHandle;
        D3DXHANDLE              m_textureParameterHandle;
        D3DXHANDLE              m_projectionTransformParameterHandle;
        D3DXHANDLE              m_viewTransformParameterHandle;
        
static  D3DXMATRIX	            s_projectionTransform;
static  D3DXMATRIX	            s_viewTransform;

    DECLARE_FACTORY (SceneObject, RenderState);
};

//-----------------------------------------------------------------------------

#endif  //  _RENDER_STATE_H_
