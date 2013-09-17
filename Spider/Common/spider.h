//------------------------------------------------------------------------------
// Copyright (C) 2006 Bretton Wade, All Rights Reserved
//------------------------------------------------------------------------------

#ifndef     _SPIDER_H_
#define     _SPIDER_H_
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
// BEGIN DECLARATIONS
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

//------------------------------------------------------------------------------
// includes
//------------------------------------------------------------------------------
#ifndef     _STACK_H_
#include    "stack.h"
#endif  //  _STACK_H_

#ifndef     _ACTION_COMPOSITE_H_
#include    "action_composite.h"
#endif  //  _ACTION_COMPOSITE_H_

#ifndef     _ACTION_SOUND_H_
#include    "action_sound.h"
#endif  //  _ACTION_SOUND_H_

//------------------------------------------------------------------------------
// types
//------------------------------------------------------------------------------
enum
{
    SPIDER_STACKS = 10,
    SPIDER_DEALS = 5,
    SPIDER_FINISHES = 8
};

//------------------------------------------------------------------------------
// classes
//------------------------------------------------------------------------------
class  Spider
{
    private:
        Stack       stack[SPIDER_STACKS];
        Stack       deal[SPIDER_DEALS];
        Stack       finish[SPIDER_FINISHES];
        Stack       drag;
        Action*     pUndo;
        Action*     pRedo;
        Action*     pDragAction;
        Stack*      pLast;
        POINT       clickOffset;
        int         dragHeight;
static  HDC		    backgroundDC;
    
    private:
        Stack*      GetStack (const POINT* pt, POINT* pClick = 0);
        Card*       GetCard (const POINT* pt, POINT* pClick = 0);
        void        SaveForUndo (Action* pAction);
        void        Do (Action* pAction);
        void        ClearActionList (Action* pAction);
        bool        CheckStackForFinish (Stack* pStack, ActionComposite* pAction);
        
    public:
        /* void */  Spider (Suits suits = DOUBLE_SUIT);
        /* void */  ~Spider (void);
        void        Undo (void);
        void        Redo (void);
        void        Draw (HDC hdc, RECT* pDirtyRect);
        void        Deal (void);
        void        ClickDown (const POINT* pPt);
        void        ClickUp (const POINT* pPt);
        void        ClickMove (const POINT* pPt, const POINT* pLast, RECT* pDirtyRect);
        bool        Won (void) const;
static  void        LoadBitmaps (void);
};

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
// END DECLARATIONS
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
#endif //   _SPIDER_H_

#ifdef      _SPIDER_CPP_
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

#ifndef     _RANDOM_H_
#include    "random.h"
#endif  //  _RANDOM_H_

//------------------------------------------------------------------------------
// static members
//------------------------------------------------------------------------------
HDC		    Spider::backgroundDC = 0;

//------------------------------------------------------------------------------
// classes
//------------------------------------------------------------------------------
Stack*
Spider::GetStack (const POINT* pPt, POINT* pClick)
{
    int     x = pPt->x - CARD_X_INDENT;
    int     y = pPt->y - CARD_Y_INDENT;
    if ((x > 0) and (y > 0))
    {
        // check to see if it is in the stack zone
        if (y > LAYOUT_HEIGHT)
        {
            // stack zone, figure out which stack is the one
            y -= LAYOUT_HEIGHT;
            unsigned    which = 0;
            while ((which < (SPIDER_STACKS - 1)) and (x > LAYOUT_WIDTH))
            {
                which++;
                x -= LAYOUT_WIDTH;
            }
            if (x < CARD_X_SIZE)
            {
                if (pClick)
                    pClick->x = x;
                return &stack[which];
            }
        }
    }
    return 0;
}

//------------------------------------------------------------------------------
Card*
Spider::GetCard (const POINT* pPt, POINT* pClick)
{
    Stack*  pStack = GetStack (pPt, pClick);
    if (pStack)
    {
        POINT   local = {pClick ? pClick->x : 0, pPt->y - (CARD_Y_INDENT + LAYOUT_HEIGHT)};
        POINT   offset_up = {0, CARD_HEIGHT_OFFSET_UP};
        POINT   offset_dn = {0, CARD_HEIGHT_OFFSET_DN};
        return pStack->GetCard (&local, &offset_up, &offset_dn, pClick);
    }
    return 0;
}

