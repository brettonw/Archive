//-----------------------------------------------------------------------------
// Copyright (C) 1997-2006 Bretton Wade
// All Rights Reserved
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// include files
//-----------------------------------------------------------------------------
#include    "precompile.h"
#include    "screen_manager.h"
#include    "window.h"
#include    "game.h"

//-----------------------------------------------------------------------------
// class methods
//-----------------------------------------------------------------------------
/* void */
ScreenManager::ScreenManager (void)
{
    // load all of the screens
    PtrToXMLNode    pXMLNode = XMLNode::FromFile ("Screens.xml");
    Assert (pXMLNode);
    LOAD_FROM_XML(Screen, pXMLNode);
}

//-----------------------------------------------------------------------------
/* void */
ScreenManager::~ScreenManager (void)
{
    Screen::RemoveAll ();
}

//-----------------------------------------------------------------------------
sInt                
ScreenManager::EventLoop (void)
{
    // loop until we are done using idle processing for the inner loop
    MSG     currentMessage;
    bool    finished = false;
    while (!finished)
        if (PeekMessage (&currentMessage, 0, 0, 0, PM_NOREMOVE))
            if (GetMessage (&currentMessage, 0, 0, 0) == TRUE)
                DispatchMessage (&currentMessage);
            else
                finished = true;
        else if (not g_bApplicationPaused)
        {
            // here we get the current screen, and pass time to it by
            // calling the heartbeat method
            // XXX for now just call the inner loop
            InnerLoop ();
        }
        else
            Sleep (0);

    // return the result of the quit message
    return static_cast<sInt> (currentMessage.wParam);
}

//-----------------------------------------------------------------------------
ScreenManager*      
ScreenManager::GetSingleton (void)
{
    if (not IsValid ())
        NewCall ScreenManager;
    return Singleton<ScreenManager>::GetSingleton ();
}

//-----------------------------------------------------------------------------
