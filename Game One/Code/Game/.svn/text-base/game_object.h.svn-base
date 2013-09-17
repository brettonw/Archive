//-----------------------------------------------------------------------------
// Copyright (C) 1997-2006 Bretton Wade
// All Rights Reserved
//-----------------------------------------------------------------------------

#ifndef     _GAME_OBJECT_H_
#define     _GAME_OBJECT_H_

//-----------------------------------------------------------------------------
// include files
//-----------------------------------------------------------------------------
#ifndef     _PHYSICS_BODY_H_
#include    "physics_body.h"
#endif  //  _PHYSICS_BODY_H_

#ifndef     _INDEXED_OBJECT_H_
#include    "indexed_object.h"
#endif  //  _INDEXED_OBJECT_H_

//-----------------------------------------------------------------------------
// useful type definitions
//-----------------------------------------------------------------------------
MAKE_PTR_TO (GameObject);

//-----------------------------------------------------------------------------
// class definitions
//-----------------------------------------------------------------------------
class GameObject : public IndexedObject<GameObject>
{
    public:
        void                    Update (float fDeltaTime);
        PtrToPhysicsBody&       GetBody (void);
        const PtrToPhysicsBody& GetBody (void) const;
        float                   GetRadius (void) const;
        
    public:
static  PtrToGameObject         CheckCollision (const PtrToGameObject& pGameObject, const D3DXVECTOR2& vDelta);
static  void                    AllUpdate (float fDeltaTime);

    protected:
        Text                    m_sceneObject;
        PtrToPhysicsBody        m_pBody;
        float                   m_fRadius;
        
    DECLARE_FACTORY (GameObject, GameObject);
};

//-----------------------------------------------------------------------------

#endif  //  _GAME_OBJECT_H_
