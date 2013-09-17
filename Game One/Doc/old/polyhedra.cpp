//-----------------------------------------------------------------------------
// Copyright (C) 2002-2004 Bretton Wade, All rights reserved
// This file may be freely transmitted for educational purposes only
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// include files
//-----------------------------------------------------------------------------
#include    "precompile.h"
#include    "polyhedra.h"
#include    "direct_x.h"

//-----------------------------------------------------------------------------
void
Polyhedra::Render (const Direct3D& pDirect3D, UINT iRenderPass) const
{
    // do the parental thang
    DrawnObject::Render (pDirect3D, iRenderPass);
    
    // check the render pass
    switch (iRenderPass)
    {
        case AMBIENT_PASS:
        case LIGHTING_PASS:
            // set the index buffer and render the indexed triangle list
            D3D_REQUIRE (pDirect3D->SetIndices (m_pIndexBuffer));
            D3D_REQUIRE (pDirect3D->DrawIndexedPrimitive (D3DPT_TRIANGLELIST, 0, 0, m_iVertexCount, 0, m_iTriangleCount));
            break;

        default:
            break;
    }
}

//-----------------------------------------------------------------------------
/* void */
Polyhedra::Polyhedra (const PtrToXMLNode& pXMLNode, uInt2 iFaceCount, uInt2 iVerticesPerFace) :
    DrawnObject (pXMLNode, iFaceCount * iVerticesPerFace)
{
    // initialize the member values from the parameters
    m_iFaceCount = iFaceCount;
    m_iVerticesPerFace = iVerticesPerFace;
    m_iTrianglesPerFace = m_iVerticesPerFace - 2;
    m_iTriangleCount = m_iTrianglesPerFace * m_iFaceCount;
}

//-----------------------------------------------------------------------------
/* void */              
Polyhedra::~Polyhedra (void) 
{
}

//-----------------------------------------------------------------------------
void
Polyhedra::ComputeNormals (Vertex* pVertexData)
{
    // loop over all of the faces of the polyhedra
    for (uInt2 i = 0; i < m_iFaceCount; i++)
    {
        // use the first three vertices (A, B, C) of the face to calculate the
        // surface normal vector. The vertex coordinates are at every other 
        // location in the array
        D3DXVECTOR3     AB = pVertexData[1].vertex - pVertexData[0].vertex;
        D3DXVECTOR3     BC = pVertexData[2].vertex - pVertexData[1].vertex;
        D3DXVECTOR3     normal;
        D3DXVec3Cross (&normal, &AB, &BC);
        D3DXVec3Normalize (&normal, &normal);

        // put the normal into all of the face vertices. The normals are at
        // every other location in the array starting at location 1
        for (uInt j = 0; j < m_iVerticesPerFace; j++)
            pVertexData[j].normal = normal;

        // skip the vertex data ahead to the next face
        pVertexData += m_iVerticesPerFace;
    }
}

//-----------------------------------------------------------------------------
void
Polyhedra::BuildIndex (Vertex* pVertexData)
{
    // fetch the direct 3d object
    const Direct3D&     pDirect3D = DirectX::GetDirect3D ();
     
    // allocate the index buffer as the number of triangles times 3 vertices
    // times 2 bytes per index
    uInt    iTotalVertexCount = m_iFaceCount * m_iTrianglesPerFace * 3;
    Assert (iTotalVertexCount < USHRT_MAX);
    uInt    iIndexBytes = iTotalVertexCount * sizeof (uInt2);
    D3D_TEST (pDirect3D->CreateIndexBuffer (iIndexBytes, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &m_pIndexBuffer, 0));
    
    // lock down the index buffer so we can fill it   
    uInt2*       pIndexDataBase;
    D3D_TEST (m_pIndexBuffer->Lock (0, 0, reinterpret_cast<void**> (&pIndexDataBase), 0));
    uInt2*       pIndexData = pIndexDataBase;
    
    // now walk over each face building the index data as a triangle list
    for (uInt2 i = 0; i < m_iFaceCount; i++)
    {
        uInt2    iStart = i * m_iVerticesPerFace;
        for (uInt2 j = 0; j < m_iTrianglesPerFace; j++)
        {
            *pIndexData++ = iStart;
            *pIndexData++ = iStart + 1 + j;
            *pIndexData++ = iStart + 2 + j;
        }
    }

    // create the shadow
    if (m_pShadow)
    {
        m_pShadow->AccumulatePartialEdges (D3DPT_TRIANGLELIST, m_iTriangleCount, pVertexData, pIndexDataBase);
        m_pShadow->MatchPartialEdges ();
    }
    
    // unlock the index buffer, since we are done filling it
    D3D_TEST (m_pIndexBuffer->Unlock ());
}

//-----------------------------------------------------------------------------
