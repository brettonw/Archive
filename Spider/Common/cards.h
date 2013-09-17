//------------------------------------------------------------------------------
// Copyright (C) 2006 Bretton Wade, All Rights Reserved
//------------------------------------------------------------------------------

#ifndef     _CARDS_H_
#define     _CARDS_H_
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
// BEGIN DECLARATIONS
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

//------------------------------------------------------------------------------
// functions
//------------------------------------------------------------------------------
void        DrawCards (HDC);
void        ClickCards (const POINT*);

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
// END DECLARATIONS
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
#endif //   _CARDS_H_

#ifdef      _CARDS_CPP_
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
// BEGIN DEFINITIONS
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

//------------------------------------------------------------------------------
// includes
//------------------------------------------------------------------------------
#ifndef     _STACK_H_
#include    "stack.h"
#endif  //  _STACK_H_

#ifndef     _RANDOM_H_
#include    "random.h"
#endif  //  _RANDOM_H_

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
    
    private:
        Stack*      GetStack (const POINT*);
        Card*       GetCard (const POINT*);
        
    public:
        /* void */  Spider (void);
        void        Draw (HDC);
        void        Click (const POINT*);
        void        Deal (void);
};

//------------------------------------------------------------------------------
Stack*
Spider::GetStack (const POINT* pt)
{
    int     x = pt->x - CARD_INDENT;
    int     y = pt->y - CARD_INDENT;
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
                return &stack[which];
            }
        }
    }
    return 0;
}

//------------------------------------------------------------------------------
Card*
Spider::GetCard (const POINT* pt)
{
    Stack*  pStack = GetStack (pt);
    if (pStack)
    {
        POINT   local = {0, pt->y - (CARD_INDENT + LAYOUT_HEIGHT)};
        POINT   offset_up = {0, CARD_HEIGHT_OFFSET_UP};
        POINT   offset_dn = {0, CARD_HEIGHT_OFFSET_DN};
        return pStack->GetCard (&local, &offset_up, &offset_dn);
    }
    return 0;
}

//------------------------------------------------------------------------------
/* void */  
Spider::Spider (void)
{
    g_pDeck = new Deck (DOUBLE_SUIT, DOUBLE_DECK);
    g_pDeck->Shuffle ();
    
    // fill the deals
    for (unsigned i = 0; i < SPIDER_DEALS; ++i)
        for (unsigned j = 0; j < SPIDER_STACKS; ++j)
            deal[i].AddCard (g_pDeck->Deal ());
            
    // fake a few cards in the finish stack XXXXXXXXXXXXXXXXXXXX
    #if 0
    for (unsigned i = 0; i < SPIDER_FINISHES; ++i)
        finish[i].AddCard (g_pDeck->Deal ());
    #endif

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
}

//------------------------------------------------------------------------------
void        
Spider::Draw (HDC hdc)
{
    POINT       pt;

    // deals at the top left with no offset, but offset left to right WRT each other
    pt.x  = CARD_INDENT; pt.y = CARD_INDENT;
    for (unsigned i = 0; i < SPIDER_DEALS; i++)
    {
        deal[i].Draw (hdc, &pt);
        pt.x += CARD_X_SIZE / 2;
    }

    // finishes at the top right
    pt.x = CARD_INDENT + (SPIDER_STACKS - 1) * (CARD_X_SIZE + CARD_SPACING);
    for (unsigned i = 0; i < SPIDER_FINISHES; i++)
    {
        finish[i].Draw (hdc, &pt);
        pt.x -= CARD_X_SIZE / 2;
    }
    
    // draw the drag somewhere XXX
    pt.x -= LAYOUT_WIDTH;
    drag.Draw (hdc, &pt);
    
    // stacks with vertical offset from left to right
    pt.x = CARD_INDENT; pt.y = CARD_INDENT + CARD_SPACING + CARD_Y_SIZE;
    POINT   offset_up = {0, CARD_HEIGHT_OFFSET_UP};
    POINT   offset_dn = {0, CARD_HEIGHT_OFFSET_DN};
    for (unsigned i = 0; i < SPIDER_STACKS; ++i)
    {
        stack[i].Draw (hdc, &pt, &offset_up, &offset_dn);
        pt.x += CARD_X_SIZE + CARD_SPACING;
    }    
}

