//-----------------------------------------------------------------------------
// Copyright (C) 1997-2006 Bretton Wade
// All Rights Reserved
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// include files
//-----------------------------------------------------------------------------
#include    "precompile.h"
#include    "window.h"
#include    "direct_3d.h"
#include    "task.h"

//-----------------------------------------------------------------------------
// global variables
//-----------------------------------------------------------------------------
bool    g_bApplicationPaused = true;
HWND    g_windowHandle = 0;

//-----------------------------------------------------------------------------
// functions
//-----------------------------------------------------------------------------
LRESULT CALLBACK
WindowProc (HWND window, UINT message, WPARAM wParam, LPARAM lParam)
{
    // check to see if the message is one we care about
    switch (message)
    {
        case WM_CREATE:
        {
            // save the window handle
            g_windowHandle = window;
            
            // let windows do the rest
            break;
        }
            
        case WM_ACTIVATE:
        {
            // pause the app if we are deactivated
            g_bApplicationPaused = (LOWORD (wParam) == WA_INACTIVE);
            
            // let windows do the rest
            break;
        }
            
        case WM_PAINT:
        {
            // stop the message from recurring
            ValidateRect (window, 0);
    
            // return 0 to indicate that we handled the message
            return 0;
        }

        case WM_CLOSE:
        {
            // quit
            PostQuitMessage (0);

            // return 0 to indicate that we handled the message
            return 0;
        }

        case WM_DESTROY:
        {
            // make sure we can never use the window again without crashing
            Assert (window == g_windowHandle);
            g_windowHandle = reinterpret_cast<HWND> (-1);

            // let Windows do the rest
            break;
        }
    }

    // let Windows default behavior rule the roost
    return DefWindowProc (window, message, wParam, lParam);
}

//-----------------------------------------------------------------------------
void
RegisterWindowClass (char* className, DWORD classStyle)
{
    // this is a helper routine to let two parameters define the window class, so
    // the operations should be pretty simple to understand. Check the MSDN help
    // on RegisterClassEx for detailed information. We set the window background
    // default color to anything but red, as red will be our debug clear color
    WNDCLASSEX    windowClass;
    ZeroMemory (&windowClass, sizeof(windowClass)); 
    windowClass.cbSize = sizeof(WNDCLASSEX);
    windowClass.style = classStyle;
    windowClass.cbWndExtra = sizeof (LONG);
    windowClass.lpfnWndProc = WindowProc;
    windowClass.hIcon = LoadIcon (0, IDI_APPLICATION);
    windowClass.hCursor = LoadCursor (0, IDC_ARROW);
    windowClass.hbrBackground = CreateSolidBrush (PALETTERGB (240, 240, 240));
    windowClass.lpszClassName = className;
    RegisterClassEx (&windowClass);
}

//-----------------------------------------------------------------------------
HWND
OpenWindow (const Text& title, uInt width, uInt height, DWORD windowStyle)
{
    // register the window class name
    char*           className = "DirectX Window Class";
    RegisterWindowClass (className, CS_BYTEALIGNCLIENT | CS_OWNDC);

    // load the xml configuration file
    PtrToXMLNode    pXMLNode = XMLNode::FromFile ("window.xml");
    PtrToXMLNode    pWindowNode = pXMLNode->GetChild ("Window");

    // pull window details out of XML
    Text            windowName = pWindowNode->GetAttribute ("name");
    uInt            windowWidth = pWindowNode->GetAttribute ("width").ConvertToUnsignedInt ();
    uInt            windowHeight = pWindowNode->GetAttribute ("height").ConvertToUnsignedInt ();
    
    // figure out what size the window should be in order to have 
    // a client rectangle of the requested size
    RECT            rect;
    SetRect (&rect, 0, 0, windowWidth, windowHeight);
    AdjustWindowRect (&rect, windowStyle, 0);

    // create the window as requested
    return CreateWindow (className, windowName, windowStyle, CW_USEDEFAULT, CW_USEDEFAULT, rect.right - rect.left, rect.bottom - rect.top, 0, 0, 0, 0);
}

//-----------------------------------------------------------------------------
// force this module to be loaded
//-----------------------------------------------------------------------------
DEFINE_FORCED_LOAD (Window);

//-----------------------------------------------------------------------------
// tasks
//-----------------------------------------------------------------------------
DEFINE_TASK (Window)
{
    DO_AFTER (COM);
}

//-----------------------------------------------------------------------------
HANDLE_MESSAGE (Window, msg)
{
    switch (msg)
    {
        case Task::STARTUP:
            OpenWindow ("Untitled", CW_USEDEFAULT, CW_USEDEFAULT, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX);
            return (g_windowHandle != 0);
            
        case Task::SHUTDOWN:
            return (DestroyWindow (g_windowHandle) != FALSE);
            
        default:
            return false;
    }
}

//-----------------------------------------------------------------------------


