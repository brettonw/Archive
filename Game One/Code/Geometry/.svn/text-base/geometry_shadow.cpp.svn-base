//-----------------------------------------------------------------------------
// Copyright (C) 1997-2006 Bretton Wade
// All Rights Reserved
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// include files
//-----------------------------------------------------------------------------
#include    "precompile.h"
#include    "geometry_shadow.h"
#include    "direct_3d.h"

//-----------------------------------------------------------------------------
// static class members
//-----------------------------------------------------------------------------
DEFINE_FACTORY (Shadow);
Shadow::PartialEdgeList     Shadow::s_edgeAccumulator;
VertexShadow*               Shadow::s_pVertexData;
uInt2                       Shadow::s_iVertex;

//-----------------------------------------------------------------------------
// class methods
//-----------------------------------------------------------------------------
void                    
Shadow::Render (const Direct3D& pDirect3D, const RenderPass& renderPass) const
{
    switch (renderPass.m_iPass)
    {
        case AMBIENT_PASS:
        case LIGHTING_PASS:
            m_shadowCaster->Render (pDirect3D, renderPass);
            break;

        case SHADOW_STENCIL_PASS:
            // XXX this should be updated to handle spot and positional lights
            const_cast<Shadow*> (this)->BuildShadowVolume (pDirect3D, renderPass.m_cacheDirection);
            
            // render the shadow volume buffer
            D3D_REQUIRE (pDirect3D->SetVertexDeclaration (m_pD3DVertexDecl));
            D3D_REQUIRE (pDirect3D->SetStreamSource (0, m_pVertexBuffer, 0, sizeof (VertexShadow)));
            D3D_REQUIRE (pDirect3D->DrawPrimitive (D3DPT_TRIANGLELIST, 0, m_iTriangleCount));
            break;
    }
}

//-----------------------------------------------------------------------------
void                
Shadow::AccumulateTriangle (const D3DXVECTOR3& A, const D3DXVECTOR3& B, const D3DXVECTOR3& C)
{
    // compute the normal for the triangle - this will be used to determine if 
    // the face is visible regardless of how it gets shaded
    D3DXVECTOR3     AB = B - A;
    D3DXVECTOR3     BC = C - B;
    D3DXVECTOR3     N;
    D3DXVec3Normalize (&AB, &AB);
    D3DXVec3Normalize (&BC, &BC);
    D3DXVec3Cross (&N, &AB, &BC);
    
    // enter the edges in normal order, this is important as we rely on the
    // information ordering in the silhouette calculation algorithm
    s_edgeAccumulator.push_back (PartialEdge (A, B, N));
    s_edgeAccumulator.push_back (PartialEdge (B, C, N));
    s_edgeAccumulator.push_back (PartialEdge (C, A, N));
}

//-----------------------------------------------------------------------------
void    
Shadow::AccumulatePartialEdges (void)
{
    // this function loops over all of the triangles in the accumulation buffer 
    // built by the geometry during its construction. It stores each edge 
    // location with a computed normal based on the orientation of the face
    // that contains the edge. The vertex normal is irrelevant
    const VertexVector* vertexVector = m_shadowCaster->GetVertexVector ();
    const IndexVector*  indexVector = m_shadowCaster->GetIndexVector ();
    
    // do something slightly different if the shadow caster is an indexed
    // triangle list or just a straight triangle list
    if (indexVector)
    {
        // every three indices defines a triangle, so iterate through the
        // index buffer three at a time, accumulating the triangle edges
        IndexVector::const_iterator     iter = indexVector->begin ();
        IndexVector::const_iterator     end = indexVector->end ();
        while (iter != end)
        {
            const Vertex&   A = vertexVector->at (*iter++);
            const Vertex&   B = vertexVector->at (*iter++);
            const Vertex&   C = vertexVector->at (*iter++);
            AccumulateTriangle (A.vertex, B.vertex, C.vertex);
        }
    }
    else
    {
        // every three vertices defines a triangle, so iterate through the
        // vertex buffer three at a time, accumulating the triangle edges
        VertexVector::const_iterator    iter = vertexVector->begin ();
        VertexVector::const_iterator    end = vertexVector->end ();
        while (iter != end)
        {
            const Vertex&   A = *iter++;
            const Vertex&   B = *iter++;
            const Vertex&   C = *iter++;
            AccumulateTriangle (A.vertex, B.vertex, C.vertex);
        }
    }
}

