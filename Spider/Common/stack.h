//------------------------------------------------------------------------------
// Copyright (C) 2006 Bretton Wade, All Rights Reserved
//------------------------------------------------------------------------------

#ifndef     _STACK_H_
#define     _STACK_H_
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
// BEGIN DECLARATIONS
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

//------------------------------------------------------------------------------
// includes
//------------------------------------------------------------------------------
#ifndef     _LAYOUT_H_
#include    "layout.h"
#endif  //  _LAYOUT_H_

#ifndef     _DECK_H_
#include    "deck.h"
#endif  //  _DECK_H_

//------------------------------------------------------------------------------
// classes
//------------------------------------------------------------------------------
class   Stack
{
    private:
        ListNode        sentinel;
        
    private:
        void            CountFaceTypes (unsigned* pUp, unsigned* pDown) const;
    
    public:
        /* void */      Stack (void);
        void            AddCard (Card*);
        Card*           GetTop (void) const;
        Card*           GetBottom (void) const;
        Card*           TakeBottom (void);
        void            Split (Card*, Stack*);
        Card*           GetCard (const POINT* pt, const POINT* offset_up = 0, const POINT* offset_dn = 0, POINT* click = 0) const;
        void            Draw (HDC hdc, const POINT* pt, const POINT* offset_up = 0, const POINT* offset_dn = 0) const;
        void            GetBound (int* pWidth, int* pHeight, const POINT* offset_up, const POINT* offset_dn) const;
};


//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
// END DECLARATIONS
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
#endif //   _STACK_H_

#ifdef      _STACK_CPP_
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
// BEGIN DEFINITIONS
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

//------------------------------------------------------------------------------
// includes
//------------------------------------------------------------------------------
#ifndef     _SCREEN_H_
#include    "screen.h"
#endif  //  _SCREEN_H_

//------------------------------------------------------------------------------
// classes
//------------------------------------------------------------------------------
void
Stack::CountFaceTypes (unsigned* pUp, unsigned* pDown) const
{
    *pUp = *pDown = 0;
    Card*       pEnd = s_cast<Card*> (c_cast<ListNode*> (&sentinel));
    Card*       pCard = s_cast<Card*> (sentinel.pNext);
    while (pCard != pEnd)
    {
        if (pCard->facing == FACE_UP)
            (*pUp)++;
        else
            (*pDown)++;
        pCard = s_cast<Card*> (pCard->pNext);
    }
}

//------------------------------------------------------------------------------
/* void */      
Stack::Stack (void)
{
    sentinel.InitSentinel ();
}

//------------------------------------------------------------------------------
void            
Stack::AddCard (Card* pCard)
{
    pCard->AddToList (sentinel.pPrev);
}

//------------------------------------------------------------------------------
Card*            
Stack::GetTop (void) const
{
    if (sentinel.pNext != (&sentinel))
        return s_cast<Card*> (sentinel.pNext);
    return 0;
}

//------------------------------------------------------------------------------
Card*
Stack::GetBottom (void) const
{
    if (sentinel.pPrev != (&sentinel))
        return s_cast<Card*> (sentinel.pPrev);
    return 0;
}

//------------------------------------------------------------------------------
Card*
Stack::TakeBottom (void)
{
    if (sentinel.pPrev != (&sentinel))
    {
        Card*   pCard = s_cast<Card*> (sentinel.pPrev);
        pCard->RemoveFromList ();
        return pCard;
    }
    return 0;
}

//------------------------------------------------------------------------------
void            
Stack::Split (Card* pCard, Stack* pStack)
{
    ListNode*   pEnd = c_cast<ListNode*> (&sentinel);
    while (pCard->pNext != pEnd)
    {
        ListNode*   pTmp = pCard->pNext;
        pCard->RemoveFromList ();
        pStack->AddCard (pCard);
        pCard = s_cast<Card*> (pTmp);
    }
    pCard->RemoveFromList ();
    pStack->AddCard (pCard);
}

//------------------------------------------------------------------------------
Card*
Stack::GetCard (const POINT* pt, const POINT* offset_up, const POINT* offset_dn, POINT* click) const
{
    // the rectangle for the top card
        RECT        rect;
        SetRect (&rect, 0, 0, CARD_X_SIZE, CARD_Y_SIZE);

    // the origin is assumed to be at the top left of the top card
    if (offset_up)
    {
        // compute the offset increments
        int         incx_dn = offset_dn ? offset_dn->x : (offset_up ? offset_up->x : 0);
        int         incy_dn = offset_dn ? offset_dn->y : (offset_up ? offset_up->y : 0);
        int         incx_up = offset_up ? offset_up->x : 0;
        int         incy_up = offset_up ? offset_up->y : 0;

        // run through all the cards in the stack, with offset
        Card*       pCard = s_cast<Card*> (sentinel.pNext);
        Card*       pEnd = s_cast<Card*> (c_cast<ListNode*> (&sentinel));
        while (pCard->pNext != pEnd)
        {
            // update the test rectangle depending on the current card facing
            if (pCard->facing == FACE_UP)
            {
                rect.left += incx_up;
                rect.right += incx_up;
                rect.top += incy_up;
                rect.bottom += incy_up;
            }
            else
            {
                rect.left += incx_dn;
                rect.right += incx_dn;
                rect.top += incy_dn;
                rect.bottom += incy_dn;
            }

            // get the next card
            pCard = s_cast<Card*> (pCard->pNext);
        }
        
        // the rectangle now represents the top card in the stack, start walking backwards up the stack
        Suit    showing_suit = pCard->suit;
        Value   expect = pCard->value + 1;
        while (pCard != pEnd)
        {
            // test the rectangle
            if (PtInRect (&rect, *pt))
            {
                // compute the click offset
                if (click)
                {
                    click->x = pt->x - rect.left;
                    click->y = pt->y - rect.top;
                }
                return pCard;
            }

            // get the previous card
            pCard = s_cast<Card*> (pCard->pPrev);
            
            // update the test rectangle depending on the current card facing if
            // this card is the same suit as the top of the deck
            if ((pCard->facing == FACE_UP) and (pCard->suit == showing_suit) and (pCard->value == expect))
            {
                rect.left -= incx_up;
                rect.right -= incx_up;
                rect.top -= incy_up;
                rect.bottom -= incy_up;
                expect++;
            }
            else
            {
                // we now have walked back down to cards that are face down or
                // are a different suit than the top card, and this won't be 
                // legal for any game, return 0
                return 0;
            }
            
        }
    }
    else
    {
        // no offset, we only need to check if the click is inside of a card rectangle
        if (PtInRect (&rect, *pt))
        {
            // compute the click offset
            if (click)
            {
                click->x = pt->x - rect.left;
                click->y = pt->y - rect.top;
            }
            return GetBottom ();
        }
    }
    return 0;
}

