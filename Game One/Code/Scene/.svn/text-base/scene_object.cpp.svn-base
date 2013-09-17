//-----------------------------------------------------------------------------
// Copyright (C) 1997-2006 Bretton Wade
// All Rights Reserved
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// include files
//-----------------------------------------------------------------------------
#include    "precompile.h"
#include    "scene_object.h"
#include    "effect.h"
#include    "transform_xml.h"

//-----------------------------------------------------------------------------
// class methods
//-----------------------------------------------------------------------------
void
SceneObject::Prologue (const D3DXMATRIX& transform)
{
    D3DXMatrixMultiply (&m_cachedTransform, &m_transform, &transform);
}

//-----------------------------------------------------------------------------
void
SceneObject::Render (const Direct3D& pDirect3D, const RenderPass& renderPass) const
{
    // set the world transform
    PtrToD3DXEffect pCurrentEffect = Effect::GetCurrent ()->GetD3DXEffect ();
    D3D_TEST (pCurrentEffect->SetMatrix ("g_worldTransform", &m_cachedTransform));
    D3D_TEST (pCurrentEffect->CommitChanges ());
}

//-----------------------------------------------------------------------------
const D3DXMATRIX&
SceneObject::GetTransform (void) const
{
    return m_transform;
}

//-----------------------------------------------------------------------------
D3DXMATRIX&
SceneObject::GetTransform (void)
{
    return m_transform;
}

//-----------------------------------------------------------------------------
void
SceneObject::SetTransform (D3DXMATRIX& transform)
{
    m_transform = transform;
}

//-----------------------------------------------------------------------------
const D3DXMATRIX&
SceneObject::GetCachedTransform (void) const
{
    return m_cachedTransform;
}

//-----------------------------------------------------------------------------
/* void */
SceneObject::SceneObject (const PtrToXMLNode& pXMLNode) :
    IndexedObject <SceneObject> (pXMLNode)
{
    // initialize the transform matrix from the xml data
    ImportTransforms (&m_transform, pXMLNode);
}

//-----------------------------------------------------------------------------