//------------------------------------------------------------------------------
void
Spider::SaveForUndo (Action* pAction)
{
    pAction->pNext = pUndo;
    pUndo = pAction;
}

//------------------------------------------------------------------------------
void
Spider::Do (Action* pAction)
{
    pAction->Do ();
    SaveForUndo (pAction);
}

//------------------------------------------------------------------------------
void
Spider::ClearActionList (Action* pAction)
{
    while (pAction)
    {
        Action* pTmp = pAction;
        pAction = pAction->pNext;
        delete pTmp;
    }
}

//------------------------------------------------------------------------------
bool
Spider::CheckStackForFinish (Stack* pStack, ActionComposite* pAction)
{
    // recognize a finish if one exists
    Card*   pBottom = pStack->GetBottom ();
    if (pBottom->value == 0)
    {
        // walk all the way up to the king of the same suit
        Card*   pEnd = s_cast<Card*> (pStack->GetTop ()->pPrev);
        Card*   pWalk = pBottom;
        Value   expect = 0;
        while (pWalk != pEnd)
        {
            if ((pWalk->facing == FACE_DOWN) or (pWalk->suit != pBottom->suit) or (pWalk->value != expect))
                return false;
            else if (pWalk->value == 12)
            {
                // figure out which finish this goes to
                unsigned    i = 0;
                while (finish[i].GetTop ())
                    ++i;
                    
                // get the cards off to the finish
                pAction->Do (new ActionSplit (pStack, &finish[i], pWalk));
                return true;
            }
            else
                expect++;
            pWalk = s_cast<Card*> (pWalk->pPrev);
        }
    }
    return false;
}

//------------------------------------------------------------------------------
/* void */  
Spider::Spider (Suits suits) :
    pUndo (0),
    pRedo (0)
{
    g_pDeck = new Deck (suits, DOUBLE_DECK);
    g_pDeck->Shuffle ();
    
    // fill the deals
    for (unsigned i = 0; i < SPIDER_DEALS; ++i)
        for (unsigned j = 0; j < SPIDER_STACKS; ++j)
            deal[i].AddCard (g_pDeck->Deal ());
            
    // deal out the stacks
    unsigned    current = 0;
    while (g_pDeck->CardsLeft () > 0)
    {
        stack[current].AddCard (g_pDeck->Deal ());
        current = (current + 1) % SPIDER_STACKS;
    }
    
    // flip the top cards in each stack
    for (unsigned i = 0; i < SPIDER_STACKS; ++i)
        stack[i].GetBottom ()->facing = FACE_UP;

    // play the start sound
    PLAY_SOUND (IDS_DEAL);
}

//------------------------------------------------------------------------------
/* void */  
Spider::~Spider (void)
{
    ClearActionList (pRedo);
    ClearActionList (pUndo);
}

//------------------------------------------------------------------------------
void
Spider::Undo (void)
{
    if (pUndo)
    {
        Action* pTmp = pUndo;
        pUndo = pUndo->pNext;
        pTmp->pNext = pRedo;
        pRedo = pTmp;
        pTmp->Undo ();
    }
}

//------------------------------------------------------------------------------
void
Spider::Redo (void)
{
    if (pRedo)
    {
        Action* pTmp = pRedo;
        pRedo = pRedo->pNext;
        Do (pTmp);
    }
}

