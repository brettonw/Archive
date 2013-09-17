//-----------------------------------------------------------------------------
// Copyright (C) 1997-2006 Bretton Wade
// All Rights Reserved
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// include files
//-----------------------------------------------------------------------------
#include    "precompile.h"
#include    "geometry_object_indexed.h"
#include    "direct_3d.h"

//-----------------------------------------------------------------------------
// static member variables
//-----------------------------------------------------------------------------
IndexVector     GeometryObjectIndexed::s_vIndices;

//-----------------------------------------------------------------------------
// class methods
//-----------------------------------------------------------------------------
void                    
GeometryObjectIndexed::Render (const Direct3D& pDirect3D, const RenderPass& renderPass) const
{
    D3D_REQUIRE (pDirect3D->SetVertexDeclaration (m_pD3DVertexDecl));
    D3D_REQUIRE (pDirect3D->SetStreamSource (0, m_pVertexBuffer, 0, sizeof (Vertex)));
    D3D_REQUIRE (pDirect3D->SetIndices (m_pIndexBuffer));
    D3D_REQUIRE (pDirect3D->DrawIndexedPrimitive (D3DPT_TRIANGLELIST, 0, 0, m_iVertexCount, 0, m_iTriangleCount));
}

//-----------------------------------------------------------------------------
const IndexVector*          
GeometryObjectIndexed::GetIndexVector (void) const
{
    return &s_vIndices;
}

//-----------------------------------------------------------------------------
void                    
GeometryObjectIndexed::ClearAccumulator (void)
{
    // clean out the memory used by the accumulator
    s_vVertices.clear ();
    s_vIndices.clear ();
}

//-----------------------------------------------------------------------------
/* void */              
GeometryObjectIndexed::GeometryObjectIndexed (const PtrToXMLNode& pXMLNode) :
    GeometryObject (pXMLNode)
{
}

//-----------------------------------------------------------------------------
/* void */              
GeometryObjectIndexed::~GeometryObjectIndexed (void)
{
}

//-----------------------------------------------------------------------------
Index                   
GeometryObjectIndexed::AddVertex (const Vertex& vertex)
{
    // loop over all of the existing vertices to see if this one matches it
    // if we find a match return that one as the index, this could be made 
    // much faster and might need to be... (XXX)
    for (uInt2 i = 0; i < s_vVertices.size (); i++)
        if (CloseMatch (s_vVertices[i], vertex))
            return i;
            
    // the vertex is not in the vector already, so add it and return the new
    // size as the index
    s_vVertices.push_back (vertex);
    return s_vVertices.size () - 1;
}

//-----------------------------------------------------------------------------
void                    
GeometryObjectIndexed::AddTriangle (const Vertex& A, const Vertex& B, const Vertex& C)
{
    // add the indices and vertices together in order
    s_vIndices.push_back (AddVertex (A));
    s_vIndices.push_back (AddVertex (B));
    s_vIndices.push_back (AddVertex (C));
}

//-----------------------------------------------------------------------------
void                    
GeometryObjectIndexed::BuildObject (bool bDynamic)
{
    // we have accumulated a triangle list in the s_vVertices vector. This is 
    // copied into a vertex buffer. Start by getting the vertex count and
    // computing the triangle count
    m_iVertexCount = s_vVertices.size ();
    uInt2               iIndexCount = s_vIndices.size ();
    m_iTriangleCount = iIndexCount / 3;
    
    // figure out the usage
    DWORD               usage = D3DUSAGE_WRITEONLY bit_or (bDynamic ? D3DUSAGE_DYNAMIC : 0);
    
    // fetch the direct 3d object
    Direct3D&           pDirect3D = *(Direct3D::GetSingleton ());

    // create the vertex declaration
    D3D_REQUIRE (pDirect3D->CreateVertexDeclaration (g_vertexDecl, &m_pD3DVertexDecl));

    // create the vertex buffer big enough to hold all of the vertices
    D3D_REQUIRE (pDirect3D->CreateVertexBuffer (m_iVertexCount * sizeof (Vertex), usage, 0, D3DPOOL_DEFAULT, &m_pVertexBuffer, 0));
    
    // lock the buffer, copy the buffer data, and unlock
    Vertex*             pVertexData;
    D3D_REQUIRE (m_pVertexBuffer->Lock (0, 0, reinterpret_cast<void**> (&pVertexData), 0));
    memcpy (pVertexData, &s_vVertices[0], sizeof (Vertex) * m_iVertexCount);
    D3D_REQUIRE (m_pVertexBuffer->Unlock ());
    
    // create the index buffer big enough to hold all of the indices
    uInt                iIndexBytes = iIndexCount * sizeof (Index);
    D3D_REQUIRE (pDirect3D->CreateIndexBuffer (iIndexBytes, usage, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &m_pIndexBuffer, 0));
    
    // lock the buffer, copy the buffer data, and unlock
    Index*              pIndexData;
    D3D_REQUIRE (m_pIndexBuffer->Lock (0, 0, reinterpret_cast<void**> (&pIndexData), 0));
    memcpy (pIndexData, &s_vIndices[0], sizeof (Index) * iIndexCount);
    D3D_REQUIRE (m_pIndexBuffer->Unlock ());
    
    // XXX There is an opportunity to optimize the triangle list for a 
    // particular piece of hardware here, could the driver do this for us? 
}

//-----------------------------------------------------------------------------
