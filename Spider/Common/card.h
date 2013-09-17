//------------------------------------------------------------------------------
// Copyright (C) 2006 Bretton Wade, All Rights Reserved
//------------------------------------------------------------------------------

#ifndef     _CARD_H_
#define     _CARD_H_
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
// BEGIN DECLARATIONS
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

//------------------------------------------------------------------------------
// includes
//------------------------------------------------------------------------------
#ifndef     _LIST_NODE_H_
#include    "list_node.h"
#endif  //  _LIST_NODE_H_

#ifndef     _LAYOUT_H_
#include    "layout.h"
#endif  //  _LAYOUT_H_

//------------------------------------------------------------------------------
// types
//------------------------------------------------------------------------------
typedef unsigned char   Value;
typedef unsigned char   Suit;
typedef unsigned char   Facing;
typedef unsigned char   Index;

//------------------------------------------------------------------------------
enum
{
    FACE_UP         =   0,
    FACE_DOWN       =   1,
    INVALID_CARD    =   0xff
};

//------------------------------------------------------------------------------
enum
{
    DRAW_LEFT_TOP       = 1,
    DRAW_LEFT_BOTTOM    = 2,
    DRAW_RIGHT_TOP      = 4,
    DRAW_RIGHT_BOTTOM   = 8,
    DRAW_LEFT           = DRAW_LEFT_TOP + DRAW_LEFT_BOTTOM,
    DRAW_TOP            = DRAW_LEFT_TOP + DRAW_RIGHT_TOP,
    DRAW_RIGHT          = DRAW_RIGHT_TOP + DRAW_RIGHT_BOTTOM,
    DRAW_BOTTOM         = DRAW_LEFT_BOTTOM + DRAW_RIGHT_BOTTOM,
    DRAW_ALL            = DRAW_LEFT_TOP + DRAW_RIGHT_TOP + DRAW_LEFT_BOTTOM + DRAW_RIGHT_BOTTOM
};

//------------------------------------------------------------------------------
// classes
//------------------------------------------------------------------------------
struct  Card : public ListNode
{
    public:
        Value           value;
        Suit            suit;
        Facing          facing;
        Index           id;
    
    private:
static  HDC		        cardfaceDC;
static  HDC		        cardbackDC;
    
    public:
        /* void */      Card (void);
        void            Draw (HDC hdc, const RECT* rect, unsigned draw = DRAW_ALL);
static  void            LoadBitmaps (void);
};

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
// END DECLARATIONS
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
#endif  //  _CARD_H_

#ifdef      _CARD_CPP_
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
// BEGIN DEFINITIONS
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

//------------------------------------------------------------------------------
// includes
//------------------------------------------------------------------------------
#ifndef     _LAYOUT_H_
#include    "layout.h"
#endif  //  _LAYOUT_H_

//------------------------------------------------------------------------------
// static members
//------------------------------------------------------------------------------
HDC		    Card::cardfaceDC = 0;
HDC		    Card::cardbackDC = 0;

//------------------------------------------------------------------------------
// classes
//------------------------------------------------------------------------------
/* void */
Card::Card (void) :
    facing (FACE_DOWN),
    value (INVALID_CARD),
    id (INVALID_CARD)
{
    InitNull ();
}

