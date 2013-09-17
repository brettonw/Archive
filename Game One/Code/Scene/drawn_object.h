//-----------------------------------------------------------------------------
// Copyright (C) 1997-2006 Bretton Wade
// All Rights Reserved
//-----------------------------------------------------------------------------

#ifndef     _DRAWN_OBJECT_H_
#define     _DRAWN_OBJECT_H_

//-----------------------------------------------------------------------------
// include files
//-----------------------------------------------------------------------------
#ifndef     _SCENE_OBJECT_H_
#include    "scene_object.h"
#endif  //  _SCENE_OBJECT_H_

#ifndef     _GEOMETRY_OBJECT_H_
#include    "geometry_object.h"
#endif  //  _GEOMETRY_OBJECT_H_

//-----------------------------------------------------------------------------
// class definitions
//-----------------------------------------------------------------------------
class DrawnObject : public SceneObject
{
    public:
virtual void                    Prologue (const D3DXMATRIX& transform);
virtual void                    Render (const Direct3D& pDirect3D, const RenderPass& renderPass) const;

    protected:
        PtrToGeometryObject     m_pGeometryObject;
        D3DXMATRIX              m_cachedInverse;

    DECLARE_FACTORY (SceneObject, DrawnObject);
};

//-----------------------------------------------------------------------------

#endif  //  _DRAWN_OBJECT_H_
