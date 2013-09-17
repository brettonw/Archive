//-----------------------------------------------------------------------------
// Copyright (C) 1997-2006 Bretton Wade
// All Rights Reserved
//-----------------------------------------------------------------------------

#ifndef     _VERTEX_H_
#define     _VERTEX_H_

//-----------------------------------------------------------------------------
// include files
//-----------------------------------------------------------------------------
#ifndef     _ADVANCED_MATH_H_
#include    "advanced_math.h"
#endif  //  _ADVANCED_MATH_H_

//-----------------------------------------------------------------------------
// global variable
//-----------------------------------------------------------------------------
extern
D3DVERTEXELEMENT9   g_vertexDecl[];

//-----------------------------------------------------------------------------
// structure definition
//-----------------------------------------------------------------------------
// a struct defining our vertex data as the vertex and normal with
// a single texture that has 2 coordinates
struct Vertex
{
    D3DXVECTOR3         vertex;
    D3DXVECTOR3         normal;
    D3DXVECTOR2         texture;
    
    /* void */          Vertex (D3DXVECTOR3& v, D3DXVECTOR3& n, D3DXVECTOR2& t) :
                            vertex (v), normal (n), texture (t) {}
};

//-----------------------------------------------------------------------------
class VertexVector : public std::vector<Vertex> {};

//-----------------------------------------------------------------------------
typedef uInt2   Index;
class IndexVector : public std::vector<Index> {};

//-----------------------------------------------------------------------------
// fuzzy math comparison of a vertex
//-----------------------------------------------------------------------------
bool    CloseMatch (const Vertex& A, const Vertex& B);

//-----------------------------------------------------------------------------

#endif  //  _VERTEX_H_
