//------------------------------------------------------------------------------
// Copyright (C) 2006 Bretton Wade, All Rights Reserved
//------------------------------------------------------------------------------

#ifndef     _LAYOUT_H_
#define     _LAYOUT_H_
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
// BEGIN DECLARATIONS
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

//------------------------------------------------------------------------------
// values
//------------------------------------------------------------------------------

// +---+ +---+ +---+ +---+ +---+ +---+ +---+ +---+ +---+ +---+
// |   | |   | |   | |   | |   | |   | |   | |   | |   | |   | 
// |   | |   | |   | |   | |   | |   | |   | |   | |   | |   | 
// +---+ +---+ +---+ +---+ +---+ +---+ +---+ +---+ +---+ +---+
// +---+ +---+ +---+ +---+ +---+ +---+ +---+ +---+ +---+ +---+
// |   | |   | |   | |   | |   | |   | |   | |   | |   | |   | 
// |   | |   | |   | |   | |   | |   | |   | |   | |   | |   | 
// +---+ +---+ +---+ +---+ +---+ +---+ +---+ +---+ +---+ +---+

//
// layout is measured from an abstract rectangle whose top left corner is at (indent, indent)
// this point is the top left corner of the first card in a stack, and offsets are used to
// distribute the cards from there
enum
{
    CARD_X_SIZE             =   21 * SCALE_FACTOR,
    CARD_Y_SIZE             =   28 * SCALE_FACTOR,
    CARD_BLIT_X_INSET       =   1 + SCALE_FACTOR,
    CARD_BLIT_Y_INSET       =   1 + SCALE_FACTOR,
    CARD_BLIT_X_SIZE        =   CARD_X_SIZE - (2 * CARD_BLIT_X_INSET),
    CARD_BLIT_Y_SIZE        =   CARD_Y_SIZE - (2 * CARD_BLIT_X_INSET),
    CARD_FACE_X_INSET       =   3,
    CARD_FACE_Y_INSET       =   2,
    CARD_VALUE_X_SIZE       =   7,
    CARD_VALUE_Y_SIZE       =   7,
    CARD_SUIT_X_SIZE        =   7,
    CARD_SUIT_Y_SIZE        =   8,
    CARD_CORNER_RADIUS      =   5 + SCALE_FACTOR,
    CARD_SPACING            =   2,
    CARD_X_INDENT           =   6,
    CARD_Y_INDENT           =   2,
    LAYOUT_WIDTH            =   (CARD_X_SIZE + CARD_SPACING),
    LAYOUT_HEIGHT           =   (CARD_Y_SIZE + CARD_SPACING),
    CARD_HEIGHT_OFFSET_DN   =   (CARD_Y_SIZE / 7),
    CARD_HEIGHT_OFFSET_UP   =   11 * SCALE_FACTOR,
    CARD_WIDTH_OFFSET       =   11 * SCALE_FACTOR
};

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
// END DECLARATIONS
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
#endif  //  _LAYOUT_H_

#ifdef      _LAYOUT_CPP_
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
// BEGIN DEFINITIONS
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
// END DEFINITIONS
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
#endif  //  _LAYOUT_CPP_
