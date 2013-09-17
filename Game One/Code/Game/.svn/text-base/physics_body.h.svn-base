//-----------------------------------------------------------------------------
// Copyright (C) 1997-2006 Bretton Wade
// All Rights Reserved
//-----------------------------------------------------------------------------

#ifndef     _PHYSICS_BODY_H_
#define     _PHYSICS_BODY_H_

//-----------------------------------------------------------------------------
// include files
//-----------------------------------------------------------------------------
#ifndef     _XML_NODE_H_
#include    "xml_node.h"
#endif  //  _XML_NODE_H_

//-----------------------------------------------------------------------------
// useful type definitions
//-----------------------------------------------------------------------------
MAKE_PTR_TO (PhysicsBody);

//-----------------------------------------------------------------------------
// class definitions
//-----------------------------------------------------------------------------
class PhysicsBody : public CountedObject
{
    public:
        /* void */          PhysicsBody (const PtrToXMLNode& pXMLNode);
virtual /* void */          ~PhysicsBody (void) {}
virtual void                Integrate (float fDeltaTime);
        void                BuildTransform (D3DXMATRIX* pTransformOut) const;
        void                ApplyForce (const D3DXVECTOR2& vForce);
        void                ApplyTorque (float fTorque);
        
        const D3DXVECTOR2&  GetPosition (void) const;
        float               GetSpinPosition (void) const;
        const D3DXVECTOR2&  GetVelocity (void) const;
        float               GetSpinVelocity (void) const;
        
        D3DXVECTOR2         GetOrientation (void) const;

static  void                Collide (const D3DXVECTOR2& vNormal, float fPenetrationDepth, PtrToPhysicsBody& pBodyA, PtrToPhysicsBody& pBodyB);

    protected:
        // translational parameters of a physical body in 2 dimensions
        float               m_fOneOverMass;
        D3DXVECTOR2         m_vPosition;
        D3DXVECTOR2         m_vVelocity;
        D3DXVECTOR2         m_vForce;
        
        // rotational parameters of a physical body in 2 dimensions, e.g. it
        // can only rotate around an axis that is perpendicular to the 2D plane
        float               m_fOneOverMoment;
        float               m_fSpinPosition;
        float               m_fSpinVelocity;
        float               m_fTorque;
        
        // a general viscosity value that will be used for damping
        float               m_fViscosity;
};

//-----------------------------------------------------------------------------

#endif  //  _PHYSICS_BODY_H_
