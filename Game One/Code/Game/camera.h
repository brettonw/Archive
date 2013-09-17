//-----------------------------------------------------------------------------
// Copyright (C) 1997-2006 Bretton Wade
// All Rights Reserved
//-----------------------------------------------------------------------------

#ifndef     _CAMERA_H
#define     _CAMERA_H

//-----------------------------------------------------------------------------
// include files
//-----------------------------------------------------------------------------
#ifndef     _TEXT_H_
#include    "text.h"
#endif  //  _TEXT_H_

//-----------------------------------------------------------------------------
// class definitions
//-----------------------------------------------------------------------------
class Camera
{
    public:
        /* void */          Camera (float fTrailDistance);
        /* void */          ~Camera (void);
        void                SetTarget (const Text& targetName);
        void                Update (void);
        
    protected:
        void                Activate (const D3DXVECTOR2& vPosition, const D3DXVECTOR2& vForward);
        void                Update (const D3DXVECTOR2& vPosition, const D3DXVECTOR2& vForward);
        void                BuildTransform (void) const;
        void                BuildTransform (D3DXMATRIX* pTransformOut) const;
        
    protected:
        struct  TrackingRecord
        {
            D3DXVECTOR2     m_vPosition;
            D3DXVECTOR2     m_vForward;
        };
        TrackingRecord*     m_pTracking;
        float               m_fTrailDistance;
        uInt                m_iCurrentRecord;
        uInt2               m_targetID;
        
        D3DXMATRIX          m_projectionMatrix;
};

//-----------------------------------------------------------------------------

#endif  //  _CAMERA_H
