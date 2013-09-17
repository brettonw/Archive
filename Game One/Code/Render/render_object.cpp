//-----------------------------------------------------------------------------
// Copyright (C) 1997-2006 Bretton Wade
// All Rights Reserved
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// include files
//-----------------------------------------------------------------------------
#include    "precompile.h"
#include    "render_object.h"

//-----------------------------------------------------------------------------
// static class members
//-----------------------------------------------------------------------------
DEFINE_FACTORY (RenderObject);

//-----------------------------------------------------------------------------
// useful types
//-----------------------------------------------------------------------------
typedef Map<uInt2, PtrToRenderObject>   RenderObjectMap;

//-----------------------------------------------------------------------------
// class methods
//-----------------------------------------------------------------------------
void                
RenderObject::AllRender (const Direct3D& pDirect3D)
{
    // start drawing
    D3D_TEST (pDirect3D->BeginScene ());

    // render all the render passes
    RenderObjectMap::iterator   iter = s_indexByID.begin ();
    RenderObjectMap::iterator   end = s_indexByID.end ();
    while (iter != end)
    {
        PtrToRenderObject pRendereObject = iter->second;
        ++iter;
        pRendereObject->Render (pDirect3D);
    }

    // finish off drawing
    D3D_TEST (pDirect3D->EndScene ());
}

//-----------------------------------------------------------------------------
void                
RenderObject::Render (const Direct3D& pDirect3D)
{
    // prepare the scene graph for rendering
    if (m_bPrologue)
    {
        D3DXMATRIX          identityTransform;
        D3DXMatrixIdentity (&identityTransform);
        m_pRoot->Prologue (identityTransform);
    }
    else
    {
        if (m_clearFlags)
        {
            D3D_TEST (pDirect3D->Clear (0, 0, m_clearFlags, m_clearColor, m_clearZ, m_clearStencil));
        }
        
        // build the render pass object based on the light
        RenderPass  renderPass (m_iPass, m_light->m_type, m_light->m_position, m_light->m_direction, m_light->m_color);
            
        // draw the scene graph for the render object
        m_pRoot->Render (pDirect3D, renderPass);
    }
}

//-----------------------------------------------------------------------------
/* void */
RenderObject::RenderObject (const PtrToXMLNode& pXMLNode) :
    IndexedObject<RenderObject> (pXMLNode),
    m_bPrologue (false),
    m_clearFlags (0),
    m_clearColor (0),
    m_clearZ (1.0f),
    m_clearStencil (0)
{
    //<RenderObject name="Ambient" pass="Ambient" light="Ambient" scene="root">
	//	<Clear buffer="TARGET" rgba="0xff0000ff"/>
	//	<Clear buffer="ZBUFFER" z="0"/>
	//  <Clear buffer="STENCIL" stencil="0"/>
    //</RenderObject>
    
    // read the root object
    m_pRoot = SceneObject::Find (pXMLNode->GetAttribute ("scene"));
    Assert (m_pRoot);
    
    // check to see if we need to run a prologue pass
    if (pXMLNode->GetAttribute ("prologue") == "true")
        m_bPrologue = true;

    // read the pass
    Text    pass = pXMLNode->GetAttribute ("pass");
    if (pass == "Ambient")
        m_iPass = AMBIENT_PASS;
    else if (pass == "ShadowStencil")
        m_iPass = SHADOW_STENCIL_PASS;
    else if (pass == "Lighting")
        m_iPass = LIGHTING_PASS;
    
    // find the light
    m_light = LightObject::Find (pXMLNode->GetAttribute ("light"));

    // read the clear nodes
	const XMLNodeList*  pClearNodeList = pXMLNode->GetChildren ("Clear");
	if (pClearNodeList)
	{
	    // loop over the clear list
	    XMLNodeList::const_iterator iter = pClearNodeList->begin ();
	    XMLNodeList::const_iterator end = pClearNodeList->end ();
	    while (iter != end)
	    {
	        // extract the current node
	        PtrToXMLNode        pClearNode = *iter++;
	        
	        // read the clear values
	        if (pClearNode->GetAttribute ("buffer") == "TARGET")
	        {
	            m_clearFlags |= D3DCLEAR_TARGET;
                sscanf (pClearNode->GetAttribute ("rgba"), "%x", &m_clearColor);
                DebugPrint ("    Clear Target (%x)\n", m_clearColor);
	        }
	        else if (pClearNode->GetAttribute ("buffer") == "ZBUFFER")
	        {
	            m_clearFlags |= D3DCLEAR_ZBUFFER;
                sscanf (pClearNode->GetAttribute ("z"), "%f", &m_clearZ);
                DebugPrint ("    Clear Z (%f)\n", m_clearZ);
	        }
	        else
	        {
	            Assert (pClearNode->GetAttribute ("buffer") == "STENCIL");
	            m_clearFlags |= D3DCLEAR_STENCIL;
                sscanf (pClearNode->GetAttribute ("stencil"), "%x", &m_clearStencil);
                DebugPrint ("    Clear Stencil (%x)\n", m_clearStencil);
	        }
   	    }
	}
}

//-----------------------------------------------------------------------------
/* void */
RenderObject::~RenderObject (void)
{
}

//-----------------------------------------------------------------------------
