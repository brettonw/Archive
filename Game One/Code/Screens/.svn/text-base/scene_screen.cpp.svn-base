//-----------------------------------------------------------------------------
// Copyright (C) 1997-2006 Bretton Wade
// All Rights Reserved
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// include files
//-----------------------------------------------------------------------------
#include    "precompile.h"
#include    "scene_screen.h"

#include    "timer.h"

#include    "direct_3d.h"
#include    "effect.h"
#include    "scene_object.h"
#include    "game_object.h"
#include    "player_object.h"
#include    "camera.h"
#include    "geometry_object.h"
#include    "render_object.h"

//-----------------------------------------------------------------------------
// static class members
//-----------------------------------------------------------------------------
DEFINE_FACTORY (SceneScreen);

//-----------------------------------------------------------------------------
// class methods
//-----------------------------------------------------------------------------
void                
SceneScreen::Prologue (void)
{
    // load the xml configuration file
    PtrToXMLNode    pXMLNode = XMLNode::FromFile ("scene.xml");
    
    LOAD_FROM_XML (Effect, pXMLNode);
    
    // load all geometry from the xml
    {
	    // fetch the list of geometry from the xml and check for validity
	    const XMLNodeList*  pGeomNodeList = pXMLNode->GetChildren ("GeometryObject");
	    if (pGeomNodeList)
	    {
	        // loop over the geometry list
	        XMLNodeList::const_iterator iter = pGeomNodeList->begin ();
	        XMLNodeList::const_iterator end = pGeomNodeList->end ();
	        while (iter != end)
	        {
	            // extract the current node
	            PtrToXMLNode        pGeomNode = *iter++;
	            
	            // create the geometry, and then clear the accumulator
	            PtrToGeometryObject pGeometryObject = GeometryObject::Create (pGeomNode);
	            pGeometryObject->ClearAccumulator ();
   	        }
	    }
    }
    
    LOAD_FROM_XML (SceneObject, pXMLNode);
    LOAD_FROM_XML (LightObject, pXMLNode);
    LOAD_FROM_XML (RenderObject, pXMLNode);
    LOAD_FROM_XML (GameObject, pXMLNode);
    LOAD_FROM_XML (PlayerObject, pXMLNode);
        
    // take a first shot at updating everything to be sure it is all in the 
    // right place when we start
    GameObject::AllUpdate (0);
    PlayerObject::AllActivate ();

/*
    //XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
    // initialize the camera, but this info belongs in the XML
    //XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
    g_pCamera = new Camera (5);
    g_pCamera->SetTarget ("playertank");
//    g_pCamera->SetTarget ("enemytank 1");
    
    // update the camera
    g_pCamera->Update ();
*/
}

//-----------------------------------------------------------------------------
void                
SceneScreen::Heartbeat (void)
{
    // start a timer
    Timer   timer;
    
    // fetch the direct 3d object
    Direct3D&           pDirect3D = *(Direct3D::GetSingleton ());

    // render all of the render passes
    RenderObject::AllRender (pDirect3D);
    
    // update all of the players    
    PlayerObject::AllThink ();
    /*

    // get the frame time
    float   fFrameTime = GetFrameTime ();
    float   fIdleTime = (GetIdleTime () + fFrameTime) * 0.5f;

    // update the game objects
    GameObject::AllUpdate (fFrameTime);

    // update the camera
    g_pCamera->Update ();

    // idle until a call to present will display on the right v-blank cycle
    //Assert (timer.Elapsed () < fFrameTime);
    while (timer.Elapsed () < fIdleTime) {}
    
    // present
    D3D_TEST (pDirect3D->Present (0, 0, 0, 0));
    
    // advance the clock
    g_fTime += fFrameTime;
    */
}

//-----------------------------------------------------------------------------
void                
SceneScreen::Epilogue (void)
{
    // clean out the game systems in dependency order
    PlayerObject::RemoveAll ();
    SceneObject::RemoveAll ();
    RenderObject::RemoveAll ();
    LightObject::RemoveAll ();
    GameObject::RemoveAll ();
    GeometryObject::RemoveAll ();
    
    // clean out the DX effects
    Effect::RemoveAll ();
}

//-----------------------------------------------------------------------------
/* void */
SceneScreen::SceneScreen (const PtrToXMLNode& pXMLNode) :
    Screen (pXMLNode)
{
}

//-----------------------------------------------------------------------------
/* void */
SceneScreen::~SceneScreen (void)
{
}

//-----------------------------------------------------------------------------