//------------------------------------------------------------------------------
void        
Spider::Draw (HDC hdc, RECT* pDirtyRect)
{
    // erase the back plane using the background
    if (backgroundDC and g_bUseBackground)
    {
        BitBlt (hdc, pDirtyRect->left, pDirtyRect->top, pDirtyRect->right - pDirtyRect->left, pDirtyRect->bottom - pDirtyRect->top, backgroundDC, pDirtyRect->left, pDirtyRect->top, SRCCOPY);
    }
    else
    {
	    HBRUSH  bgBrush = CreateSolidBrush (RGB (220, 149, 80));
        FillRect (hdc, pDirtyRect, bgBrush);
        DeleteObject (bgBrush);
    }

    // deals at the top left with no offset, but offset left to right WRT each other
    POINT       pt;
    if (pDirtyRect->top < (CARD_Y_INDENT + LAYOUT_HEIGHT))
    {
        pt.x  = CARD_X_INDENT; pt.y = CARD_Y_INDENT;
        for (unsigned i = 0; i < SPIDER_DEALS; i++)
        {
            deal[i].Draw (hdc, &pt);
            pt.x += CARD_WIDTH_OFFSET;
        }

        // finishes at the top right
        pt.x = CARD_X_INDENT + (SPIDER_STACKS - 1) * (CARD_X_SIZE + CARD_SPACING);
        for (unsigned i = 0; i < SPIDER_FINISHES; i++)
        {
            finish[i].Draw (hdc, &pt);
            pt.x -= CARD_WIDTH_OFFSET;
        }
    }
    
    // stacks with vertical offset from left to right
    pt.x = CARD_X_INDENT; pt.y = CARD_Y_INDENT + CARD_SPACING + CARD_Y_SIZE;
    POINT   offset_up = {0, CARD_HEIGHT_OFFSET_UP};
    POINT   offset_dn = {0, CARD_HEIGHT_OFFSET_DN};
    for (unsigned i = 0; i < SPIDER_STACKS; ++i)
    {
        if ((pt.x < pDirtyRect->right) and ((pt.x + CARD_X_SIZE) > pDirtyRect->left))
            stack[i].Draw (hdc, &pt, &offset_up, &offset_dn);
        pt.x += LAYOUT_WIDTH;
    }    

    // draw the drag somewhere, this should always be in the dirty rect (right?)
    pt.x = g_cursorPos.x - clickOffset.x;
    pt.y = g_cursorPos.y - clickOffset.y;
    drag.Draw (hdc, &pt, &offset_up, &offset_dn);
}

//------------------------------------------------------------------------------
void 
Spider::Deal (void)
{
    // figure out which deal this is going to be from
    unsigned    i = SPIDER_DEALS - 1;
    while ((i < SPIDER_DEALS) and (deal[i].GetBottom () == 0))
        --i;
        
    // if a valid deal was found, e.g. all the cards aren't exhausted
    if (i < SPIDER_DEALS)
    {
        // create the composite action
        ActionComposite*    pAction = new ActionComposite ();
        
        // do all of the splits and flips that make up a deal
        for (unsigned j = 0; j < SPIDER_STACKS; ++j)
        {
            Stack*  pStack = &stack[j];
            Card*   pCard = deal[i].GetBottom ();
            pAction->Do (new ActionFlip (pCard));
            pAction->Do (new ActionSplit (&deal[i], pStack, pCard));
            
            // check for a finish on this stack
            pCard = pStack->GetBottom ();
            if (pCard and (pCard->facing == FACE_UP))   // is this card always face up?
                if (CheckStackForFinish (pStack, pAction))
                {
                    pCard = pStack->GetBottom ();
                    if (pCard and (pCard->facing == FACE_DOWN))
                        pAction->Do (new ActionFlip (pCard));
                }
        }
        
        // play the deal sound
        pAction->Do (new ActionSound (IDS_DEAL));

        // save the whole kit and kaboodle so it can be undone
        SaveForUndo (pAction);
    }
}

//------------------------------------------------------------------------------
void 
Spider::ClickDown (const POINT* pPt)
{
    // clear out the redo list
    ClearActionList (pRedo);
    pRedo = 0;

    // check if it's a click in the game area
    if (pPt->y > (CARD_Y_INDENT + LAYOUT_HEIGHT))
    {
        Card*   pCard = GetCard (pPt, &clickOffset);
        if (pCard)
        {
            pLast = GetStack (pPt);
            pDragAction = new ActionSplit (pLast, &drag, pCard);
            pDragAction->Do ();
            
            // get the bound of the drag stack
            int     unused;
            POINT   offset_up = {0, CARD_HEIGHT_OFFSET_UP};
            POINT   offset_dn = {0, CARD_HEIGHT_OFFSET_DN};
            drag.GetBound (&unused, &dragHeight, &offset_up, &offset_dn);
        }
    }
}

