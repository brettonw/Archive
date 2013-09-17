//-----------------------------------------------------------------------------
// Copyright (C) 1997-2006 Bretton Wade
// All Rights Reserved
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// include files
//-----------------------------------------------------------------------------
#include    "precompile.h"
#include    "physics_body.h"
#include    "advanced_math.h"

//-----------------------------------------------------------------------------
// class methods
//-----------------------------------------------------------------------------
/* void */
PhysicsBody::PhysicsBody (const PtrToXMLNode& pXMLNode) :
    m_vVelocity (0, 0),
    m_fSpinVelocity (0),
    m_vForce (0, 0),
    m_fTorque (0)
{
    // extract the mass and moment of inertia from the xml
    float   fMass = pXMLNode->GetAttribute ("mass").ConvertToFloat ();
    float   fMoment = pXMLNode->GetAttribute ("moment").ConvertToFloat ();
    
    // set the mass and moment of inertia values. note that we use 1/x
    // as the stored value, since this is how mass and moment are used.
    // a computational trick for these to represent infinite mass objects
    // is to set these values to 0.
    m_fOneOverMass = (fMass == 0 ? 0 : 1 / fMass);
    m_fOneOverMoment = (fMoment == 0 ? 0 : 1 / fMoment);
    
    // extract the position and orientation from the xml
    Text    position = pXMLNode->GetAttribute ("position");
    sscanf (position, "(%f, %f)", &m_vPosition.x, &m_vPosition.y);
    m_fSpinPosition = (pXMLNode->GetAttribute ("orientation").ConvertToFloat () / 180) * D3DX_PI;

    // extract the damping viscosity from the xml    
    m_fViscosity = -1 * pXMLNode->GetAttribute ("viscosity").ConvertToFloat ();
}

//-----------------------------------------------------------------------------
void
PhysicsBody::Integrate (float fDeltaTime)
{
    // compute forces due to viscous drag
    m_vForce += m_vVelocity * m_fViscosity;
    m_fTorque += m_fSpinVelocity * m_fViscosity;
    
    // compute accelerations from the forces, then clear out the forces
    D3DXVECTOR2 vDeltaVelocity = m_vForce * (m_fOneOverMass * fDeltaTime);
    float       fDeltaSpinVelocity = m_fTorque * m_fOneOverMoment * fDeltaTime;
    m_vForce = D3DXVECTOR2 (0, 0);
    m_fTorque = 0;
    
    // using the midpoint method, compute the position changes
    m_vPosition += ((vDeltaVelocity * 0.5f) + m_vVelocity) * fDeltaTime;
    m_fSpinPosition += ((fDeltaSpinVelocity * 0.5f) + m_fSpinVelocity) * fDeltaTime;
    
    // update the velocities from the deltas
    m_vVelocity += vDeltaVelocity;
    m_fSpinVelocity += fDeltaSpinVelocity;

    // keep the spin position in a math friendly range
    const float fTwoPi = D3DX_PI * 2;
	while (m_fSpinPosition >= fTwoPi)
		m_fSpinPosition -= fTwoPi;
	while (m_fSpinPosition < 0)
		m_fSpinPosition += fTwoPi;
}

//-----------------------------------------------------------------------------
void
PhysicsBody::BuildTransform (D3DXMATRIX* pTransformOut) const
{
    // build the translation
    D3DXMATRIX  translation;
    D3DXMatrixTranslation (&translation, m_vPosition.x, 0, m_vPosition.y);
    
    // build the spin, note that DX works in a left hand system, so we have
    // to pass the inverse of our angle...
    D3DXMATRIX  spin;
    D3DXMatrixRotationY (&spin, -m_fSpinPosition);
    
    // concatenate the matrices into the exported transform
    D3DXMatrixMultiply (pTransformOut, &spin, &translation);
}

//-----------------------------------------------------------------------------
void
PhysicsBody::ApplyForce (const D3DXVECTOR2& vForce)
{
    m_vForce = vForce;
}

//-----------------------------------------------------------------------------
void
PhysicsBody::ApplyTorque (float fTorque)
{
    m_fTorque = fTorque;
}

//-----------------------------------------------------------------------------
const D3DXVECTOR2&
PhysicsBody::GetPosition (void) const
{
    return m_vPosition;
}

//-----------------------------------------------------------------------------
float
PhysicsBody::GetSpinPosition (void) const
{
    return m_fSpinPosition;
}

//-----------------------------------------------------------------------------
const D3DXVECTOR2&
PhysicsBody::GetVelocity (void) const
{
    return m_vVelocity;
}

//-----------------------------------------------------------------------------
float
PhysicsBody::GetSpinVelocity (void) const
{
    return m_fSpinVelocity;
}

//-----------------------------------------------------------------------------
D3DXVECTOR2
PhysicsBody::GetOrientation (void) const
{
    return D3DXVECTOR2 (cosf (m_fSpinPosition), sinf (m_fSpinPosition));
}

//-----------------------------------------------------------------------------
void
PhysicsBody::Collide (const D3DXVECTOR2& vNormal, float fPenetrationDepth, PtrToPhysicsBody& pBodyA, PtrToPhysicsBody& pBodyB)
{
    // compute the mass sum that we'll use for separating the bodies, and
    // for computing the resulting impulse to apply
    float           fOverMassSum = pBodyA->m_fOneOverMass + pBodyB->m_fOneOverMass;
    
    // separate the two bodies by the penetration depth and a little bit
    // more to be sure we don't detect another collision
    float           fRatioA = pBodyA->m_fOneOverMass / fOverMassSum;
    float           fRatioB = 1 - fRatioA;
    pBodyA->m_vPosition += vNormal * (fRatioA * (1 + g_fEpsilon) * fPenetrationDepth);
    pBodyB->m_vPosition -= vNormal * (fRatioB * (1 + g_fEpsilon) * fPenetrationDepth);
    
    // figure the relative velocity along the separating vector
    D3DXVECTOR2     vRelativeVelocity = pBodyA->m_vVelocity - pBodyB->m_vVelocity;
    float           fCollisionVelocity = D3DXVec2Dot (&vNormal, &vRelativeVelocity);
    
    // compute the repulsion impulse
    float           fElasticity = -2;
    float           fImpulse = (fElasticity * fCollisionVelocity) / fOverMassSum;
    
    // apply the impulse to both bodies
    pBodyA->m_vVelocity += vNormal * (fImpulse * pBodyA->m_fOneOverMass);
    pBodyB->m_vVelocity -= vNormal * (fImpulse * pBodyB->m_fOneOverMass);
}

//-----------------------------------------------------------------------------
