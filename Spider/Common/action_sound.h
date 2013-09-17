//------------------------------------------------------------------------------
// Copyright (C) 2006 Bretton Wade, All Rights Reserved
//------------------------------------------------------------------------------

#ifndef     _ACTION_SOUND_H_
#define     _ACTION_SOUND_H_
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
// BEGIN DECLARATIONS
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

//------------------------------------------------------------------------------
// includes
//------------------------------------------------------------------------------
#ifndef     _ACTION_H_
#include    "action.h"
#endif  //  _ACTION_H_

//------------------------------------------------------------------------------
// classes
//------------------------------------------------------------------------------
struct  ActionSound : public Action
{
    private:
        int             id;            
        
    public:
        /* void */      ActionSound (int _id);
virtual /* void */      ~ActionSound (void);
virtual void            Do (void);
virtual void            Undo (void);
};

//------------------------------------------------------------------------------
// macros
//------------------------------------------------------------------------------
#define PLAY_SOUND(id)  { ActionSound as (id); as.Do (); }

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
// END DECLARATIONS
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
#endif  //  _ACTION_SOUND_H_

#ifdef      _ACTION_SOUND_CPP_
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
// BEGIN DEFINITIONS
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

//------------------------------------------------------------------------------
// classes
//------------------------------------------------------------------------------
/* void */
ActionSound::ActionSound (int _id) :
    id (_id)
{
}

//------------------------------------------------------------------------------
/* void */
ActionSound::~ActionSound (void)
{
}

//------------------------------------------------------------------------------
void
ActionSound::Do (void)
{
    if (g_bPlaySound)
        PlaySound (r_cast<LPCWSTR> (id), g_hInst, SND_ASYNC bit_or SND_RESOURCE);
}

//------------------------------------------------------------------------------
void
ActionSound::Undo (void)
{
    if (g_bPlaySound)
        PlaySound (r_cast<LPCWSTR> (IDS_UNDO), g_hInst, SND_ASYNC bit_or SND_RESOURCE);
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
// END DEFINITIONS
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
#endif  //  _ACTION_SOUND_CPP_
