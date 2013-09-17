//-----------------------------------------------------------------------------
// Copyright (C) 1997-2006 Bretton Wade
// All Rights Reserved
//-----------------------------------------------------------------------------

#ifndef     _GEOMETRY_SHADOW_H_
#define     _GEOMETRY_SHADOW_H_

//-----------------------------------------------------------------------------
// include files
//-----------------------------------------------------------------------------
#ifndef     _GEOMETRY_OBJECT_H_
#include    "geometry_object.h"
#endif  //  _GEOMETRY_OBJECT_H_

#ifndef     _VERTEX_SHADOW_H_
#include    "vertex_shadow.h"
#endif  //  _VERTEX_SHADOW_H_

//-----------------------------------------------------------------------------
// class definitions
//-----------------------------------------------------------------------------
class Shadow : public GeometryObject
{
    public:
virtual void                        Render (const Direct3D& pDirect3D, const RenderPass& renderPass) const;

    protected:
        struct PartialEdge
        {
            VertexShadow            ptA;
            VertexShadow            ptB;
            D3DXVECTOR3             normal;
            /* void */              PartialEdge (const VertexShadow& a, const VertexShadow& b, const D3DXVECTOR3& n) :
                                        ptA (a), ptB (b), normal (n) {}
        };
        
        class PartialEdgeList : public std::list<PartialEdge> {};

        struct Edge
        {
            VertexShadow            ptA;
            VertexShadow            ptB;
            D3DXVECTOR3             normalA;
            D3DXVECTOR3             normalB;
            /* void */              Edge (const VertexShadow& a, const VertexShadow& b, const D3DXVECTOR3& na, const D3DXVECTOR3& nb) :
                                        ptA (a), ptB (b), normalA (na), normalB (nb) {}
        };
        
        class EdgeVector : public std::vector<Edge> {};
        // XXX this could use a partitioning scheme of some sort

static  PartialEdgeList             s_edgeAccumulator;
        EdgeVector                  m_edgeVec;
        float                       m_length;
        PtrToGeometryObject         m_shadowCaster;
        
        uInt2                       m_iVertexCapacity;
static  VertexShadow*               s_pVertexData;
static  uInt2                       s_iVertex;

    protected:
        void                        AccumulateTriangle (const D3DXVECTOR3& A, const D3DXVECTOR3& B, const D3DXVECTOR3& C);
        void                        AccumulatePartialEdges (void);
        void                        MatchPartialEdges (void);
        
        void                        ResizeVB (const Direct3D& pDirect3D);
        void                        ExtrudeSilhouetteEdge (const Direct3D& pDirect3D, const D3DXVECTOR3& lightDirection, const VertexShadow& A, const VertexShadow& B);
        void                        BuildShadowVolume (const Direct3D& pDirect3D, const D3DXVECTOR3& lightDirection);

    DECLARE_FACTORY (GeometryObject, Shadow);
};

//-----------------------------------------------------------------------------

#endif  //  _GEOMETRY_OBJECT_SHADOW_H_
