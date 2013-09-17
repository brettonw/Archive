//-----------------------------------------------------------------------------
// Copyright (C) 2002-2004 Bretton Wade, All rights reserved
// This file may be freely transmitted for educational purposes only
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// include files
//-----------------------------------------------------------------------------
#include    "precompile.h"
#include    "shadow.h"
#include    "drawn_object.h"
#include    "advanced_math.h"

//-----------------------------------------------------------------------------
// class static members
//-----------------------------------------------------------------------------
std::list<ShadowA::PartialEdge>  ShadowA::s_edgeAccumulator;

//-----------------------------------------------------------------------------
// class methods
//-----------------------------------------------------------------------------
void                
ShadowA::PushTriangle (const D3DXVECTOR3& A, const D3DXVECTOR3& B, const D3DXVECTOR3& C, bool bFlip)
{
    // compute the normal for the triangle
    D3DXVECTOR3     AB = B - A;
    D3DXVECTOR3     BC = C - B;
    D3DXVECTOR3     N;
    D3DXVec3Normalize (&AB, &AB);
    D3DXVec3Normalize (&BC, &BC);
    D3DXVec3Cross (&N, &AB, &BC);
    
    // check if the triangle should be flipped, this might be the result of
    // a triangle in a strip, for instance.
    if (bFlip)
    {
        // it should, reverse the normal and enter the edges backwards
        N = -N;
        s_edgeAccumulator.push_back (PartialEdge (B, A, N));
        s_edgeAccumulator.push_back (PartialEdge (C, B, N));
        s_edgeAccumulator.push_back (PartialEdge (A, C, N));
    }
    else
    {
        // otherwise enter the edges in normal order
        s_edgeAccumulator.push_back (PartialEdge (A, B, N));
        s_edgeAccumulator.push_back (PartialEdge (B, C, N));
        s_edgeAccumulator.push_back (PartialEdge (C, A, N));
    }
}

//-----------------------------------------------------------------------------
/* void */                  
ShadowA::ShadowA (const PtrToXMLNode& pXMLNode)
{
    sscanf (pXMLNode->GetAttribute ("length"), "%f", &m_length);
}

//-----------------------------------------------------------------------------
/* void */                  
ShadowA::~ShadowA (void)
{
}

//-----------------------------------------------------------------------------
void    
ShadowA::AccumulatePartialEdges (D3DPRIMITIVETYPE primType, uInt2 primCount, Vertex* pVertices)
{
    // this function needs to loop over all of the triangles in the buffer storing 
    // each edge location with a computed normal. I don't care about the normal
    // attached to the vertices.
    switch (primType)
    {
        case D3DPT_TRIANGLELIST:
        {
            // every 3 points defines a triangle
            for (int i = 0; i < primCount; i++)
            {
                int             a = i * 3;
                PushTriangle (pVertices[a].vertex, pVertices[a + 1].vertex, pVertices[a + 2].vertex, false);
            }
            break;
        }
        case D3DPT_TRIANGLESTRIP:
        {
            // after first two points, every subsequent point is a triangle,
            // meaning that each subsequent point is an edge. primCount is 
            // the number of triangles
            D3DXVECTOR3     A = pVertices[0].vertex;
            D3DXVECTOR3     B = pVertices[1].vertex;
            for (int i = 0; i < primCount; i++)
            {
                D3DXVECTOR3 C = pVertices[i + 2].vertex;
                PushTriangle (A, B, C, (i & 0x01) ? true : false);
                A = B;
                B = C;
            }
            break;
        }
        case D3DPT_TRIANGLEFAN:
        {
            // after first two points, every subsequent point is a triangle,
            // meaning that each subsequent point is an edge. primCount is 
            // the number of triangles
            D3DXVECTOR3     B = pVertices[1].vertex;
            for (int i = 0; i < primCount; i++)
            {
                D3DXVECTOR3 C = pVertices[i + 2].vertex;
                PushTriangle (pVertices[0].vertex, B, C, false);
                B = C;
            }
            break;
        }
    }
}

