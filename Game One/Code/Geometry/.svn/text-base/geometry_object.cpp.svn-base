//-----------------------------------------------------------------------------
// Copyright (C) 1997-2006 Bretton Wade
// All Rights Reserved
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// include files
//-----------------------------------------------------------------------------
#include    "precompile.h"
#include    "geometry_object.h"
#include    "direct_3d.h"

//-----------------------------------------------------------------------------
// static member variables
//-----------------------------------------------------------------------------
VertexVector    GeometryObject::s_vVertices;

//-----------------------------------------------------------------------------
// class methods
//-----------------------------------------------------------------------------
void                    
GeometryObject::Render (const Direct3D& pDirect3D, const RenderPass& renderPass) const
{
    D3D_REQUIRE (pDirect3D->SetVertexDeclaration (m_pD3DVertexDecl));
    D3D_REQUIRE (pDirect3D->SetStreamSource (0, m_pVertexBuffer, 0, sizeof (Vertex)));
    D3D_REQUIRE (pDirect3D->DrawPrimitive (D3DPT_TRIANGLELIST, 0, m_iTriangleCount));
}

//-----------------------------------------------------------------------------
const VertexVector*         
GeometryObject::GetVertexVector (void) const
{
    return &s_vVertices;
}

//-----------------------------------------------------------------------------
const IndexVector*          
GeometryObject::GetIndexVector (void) const
{
    return 0;
}

//-----------------------------------------------------------------------------
void                    
GeometryObject::ClearAccumulator (void)
{
    // clean out the memory used by the accumulator
    s_vVertices.clear ();
}

//-----------------------------------------------------------------------------
/* void */              
GeometryObject::GeometryObject (const PtrToXMLNode& pXMLNode) :
    IndexedObject <GeometryObject> (pXMLNode)
{
}

//-----------------------------------------------------------------------------
/* void */              
GeometryObject::~GeometryObject (void)
{
}

//-----------------------------------------------------------------------------
void                    
GeometryObject::AddTriangle (const Vertex& A, const Vertex& B, const Vertex& C)
{
    s_vVertices.push_back (A);
    s_vVertices.push_back (B);
    s_vVertices.push_back (C);
}

//-----------------------------------------------------------------------------
void                    
GeometryObject::BuildObject (bool bDynamic)
{
    // we have accumulated a triangle list in the s_vVertices vector. This is 
    // copied into a vertex buffer. Start by getting the vertex count and
    // computing the triangle count
    uInt2               iVertexCount = s_vVertices.size ();
    m_iTriangleCount = iVertexCount / 3;
    
    // fetch the direct 3d object
    Direct3D&           pDirect3D = *(Direct3D::GetSingleton ());

    // create the vertex declaration
    D3D_REQUIRE (pDirect3D->CreateVertexDeclaration (g_vertexDecl, &m_pD3DVertexDecl));

    // create the vertex buffer big enough to hold all of the vertices
    DWORD               usage = D3DUSAGE_WRITEONLY bit_or (bDynamic ? D3DUSAGE_DYNAMIC : 0);
    D3D_REQUIRE (pDirect3D->CreateVertexBuffer (iVertexCount * sizeof (Vertex), usage, 0, D3DPOOL_DEFAULT, &m_pVertexBuffer, 0));
    
    // lock the buffer, copy the buffer data, and unlock
    Vertex*             pVertexData;
    D3D_REQUIRE (m_pVertexBuffer->Lock (0, 0, reinterpret_cast<void**> (&pVertexData), 0));
    memcpy (pVertexData, &s_vVertices[0], sizeof (Vertex) * iVertexCount);
    D3D_REQUIRE (m_pVertexBuffer->Unlock ());
}

//-----------------------------------------------------------------------------
