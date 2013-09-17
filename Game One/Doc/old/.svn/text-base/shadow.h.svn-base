//-----------------------------------------------------------------------------
// Copyright (C) 2002-2004 Bretton Wade, All rights reserved
// This file may be freely transmitted for educational purposes only
//-----------------------------------------------------------------------------

#ifndef     _SHADOW_H_
#define     _SHADOW_H_

//-----------------------------------------------------------------------------
// include files
//-----------------------------------------------------------------------------
#ifndef     _VERTEX_H_
#include    "vertex.h"
#endif  //  _VERTEX_H_

#ifndef     _DIRECT_3D_H_
#include    "direct_3d.h"
#endif  //  _DIRECT_3D_H_

//-----------------------------------------------------------------------------
// class definitions
//-----------------------------------------------------------------------------
class ShadowA
{
    protected:
        struct PartialEdge
        {
            D3DXVECTOR3             ptA;
            D3DXVECTOR3             ptB;
            D3DXVECTOR3             normal;
            /* void */              PartialEdge (const D3DXVECTOR3& a, const D3DXVECTOR3& b, const D3DXVECTOR3& n) :
                                        ptA (a), ptB (b), normal (n) {}
        };
        
        struct Edge
        {
            D3DXVECTOR3             ptA;
            D3DXVECTOR3             ptB;
            D3DXVECTOR3             normalA;
            D3DXVECTOR3             normalB;
            /* void */              Edge (const D3DXVECTOR3& a, const D3DXVECTOR3& b, const D3DXVECTOR3& na, const D3DXVECTOR3& nb) :
                                        ptA (a), ptB (b), normalA (na), normalB (nb) {}
        };

static  std::list<PartialEdge>      s_edgeAccumulator;

        std::vector<Edge>           m_edgeVec;
        std::vector<D3DXVECTOR3>    m_silhouetteVec;
        std::vector<D3DXVECTOR3>    m_polyVec;
        float                       m_length;

    protected:
        void                        PushTriangle (const D3DXVECTOR3& A, const D3DXVECTOR3& B, const D3DXVECTOR3& C, bool bFlip);
        
    public:
        /* void */                  ShadowA (const PtrToXMLNode& pXMLNode);
        /* void */                  ~ShadowA (void);
        
        void                        AccumulatePartialEdges (D3DPRIMITIVETYPE primType, uInt2 primCount, Vertex* pVertices);
        void                        AccumulatePartialEdges (D3DPRIMITIVETYPE primType, uInt2 primCount, Vertex* pVertices, uInt2* pIndices);
        void                        MatchPartialEdges (void);
        
        void                        FindSilhouetteEdges (const D3DXVECTOR3& lightDirection);
        void                        ExtrudeSilhouetteEdges (const D3DXVECTOR3& lightDirection);
        
        void                        DebugRender (const Direct3D& pDirect3D, const D3DXVECTOR3& lightLocation);
};

//-----------------------------------------------------------------------------

#endif  //  _SHADOW_H_
