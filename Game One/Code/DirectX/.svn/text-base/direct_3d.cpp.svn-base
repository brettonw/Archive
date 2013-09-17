//-----------------------------------------------------------------------------
// Copyright (C) 1997-2006 Bretton Wade
// All Rights Reserved
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// include files
//-----------------------------------------------------------------------------
#include    "precompile.h"
#include    "direct_3d.h"
#include    "timer.h"
#include    "window.h"
#include    "task.h"

//-----------------------------------------------------------------------------
// force this module to be loaded
//-----------------------------------------------------------------------------
DEFINE_FORCED_LOAD (Direct3D);

//-----------------------------------------------------------------------------
// tasks
//-----------------------------------------------------------------------------
DEFINE_TASK (Direct3D)
{
    DO_AFTER (Window);
}

//-----------------------------------------------------------------------------
HANDLE_MESSAGE (Direct3D, msg)
{
    switch (msg)
    {
        case Task::STARTUP:
            NewCall Direct3D;
            return true;
            
        case Task::SHUTDOWN:
            delete Direct3D::GetSingleton ();
            return true;
            
        default:
            return false;
    }
}

//-----------------------------------------------------------------------------
// static class variables
//-----------------------------------------------------------------------------
bool                Direct3D::s_bOK = true;

//-----------------------------------------------------------------------------
// macros
//-----------------------------------------------------------------------------
#define     SET_VALUE(member)                                                   \
    valueMap[#member] = member
    
#define     LOOKUP_PRESENT_PARAMETER(member, type)                              \
{                                                                               \
    Text                    member = pDeviceNode->GetAttribute (#member);       \
    report += Text (#member) + "=" + member + "\n";                             \
    if (member.Length () > 0)                                                   \
    {                                                                           \
        TextMap<UINT>::iterator  iter = valueMap.find (member);                 \
        if (iter != valueMap.end ())                                            \
            presentParameters.member = type (iter->second);                     \
    }                                                                           \
}

//-----------------------------------------------------------------------------
// class methods
//-----------------------------------------------------------------------------
/* void */
Direct3D::Direct3D (void) :
    m_pDirect3D (0),
    m_iRefreshRate (0),
    m_fAspectRatio (0)
{
    // check to see if we need to set up the d3d object
    if (not m_pDirect3D)
    {
        // create the d3d object and check for success
        m_pDirect3D = Direct3DCreate9 (D3D_SDK_VERSION);
        if (m_pDirect3D)
        {
            // fetch the display mode, and store off the refresh rate
            D3D_TEST (m_pDirect3D->GetAdapterDisplayMode (D3DADAPTER_DEFAULT, &m_displayMode));
            m_iRefreshRate = m_displayMode.RefreshRate;
        }
    }
    
    // set up the value map
    TextMap<UINT>   valueMap;
    SET_VALUE (TRUE);
    SET_VALUE (FALSE);
    SET_VALUE (60);
    SET_VALUE (70);
    SET_VALUE (72);
    SET_VALUE (75);
    SET_VALUE (480);
    SET_VALUE (600);
    SET_VALUE (640);
    SET_VALUE (768);
    SET_VALUE (800);
    SET_VALUE (1024);
    SET_VALUE (1200);
    SET_VALUE (1280);
    SET_VALUE (1600);
    SET_VALUE (D3DFMT_A8R8G8B8);
    SET_VALUE (D3DFMT_X8R8G8B8);
    SET_VALUE (D3DFMT_D24S8);
    SET_VALUE (D3DFMT_D24X8);
    SET_VALUE (D3DFMT_D16);
    SET_VALUE (D3DMULTISAMPLE_2_SAMPLES);
    SET_VALUE (D3DMULTISAMPLE_4_SAMPLES);
    SET_VALUE (D3DPRESENT_INTERVAL_ONE);
    SET_VALUE (D3DPRESENT_INTERVAL_IMMEDIATE);
    
    // load the xml configuration file
    PtrToXMLNode    pXMLNode = XMLNode::FromFile ("Direct3D.xml");
    Assert (pXMLNode);

    // check to see if everything is good so far and if the xml is valid
	if (pXMLNode and m_pDirect3D and s_bOK)
    {
	    // fetch the list of D3D devices from the xml and check for validity
	    const XMLNodeList*  pDeviceNodeList = pXMLNode->GetChildren ("Device");
	    if (pDeviceNodeList)
	    {
	        // loop over the device list until all options are exhausted or one succeeds
	        XMLNodeList::const_iterator iter = pDeviceNodeList->begin ();
	        XMLNodeList::const_iterator end = pDeviceNodeList->end ();
	        while ((iter != end) and (not m_pDevice))
	        {
	            // extract the current node
	            PtrToXMLNode            pDeviceNode = *iter++;
	            
	            // basic present parameters defaults
                D3DPRESENT_PARAMETERS   presentParameters; 
                ZeroMemory (&presentParameters, sizeof(presentParameters)); 
                presentParameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
                presentParameters.hDeviceWindow = g_windowHandle;
                presentParameters.BackBufferFormat = m_displayMode.Format; 

	            // lookup present parameters
	            Text                    report;
	            LOOKUP_PRESENT_PARAMETER (Windowed,                     BOOL);
	            LOOKUP_PRESENT_PARAMETER (BackBufferFormat,             D3DFORMAT);
	            LOOKUP_PRESENT_PARAMETER (BackBufferWidth,              UINT);
	            LOOKUP_PRESENT_PARAMETER (BackBufferHeight,             UINT);
	            LOOKUP_PRESENT_PARAMETER (FullScreen_RefreshRateInHz,   UINT);
	            LOOKUP_PRESENT_PARAMETER (EnableAutoDepthStencil,       BOOL);
	            LOOKUP_PRESENT_PARAMETER (AutoDepthStencilFormat,       D3DFORMAT);
	            LOOKUP_PRESENT_PARAMETER (MultiSampleType,              D3DMULTISAMPLE_TYPE);
	            LOOKUP_PRESENT_PARAMETER (PresentationInterval,         UINT);

                // try to actually create the device now
                m_pDirect3D->CreateDevice (D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, g_windowHandle, D3DCREATE_HARDWARE_VERTEXPROCESSING, &presentParameters, &m_pDevice);
                
                // check if this succeeded
                if (m_pDevice)
                {
                    DebugPrint ("Successfully created device with the following settings:\n");
                    
        	        // show the window if we are in windowed mode
                    if (presentParameters.Windowed)
                        ShowWindow (g_windowHandle, SW_SHOWNORMAL);
                }
                else
                {
                    DebugPrint ("Failed to create device with the following settings:\n");
                }

                // report the attempt
                DebugPrint (report + "\n");
   	        }
	    }
	    
        // check to see if everything is good so far
	    if (m_pDevice)
	    {
            // fetch the client rectangle of the window and compute the aspect ratio assuming square pixels
            RECT            clientRect;
            GetClientRect (g_windowHandle, &clientRect);
            m_fAspectRatio = static_cast<float> (clientRect.right) / static_cast<float> (clientRect.bottom);

            // ensure that we have a valid refresh rate
            TimeRefreshRate ();
	    }
	    else
	    {
	        // handle the case of no device getting created
	        Assert ("No device found" and false);
	        exit (0);
        }
    }
}

//-----------------------------------------------------------------------------
/* void */              
Direct3D::~Direct3D (void)
{
}

//-----------------------------------------------------------------------------
void
Direct3D::TimeRefreshRate (void)
{
    // check to see if we have an invalid refresh rate
    if (m_iRefreshRate == 0)
    {
        // clear the back buffer so we don't show ugly stuff to the player
        D3D_TEST (m_pDevice->Clear (0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA (255, 0, 0, 255), 1.0, 0));

        // make sure we are at a frame boundary
        D3D_TEST (m_pDevice->Present (0, 0, 0, 0));
            
        // start a timer
        Timer   timer;
        
        // loop for some number of "frames" to get a sense for the refresh rate
        uInt    iTestFrameCount = 10;
        for (uInt i = 0; i < iTestFrameCount; i++)
            D3D_TEST (m_pDevice->Present (0, 0, 0, 0));
        
        // fetch the elapsed time
        float   fElapsedTime = timer.Elapsed ();
        m_iRefreshRate = static_cast<uInt> ((static_cast<float> (iTestFrameCount) * (1 / fElapsedTime)) + 0.5f);
    }
    Assert (m_iRefreshRate > 0);           
    Assert (m_iRefreshRate < 100);
}

//-----------------------------------------------------------------------------
