//-----------------------------------------------------------------------------
// Copyright (C) 1997-2006 Bretton Wade
// All Rights Reserved
//-----------------------------------------------------------------------------

#ifndef     _PLAYER_ROBOT_H_
#define     _PLAYER_ROBOT_H_

//-----------------------------------------------------------------------------
// include files
//-----------------------------------------------------------------------------
#ifndef     _PLAYER_OBJECT_H_
#include    "player_object.h"
#endif  //  _PLAYER_OBJECT_H_

//-----------------------------------------------------------------------------
// class definitions
//-----------------------------------------------------------------------------
class PlayerRobot : public PlayerObject
{
    public:
virtual void                GetInput (void);
virtual void                Activate (void);
    
    protected:
        D3DXVECTOR2         AvoidObstacles (const PtrToGameObject& pGameObject, const D3DXVECTOR2& vDirection);
        
    protected:
        Text                m_targetName;
        uInt2               m_targetID;
        bool                bAttack;
        
        uInt2               m_iTrackstate;
        
    DECLARE_FACTORY (PlayerObject, PlayerRobot);
};

//-----------------------------------------------------------------------------

#endif  //  _PLAYER_ROBOT_H_
