//-----------------------------------------------------------------------------
// Copyright (C) 1997-2006 Bretton Wade
// All Rights Reserved
//-----------------------------------------------------------------------------

#ifndef     _SCENE_GRAPH_NODE_H_
#define     _SCENE_GRAPH_NODE_H_

//-----------------------------------------------------------------------------
// include files
//-----------------------------------------------------------------------------
#ifndef     _SCENE_OBJECT_H_
#include    "scene_object.h"
#endif  //  _SCENE_OBJECT_H_

//-----------------------------------------------------------------------------
// class definitions
//-----------------------------------------------------------------------------
class SceneGraphNode : public SceneObject
{
    public:
virtual void                    Prologue (const D3DXMATRIX& transform);
virtual void                    Render (const Direct3D& pDirect3D, const RenderPass& renderPass) const;
        bool                    AddChild (const PtrToSceneObject& pChild);
    
    protected:
        SceneObjectMap          m_children;

    DECLARE_FACTORY (SceneObject, SceneGraphNode);
};

//-----------------------------------------------------------------------------

#endif  //  _SCENE_GRAPH_NODE_H_
