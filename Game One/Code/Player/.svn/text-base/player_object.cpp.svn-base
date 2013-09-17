//-----------------------------------------------------------------------------
// Copyright (C) 1997-2006 Bretton Wade
// All Rights Reserved
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// include files
//-----------------------------------------------------------------------------
#include    "precompile.h"
#include    "player_object.h"
#include    "advanced_math.h"

//-----------------------------------------------------------------------------
// useful types
//-----------------------------------------------------------------------------
typedef Map<uInt2, PtrToPlayerObject>   PlayerMap;

//-----------------------------------------------------------------------------
// class methods
//-----------------------------------------------------------------------------
void
PlayerObject::Activate (void)
{
    // find the object by its name, and store off its id
    PtrToGameObject     pGameObject = GameObject::Find (m_gameObjectName);
    Assert (pGameObject);
    m_gameObjectID = pGameObject->GetID ();
}

//-----------------------------------------------------------------------------
void
PlayerObject::AllThink (void)
{
    // loop over all of the players, getting their inputs
    PlayerMap::iterator iter = s_indexByID.begin ();
    PlayerMap::iterator end = s_indexByID.end ();
    while (iter != end)
    {
        PtrToPlayerObject   pPlayer = iter->second;
        ++iter;
        pPlayer->GetInput ();
    }
    
    // loop again to apply the inputs
    iter = s_indexByID.begin ();
    while (iter != end)
    {
        PtrToPlayerObject   pPlayer = iter->second;
        ++iter;
        pPlayer->ApplyInput ();
    }
}

//-----------------------------------------------------------------------------
void
PlayerObject::AllActivate (void)
{
    // loop over all of the players, activating them
    PlayerMap::iterator iter = s_indexByID.begin ();
    PlayerMap::iterator end = s_indexByID.end ();
    while (iter != end)
    {
        PtrToPlayerObject   pPlayer = iter->second;
        ++iter;
        pPlayer->Activate ();
    }
    
    // do a first round of thinking
    AllThink ();
}

//-----------------------------------------------------------------------------
/* void */
PlayerObject::PlayerObject (const PtrToXMLNode& pXMLNode) :
    IndexedObject<PlayerObject> (pXMLNode),
    m_gameObjectID (0)
{
    // get the name of our scene object
    m_gameObjectName = pXMLNode->GetAttribute ("gameobject");
    
    // get the applied forces for this player
    m_fForce = pXMLNode->GetAttribute ("force").ConvertToFloat ();
    m_fTorque = pXMLNode->GetAttribute ("torque").ConvertToFloat ();
    
    // zero the inputs to start
    m_vInput.x = m_vInput.y = 0;
}

//-----------------------------------------------------------------------------
void
PlayerObject::ApplyInput (void)
{
    // apply the inputs to the game object
    PtrToGameObject     pGameObject = GameObject::Find (m_gameObjectID);
    PtrToPhysicsBody    pBody = pGameObject->GetBody ();
    D3DXVECTOR2         vOrientation = pBody->GetOrientation ();
    pBody->ApplyForce (vOrientation * (m_vInput.y * m_fForce));
    pBody->ApplyTorque (m_vInput.x * m_fTorque);
}

