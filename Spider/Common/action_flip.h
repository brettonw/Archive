//------------------------------------------------------------------------------
// Copyright (C) 2006 Bretton Wade, All Rights Reserved
//------------------------------------------------------------------------------

#ifndef     _ACTION_FLIP_H_
#define     _ACTION_FLIP_H_
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
// BEGIN DECLARATIONS
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

//------------------------------------------------------------------------------
// includes
//------------------------------------------------------------------------------
#ifndef     _ACTION_H_
#include    "action.h"
#endif  //  _ACTION_H_

#ifndef     _CARD_H_
#include    "card.h"
#endif  //  _CARD_H_

//------------------------------------------------------------------------------
// classes
//------------------------------------------------------------------------------
struct  ActionFlip : public Action
{
    private:
        Card*           pCard;
        
    public:
        /* void */      ActionFlip (Card* pCard);
virtual /* void */      ~ActionFlip (void);
virtual void            Do (void);
virtual void            Undo (void);
};

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
// END DECLARATIONS
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
#endif  //  _ACTION_FLIP_H_

#ifdef      _ACTION_FLIP_CPP_
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
// BEGIN DEFINITIONS
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

//------------------------------------------------------------------------------
// classes
//------------------------------------------------------------------------------
/* void */
ActionFlip::ActionFlip (Card* _pCard) :
    pCard (_pCard)
{
}

//------------------------------------------------------------------------------
/* void */
ActionFlip::~ActionFlip (void)
{
}

//------------------------------------------------------------------------------
void
ActionFlip::Do (void)
{
    pCard->facing = FACE_UP;
}

//------------------------------------------------------------------------------
void
ActionFlip::Undo (void)
{
    pCard->facing = FACE_DOWN;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
// END DEFINITIONS
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
#endif  //  _ACTION_FLIP_CPP_