//------------------------------------------------------------------------------
void
Stack::Draw (HDC hdc, const POINT* pt, const POINT* offset_up, const POINT* offset_dn) const
{
    ListNode*   pEnd = c_cast<ListNode*> (&sentinel);
    
    // create the target rectangle
    RECT        rect;
    SetRect (&rect, pt->x, pt->y, pt->x + CARD_X_SIZE, pt->y + CARD_Y_SIZE);
    
    // if the stack is spread out, we need to draw each card
    if (offset_up)
    {
        // XXX this should handle if the stack is going to go off screen
        if (sentinel.pPrev != pEnd)
        {
            // calculate the target offsets
            int     incx_dn = offset_dn ? offset_dn->x : (offset_up ? offset_up->x : 0);
            int     incy_dn = offset_dn ? offset_dn->y : (offset_up ? offset_up->y : 0);
            int     incx_up = offset_up ? offset_up->x : 0;
            int     incy_up = offset_up ? offset_up->y : 0;
            
            // count the number of face down cards, and the number of face up cards
            /*
            unsigned    up;
            unsigned    dn;
            CountFaceTypes (&up, &dn);
            unsigned    sum = up + dn;
            
            // figure how much room I have
            int         avail_width = SCREEN_WIDTH - pt->x;
            int         avail_height = SCREEN_WIDTH - pt->y;
            int         needed_width = (incx_up * up) + (incx_dn * dn);
            int         needed_height = (incy_up * up) + (incy_dn * dn);
            int         up_dn_delta_x = incx_up - incx_dn;
            int         up_dn_delta_y = incy_up - incy_dn;
            */

            // draw all the cards in the stack, with offset
            Card*   pCard = s_cast<Card*> (sentinel.pNext);
            pCard->Draw (hdc, &rect);
            while (pCard->pNext != pEnd)
            {
                // update the draw rectangle depending on the current card facing
                if (pCard->facing == FACE_UP)
                {
                    rect.left += incx_up;
                    rect.right += incx_up;
                    rect.top += incy_up;
                    rect.bottom += incy_up;
                }
                else
                {
                    rect.left += incx_dn;
                    rect.right += incx_dn;
                    rect.top += incy_dn;
                    rect.bottom += incy_dn;
                }

                // get the next card
                pCard = s_cast<Card*> (pCard->pNext);
                
                // draw
                pCard->Draw (hdc, &rect, (pCard->pNext != pEnd) ? DRAW_TOP : DRAW_ALL);
            }
        }
    }
    else
    {
        // just draw the top card
        Card*   pCard = s_cast<Card*> (sentinel.pPrev);
        if (pCard != s_cast<Card*> (pEnd))
            pCard->Draw (hdc, &rect, DRAW_ALL);
    }
}

//------------------------------------------------------------------------------
void
Stack::GetBound (int* pWidth, int* pHeight, const POINT* offset_up, const POINT* offset_dn) const
{
    // XXX this should handle if the stack is going to go off screen, too
    ListNode*   pEnd = c_cast<ListNode*> (&sentinel);
    
    // start with the size of a card
    *pWidth = CARD_X_SIZE;
    *pHeight = CARD_Y_SIZE;
    
    // if the stack is spread out, we need to check each card
    if (offset_up and (sentinel.pPrev != pEnd))
    {
        // count the face types
        unsigned    up;
        unsigned    dn;
        CountFaceTypes (&up, &dn);

        // calculate the target offsets
        int     incx_dn = offset_dn ? offset_dn->x : (offset_up ? offset_up->x : 0);
        int     incy_dn = offset_dn ? offset_dn->y : (offset_up ? offset_up->y : 0);
        int     incx_up = offset_up ? offset_up->x : 0;
        int     incy_up = offset_up ? offset_up->y : 0;
            
        // add the offsets into the bounds
        *pWidth += (incx_dn * dn) + (incx_up * up);
        *pHeight += (incy_dn * dn) + (incy_up * up);
    }
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
// END DEFINITIONS
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
#endif  //  _STACK_CPP_
