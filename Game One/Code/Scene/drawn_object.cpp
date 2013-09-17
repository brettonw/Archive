//-----------------------------------------------------------------------------
// Copyright (C) 1997-2006 Bretton Wade
// All Rights Reserved
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// include files
//-----------------------------------------------------------------------------
#include    "precompile.h"
#include    "drawn_object.h"
#include    "direct_3d.h"

//-----------------------------------------------------------------------------
// static class members
//-----------------------------------------------------------------------------
DEFINE_FACTORY (DrawnObject);

//-----------------------------------------------------------------------------
void
DrawnObject::Prologue (const D3DXMATRIX& transform)
{
    // do the parental thang
    SceneObject::Prologue (transform);
    
    // compute the inverse of the cached transform
    D3DXMatrixInverse (&m_cachedInverse, 0, &m_cachedTransform);
}

//-----------------------------------------------------------------------------
void
DrawnObject::Render (const Direct3D& pDirect3D, const RenderPass& renderPass) const
{
    // do the parental thang
    SceneObject::Render (pDirect3D, renderPass);
    
    // update the light position and direction from the cached transform
    D3DXVec3TransformCoord (&renderPass.m_cachePosition, &renderPass.m_lightPosition, &m_cachedInverse);
    D3DXVec3TransformNormal (&renderPass.m_cacheDirection, &renderPass.m_lightDirection, &m_cachedInverse);
    
    // render the attached geometry
    m_pGeometryObject->Render (pDirect3D, renderPass);
}

//-----------------------------------------------------------------------------
/* void */
DrawnObject::DrawnObject (const PtrToXMLNode& pXMLNode) :
    SceneObject (pXMLNode)
{
    
    // get the geometry by name
    m_pGeometryObject = GeometryObject::Find (pXMLNode->GetAttribute ("geometry"));
}

//-----------------------------------------------------------------------------
/* void */              
DrawnObject::~DrawnObject (void)
{
}

//-----------------------------------------------------------------------------
