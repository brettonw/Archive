//-----------------------------------------------------------------------------
// Copyright (C) 1997-2006 Bretton Wade
// All Rights Reserved
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// include files
//-----------------------------------------------------------------------------
#include    "precompile.h"
#include    "scene_graph_node.h"
#include    "direct_3d.h"

//-----------------------------------------------------------------------------
// static class members
//-----------------------------------------------------------------------------
DEFINE_FACTORY (SceneGraphNode);

//-----------------------------------------------------------------------------
// class methods
//-----------------------------------------------------------------------------
void
SceneGraphNode::Prologue (const D3DXMATRIX& transform)
{
    // do the parental thang
    SceneObject::Prologue (transform);
    
    // loop over the children prologue them
    SceneObjectMap::iterator    childIterator = m_children.begin ();
    SceneObjectMap::iterator    end = m_children.end ();
    while (childIterator != end)
    {
        PtrToSceneObject& ptrToSceneObject = childIterator->second;
        ++childIterator;
        ptrToSceneObject->Prologue (m_cachedTransform);
    }
}

//-----------------------------------------------------------------------------
void
SceneGraphNode::Render (const Direct3D& pDirect3D, const RenderPass& renderPass) const
{
    // loop over the children rendering them
    SceneObjectMap::const_iterator   childIterator = m_children.begin ();
    SceneObjectMap::const_iterator   end = m_children.end ();
    while (childIterator != end)
    {
        const PtrToSceneObject& ptrToSceneObject = childIterator->second;
        ++childIterator;
        ptrToSceneObject->Render (pDirect3D, renderPass);
    }
}

//-----------------------------------------------------------------------------
bool
SceneGraphNode::AddChild (const PtrToSceneObject& pSceneObject)
{
    // fetch the name
    Text    name = pSceneObject->GetName ();
    
    // check that it isn't already a defined name
    if (m_children.find (name) == m_children.end ())
    {
        // add the child with that name
        m_children[name] = pSceneObject;
        
        // return true to indicate success
        return true;
    }
    
    // return false to indicate failure
    Assert (false and "Named child is already defined");
    return false;
}

//-----------------------------------------------------------------------------
/* void */
SceneGraphNode::SceneGraphNode (const PtrToXMLNode& pXMLNode) : 
    SceneObject (pXMLNode)
{
    // read in the child scene graph description nodes, and loop over
    // them creating the child nodes and adding them as my children
    const XMLNodeList*          pChildDescriptionList = pXMLNode->GetChildren ("SceneObject");
    XMLNodeList::const_iterator iter = pChildDescriptionList->begin ();
    XMLNodeList::const_iterator end = pChildDescriptionList->end ();
    while (iter != end)
    {
        PtrToXMLNode        pChildDescriptionNode = *iter++;
        PtrToSceneObject    pChildObject = SceneObject::Create (pChildDescriptionNode);
        if (pChildObject)
            AddChild (pChildObject);
    }
}

//-----------------------------------------------------------------------------
/* void */
SceneGraphNode::~SceneGraphNode (void)
{
}

//-----------------------------------------------------------------------------
