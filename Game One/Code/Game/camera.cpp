//-----------------------------------------------------------------------------
// Copyright (C) 1997-2006 Bretton Wade
// All Rights Reserved
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// include files
//-----------------------------------------------------------------------------
#include    "precompile.h"
#include    "camera.h"
#include    "game_object.h"
#include    "direct_3d.h"
#include    "render_state.h"
#include    <xinput.h>

//-----------------------------------------------------------------------------
// constants
//-----------------------------------------------------------------------------
const uInt      c_iTrackingRecordCount = 16;
const uInt      c_iTrackingRecordMask = c_iTrackingRecordCount - 1;
const uInt      c_iCameraTrail = 8;
const uInt      c_iTargetTrail = 2;

//-----------------------------------------------------------------------------
// helper functions
//-----------------------------------------------------------------------------
uInt    NextRecord (uInt record)
{
    return (record + 1) bit_and c_iTrackingRecordMask;
}

//-----------------------------------------------------------------------------
uInt    RecordOffset (uInt record, uInt offset)
{
    return (record - offset) bit_and c_iTrackingRecordMask;
}

//-----------------------------------------------------------------------------
// class methods
//-----------------------------------------------------------------------------
/* void */
Camera::Camera (float fTrailDistance) :
    m_fTrailDistance (fTrailDistance),
    m_iCurrentRecord (0)
{
    // allocate the tracking history
    m_pTracking = NewCall TrackingRecord[c_iTrackingRecordCount];
    
    //XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXx
    // this should be taking an XML node and build projections and camera positions
    // based on information in that node, well, technically the window area should
    // be part of a camera too, and the render loop should loop over cameras... This
    // would enable radar windows in the upper corner, and such like.
    //XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXx
    
    // compute and set the projection transformation.
    //float               fFOV = D3DX_PI * 0.3f;
    float               fFOV = D3DX_PI * 0.4f;
    float               fNearClip = 1;
    float               fFarClip = 100;
    D3DXMatrixPerspectiveFovLH (&m_projectionMatrix, fFOV, Direct3D::GetSingleton ()->GetAspectRatio (), fNearClip, fFarClip);
}

//-----------------------------------------------------------------------------
/* void */
Camera::~Camera (void)
{
    // release the tracking
    delete[] m_pTracking;
}

//-----------------------------------------------------------------------------
void
Camera::SetTarget (const Text& targetName)
{
    // fetch the target
    PtrToGameObject     pTarget = GameObject::Find (targetName);
    
    // store the new target id
    m_targetID = pTarget->GetID ();
    
    // get the target body and use it to initialize the camera state
    PtrToPhysicsBody    pTargetBody = pTarget->GetBody ();
    Activate (pTargetBody->GetPosition (), pTargetBody->GetOrientation ());
}

//-----------------------------------------------------------------------------
void
Camera::Update (void)
{
    // get the target body
    PtrToGameObject     pTarget = GameObject::Find (m_targetID);
    PtrToPhysicsBody    pTargetBody = pTarget->GetBody ();
    
    // update the camera values from the target
    Update (pTargetBody->GetPosition (), pTargetBody->GetOrientation ());
    
    // build the transform
    BuildTransform ();
}

//-----------------------------------------------------------------------------
void
Camera::Activate (const D3DXVECTOR2& vPosition, const D3DXVECTOR2& vForward)
{
    // fill the tracking array with the current parameters
    for (uInt i = 0; i < c_iTrackingRecordCount; i++)
    {
        m_pTracking[i].m_vPosition = vPosition;
        m_pTracking[i].m_vForward = vForward;
    }
}

//-----------------------------------------------------------------------------
void
Camera::Update (const D3DXVECTOR2& vPosition, const D3DXVECTOR2& vForward)
{
    // increment the current record counter
    m_iCurrentRecord = NextRecord (m_iCurrentRecord);
    
    // update the tracking array with our new values
    m_pTracking[m_iCurrentRecord].m_vPosition = vPosition;
    m_pTracking[m_iCurrentRecord].m_vForward = vForward;
}

//-----------------------------------------------------------------------------
void
Camera::BuildTransform (void) const
{
    // fetch the direct 3d object
    Direct3D&           pDirect3D = *(Direct3D::GetSingleton ());
    
    // build and apply the viewing matrix
    D3DXMATRIX          viewingMatrix;
    BuildTransform (&viewingMatrix);
    
    // set the viewing transform for the programmable pipeline
    RenderState::SetProjectionTransform (&m_projectionMatrix);
    RenderState::SetViewingTransform (&viewingMatrix);
}

//-----------------------------------------------------------------------------
void
Camera::BuildTransform (D3DXMATRIX* pTransformOut) const
{
    // look backwards in the array to two previous records for our look
    // from and look at positions
    uInt        iCameraRecord = RecordOffset (m_iCurrentRecord, c_iCameraTrail);
    uInt        iTargetRecord = RecordOffset (m_iCurrentRecord, c_iTargetTrail);
    
    // xinput to get input from the controller
    float           fEyePointY = 15.0f;
    XINPUT_STATE    inputState;
    if (XInputGetState (0, &inputState) == ERROR_SUCCESS)
        fEyePointY += 10.0f * (float (inputState.Gamepad.sThumbRY) / float (-SHRT_MAX));

    // compute the camera and target positions
    D3DXVECTOR2 vCameraPosition = m_pTracking[iCameraRecord].m_vPosition - (m_pTracking[iCameraRecord].m_vForward * m_fTrailDistance);
    D3DXVECTOR2 vTargetPosition = m_pTracking[iTargetRecord].m_vPosition;
    
    // build the camera transform matrix
    D3DXVECTOR3 vEyePoint (vCameraPosition.x, fEyePointY, vCameraPosition.y);
    D3DXVECTOR3 vLookAtPoint (vTargetPosition.x, 0, vTargetPosition.y);
    D3DXVECTOR3 vUpVector (0, 1, 0);
    D3DXMatrixLookAtLH (pTransformOut, &vEyePoint, &vLookAtPoint, &vUpVector);
}

//-----------------------------------------------------------------------------
