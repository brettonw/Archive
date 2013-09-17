//-----------------------------------------------------------------------------
// Copyright (C) 1997-2006 Bretton Wade
// All Rights Reserved
//-----------------------------------------------------------------------------

#ifndef     _RENDER_OBJECT_H_
#define     _RENDER_OBJECT_H_

//-----------------------------------------------------------------------------
// include files
//-----------------------------------------------------------------------------
#ifndef     _LIGHT_OBJECT_H_
#include    "light_object.h"
#endif  //  _LIGHT_OBJECT_H_

#ifndef     _SCENE_OBJECT_H_
#include    "scene_object.h"
#endif  //  _SCENE_OBJECT_H_

//-----------------------------------------------------------------------------
// useful type definitions
//-----------------------------------------------------------------------------
MAKE_PTR_TO (RenderObject);

//-----------------------------------------------------------------------------
// class definitions
//-----------------------------------------------------------------------------
class RenderObject : public IndexedObject<RenderObject>
{
    public:
static  void                AllRender (const Direct3D& pDirect3D);
		void                Render (const Direct3D& pDirect3D);

    protected:
        PtrToSceneObject    m_pRoot;
        bool                m_bPrologue;
        DWORD               m_clearFlags;
        D3DCOLOR            m_clearColor;
        float               m_clearZ;
        DWORD               m_clearStencil;
        uInt                m_iPass;
        PtrToLightObject    m_light;

    DECLARE_FACTORY (RenderObject, RenderObject);
};

//-----------------------------------------------------------------------------

#endif  //  _RENDER_OBJECT_H_