//------------------------------------------------------------------------------
void
Card::Draw (HDC hdc, const RECT* pRect, unsigned draw)
{
    if (facing == FACE_UP)
    {
        // place the card
        HPEN	old_pen = s_cast<HPEN> (SelectObject (hdc, GetStockObject (BLACK_PEN)));
        HBRUSH	old_brush = s_cast<HBRUSH> (SelectObject (hdc, GetStockObject (WHITE_BRUSH)));
        RoundRect (hdc, pRect->left, pRect->top, pRect->right, pRect->bottom, CARD_CORNER_RADIUS, CARD_CORNER_RADIUS);
        SelectObject (hdc, old_pen);
        SelectObject (hdc, old_brush);
        
        if (draw == DRAW_ALL)
            BitBlt (hdc, pRect->left + CARD_BLIT_X_INSET, pRect->top + CARD_BLIT_Y_INSET, CARD_BLIT_X_SIZE, CARD_BLIT_Y_SIZE, cardfaceDC, (value * CARD_X_SIZE) + CARD_BLIT_X_INSET, (suit * CARD_Y_SIZE) + CARD_BLIT_Y_INSET, SRCCOPY);
        else // if (draw == DRAW_TOP)
            BitBlt (hdc, pRect->left + CARD_BLIT_X_INSET, pRect->top + CARD_BLIT_Y_INSET, CARD_BLIT_X_SIZE, CARD_HEIGHT_OFFSET_UP, cardfaceDC, (value * CARD_X_SIZE) + CARD_BLIT_X_INSET, (suit * CARD_Y_SIZE) + CARD_BLIT_Y_INSET, SRCCOPY);
        /*
        // draw the value and suit
        if (draw bit_and DRAW_LEFT_TOP)
            BitBlt (hdc, pRect->left + CARD_FACE_X_INSET, pRect->top + CARD_FACE_Y_INSET, CARD_SUIT_X_SIZE, CARD_SUIT_Y_SIZE, suitsDC, suit * CARD_SUIT_X_SIZE, 0, SRCCOPY);
        if (draw bit_and DRAW_RIGHT_BOTTOM)
            BitBlt (hdc, pRect->right - (CARD_FACE_X_INSET + CARD_SUIT_X_SIZE), pRect->bottom - (CARD_FACE_Y_INSET + CARD_SUIT_Y_SIZE), CARD_SUIT_X_SIZE, CARD_SUIT_Y_SIZE, suitsDC, suit * CARD_SUIT_X_SIZE, CARD_SUIT_Y_SIZE, SRCCOPY);
        if (draw bit_and DRAW_RIGHT_TOP)
            BitBlt (hdc, pRect->right - (CARD_FACE_X_INSET + CARD_VALUE_X_SIZE), pRect->top + CARD_FACE_Y_INSET, CARD_VALUE_X_SIZE, CARD_VALUE_Y_SIZE, valuesDC, value * CARD_VALUE_X_SIZE, (suit bit_and 0x01) * 2 * CARD_VALUE_Y_SIZE, SRCCOPY);
        if (draw bit_and DRAW_LEFT_BOTTOM)
            BitBlt (hdc, pRect->left + CARD_FACE_X_INSET, pRect->bottom - (CARD_FACE_Y_INSET + CARD_VALUE_Y_SIZE), CARD_VALUE_X_SIZE, CARD_VALUE_Y_SIZE, valuesDC, value * CARD_VALUE_X_SIZE, (((suit bit_and 0x01) * 2) + 1) * CARD_VALUE_Y_SIZE, SRCCOPY);
        */
    }
    else
    {
        // place the card
        HPEN	old_pen = s_cast<HPEN> (SelectObject (hdc, GetStockObject (BLACK_PEN)));
        HBRUSH  card_brush = CreateSolidBrush (RGB (164,176,193));
        HBRUSH	old_brush = s_cast<HBRUSH> (SelectObject (hdc, card_brush));
        RoundRect (hdc, pRect->left, pRect->top, pRect->right, pRect->bottom, CARD_CORNER_RADIUS, CARD_CORNER_RADIUS);
        SelectObject (hdc, old_pen);
        SelectObject (hdc, old_brush);
        DeleteObject (card_brush);
        if (g_bUseCardback)
            BitBlt (hdc, pRect->left + CARD_BLIT_X_INSET, pRect->top + CARD_BLIT_Y_INSET, CARD_BLIT_X_SIZE, CARD_BLIT_Y_SIZE, cardbackDC, CARD_BLIT_X_INSET, CARD_BLIT_Y_INSET, SRCCOPY);
    }
}

//------------------------------------------------------------------------------
void
Card::LoadBitmaps (void)
{
    HDC screenDC = GetDC (g_hWnd);
    cardbackDC = CreateCompatibleDC (screenDC);
    cardfaceDC = CreateCompatibleDC (screenDC);
	SelectObject (cardfaceDC, LoadBitmap (g_hInst, (LPCTSTR) IDB_CARDFACE));
	SelectObject (cardbackDC, LoadBitmap (g_hInst, (LPCTSTR) IDB_CARDBACK));
    ReleaseDC (g_hWnd, screenDC);
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
// END DEFINITIONS
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
#endif  //  _CARD_CPP_
