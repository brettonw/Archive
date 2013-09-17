//-----------------------------------------------------------------------------
// Copyright (C) 2002 Bretton Wade, All rights reserved
// Project 5 source file
// This file may be freely transmitted for educational purposes only
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// include files
//-----------------------------------------------------------------------------
#include    "precompile.h"
#include    "physics_body_rocket.h"

//-----------------------------------------------------------------------------
// template functions
//-----------------------------------------------------------------------------
template<class someType>
someType
ComputeDerivative (const someType& velocity, const someType& force, float fViscosity, float fOneOverInertia)
{
    // compute the acceleration and return it
    return (force + (velocity * fViscosity)) * fOneOverInertia;
}

//-----------------------------------------------------------------------------
template<class someType>
someType
IntegrateVelocity (float fDeltaTime, const someType& velocity, const someType& force, float fViscosity, float fOneOverInertia)
{
    someType    k1 = fDeltaTime * ComputeDerivative<someType> (velocity, force, fViscosity, fOneOverInertia);
    someType    k2 = fDeltaTime * ComputeDerivative<someType> (velocity +  (k1 * 0.5f), force, fViscosity, fOneOverInertia);
    someType    k3 = fDeltaTime * ComputeDerivative<someType> (velocity + (k2 * 0.5f), force, fViscosity, fOneOverInertia);
    someType    k4 = fDeltaTime * ComputeDerivative<someType> (velocity + k3, force, fViscosity, fOneOverInertia);
    
    // compute the final velocity change and return it
    return (k2 + k2 + k2 + k3 + k3 + k4) / 6;
}

//-----------------------------------------------------------------------------
// class methods
//-----------------------------------------------------------------------------
/* void */
PhysicsBodyRocket::PhysicsBodyRocket (const PtrToXMLNode& pXMLNode) :
    PhysicsBodyDamped (pXMLNode)
{
}

//-----------------------------------------------------------------------------
void
PhysicsBodyRocket::Integrate (float fDeltaTime)
{
    // deal with thrust
    m_vVelocity += IntegrateVelocity<D3DXVECTOR2> (fDeltaTime, m_vVelocity, m_vForce, m_fViscosity, m_fOneOverMass);
    m_vPosition += m_vVelocity * fDeltaTime;
    
    // deal with spin
    m_fSpinVelocity += IntegrateVelocity<float> (fDeltaTime, m_fSpinVelocity, m_fTorque, m_fViscosity, m_fOneOverMoment);
    m_fSpinPosition += m_fSpinVelocity * fDeltaTime;
    
    // keep the spin position in a math friendly range
    float   fTwoPi = D3DX_PI * 2;
	while (m_fSpinPosition >= fTwoPi)
		m_fSpinPosition -= fTwoPi;
	while (m_fSpinPosition < 0)
		m_fSpinPosition += fTwoPi;
}

//-----------------------------------------------------------------------------
