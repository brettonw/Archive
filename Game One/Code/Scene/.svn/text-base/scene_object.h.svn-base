//-----------------------------------------------------------------------------
// Copyright (C) 1997-2006 Bretton Wade
// All Rights Reserved
//-----------------------------------------------------------------------------

#ifndef     _SCENE_OBJECT_H_
#define     _SCENE_OBJECT_H_

//-----------------------------------------------------------------------------
// include files
//-----------------------------------------------------------------------------
#ifndef     _DIRECT_3D_H_
#include    "direct_3d.h"
#endif  //  _DIRECT_3D_H_

#ifndef     _INDEXED_OBJECT_H_
#include    "indexed_object.h"
#endif  //  _INDEXED_OBJECT_H_

#ifndef     _RENDER_PASS_H_
#include    "render_pass.h"
#endif  //  _RENDER_PASS_H_

//-----------------------------------------------------------------------------
// useful type definitions
//-----------------------------------------------------------------------------
MAKE_PTR_TO (SceneObject);
typedef TextMap<PtrToSceneObject>   SceneObjectMap;

//-----------------------------------------------------------------------------
// class definitions
//-----------------------------------------------------------------------------
class SceneObject : public IndexedObject <SceneObject>
{
    public:
virtual void                    Prologue (const D3DXMATRIX& transform);
virtual void                    Render (const Direct3D& pDirect3D, const RenderPass& renderPass) const;

        const D3DXMATRIX&       GetTransform (void) const;
        D3DXMATRIX&             GetTransform (void);
        void                    SetTransform (D3DXMATRIX& transform);
        const D3DXMATRIX&       GetCachedTransform (void) const;

    protected:
        /* void */              SceneObject (const PtrToXMLNode& pXMLNode);
virtual /* void */              ~SceneObject (void) {}

    protected:
        D3DXMATRIX              m_transform;
        D3DXMATRIX              m_cachedTransform;
};

//-----------------------------------------------------------------------------

#endif  //  _SCENE_OBJECT_H_
