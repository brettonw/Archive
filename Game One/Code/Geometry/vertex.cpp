//-----------------------------------------------------------------------------
// Copyright (C) 1997-2006 Bretton Wade
// All Rights Reserved
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// include files
//-----------------------------------------------------------------------------
#include    "precompile.h"
#include    "vertex.h"

//-----------------------------------------------------------------------------
// global variable
//-----------------------------------------------------------------------------
D3DVERTEXELEMENT9   g_vertexDecl[] = 
{  
    {0, 0,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
    {0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,   0},
    {0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
    D3DDECL_END ()
};

//-----------------------------------------------------------------------------
// fuzzy math comparison of a vertex
//-----------------------------------------------------------------------------
bool    
CloseMatch (const Vertex& A, const Vertex& B)
{
    return (CloseMatch (A.vertex, B.vertex) and 
            CloseMatch (A.normal, B.normal) and 
            CloseMatch (A.texture, B.texture)); 
}

//-----------------------------------------------------------------------------