//-----------------------------------------------------------------------------
void    
ShadowA::AccumulatePartialEdges (D3DPRIMITIVETYPE primType, uInt2 primCount, Vertex* pVertices, uInt2* pIndices)
{
    // this function needs to loop over all of the triangles in the buffer storing 
    // each edge location with a computed normal. I don't care about the normal
    // attached to the vertices.
    switch (primType)
    {
        case D3DPT_TRIANGLELIST:
        {
            // every 3 points defines a triangle
            for (int i = 0; i < primCount; i++)
            {
                int             a = i * 3;
                PushTriangle (pVertices[pIndices[a]].vertex, pVertices[pIndices[a + 1]].vertex, pVertices[pIndices[a + 2]].vertex, false);
            }
            break;
        }
        case D3DPT_TRIANGLESTRIP:
        {
            // after first two points, every subsequent point is a triangle,
            // meaning that each subsequent point is an edge. primCount is 
            // the number of triangles
            D3DXVECTOR3     A = pVertices[pIndices[0]].vertex;
            D3DXVECTOR3     B = pVertices[pIndices[1]].vertex;
            for (int i = 0; i < primCount; i++)
            {
                D3DXVECTOR3 C = pVertices[pIndices[i + 2]].vertex;
                PushTriangle (A, B, C, (i & 0x01) ? true : false);
                A = B;
                B = C;
            }
            break;
        }
        case D3DPT_TRIANGLEFAN:
        {
            // after first two points, every subsequent point is a triangle,
            // meaning that each subsequent point is an edge. primCount is 
            // the number of triangles
            D3DXVECTOR3     A = pVertices[pIndices[0]].vertex;
            D3DXVECTOR3     B = pVertices[pIndices[1]].vertex;
            for (int i = 0; i < primCount; i++)
            {
                D3DXVECTOR3 C = pVertices[pIndices[i + 2]].vertex;
                PushTriangle (A, B, C, false);
                B = C;
            }
            break;
        }
    }
}

//-----------------------------------------------------------------------------
void    
ShadowA::MatchPartialEdges (void)
{
    // this function will loop over the accumulated edges in an n^2 algorithm for matching 
    // vertices and building real edges. in a closed object, all edges will have a match
    std::list<PartialEdge>::iterator    iter = s_edgeAccumulator.begin ();
    while (iter != s_edgeAccumulator.end ())
    {
        PartialEdge&                        iterEdge = *iter;
        std::list<PartialEdge>::iterator    check = iter;
        bool                                bMatchFound = false;
        ++check;
        
        while ((not bMatchFound) and (check != s_edgeAccumulator.end ()))
        {
            PartialEdge&  checkEdge = *check;
            // all matching edges must have counter rotating edges, so I check
            // for the backwards ordering
            if (CloseMatch (checkEdge.ptB, iterEdge.ptA) and CloseMatch (checkEdge.ptA, iterEdge.ptB))
            {
                // the edges match, now I want to examine the two normals to determine 
                // whether or not this is a convex edge (concave edges will never be a
                // part of a silhouette). The basic approach is to compute the cross
                // product of the two normals and check two things:
                //     1) If the length is 0, the normals are coincident and the edge
                //        is not a silhouette edge
                D3DXVECTOR3 xProduct;
                D3DXVec3Cross (&xProduct, &iterEdge.normal, &checkEdge.normal);
                if (D3DXVec3Length (&xProduct) > 1.0e-5)
                {
                    //     2) If the cross product is running the same direction as the 
                    //        base edge, then the edge is convex and we want to store it
                    D3DXVECTOR3 baseEdge = iterEdge.ptB - iterEdge.ptA;
                    if (D3DXVec3Dot (&xProduct, &baseEdge) > 0)
                        m_edgeVec.push_back (Edge (iterEdge.ptA, iterEdge.ptB, iterEdge.normal, checkEdge.normal));
                }

                // remove the partial edges from the edge list
                s_edgeAccumulator.erase (check);
                iter = s_edgeAccumulator.erase (iter);
                
                // terminate the search
                bMatchFound = true;
            }
            else
            {
                // advance to the next partial edge
                ++check;
            }
        }
        
        // if we get here and haven't found a match, then we have a lonely edge. This
        // could indicate an error in the matching algorithm, or the object is not 
        // closed (some might say 2-manifold). In either case, the algorithm will hang
        // if we don't give up on this partial edge and advance, so we spew a message 
        // and advance to the next partial edge
        if (not bMatchFound)
        {
            DebugPrint ("Lonely edge found (%f, %f, %f) to (%5f, %f, %f)\n", iterEdge.ptA.x, iterEdge.ptA.y, iterEdge.ptA.z, iterEdge.ptB.x, iterEdge.ptB.y, iterEdge.ptB.z);
            ++iter;
        }
    }
    
    // clear the accumulated edges and reserve some space for the silhouette
    s_edgeAccumulator.clear ();
    m_silhouetteVec.reserve (m_edgeVec.size () / 2);
    DebugPrint ("    Constructed shadow edge list with %d edges\n", int (m_edgeVec.size ()));
}

