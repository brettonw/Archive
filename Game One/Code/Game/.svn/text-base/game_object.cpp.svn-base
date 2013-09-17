//-----------------------------------------------------------------------------
// Copyright (C) 1997-2006 Bretton Wade
// All Rights Reserved
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// include files
//-----------------------------------------------------------------------------
#include    "precompile.h"
#include    "game_object.h"
#include    "scene_object.h"
#include    "direct_music.h"
#include    "advanced_math.h"

//-----------------------------------------------------------------------------
// static class members
//-----------------------------------------------------------------------------
DEFINE_FACTORY (GameObject);

//-----------------------------------------------------------------------------
// useful types
//-----------------------------------------------------------------------------
typedef Map<uInt2, PtrToGameObject>    GameObjectMap;

//-----------------------------------------------------------------------------
// functions
//-----------------------------------------------------------------------------
float
SectCircleCircle (const PtrToGameObject& p0, const D3DXVECTOR2& vVel0, const PtrToGameObject& p1)
{
	PtrToPhysicsBody    pBody0 = p0->GetBody ();
	PtrToPhysicsBody    pBody1 = p1->GetBody ();
	D3DXVECTOR2         C = pBody0->GetPosition () - pBody1->GetPosition ();
	D3DXVECTOR2         V = vVel0 - pBody1->GetVelocity ();
	float               d = p0->GetRadius () + p1->GetRadius ();
	float               fRoot1;
	float               fRoot2;
	if (Dist (C, V, d, fRoot1, fRoot2))
	{
        if (fRoot1 < 0)
        {
            if (fRoot2 > -fRoot1)
	            return 0;
            else
	            return FLT_MAX;
        }
        else
	        return fRoot1;
    }
	return FLT_MAX;
}

//-----------------------------------------------------------------------------
// class methods
//-----------------------------------------------------------------------------
void
GameObject::Update (float fDeltaTime)
{
    // move the object this piece represents
    m_pBody->Integrate (fDeltaTime);
    
    // update the transform for this piece
    PtrToSceneObject    pSceneObject = SceneObject::Find (m_sceneObject);
    if (pSceneObject)
    {
        D3DXMATRIX  transform;
        m_pBody->BuildTransform (&transform);
        pSceneObject->SetTransform (transform);
    }
}

//-----------------------------------------------------------------------------
PtrToPhysicsBody&
GameObject::GetBody (void)
{
    return m_pBody;
}

//-----------------------------------------------------------------------------
const PtrToPhysicsBody&
GameObject::GetBody (void) const
{
    return m_pBody;
}

//-----------------------------------------------------------------------------
float
GameObject::GetRadius (void) const
{
    return m_fRadius;
}

//-----------------------------------------------------------------------------
PtrToGameObject
GameObject::CheckCollision (const PtrToGameObject& pGameObject, const D3DXVECTOR2& vDelta)
{
    // set up the result values
    PtrToGameObject         pResult;
    float                   fClosest = 1;
    
    // now loop over all the objects to find the closest object the intersects
    // the ray from our position in the delta direction
    GameObjectMap::iterator iter = s_indexByID.begin ();
    GameObjectMap::iterator end = s_indexByID.end ();
    while (iter != end)
    {
        // get the current game object, but skip myself
        PtrToGameObject pTestGameObject = iter->second;
        ++iter;
        if (pTestGameObject != pGameObject)
        {
            float   fSect = SectCircleCircle (pGameObject, vDelta, pTestGameObject);
            if (fSect < fClosest)
            {
                fClosest = fSect;
                pResult = pTestGameObject;
            }
        }
    }
    
    // return the closest found object that actually represents a collision
    return pResult;
}

//-----------------------------------------------------------------------------
void
GameObject::AllUpdate (float fDeltaTime)
{
    // update the game objects
    GameObjectMap::iterator iter = s_indexByID.begin ();
    GameObjectMap::iterator end = s_indexByID.end ();
    while (iter != end)
    {
        PtrToGameObject pGameObject = iter->second;
        ++iter;
        pGameObject->Update (fDeltaTime);
    }
    
    // loop over all of the objects to do some simple collision detection
    iter = s_indexByID.begin ();
    while (iter != end)
    {
        // fetch the game object to test, and cache its body and
        // radius since we will use those values a lot
        PtrToGameObject     pGameObjectA = iter->second;
        float               fRadiusA = pGameObjectA->GetRadius ();
        PtrToPhysicsBody    pBodyA = pGameObjectA->GetBody ();
        
        // increment the iterator
        ++iter;
        
        // loop over all of the remaining game objects
        GameObjectMap::iterator iter2 = iter;
        while (iter2 != end)
        {
            // fetch the second game object to test
            PtrToGameObject pGameObjectB = iter2->second;
            ++iter2;
            
            // fetch the body for the second object
            PtrToPhysicsBody    pBodyB = pGameObjectB->GetBody ();
            
            // compute the normal vector of the collision plane, and
            // its length
            D3DXVECTOR2         vNormal = pBodyA->GetPosition () - pBodyB->GetPosition ();
            float               fNormalLength = D3DXVec2Length (&vNormal);
            
            // compute the length below which we are in collision, and check
            // to see if we are below that
            float               fMinSeparatingDistance = fRadiusA + pGameObjectB->GetRadius ();
            if (fNormalLength < fMinSeparatingDistance)
            {
                // play a sound to signify the impact
                DirectMusic::GetSingleton ()->PlaySound ("impact");
                
                // compute the penetration depth
                float   fPenetrationDepth = fMinSeparatingDistance - fNormalLength;
                
                // compute the collision
                PhysicsBody::Collide (vNormal / fNormalLength, fPenetrationDepth, pBodyA, pBodyB);
            }
        }
    }
}

//-----------------------------------------------------------------------------
/* void */
GameObject::GameObject (const PtrToXMLNode& pXMLNode) :
    IndexedObject<GameObject> (pXMLNode)
{
    // get the name of our scene object
    m_sceneObject = pXMLNode->GetAttribute ("sceneobject");
    
    // load the physics object from the xml
    PtrToXMLNode    pPhysicsBodyNode = pXMLNode->GetChild ("PhysicsBody");
    m_pBody = new PhysicsBody (pPhysicsBodyNode);

    // get the bounding radius of our scene object
    m_fRadius = pXMLNode->GetAttribute ("radius").ConvertToFloat ();
}

//-----------------------------------------------------------------------------
/* void */
GameObject::~GameObject (void)
{
}

//-----------------------------------------------------------------------------
