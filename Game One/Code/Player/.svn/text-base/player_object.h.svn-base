//-----------------------------------------------------------------------------
// Copyright (C) 1997-2006 Bretton Wade
// All Rights Reserved
//-----------------------------------------------------------------------------

#ifndef     _PLAYER_OBJECT_H_
#define     _PLAYER_OBJECT_H_

//-----------------------------------------------------------------------------
// include files
//-----------------------------------------------------------------------------
#ifndef     _INDEXED_OBJECT_H_
#include    "indexed_object.h"
#endif  //  _INDEXED_OBJECT_H_

#ifndef     _GAME_OBJECT_H_
#include    "game_object.h"
#endif  //  _GAME_OBJECT_H_

//-----------------------------------------------------------------------------
// useful type definitions
//-----------------------------------------------------------------------------
MAKE_PTR_TO (PlayerObject);

//-----------------------------------------------------------------------------
// class definitions
//-----------------------------------------------------------------------------
class PlayerObject : public IndexedObject<PlayerObject>
{
    public:
virtual void                GetInput (void) = 0;
virtual void                Activate (void);
        
    public:
static  void                AllThink (void);
static  void                AllActivate (void);

    protected:
        /* void */          PlayerObject (const PtrToXMLNode& pXMLNode);
virtual /* void */          ~PlayerObject (void) {}
        void                ApplyInput (void);
        float               Go (const PtrToPhysicsBody& pBody, const D3DXVECTOR2& vDirection);
        float               GoTo (const PtrToPhysicsBody& pBody, const D3DXVECTOR2& vPosition);
        float               Point (const PtrToPhysicsBody& pBody, const D3DXVECTOR2& vDirection);
        
    protected:
        Text                m_gameObjectName;
        uInt2               m_gameObjectID;
        float               m_fForce;
        float               m_fTorque;
        D3DXVECTOR2         m_vInput;
};

//-----------------------------------------------------------------------------

#endif  //  _PLAYER_OBJECT_H_
