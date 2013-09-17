//-----------------------------------------------------------------------------
// Copyright (C) 1997-2006 Bretton Wade
// All Rights Reserved
//-----------------------------------------------------------------------------

#ifndef     _SCENE_SCREEN_H_
#define     _SCENE_SCREEN_H_

//-----------------------------------------------------------------------------
// include files
//-----------------------------------------------------------------------------
#ifndef     _SCREEN_H_
#include    "screen.h"
#endif  //  _SCREEN_H_

//-----------------------------------------------------------------------------
// useful type definitions
//-----------------------------------------------------------------------------
MAKE_PTR_TO (SceneScreen);

//-----------------------------------------------------------------------------
// class definitions
//-----------------------------------------------------------------------------
class SceneScreen : public Screen
{
    public:
virtual void                Prologue (void);
virtual void                Heartbeat (void);
virtual void                Epilogue (void);
        
    protected:
        Text                m_levelFileName;
        
    DECLARE_FACTORY (Screen, SceneScreen);
};

//-----------------------------------------------------------------------------

#endif  //  _SCENE_SCREEN_H_
