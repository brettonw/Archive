//------------------------------------------------------------------------------
// Copyright (C) 2006 Bretton Wade, All Rights Reserved
//------------------------------------------------------------------------------

#ifndef     _PLAY_H_
#define     _PLAY_H_
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
// BEGIN DECLARATIONS
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

//------------------------------------------------------------------------------
// includes
//------------------------------------------------------------------------------
#ifndef     _SPIDER_H_
#include    "spider.h"
#endif  //  _SPIDER_H_

//------------------------------------------------------------------------------
// functions
//------------------------------------------------------------------------------
void        NewGame (void);
void        ResetGame (void);
void        Heartbeat (void);
void        ClickDown (const POINT* pPt);
void        ClickUp (const POINT* pPt);
void        Undo (void);
void        Redo (void);
void        SetSuits (Suits suits);

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
// END DECLARATIONS
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
#endif //   _PLAY_H_

#ifdef      _PLAY_CPP_
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
// BEGIN DEFINITIONS
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

//------------------------------------------------------------------------------
// includes
//------------------------------------------------------------------------------
#ifndef     _ACTION_SPLIT_H_
#include    "action_split.h"
#endif  //  _ACTION_SPLIT_H_

#ifndef     _ACTION_FLIP_H_
#include    "action_flip.h"
#endif  //  _ACTION_FLIP_H_

#ifndef     _ACTION_COMPOSITE_H_
#include    "action_composite.h"
#endif  //  _ACTION_COMPOSITE_H_

#ifndef     _RANDOM_H_
#include    "random.h"
#endif  //  _RANDOM_H_

#ifndef     _REGISTRY_H_
#include    "registry.h"
#endif  //  _REGISTRY_H_

//------------------------------------------------------------------------------
// globals
//------------------------------------------------------------------------------
Spider*     g_pSpider = 0;
unsigned    g_randomSeed;
bool        g_bIsDrag = false;
Suits       g_suits = DOUBLE_SUIT;
bool        g_bDirty = true;
RECT        g_dirtyRect;
bool        g_bUseBackground = true;
bool        g_bUseCardback = false;
bool        g_bPlaySound = true;

//------------------------------------------------------------------------------
// functions
//------------------------------------------------------------------------------
void        
NewGame (void)
{
    static  bool    bFirstTime = true;
    if (bFirstTime)
    {
        // read the registry to get the suits value
        g_suits = s_cast<Suits> (ReadRegistry (L"Suits", L"Count", 2));
        bFirstTime = false;
    }
    else
    {
        // clean up the existing game
        if (g_pSpider)
            delete g_pSpider;
    }    
    // set a new random seed for shuffling
    g_randomSeed = Random (0, 20000, true);
    srand (g_randomSeed);

    // create a new game, and set it to draw
    g_pSpider = new Spider (g_suits);
    GetClientRect (g_hWnd, &g_dirtyRect);
    g_bDirty = true;
}

//------------------------------------------------------------------------------
void        
ResetGame (void)
{
    assert (g_pSpider);
    
    // clean up the existing game
    delete g_pSpider;

    // reset the random seed to our saved value to insure the same user experience
    srand (g_randomSeed);
    
    // create a new game, and set it to draw
    g_pSpider = new Spider (g_suits);
    GetClientRect (g_hWnd, &g_dirtyRect);
    g_bDirty = true;
}

//------------------------------------------------------------------------------
void
Heartbeat (void)
{
    // check if we are dragging
    if (g_bIsDrag)
    {
        // do the drag, and set the dirty flag
        g_pSpider->ClickMove (&g_cursorPos, &g_lastCursorPos, &g_dirtyRect);
        g_lastCursorPos = g_cursorPos;
        g_bDirty = true;
    }
    
    // check if we need to do any kind of drawing
    if (g_bDirty)
    {
        #ifdef USE_OFFSCREEN_RENDERING
        g_pSpider->Draw (g_offscreenDC, &g_dirtyRect);
        InvalidateRect (g_hWnd, &g_dirtyRect, FALSE);
        #else
        PAINTSTRUCT	paint;
		BeginPaint (g_hWnd, &paint);
        g_pSpider->Draw (paint.hdc, &g_dirtyRect);
		EndPaint (g_hWnd, &paint);
        #endif
        g_bDirty = false;
    }
}

//-----------------------------------------------------------------------------
void   
ClickDown (const POINT* pPt)
{
    if ((not g_pSpider) or (g_pSpider->Won ()))
        NewGame ();
    g_pSpider->ClickDown (pPt);
    g_bIsDrag = true;
}

//------------------------------------------------------------------------------
void   
ClickUp (const POINT* pPt)
{
    g_pSpider->ClickUp (pPt);
    g_bIsDrag = false;
    GetClientRect (g_hWnd, &g_dirtyRect);   // can something more efficient be done?
    g_bDirty = true;
}

//------------------------------------------------------------------------------
void
Undo (void)
{
    if (not g_bIsDrag)
    {
        g_pSpider->Undo ();
        GetClientRect (g_hWnd, &g_dirtyRect);   // can something more efficient be done?
        g_bDirty = true;
    }
}

//------------------------------------------------------------------------------
void
Redo (void)
{
    if (not g_bIsDrag)
    {
        g_pSpider->Redo ();
        GetClientRect (g_hWnd, &g_dirtyRect);   // can something more efficient be done?
        g_bDirty = true;
    }
}

//------------------------------------------------------------------------------
void        
SetSuits (Suits suits)
{
    g_suits = suits;
    
    // write the registry to preserve this value across runs
    WriteRegistry (L"Suits", L"Count", s_cast<DWORD> (g_suits));
    
    NewGame ();
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
// END DEFINITIONS
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
#endif  //  _PLAY_CPP_
