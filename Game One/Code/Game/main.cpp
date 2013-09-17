//-----------------------------------------------------------------------------
// Copyright (C) 1997-2006 Bretton Wade
// All Rights Reserved
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// include files
//-----------------------------------------------------------------------------
#include    "precompile.h"
#include    "screen_manager.h"
#include    "task_registrar.h"
#include    "advanced_math.h"

//-----------------------------------------------------------------------------
// force this module to be loaded
//-----------------------------------------------------------------------------
DEFINE_FORCED_LOAD (Main);

//-----------------------------------------------------------------------------
// tasks
//-----------------------------------------------------------------------------
DEFINE_TASK (Main)
{
}

//-----------------------------------------------------------------------------
HANDLE_MESSAGE (Main, msg)
{
    switch (msg)
    {
        case Task::STARTUP:
        {
            #ifndef NDEBUG
            // get the current memory tracking flags value, combine it with the 
            // flags to turn on leak-checking  and boundary checking, and set 
            // the memory tracking flags in the C runtime
            int tmpFlag = _CrtSetDbgFlag (_CRTDBG_REPORT_FLAG);
            tmpFlag |= _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF;
            _CrtSetDbgFlag (tmpFlag);
            
            // Talk about what we just did
            DebugPrint ("Activated memory leak checking and validation!\n");
            #endif
            
            // XXX this should come from the scene
            SetFloatRange (-60.0f, 60.0f);

            // return success so far
            return true;
        }
        case Task::SHUTDOWN:
            return true;
            
        default:
            return false;
    }
}

//-----------------------------------------------------------------------------
// I'm a little black rain cloud, of course
//-----------------------------------------------------------------------------
#include    "game.h"
sInt __stdcall
WinMain (HINSTANCE instance, HINSTANCE prevInstance, LPSTR cmdLine, sInt cmdShow)
{
    // get the task registrar and send the startup message
    TaskRegistrar*  pTaskRegistrar = TaskRegistrar::GetSingleton ();
    if (pTaskRegistrar->SendMessage (Task::STARTUP))
    {
        // XXX Need to do something about parsing the command line, maybe a startup task?
        
        // the result value
        sInt     result = 0;

        // establish a new scope to track memory usage
        {
            // track memory use from here (??? Is that right)
            SNITCH;
            {
                {
                    // load the xml configuration file
                    PtrToXMLNode    pXMLNode = XMLNode::FromFile ("scene.xml");
        
                    // create the scene
                    // XXX this should move to someplace that handles levels
                    CreateScene (pXMLNode);
                }
        
                // message loop
                ScreenManager   scrnm;
                result = scrnm.EventLoop ();
                
                // destroy the scene
                // XXX this should move to someplace that handles levels
                DestroyScene ();
            }
        }
        
        // clean up the task manager to shut down, we deliberately don't do this
        // if start up failed, as we expect Windows to clean up for us
        delete pTaskRegistrar;

        // return the result
        return result;
    }

    // per the WinMain documentation, the application should return 0 if it 
    // failed to enter the message loop
    return 0;
}

//-----------------------------------------------------------------------------
