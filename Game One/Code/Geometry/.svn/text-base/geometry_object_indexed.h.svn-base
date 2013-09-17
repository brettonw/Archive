//-----------------------------------------------------------------------------
// Copyright (C) 1997-2006 Bretton Wade
// All Rights Reserved
//-----------------------------------------------------------------------------

#ifndef     _GEOMETRY_OBJECT_INDEXED_H_
#define     _GEOMETRY_OBJECT_INDEXED_H_

//-----------------------------------------------------------------------------
// include files
//-----------------------------------------------------------------------------
#ifndef     _GEOMETRY_OBJECT_H_
#include    "geometry_object.h"
#endif  //  _GEOMETRY_OBJECT_H_

//-----------------------------------------------------------------------------
// class definitions
//-----------------------------------------------------------------------------
class GeometryObjectIndexed : public GeometryObject
{
    public:
virtual void                        Render (const Direct3D& pDirect3D, const RenderPass& renderPass) const;
virtual const IndexVector*          GetIndexVector (void) const;
virtual void                        ClearAccumulator (void);

    protected:
        uInt2                       m_iVertexCount;
        PtrToD3DIndexBuffer         m_pIndexBuffer;
static  IndexVector                 s_vIndices;

    protected:
        /* void */                  GeometryObjectIndexed (const PtrToXMLNode& pXMLNode);
virtual /* void */                  ~GeometryObjectIndexed (void);

        Index                       AddVertex (const Vertex& vertex);
virtual void                        AddTriangle (const Vertex& A, const Vertex& B, const Vertex& C);
virtual void                        BuildObject (bool bDynamic = false);
};

//-----------------------------------------------------------------------------

#endif  //  _GEOMETRY_OBJECT_INDEXED_H_