//------------------------------------------------------------------------------
void 
Spider::Click (const POINT* pt)
{
    // figure what stack is clicked on
    Stack*  pStack = GetStack (pt);
    
    // check if we are dragging something
    if (drag.GetBottom ())
    {
        if (pStack)
        {
            // check to see if this is a legal move
            Card*   pBottom = pStack->GetBottom ();
            if ((not pBottom) or ((pBottom->value - drag.GetTop ()->value) == 1) or (pBottom->facing == FACE_DOWN))
            {
                // this is the target for the drag
                drag.Split (drag.GetTop (), pStack);
                
                // recognize a finish if one was created
                pBottom = pStack->GetBottom ();
                if (pBottom->value == 0)
                {
                    // walk all the way up to the king of the same suit
                    Card*   pTop = pStack->GetTop ();
                    Card*   pWalk = pBottom;
                    bool    bSuccess = true;
                    Value   expect = 0;
                    while ((pWalk != pTop) and bSuccess)
                    {
                        if ((pWalk->facing == FACE_DOWN) or (pWalk->suit != pBottom->suit) or (pWalk->value != expect))
                            bSuccess = false;
                        else if (pWalk->value == 12)
                        {
                            // figure out which finish this goes to
                            unsigned    i = 0;
                            while (finish[i].GetTop ())
                                ++i;
                                
                            // get the cards off to the finish
                            pStack->Split (pWalk, &finish[i]);
                        }
                        else
                            expect++;
                        pWalk = s_cast<Card*> (pWalk->pPrev);
                    }
                    if ((pWalk == pTop) and bSuccess)
                    {
                        if ((pWalk->facing != FACE_DOWN) and (pWalk->suit == pBottom->suit) and (pWalk->value == expect) and (expect == 12))
                        {
                            // figure out which finish this goes to
                            unsigned    i = 0;
                            while (finish[i].GetTop ())
                                ++i;
                                
                            // get the cards off to the finish
                            pStack->Split (pWalk, &finish[i]);
                        }
                    }
                }
                
                // flip the bottom card of all the stacks
                for (unsigned i = 0; i < SPIDER_STACKS; ++i)
                {
                    // check to see if we need to flip the bottom card of the current stack
                    Card*   pCard = stack[i].GetBottom ();
                    if (pCard)
                        pCard->facing = FACE_UP;
                }
                
            }
        }
    }
    else
    {
        // check if its a deal
        if (pt->y < (CARD_INDENT + LAYOUT_HEIGHT))
        {
            Deal ();
        }
        else
        {
            Card*   pCard = GetCard (pt);
            if (pCard)
            {
                // XXX also make sure we are only allowing same suit incremental stacks to be dragged
                
                // start dragging, we got a card so there must be a stack
                pStack->Split (pCard, &drag);
            }
        }
    }
}

//------------------------------------------------------------------------------
void 
Spider::Deal (void)
{
    unsigned    i = SPIDER_DEALS - 1;
    while ((i < SPIDER_DEALS) and (deal[i].GetBottom () == 0))
        --i;
    if (i < SPIDER_DEALS)
    {
        unsigned    j = 0;
        Card*       pCard = deal[i].TakeBottom ();
        while (pCard)
        {
            pCard->facing = FACE_UP;
            stack[j].AddCard (pCard);
            j = (j + 1) % SPIDER_STACKS;
            pCard = deal[i].TakeBottom ();
        }
    }
}

//------------------------------------------------------------------------------
// functions
//------------------------------------------------------------------------------
Spider* g_pSpider = 0;

//------------------------------------------------------------------------------
// functions
//------------------------------------------------------------------------------
void   
DrawCards (HDC hdc)
{
    if (not g_pSpider)
        g_pSpider = new Spider;
        
    g_pSpider->Draw (hdc);
}

//------------------------------------------------------------------------------
void   
ClickCards (const POINT* pt)
{
    g_pSpider->Click (pt);
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
// END DEFINITIONS
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
#endif  //  _CARDS_CPP_
