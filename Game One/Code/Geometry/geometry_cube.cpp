//-----------------------------------------------------------------------------
// Copyright (C) 1997-2006 Bretton Wade
// All Rights Reserved
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// include files
//-----------------------------------------------------------------------------
#include    "precompile.h"
#include    "geometry_cube.h"

//-----------------------------------------------------------------------------
// static class members
//-----------------------------------------------------------------------------
DEFINE_FACTORY (Cube);

//-----------------------------------------------------------------------------
// class methods
//-----------------------------------------------------------------------------
/* void */
Cube::Cube (const PtrToXMLNode& pXMLNode) : 
    GeometryObjectIndexed (pXMLNode)
{
    Vertex  v[24] = 
    {
        Vertex (D3DXVECTOR3 ( 1, 1, 1), D3DXVECTOR3 ( 0, 0, 1), D3DXVECTOR2 ( 1, 1)),
        Vertex (D3DXVECTOR3 (-1, 1, 1), D3DXVECTOR3 ( 0, 0, 1), D3DXVECTOR2 ( 0, 1)),
        Vertex (D3DXVECTOR3 (-1,-1, 1), D3DXVECTOR3 ( 0, 0, 1), D3DXVECTOR2 ( 0, 0)),
        Vertex (D3DXVECTOR3 ( 1,-1, 1), D3DXVECTOR3 ( 0, 0, 1), D3DXVECTOR2 ( 1, 0)),

        Vertex (D3DXVECTOR3 ( 1, 1,-1), D3DXVECTOR3 ( 0, 0,-1), D3DXVECTOR2 ( 1, 1)),
        Vertex (D3DXVECTOR3 ( 1,-1,-1), D3DXVECTOR3 ( 0, 0,-1), D3DXVECTOR2 ( 0, 1)),
        Vertex (D3DXVECTOR3 (-1,-1,-1), D3DXVECTOR3 ( 0, 0,-1), D3DXVECTOR2 ( 0, 0)),
        Vertex (D3DXVECTOR3 (-1, 1,-1), D3DXVECTOR3 ( 0, 0,-1), D3DXVECTOR2 ( 1, 0)),

        Vertex (D3DXVECTOR3 ( 1, 1,-1), D3DXVECTOR3 ( 0, 1, 0), D3DXVECTOR2 ( 1, 1)),
        Vertex (D3DXVECTOR3 (-1, 1,-1), D3DXVECTOR3 ( 0, 1, 0), D3DXVECTOR2 ( 0, 1)),
        Vertex (D3DXVECTOR3 (-1, 1, 1), D3DXVECTOR3 ( 0, 1, 0), D3DXVECTOR2 ( 0, 0)),
        Vertex (D3DXVECTOR3 ( 1, 1, 1), D3DXVECTOR3 ( 0, 1, 0), D3DXVECTOR2 ( 1, 0)),

        Vertex (D3DXVECTOR3 ( 1,-1, 1), D3DXVECTOR3 ( 0,-1, 0), D3DXVECTOR2 ( 1, 1)),
        Vertex (D3DXVECTOR3 (-1,-1, 1), D3DXVECTOR3 ( 0,-1, 0), D3DXVECTOR2 ( 0, 1)),
        Vertex (D3DXVECTOR3 (-1,-1,-1), D3DXVECTOR3 ( 0,-1, 0), D3DXVECTOR2 ( 0, 0)),
        Vertex (D3DXVECTOR3 ( 1,-1,-1), D3DXVECTOR3 ( 0,-1, 0), D3DXVECTOR2 ( 1, 0)),

        Vertex (D3DXVECTOR3 ( 1, 1, 1), D3DXVECTOR3 ( 1, 0, 0), D3DXVECTOR2 ( 1, 1)),
        Vertex (D3DXVECTOR3 ( 1,-1, 1), D3DXVECTOR3 ( 1, 0, 0), D3DXVECTOR2 ( 0, 1)),
        Vertex (D3DXVECTOR3 ( 1,-1,-1), D3DXVECTOR3 ( 1, 0, 0), D3DXVECTOR2 ( 0, 0)),
        Vertex (D3DXVECTOR3 ( 1, 1,-1), D3DXVECTOR3 ( 1, 0, 0), D3DXVECTOR2 ( 1, 0)),

        Vertex (D3DXVECTOR3 (-1, 1, 1), D3DXVECTOR3 (-1, 0, 0), D3DXVECTOR2 ( 1, 1)),
        Vertex (D3DXVECTOR3 (-1, 1,-1), D3DXVECTOR3 (-1, 0, 0), D3DXVECTOR2 ( 0, 1)),
        Vertex (D3DXVECTOR3 (-1,-1,-1), D3DXVECTOR3 (-1, 0, 0), D3DXVECTOR2 ( 0, 0)),
        Vertex (D3DXVECTOR3 (-1,-1, 1), D3DXVECTOR3 (-1, 0, 0), D3DXVECTOR2 ( 1, 0))
    };
    
    AddTriangle (v[ 0], v[ 1], v[ 2]);
    AddTriangle (v[ 0], v[ 2], v[ 3]);
    
    AddTriangle (v[ 4], v[ 5], v[ 6]);
    AddTriangle (v[ 4], v[ 6], v[ 7]);
    
    AddTriangle (v[ 8], v[ 9], v[10]);
    AddTriangle (v[ 8], v[10], v[11]);

    AddTriangle (v[12], v[13], v[14]);
    AddTriangle (v[12], v[14], v[15]);

    AddTriangle (v[16], v[17], v[18]);
    AddTriangle (v[16], v[18], v[19]);

    AddTriangle (v[20], v[21], v[22]);
    AddTriangle (v[20], v[22], v[23]);
    
    // finish up...
    BuildObject ();
}

//-----------------------------------------------------------------------------
/* void */
Cube::~Cube (void)
{
}

//-----------------------------------------------------------------------------
