//-----------------------------------------------------------------------------
// Copyright (C) 2002-2004 Bretton Wade, All rights reserved
// This file may be freely transmitted for educational purposes only
//-----------------------------------------------------------------------------

#ifndef     _POLYHEDRA_H_
#define     _POLYHEDRA_H_

//-----------------------------------------------------------------------------
// include files
//-----------------------------------------------------------------------------
#ifndef     _DRAWN_OBJECT_H_
#include    "drawn_object.h"
#endif  //  _DRAWN_OBJECT_H_

//-----------------------------------------------------------------------------
// class definitions
//-----------------------------------------------------------------------------
class Polyhedra : public DrawnObject
{
    public:
virtual void                    Render (const Direct3D& pDirect3D, UINT iRenderPass) const;

    protected:
        /* void */              Polyhedra (const PtrToXMLNode& pXMLNode, uInt2 iFaceCount, uInt2 iVerticesPerFace);
virtual /* void */              ~Polyhedra (void);
        void                    ComputeNormals (Vertex* pVertexData);
        void                    BuildIndex (Vertex* pVertexData);

    protected:
        uInt2                   m_iFaceCount;
        uInt2                   m_iVerticesPerFace;
        uInt2                   m_iTrianglesPerFace;
        uInt2                   m_iTriangleCount;
        PtrToD3DIndexBuffer     m_pIndexBuffer;
};

//-----------------------------------------------------------------------------

#endif  //  _POLYHEDRA_H_
