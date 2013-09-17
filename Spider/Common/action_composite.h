//------------------------------------------------------------------------------
// Copyright (C) 2006 Bretton Wade, All Rights Reserved
//------------------------------------------------------------------------------

#ifndef     _ACTION_COMPOSITE_H_
#define     _ACTION_COMPOSITE_H_
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
struct  ActionComposite : public Action
{
    private:
        Action*         pActionList;
        
    private:
        void            Unwind (Action* pAction);
        
    public:
        /* void */      ActionComposite (void);
virtual /* void */      ~ActionComposite (void);
        void            Do (Action* pAction);
virtual void            Do (void);
virtual void            Undo (void);
};

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
// END DECLARATIONS
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
#endif  //  _ACTION_COMPOSITE_H_

#ifdef      _ACTION_COMPOSITE_CPP_
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
// BEGIN DEFINITIONS
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

//------------------------------------------------------------------------------
// classes
//------------------------------------------------------------------------------
void
ActionComposite::Unwind (Action* pAction)
{
    if (pAction)
    {
        Unwind (pAction->pNext);
        pAction->Do ();
    }
}

//------------------------------------------------------------------------------
/* void */
ActionComposite::ActionComposite (void) :
    pActionList (0)
{
}

//------------------------------------------------------------------------------
/* void */
ActionComposite::~ActionComposite (void)
{
    Action* pAction = pActionList;
    while (pAction)
    {
        pActionList = pAction;
        pAction = pAction->pNext;
        delete pActionList;
    }
}

//------------------------------------------------------------------------------
void
ActionComposite::Do (Action* pAction)
{
    pAction->pNext = pActionList;
    pActionList = pAction;
    pAction->Do ();
}

//------------------------------------------------------------------------------
void
ActionComposite::Do (void)
{
    // this is weird, but we need to run the list in reverse order to re-do
    Unwind (pActionList);
}

//------------------------------------------------------------------------------
void
ActionComposite::Undo (void)
{
    Action* pAction = pActionList;
    while (pAction)
    {
        pAction->Undo ();
        pAction = pAction->pNext;
    }
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
// END DEFINITIONS
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
#endif  //  _ACTION_COMPOSITE_CPP_
