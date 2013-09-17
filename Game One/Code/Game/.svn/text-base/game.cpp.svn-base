//-----------------------------------------------------------------------------
// Copyright (C) 1997-2006 Bretton Wade
// All Rights Reserved
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// include files
//-----------------------------------------------------------------------------
#include    "precompile.h"
#include    "direct_3d.h"
#include    "effect.h"
#include    "scene_object.h"
#include    "timer.h"
#include    "game_object.h"
#include    "player_object.h"
#include    "camera.h"
#include    "geometry_object.h"
#include    "render_object.h"

//-----------------------------------------------------------------------------
// constant values
//-----------------------------------------------------------------------------
const float         c_fRefreshSwapRate = 2;

//-----------------------------------------------------------------------------
// global variables
//-----------------------------------------------------------------------------
float               g_fTime = 0;
Camera*             g_pCamera = 0;

//-----------------------------------------------------------------------------
// code for dealing with frame time
//-----------------------------------------------------------------------------
inline 
float
GetFrameTime (void)
{
    // this code uses a base number of vsyncs to pass per frame, and calculates
    // what that time value is
    return c_fRefreshSwapRate / static_cast<float> (Direct3D::GetSingleton ()->GetRefreshRate ());
}

//-----------------------------------------------------------------------------
inline 
float
GetIdleTime (void)
{
    // this code uses a base numbr of vsyncs to pass per frame, and calculates
    // what that time value is
    return (c_fRefreshSwapRate - 1) / static_cast<float> (Direct3D::GetSingleton ()->GetRefreshRate ());
}

//-----------------------------------------------------------------------------
// code for creating the scene
//-----------------------------------------------------------------------------
void
CreateScene (const PtrToXMLNode& pXMLNode)
{
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

    //XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
    // initialize the camera, but this info belongs in the XML
    //XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
    g_pCamera = new Camera (4);
    g_pCamera->SetTarget ("playertank");
//    g_pCamera->SetTarget ("enemytank 1");
    
    // update the camera
    g_pCamera->Update ();
}

//-----------------------------------------------------------------------------
void
DestroyScene (void)
{
    // clean up the camera
    delete g_pCamera;
    g_pCamera = 0;
    
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
void
InnerLoop (void)
{
    // start a timer
    Timer   timer;
    
    // fetch the direct 3d object
    Direct3D&           pDirect3D = *(Direct3D::GetSingleton ());

    // render all of the render passes
    RenderObject::AllRender (pDirect3D);
    
    // update all of the players    
    PlayerObject::AllThink ();

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
}

//-----------------------------------------------------------------------------