//-----------------------------------------------------------------------------
void    
ShadowA::FindSilhouetteEdges (const D3DXVECTOR3& lightDirection)
{
    m_silhouetteVec.erase (m_silhouetteVec.begin (), m_silhouetteVec.end ());
    std::vector<Edge>::iterator     iter = m_edgeVec.begin ();
    std::vector<Edge>::iterator     end = m_edgeVec.end ();
    while (iter != end)
    {
        Edge&   edge = *iter++;
        if ((D3DXVec3Dot (&edge.normalA, &lightDirection) >= 0) and (D3DXVec3Dot (&edge.normalB, &lightDirection) < 0))
        {
            // the edge normal is pointing to the eye, so the two points are 
            // from a face defined with that normal. They get pushed into the
            // vector in order so that when we extrude this edge it will 
            // result in a front facing polygon
            m_silhouetteVec.push_back (edge.ptA);
            m_silhouetteVec.push_back (edge.ptB);
        }
        else if ((D3DXVec3Dot (&edge.normalA, &lightDirection) < 0) and (D3DXVec3Dot (&edge.normalB, &lightDirection) >= 0))
        {
            // the edge normal is pointing away from the eye, so the two 
            // points are from the opposite face defined with that normal. 
            // They get pushed into the vector in reverse order so that 
            // when we extrude this edge it will result in a front facing 
            // polygon
            m_silhouetteVec.push_back (edge.ptB);
            m_silhouetteVec.push_back (edge.ptA);
        }
    }
}

//-----------------------------------------------------------------------------
void                        
ShadowA::ExtrudeSilhouetteEdges (const D3DXVECTOR3& lightDirection)
{
    m_polyVec.erase (m_polyVec.begin (), m_polyVec.end ());
    std::vector<D3DXVECTOR3>::iterator  iter = m_silhouetteVec.begin ();
    std::vector<D3DXVECTOR3>::iterator  end = m_silhouetteVec.end ();
    while (iter != end)
    {
        D3DXVECTOR3&    A = *iter++;
        D3DXVECTOR3&    B = *iter++;
        D3DXVECTOR3     C = B + (m_length * lightDirection);
        D3DXVECTOR3     D = A + (m_length * lightDirection);
        m_polyVec.push_back (A);
        m_polyVec.push_back (B);
        m_polyVec.push_back (C);
        
        m_polyVec.push_back (A);
        m_polyVec.push_back (C);
        m_polyVec.push_back (D);
    }
}

//-----------------------------------------------------------------------------
void    
ShadowA::DebugRender (const Direct3D& pDirect3D, const D3DXVECTOR3& lightDirection)
{
    FindSilhouetteEdges (lightDirection);
    ExtrudeSilhouetteEdges (lightDirection);
    pDirect3D->SetFVF (D3DFVF_XYZ);
    D3D_REQUIRE (pDirect3D->DrawPrimitiveUP (D3DPT_TRIANGLELIST, m_polyVec.size () / 3, &m_polyVec[0], sizeof (D3DXVECTOR3)));
    pDirect3D->SetRenderState (D3DRS_FILLMODE, D3DFILL_SOLID);
}

//-----------------------------------------------------------------------------