//------------------------------------------------------------------------------
void 
Spider::ClickUp (const POINT* pPt)
{
    // check if we are dragging something, and save the top of the drag stack
    // because it is the key to finishing the drag
    Card*   pCard = drag.GetTop ();
    if (pCard)
    {
        // undo the drag operation
        pDragAction->Undo ();
        delete pDragAction;
        
        // figure what stack is being dropped on using the center of the card
        POINT   targetPt;
        targetPt.x = (pPt->x - clickOffset.x) + (CARD_X_SIZE / 2);
        targetPt.y = (pPt->y - clickOffset.y) + (CARD_Y_SIZE / 2);
        Stack*  pStack = GetStack (&targetPt);
        if (pStack)
        {
            // check to see if this is a legal move by comparing the bottom 
            // card of the target and the top card of the dragging stack,
            // note that an empty stack is a legal target
            Card*   pBottom = pStack->GetBottom ();
            if ((not pBottom) or ((pBottom->facing == FACE_UP) and ((pBottom->value - pCard->value) == 1)))
            {                
                ActionComposite*    pAction = new ActionComposite;
                
                // set up for the sound to play, default to a simple card drop
                int                 sound = IDS_DROPCARD;
                
                // this is the target for the drag
                pAction->Do (new ActionSplit (pLast, pStack, pCard));
                
                // check for finishes, and the need to flip a card after a finish
                pCard = pStack->GetBottom ();
                if (pCard and (pCard->facing == FACE_UP))   // is this card always face up?
                    if (CheckStackForFinish (pStack, pAction))
                    {
                        // we might have won here, but either way we need to play a
                        // different sound
                        sound = Won () ? IDS_WIN : IDS_CLEARSTACK;

                        // flip the card under if one exists
                        pCard = pStack->GetBottom ();
                        if (pCard and (pCard->facing == FACE_DOWN))
                            pAction->Do (new ActionFlip (pCard));
                    }

                // check for face down cards on the last drag from spot
                pCard = pLast->GetBottom ();
                if (pCard and (pCard->facing == FACE_DOWN))
                    pAction->Do (new ActionFlip (pCard));
 
                // play the drop card/stack sound
                pAction->Do (new ActionSound (sound));
                
                // save all of this off so it can be undone
                SaveForUndo (pAction);
            }
            else
                PLAY_SOUND (IDS_BUZZ);
        }
        else
            PLAY_SOUND (IDS_ABANDON);
    }
    else if (pPt->y < (CARD_Y_INDENT + LAYOUT_HEIGHT))
    {
        Deal ();
    }
}

//------------------------------------------------------------------------------
void
Spider::ClickMove (const POINT* pPt, const POINT* pLast, RECT* pDirtyRect)
{
    // calculate the dirty rect, including drag offset
    g_dirtyRect.left = min (pPt->x, pLast->x) - clickOffset.x;
    g_dirtyRect.right = max (pPt->x, pLast->x) + (CARD_X_SIZE - clickOffset.x);
    g_dirtyRect.top = min (pPt->y, pLast->y) - clickOffset.y;
    g_dirtyRect.bottom = max (pPt->y, pLast->y) + dragHeight;
}

//------------------------------------------------------------------------------
bool
Spider::Won (void) const
{
    for (unsigned i = 0; i < SPIDER_STACKS; ++i)
        if (stack[i].GetBottom ())
            return false;
    return true;
}

//------------------------------------------------------------------------------
void
Spider::LoadBitmaps (void)
{
    HDC screenDC = GetDC (g_hWnd);
    backgroundDC = CreateCompatibleDC (screenDC);
	SelectObject (backgroundDC, LoadBitmap (g_hInst, (LPCTSTR) IDB_BACKGROUND));
    ReleaseDC (g_hWnd, screenDC);
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
// END DEFINITIONS
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
#endif  //  _SPIDER_CPP_
