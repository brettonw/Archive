//-----------------------------------------------------------------------------
// Copyright (C) 1997-2006 Bretton Wade
// All Rights Reserved
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// include files
//-----------------------------------------------------------------------------
#include    "precompile.h"
#include    "player_robot.h"
#include    "game_object.h"
#include    "advanced_math.h"

//-----------------------------------------------------------------------------
// static class members
//-----------------------------------------------------------------------------
DEFINE_FACTORY (PlayerRobot);


#define TRACKPOINTCOUNT 12
#define CHECKPOINTRADIUS 18
static float     trackPoints[TRACKPOINTCOUNT][2];// = {{0.0f,40.0f}, {28.28f,28.28f}, {40.0f,0.0f}, {28.28f,-28.28f}, {0.0f,-40.0f}, {-28.28f,-28.28f}, {-40.0f,0.0f}, {-28.28f,28.28f}};
static float   fTrackRadius = 35.0f;

//-----------------------------------------------------------------------------
// class methods
//-----------------------------------------------------------------------------
void
PlayerRobot::GetInput (void)
{
    // get the robot body
    PtrToGameObject     pGameObject = GameObject::Find (m_gameObjectID);
    PtrToPhysicsBody    pBody = pGameObject->GetBody ();
    
    // we use a simple state variable to determine if we are getting too close 
    // to the arena edge, and change our behavior if we are
    D3DXVECTOR2         vDelta;
    float               fDistanceFromOrigin = D3DXVec2Length (&pBody->GetPosition ());
    if (bAttack)
    {
        if (fDistanceFromOrigin < 40)
        {
            // get the target body
            PtrToGameObject     pTarget = GameObject::Find (m_targetID);
            PtrToPhysicsBody    pTargetBody = pTarget->GetBody ();
    
            // compute the delta vector to the target
            vDelta = pTargetBody->GetPosition () - pBody->GetPosition ();
            
            // a simple prediction scheme is to use the current distance to the target
            // and our current speed to figure a lookahead time, then use that time
            // to predict the location of our target. We then recalculate the delta
            // vector to that predicted location. This has error, but the error gets 
            // smaller as we get closer to the target
            float               fDistance = D3DXVec2Length (&vDelta);
            float               fSpeed = D3DXVec2Length (&pBody->GetVelocity ());
            if (fSpeed < 1)
                fSpeed = 1;
            float               fLookaheadTime = (fDistance / fSpeed) * 0.25f;
            D3DXVECTOR2         vPredictedPosition = pTargetBody->GetPosition () + (pTargetBody->GetVelocity () * fLookaheadTime);
            vDelta = vPredictedPosition - pBody->GetPosition ();
        }
        else
        {
            bAttack = false;
        }
    }
    else
    {
        vDelta = D3DXVECTOR2 (trackPoints[m_iTrackstate]) - pBody->GetPosition ();
        if (D3DXVec2Length (&vDelta) < float (CHECKPOINTRADIUS))
            m_iTrackstate = (m_iTrackstate + 1) % TRACKPOINTCOUNT;
    /*
        // if we aren't attacking, we're headed for the center - keep going
        // straight towards the origin until we get pretty close, then attack
        if (fDistanceFromOrigin < 10)
            bAttack = true;
        else
            vDelta = -pBody->GetPosition ();
    */
    }
    
    // make sure we don't hit anything besides our target along the way
    vDelta = AvoidObstacles (pGameObject, vDelta * 0.9f);
    
    // go in the direction we've chosen (vDelta)
    Go (pBody, vDelta);
}

//-----------------------------------------------------------------------------
void
PlayerRobot::Activate (void)
{
    // find the target object by its name, and store off its id
    PtrToGameObject     pGameObject = GameObject::Find (m_targetName);
    Assert (pGameObject);
    m_targetID = pGameObject->GetID ();
    
    // do the parental thang
    PlayerObject::Activate ();
}

//-----------------------------------------------------------------------------
D3DXVECTOR2
PlayerRobot::AvoidObstacles (const PtrToGameObject& pGameObject, const D3DXVECTOR2& vDirection)
{
    // copy the target direction vector so we can modify it
    D3DXVECTOR2         vDelta = vDirection;
    
    // check to see if this target direction will cause a collision with
    // something we don't want to collide with
    PtrToGameObject     pCollisionCheck = GameObject::CheckCollision (pGameObject, vDelta);
    if (pCollisionCheck)
    {
        // get the collision body
        PtrToPhysicsBody    pBody = pGameObject->GetBody ();
        PtrToPhysicsBody    pCollisionBody = pCollisionCheck->GetBody ();
        
        // compute the vector to the collision, it's length, and normalize it
        D3DXVECTOR2         vCollision = pCollisionBody->GetPosition () - pBody->GetPosition ();
        float               fCollisionLength = D3DXVec2Length (&vCollision);
        vCollision /= fCollisionLength;
        
        // compute the scale that must be applied to the avoidance vector to 
        // make it miss the obstacle, and adjust the collision radius with it
        float               fCollisionRadius = pCollisionCheck->GetRadius () + pGameObject->GetRadius ();
        float               fSinTheta = fCollisionRadius / fCollisionLength;
        float               fTheta = asinf (fSinTheta);
        float               fCosTheta = cosf (fTheta);
        fCollisionRadius /= fCosTheta;
        fCollisionRadius *= 1.05f;
        
        // compute the avoidance vector
        D3DXVECTOR2         vAvoid (vCollision.y, -vCollision.x);
        vAvoid *= fCollisionRadius;
        
        // figure out which side of the obstacle the target direction is on
        if ((vCollision cross vDelta) < 0)
        {
            // obstacle is on the left side, so veer to its right
            vDelta = (pCollisionBody->GetPosition () + vAvoid) - pBody->GetPosition ();
        }
        else
        {
            // obstacle is on the right side, so veer to its left
            vDelta = (pCollisionBody->GetPosition () - vAvoid) - pBody->GetPosition ();
        }
    }
    
    // return the adjusted vector
    return vDelta;
}

//-----------------------------------------------------------------------------
/* void */
PlayerRobot::PlayerRobot (const PtrToXMLNode& pXMLNode) :
    PlayerObject (pXMLNode),
    m_targetID (0),
    bAttack (false),
    m_iTrackstate (0)
{
    // get the name of our scene object
    m_targetName = pXMLNode->GetAttribute ("target");
    
    for (uInt2 i = 0; i < TRACKPOINTCOUNT; i++)
    {
        float   theta = ((D3DX_PI * 2.0f) / float (TRACKPOINTCOUNT)) * float (i);
        trackPoints[i][0] = cosf (theta) * fTrackRadius;
        trackPoints[i][1] = sinf (theta) * fTrackRadius;
    }
}

//-----------------------------------------------------------------------------
/* void */
PlayerRobot::~PlayerRobot (void)
{
}

//-----------------------------------------------------------------------------
