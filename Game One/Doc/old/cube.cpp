//-----------------------------------------------------------------------------
// Copyright (C) 2002-2004 Bretton Wade, All rights reserved
// This file may be freely transmitted for educational purposes only
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// include files
//-----------------------------------------------------------------------------
#include    "precompile.h"
#include    "cube.h"
#include    "transform_xml.h"

//-----------------------------------------------------------------------------
// static class members
//-----------------------------------------------------------------------------
DEFINE_FACTORY (Cube);

//-----------------------------------------------------------------------------
// class methods
//-----------------------------------------------------------------------------
/* void */
Cube::Cube (const PtrToXMLNode& pXMLNode) : 
    Polyhedra (pXMLNode, 6, 4)
{
    // lock the buffer to prepare for writing
    Vertex*     pVertexData;
    D3D_TEST (m_pVertexBuffer->Lock (0, 0, reinterpret_cast<void**> (&pVertexData), 0));

    // six faces with four vertices each...

    pVertexData[ 0].vertex = D3DXVECTOR3 ( 1, 1, 1); pVertexData[ 0].texture = D3DXVECTOR2 ( 1, 1);
    pVertexData[ 1].vertex = D3DXVECTOR3 (-1, 1, 1); pVertexData[ 1].texture = D3DXVECTOR2 ( 0, 1);
    pVertexData[ 2].vertex = D3DXVECTOR3 (-1,-1, 1); pVertexData[ 2].texture = D3DXVECTOR2 ( 0, 0);
    pVertexData[ 3].vertex = D3DXVECTOR3 ( 1,-1, 1); pVertexData[ 3].texture = D3DXVECTOR2 ( 1, 0);

    pVertexData[ 4].vertex = D3DXVECTOR3 ( 1, 1,-1); pVertexData[ 4].texture = D3DXVECTOR2 ( 1, 1);
    pVertexData[ 5].vertex = D3DXVECTOR3 ( 1,-1,-1); pVertexData[ 5].texture = D3DXVECTOR2 ( 0, 1);
    pVertexData[ 6].vertex = D3DXVECTOR3 (-1,-1,-1); pVertexData[ 6].texture = D3DXVECTOR2 ( 0, 0);
    pVertexData[ 7].vertex = D3DXVECTOR3 (-1, 1,-1); pVertexData[ 7].texture = D3DXVECTOR2 ( 1, 0);

    pVertexData[ 8].vertex = D3DXVECTOR3 ( 1, 1,-1); pVertexData[ 8].texture = D3DXVECTOR2 ( 1, 1);
    pVertexData[ 9].vertex = D3DXVECTOR3 (-1, 1,-1); pVertexData[ 9].texture = D3DXVECTOR2 ( 0, 1);
    pVertexData[10].vertex = D3DXVECTOR3 (-1, 1, 1); pVertexData[10].texture = D3DXVECTOR2 ( 0, 0);
    pVertexData[11].vertex = D3DXVECTOR3 ( 1, 1, 1); pVertexData[11].texture = D3DXVECTOR2 ( 1, 0);

    pVertexData[12].vertex = D3DXVECTOR3 ( 1,-1, 1); pVertexData[12].texture = D3DXVECTOR2 ( 1, 1);
    pVertexData[13].vertex = D3DXVECTOR3 (-1,-1, 1); pVertexData[13].texture = D3DXVECTOR2 ( 0, 1);
    pVertexData[14].vertex = D3DXVECTOR3 (-1,-1,-1); pVertexData[14].texture = D3DXVECTOR2 ( 0, 0);
    pVertexData[15].vertex = D3DXVECTOR3 ( 1,-1,-1); pVertexData[15].texture = D3DXVECTOR2 ( 1, 0);

    pVertexData[16].vertex = D3DXVECTOR3 ( 1, 1, 1); pVertexData[16].texture = D3DXVECTOR2 ( 1, 1);
    pVertexData[17].vertex = D3DXVECTOR3 ( 1,-1, 1); pVertexData[17].texture = D3DXVECTOR2 ( 0, 1);
    pVertexData[18].vertex = D3DXVECTOR3 ( 1,-1,-1); pVertexData[18].texture = D3DXVECTOR2 ( 0, 0);
    pVertexData[19].vertex = D3DXVECTOR3 ( 1, 1,-1); pVertexData[19].texture = D3DXVECTOR2 ( 1, 0);

    pVertexData[20].vertex = D3DXVECTOR3 (-1, 1, 1); pVertexData[20].texture = D3DXVECTOR2 ( 1, 1);
    pVertexData[21].vertex = D3DXVECTOR3 (-1, 1,-1); pVertexData[21].texture = D3DXVECTOR2 ( 0, 1);
    pVertexData[22].vertex = D3DXVECTOR3 (-1,-1,-1); pVertexData[22].texture = D3DXVECTOR2 ( 0, 0);
    pVertexData[23].vertex = D3DXVECTOR3 (-1,-1, 1); pVertexData[23].texture = D3DXVECTOR2 ( 1, 0);
    
    // build the index we'll use for rasterizing as an indexed triangle list
    BuildIndex (pVertexData);

    // compute the surface normals
    ComputeNormals (pVertexData);

    // unlock the buffer
    D3D_TEST (m_pVertexBuffer->Unlock ());
}

//-----------------------------------------------------------------------------
/* void */
Cube::~Cube (void)
{
}

//-----------------------------------------------------------------------------