//-----------------------------------------------------------------------------
float
PlayerObject::Go (const PtrToPhysicsBody& pBody, const D3DXVECTOR2& vDirection)
{
    // vDirection holds the vector that represents the direction we want to go. 
    // This is essentially ignoring what direction we are already going, which 
    // gets ugly, because of the physics. If we are already generally 
    // travelling towards the target direction, then we mirror our velocity
    // around the target direction. If we aren't, we want to flip our velocity
    // and average it with the target vector
    D3DXVECTOR2         vVelocity = pBody->GetVelocity ();
    D3DXVECTOR2         vTarget = vDirection;
    D3DXVec2Normalize (&vVelocity, &vVelocity);
    D3DXVec2Normalize (&vTarget, &vTarget);
    if ((vVelocity dot vTarget) > 0)
        vTarget = -Reflect (vVelocity, vTarget);
    else
        vTarget = vTarget - (2.0f * vVelocity);
    
    // point in the delta direction, and go at a velocity proportional to
    // how accurately we are pointing the direction we want to go. 0.75 was 
    // derived by experimentation as the most usable value
    float               fDeltaAngle = fabsf (Point (pBody, vTarget));
    float               fDeltaAngleCap = D3DX_PI * 0.75f;
    float               fInputMin = 0.1f;
    float               fInputOffset = 1 - fInputMin;
    if (fDeltaAngle < fDeltaAngleCap)
        m_vInput.y = (((fDeltaAngleCap - fDeltaAngle) / fDeltaAngleCap) * fInputOffset) + fInputMin;
    else
        m_vInput.y = 0;

    // return how accurately we are pointing
    return fDeltaAngle;
}

//-----------------------------------------------------------------------------
float
PlayerObject::GoTo (const PtrToPhysicsBody& pBody, const D3DXVECTOR2& vPosition)
{
    // compute as if we are just trying to go in the right direction, this is 
    // what we'll do until the target position is getting close
    float       fDeltaAngle = Go (pBody, vPosition - pBody->GetPosition ());
    
    // now update the thrust according to our distance to the target position
    // relative to our velocity
    D3DXVECTOR2 vDelta = vPosition - pBody->GetPosition ();
    D3DXVECTOR2 vVelocity = pBody->GetVelocity ();
    float       fDistance = D3DXVec2Length (&vDelta);
    float       fVelocity = D3DXVec2Length (&vVelocity);
    D3DXVec2Scale (&vDelta, &vDelta, 1.0f / fDistance);
    float       fVelocityToPosition = vDelta dot vVelocity;
    
    // check if we're there or close
    float   fCapInput = 20.0f;
    if (fDistance < fCapInput)
    {
        // check if we need to brake
        if (fDistance < fVelocityToPosition)
        {
            // at this point, we don't want to proceed unless the delta angle was
            // very small, and we want to brake or accellerate to make the velocity
            // match the distance to travel. The right way to do this would take 
            // into account the object's mass, but the hack is to heel and toe
            if (fDeltaAngle < (D3DX_PI * 0.33f))
                m_vInput.y *= -1.0f;
        }
        
        // if we didn't brake, then reduce down our accelleration proportionate to 
        // how close we are to the target
        if (m_vInput.y > 0)
        {
            float   fScaleInput = fDistance / fCapInput;
            m_vInput.y *= powf (fScaleInput, 1.5f);
        }
    }
    return fDeltaAngle;
}

//-----------------------------------------------------------------------------
float
PlayerObject::Point (const PtrToPhysicsBody& pBody, const D3DXVECTOR2& vDirection)
{
    // convert the pointing direction vector to an angle
    float               fTargetSpinPosition = atan2f (vDirection.y, vDirection.x);

    // make sure the target spin position is in the same range as the spin
    // position representation used by the physics body
    const float         fTwoPi = D3DX_PI * 2;
	if (fTargetSpinPosition < 0)
		fTargetSpinPosition += fTwoPi;

    // compute the offset between the target angle and the robot orientation
    float               fDeltaSpin = fTargetSpinPosition - pBody->GetSpinPosition ();
    
    // move delta spin into the range -pi .. pi
    if (fDeltaSpin < -D3DX_PI)
        fDeltaSpin += fTwoPi;
    if (fDeltaSpin > D3DX_PI)
        fDeltaSpin -= fTwoPi;
    
    // the goal is to get the spin velocity equal to the delta, we'll do that
    // using three input states
    m_vInput.x = 0;
    if (pBody->GetSpinVelocity () > fDeltaSpin)
        m_vInput.x = -1;
    else if (pBody->GetSpinVelocity () < fDeltaSpin)
        m_vInput.x = 1;

    // return the delta angle
    return fDeltaSpin;
}

//-----------------------------------------------------------------------------
