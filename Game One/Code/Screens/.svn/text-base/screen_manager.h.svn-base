//-----------------------------------------------------------------------------
// Copyright (C) 1997-2006 Bretton Wade
// All Rights Reserved
//-----------------------------------------------------------------------------

#ifndef     _SCREEN_MANAGER_H_
#define     _SCREEN_MANAGER_H_

//-----------------------------------------------------------------------------
// include files
//-----------------------------------------------------------------------------
#ifndef     _SINGLETON_H_
#include    "singleton.h"
#endif  //  _SINGLETON_H_

#ifndef     _SCREEN_H_
#include    "screen.h"
#endif  //  _SCREEN_H_

//-----------------------------------------------------------------------------
// class definitions
//-----------------------------------------------------------------------------
class ScreenManager : public Singleton<ScreenManager>
{
    public:
        /* void */          ScreenManager (void);
        /* void */          ~ScreenManager (void);
        sInt                EventLoop (void);
        
static  ScreenManager*      GetSingleton (void);

    protected:
    // stack of screens, need a default screen that we'll look for - it will epilogue fade out
        //List<Screen>        m_screenStack;
};

//-----------------------------------------------------------------------------

#endif  //  _SCREEN_MANAGER_H_
