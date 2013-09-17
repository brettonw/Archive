//-----------------------------------------------------------------------------
// Copyright (C) 1997-2006 Bretton Wade
// All Rights Reserved
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// include files
//-----------------------------------------------------------------------------
#include    "precompile.h"
#include    "player_human.h"
#include    "window.h"
#include    "direct_3d.h"
#include    <xinput.h>

//-----------------------------------------------------------------------------
// static class members
//-----------------------------------------------------------------------------
DEFINE_FACTORY (PlayerHuman);

//-----------------------------------------------------------------------------
// class methods
//-----------------------------------------------------------------------------
void
PlayerHuman::GetInput (void)
{
    // xinput to get input from the controller
    XINPUT_STATE    inputState;
    switch (XInputGetState (0, &inputState))
    {
        case ERROR_DEVICE_NOT_CONNECTED:
        {
            // get our inputs off the mouse
            POINT               mousePos;
            GetCursorPos (&mousePos);
            ScreenToClient (g_windowHandle, &mousePos);
            RECT                clientRect;
            GetClientRect (g_windowHandle, &clientRect);
            
            // compute the game input positions
            m_vInput.x = ((static_cast<float> (2 * mousePos.x) / static_cast<float> (clientRect.right)) - 1) * -1;
            m_vInput.y = ((static_cast<float> (2 * mousePos.y) / static_cast<float> (clientRect.bottom)) - 1) * -1;
            
            // clamp the input range to -1..1
            if (fabsf (m_vInput.x) > 1)
                m_vInput.x /= fabsf (m_vInput.x);
            if (fabsf (m_vInput.y) > 1) 
                m_vInput.y /= fabsf (m_vInput.y);
        }
        break;
        
        case ERROR_SUCCESS:
        {
            if (inputState.Gamepad.wButtons)
            {
                PtrToGameObject     pGameObject = GameObject::Find (m_gameObjectID);
                PtrToPhysicsBody    pBody = pGameObject->GetBody ();
                GoTo (pBody, D3DXVECTOR2 (0, 0));
            }
            else
            {
                // compute the game input positions
                m_vInput.x = float (inputState.Gamepad.sThumbLX) / float (-SHRT_MAX);
                m_vInput.y = float (inputState.Gamepad.sThumbLY) / float (SHRT_MAX);
            }
        }
        break;
            
        default:
            // an error occurred here
            break;
    }



}

//-----------------------------------------------------------------------------
/* void */
PlayerHuman::PlayerHuman (const PtrToXMLNode& pXMLNode) :
    PlayerObject (pXMLNode)
{
}

//-----------------------------------------------------------------------------
/* void */
PlayerHuman::~PlayerHuman (void)
{
}

//-----------------------------------------------------------------------------
