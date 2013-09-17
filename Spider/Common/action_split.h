//------------------------------------------------------------------------------
// Copyright (C) 2006 Bretton Wade, All Rights Reserved
//------------------------------------------------------------------------------

#ifndef     _ACTION_SPLIT_H_
#define     _ACTION_SPLIT_H_
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
// BEGIN DECLARATIONS
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

//------------------------------------------------------------------------------
// includes
//------------------------------------------------------------------------------
#ifndef     _ACTION_H_
#include    "action.h"
#endif  //  _ACTION_H_

#ifndef     _STACK_H_
#include    "stack.h"
#endif  //  _STACK_H_

//------------------------------------------------------------------------------
// classes
//------------------------------------------------------------------------------
struct  ActionSplit : public Action
{
    private:
        Stack*          pFrom;
        Stack*          pTo;
        Card*           pCard;
        
    public:
        /* void */      ActionSplit (Stack* pFrom, Stack* pTo, Card* pCard);
virtual /* void */      ~ActionSplit (void);
virtual void            Do (void);
virtual void            Undo (void);
};

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
// END DECLARATIONS
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
#endif  //  _ACTION_SPLIT_H_

#ifdef      _ACTION_SPLIT_CPP_
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
// BEGIN DEFINITIONS
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

//------------------------------------------------------------------------------
// classes
//------------------------------------------------------------------------------
/* void */
ActionSplit::ActionSplit (Stack* _pFrom, Stack* _pTo, Card* _pCard) :
    pFrom (_pFrom),
    pTo (_pTo),
    pCard (_pCard)
{
}

//------------------------------------------------------------------------------
/* void */
ActionSplit::~ActionSplit (void)
{
}

//------------------------------------------------------------------------------
void
ActionSplit::Do (void)
{
    pFrom->Split (pCard, pTo);
}

//------------------------------------------------------------------------------
void
ActionSplit::Undo (void)
{
    pTo->Split (pCard, pFrom);
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
// END DEFINITIONS
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
#endif  //  _ACTION_SPLIT_CPP_