//-----------------------------------------------------------------------------
void    
Shadow::MatchPartialEdges (void)
{
    // this function will loop over the accumulated edges in an n^2 algorithm 
    // for matching vertices and building real edges. in a closed object, all 
    // edges will have a match
    PartialEdgeList::iterator   iter = s_edgeAccumulator.begin ();
    while (iter != s_edgeAccumulator.end ())
    {
        // grab the current edge, and prepare to loop over all of the ones that
        // follow it to find a match
        PartialEdge&                iterEdge = *iter;
        PartialEdgeList::iterator   check = iter;
        bool                        bMatchFound = false;
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
                if (D3DXVec3Length (&xProduct) > -g_fEpsilon)
                {
                    //     2) If the cross product is running the same direction as the 
                    //        base edge, then the edge is convex and we want to store it
                    D3DXVECTOR3 baseEdge = iterEdge.ptB - iterEdge.ptA;
                    if (D3DXVec3Dot (&xProduct, &baseEdge) > 0)
                        m_edgeVec.push_back (Edge (iterEdge.ptA, iterEdge.ptB, iterEdge.normal, checkEdge.normal));
                }

                // remove the processed partial edges from the edge list
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
    DebugPrint ("    Constructed shadow edge list with %d edges\n", int (m_edgeVec.size ()));
}

//-----------------------------------------------------------------------------
void                        
Shadow::ResizeVB (const Direct3D& pDirect3D)
{
    // we want to grow the buffer size to accomodate a demand for more capacity
    // but we don't want to unnecessarily allocate too much memory so the 
    // growth rate is limited to 25%. The capacity also needs to be a multiple
    // of 6 to accomodate complete quads
    Assert (m_iVertexCapacity <= 52000);
    PtrToD3DVertexBuffer    pNewVertexBuffer;
    uInt2                   iNewVertexCapacity = (m_iVertexCapacity * 5) / 4;
    iNewVertexCapacity += 6 - (iNewVertexCapacity % 6);
    Assert ((iNewVertexCapacity % 6 == 0) and (iNewVertexCapacity > m_iVertexCapacity)); 
    //DebugPrint ("    Resized Shadow VB from %hu to %hu\n", m_iVertexCapacity, iNewVertexCapacity);

    // create the new vertex buffer
    D3D_REQUIRE (pDirect3D->CreateVertexBuffer (iNewVertexCapacity * sizeof (VertexShadow), D3DUSAGE_WRITEONLY bit_or D3DUSAGE_DYNAMIC, 0, D3DPOOL_DEFAULT, &pNewVertexBuffer, 0));
    
    // lock the buffer and copy the old vertex data to the new buffer
    VertexShadow*           pVertexData;
    D3D_REQUIRE (pNewVertexBuffer->Lock (0, 0, reinterpret_cast<void**> (&pVertexData), D3DLOCK_DISCARD));
    memcpy (pVertexData, s_pVertexData, sizeof (VertexShadow) * m_iVertexCapacity);
    
    // we want to leave the buffer locked while we work with it, but the 
    // old buffer needs to be unlocked and released
    D3D_REQUIRE (m_pVertexBuffer->Unlock ());
    m_pVertexBuffer = pNewVertexBuffer;

    // propogate the size change and the locked buffer pointer
    m_iVertexCapacity = iNewVertexCapacity;
    s_pVertexData = pVertexData;
}

//-----------------------------------------------------------------------------
void                        
Shadow::ExtrudeSilhouetteEdge (const Direct3D& pDirect3D, const D3DXVECTOR3& lightDirection, const VertexShadow& A, const VertexShadow& B)
{
    // build the extrusion points
    VertexShadow    C = B + (m_length * lightDirection);
    VertexShadow    D = A + (m_length * lightDirection);
    
    // make sure the vertex buffer has enough room
    if (s_iVertex == m_iVertexCapacity)
        ResizeVB (pDirect3D);
    
    // emit two triangles to define the quad extrusion
    s_pVertexData[s_iVertex++] = A;
    s_pVertexData[s_iVertex++] = B;
    s_pVertexData[s_iVertex++] = C;

    s_pVertexData[s_iVertex++] = A;
    s_pVertexData[s_iVertex++] = C;
    s_pVertexData[s_iVertex++] = D;
}

//-----------------------------------------------------------------------------
void                        
Shadow::BuildShadowVolume (const Direct3D& pDirect3D, const D3DXVECTOR3& lightDirection)
{
    // lock the vertex buffer and start at the beginning
    D3D_REQUIRE (m_pVertexBuffer->Lock (0, 0, reinterpret_cast<void**> (&s_pVertexData), D3DLOCK_DISCARD));
    s_iVertex = 0;

    // prepare to loop over all of the edges
    EdgeVector::iterator    iter = m_edgeVec.begin ();
    EdgeVector::iterator    end = m_edgeVec.end ();
    while (iter != end)
    {
        Edge&   edge = *iter++;
        
        // a silhouette edge is defined as the edge between two faces where one 
        // of the faces is visible to the light, and the other is not. The 
        // visibility calculation is done using the face normals like a back-
        // face culling algorithm. We are looking for the dot product of the
        // lighting vector and one face to be positive while the dot product
        // of the lighting vector and the other face is negative. Since the 
        // edge is a two sided object, there are two possible combinations
        // that make for a silhouette edge, and both are accounted for here.
        if ((D3DXVec3Dot (&edge.normalA, &lightDirection) >= 0) and (D3DXVec3Dot (&edge.normalB, &lightDirection) < 0))
        {
            // the edge normal is pointing to the eye, so the two points are 
            // from a face defined with that normal. They get extruded in 
            // order so that the extruded edge is a front facing polygon
            ExtrudeSilhouetteEdge (pDirect3D, lightDirection, edge.ptA, edge.ptB);
        }
        else if ((D3DXVec3Dot (&edge.normalA, &lightDirection) < 0) and (D3DXVec3Dot (&edge.normalB, &lightDirection) >= 0))
        {
            // the edge normal is pointing away from the eye, so the two 
            // points are from the opposite face defined with that normal. 
            // They get extruded in reverse order so that the extruded 
            // edge is a front facing polygon
            ExtrudeSilhouetteEdge (pDirect3D, lightDirection, edge.ptB, edge.ptA);
        }
    }
    
    // unlock the buffer since we're finished filling it
    D3D_REQUIRE (m_pVertexBuffer->Unlock ());
    
    // set the triangle count
    m_iTriangleCount = s_iVertex / 3;
}

//-----------------------------------------------------------------------------
/* void */              
Shadow::Shadow (const PtrToXMLNode& pXMLNode) :
    GeometryObject (pXMLNode)
{
    // get the child node that defines the actual shadow caster and build it
    // this is done this way because the code responsible for loading the object 
    // from XML will clear the accumulators used to build it. Once the 
    // accumulator is cleared, static vertex data is no longer accessible to the 
    // shadow builder.
    PtrToXMLNode        pShadowCasterXMLNode = pXMLNode->GetChild ("GeometryObject");
    Assert (pShadowCasterXMLNode);
    m_shadowCaster = GeometryObject::Create (pShadowCasterXMLNode);
    
    // build the silhouette edge vectors by running through the caster's 
    // accumulator and gathering all the edges
    AccumulatePartialEdges ();

    // we're done with the caster accumulators so those can be cleaned up
    m_shadowCaster->ClearAccumulator ();

    // now loop over the complete edge list, and convert that into possible
    // silhouette edges
    MatchPartialEdges ();
    
    // fetch the direct 3d object and create the vertex declaration
    Direct3D&           pDirect3D = *(Direct3D::GetSingleton ());
    D3D_REQUIRE (pDirect3D->CreateVertexDeclaration (g_vertexShadowDecl, &m_pD3DVertexDecl));

    // XXX this should come from someplace else
    m_length = 6.0f;
    
    // The capacity needs to be a multiple of 6 to accomodate complete quads. 
    // A minimum of 3 faces is needed to define a volume, so we start with that
    m_iVertexCapacity = 6 * 3;
    
    // create the vertex buffer
    D3D_REQUIRE (pDirect3D->CreateVertexBuffer (m_iVertexCapacity * sizeof (VertexShadow), D3DUSAGE_WRITEONLY bit_or D3DUSAGE_DYNAMIC, 0, D3DPOOL_DEFAULT, &m_pVertexBuffer, 0));
}

//-----------------------------------------------------------------------------
/* void */              
Shadow::~Shadow (void)
{
}

//-----------------------------------------------------------------------------
