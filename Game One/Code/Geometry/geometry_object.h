//-----------------------------------------------------------------------------
// Copyright (C) 1997-2006 Bretton Wade
// All Rights Reserved
//-----------------------------------------------------------------------------

#ifndef     _GEOMETRY_OBJECT_H_
#define     _GEOMETRY_OBJECT_H_

//-----------------------------------------------------------------------------
// include files
//-----------------------------------------------------------------------------
#ifndef     _DIRECT_3D_H_
#include    "direct_3d.h"
#endif  //  _DIRECT_3D_H_

#ifndef     _INDEXED_OBJECT_H_
#include    "indexed_object.h"
#endif  //  _INDEXED_OBJECT_H_

#ifndef     _SCENE_OBJECT_H_
#include    "scene_object.h"
#endif  //  _SCENE_OBJECT_H_

#ifndef     _VERTEX_H_
#include    "vertex.h"
#endif  //  _VERTEX_H_

//-----------------------------------------------------------------------------
// useful type definitions
//-----------------------------------------------------------------------------
MAKE_PTR_TO (GeometryObject);
typedef TextMap<PtrToGeometryObject>   GeometryObjectMap;

//-----------------------------------------------------------------------------
// class definitions
//-----------------------------------------------------------------------------
class GeometryObject : public IndexedObject <GeometryObject>
{
    public:
virtual void                        Render (const Direct3D& pDirect3D, const RenderPass& renderPass) const;
virtual const VertexVector*         GetVertexVector (void) const;
virtual const IndexVector*          GetIndexVector (void) const;
virtual void                        ClearAccumulator (void);

    protected:
        uInt2                       m_iTriangleCount;
        PtrToD3DVertexBuffer        m_pVertexBuffer;
        PtrToD3DVertexDecl          m_pD3DVertexDecl;
static  VertexVector                s_vVertices;

    protected:
        /* void */                  GeometryObject (const PtrToXMLNode& pXMLNode);
virtual /* void */                  ~GeometryObject (void);

virtual void                        AddTriangle (const Vertex& A, const Vertex& B, const Vertex& C);
virtual void                        BuildObject (bool bDynamic = false);
};

//-----------------------------------------------------------------------------

#endif  //  _GEOMETRY_OBJECT_H_
