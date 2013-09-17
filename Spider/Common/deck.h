//------------------------------------------------------------------------------
// Copyright (C) 2006 Bretton Wade, All Rights Reserved
//------------------------------------------------------------------------------

#ifndef     _DECK_H_
#define     _DECK_H_
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
// BEGIN DECLARATIONS
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

//------------------------------------------------------------------------------
// includes
//------------------------------------------------------------------------------
#ifndef     _CARD_H_
#include    "card.h"
#endif  //  _CARD_H_

//------------------------------------------------------------------------------
// types
//------------------------------------------------------------------------------
enum        Suits
{
    SINGLE_SUIT     =   1,
    DOUBLE_SUIT     =   2,
    QUAD_SUIT       =   4
};

//------------------------------------------------------------------------------
enum        Decks
{
    SINGLE_DECK     =   1,
    DOUBLE_DECK     =   2,
    TRIPLE_DECK     =   3,
    QUAD_DECK       =   4
};

//------------------------------------------------------------------------------
enum
{
    CARDS_IN_SUIT   =   13,
    SUITS_IN_DECK   =   4,
    CARDS_IN_DECK   =   CARDS_IN_SUIT * SUITS_IN_DECK,
    MAX_CARDS       =   CARDS_IN_DECK * QUAD_DECK
};

//------------------------------------------------------------------------------
// The deck is the source of cards, and cards are dealt from the deck into
// stacks that are not managed by the deck. *All* cards in the game must come 
// from the same deck, as that is how they reference each other. The deck always
// knows where its cards are, so use it to get to a card by id. Only 4 decks
// (208 cards) are supported
//------------------------------------------------------------------------------
class   Deck
{
    private:
        Index           top;
        Card*           stack;
    
    public:
        /* void */      Deck (Suits suits = QUAD_SUIT, Decks decks = SINGLE_DECK);
        /* void */      ~Deck (void);
        void            Shuffle (void);
        
        Index           CardsLeft (void) const              { return top; }
        Card*           Deal (void)                         { return stack + (--top); }    // XXX this doesn't handle trying to deal more cards than we have
};

//------------------------------------------------------------------------------
// global variables
//------------------------------------------------------------------------------
extern Deck*           g_pDeck;

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
// END DECLARATIONS
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
#endif //   _DECK_H_

#ifdef      _DECK_CPP_
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
// BEGIN DEFINITIONS
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

//------------------------------------------------------------------------------
// includes
//------------------------------------------------------------------------------
#ifndef     _RANDOM_H_
#include    "random.h"
#endif  //  _RANDOM_H_

//------------------------------------------------------------------------------
// globals
//------------------------------------------------------------------------------
Deck*   g_pDeck = 0;

//------------------------------------------------------------------------------
// classes
//------------------------------------------------------------------------------
/* void */  
Deck::Deck (Suits suits, Decks decks) :
    top (s_cast<Index> (decks) * CARDS_IN_DECK),
    stack (new Card[top])
{
    // temporary counter
    Index   tmp = 0;
    
    // create each deck requested in order from low to high
    for (Index i = 0; i < s_cast<Index> (decks); ++i)
    {
        // create all the suits, with all of the values
        for (Index suit = 0; suit < SUITS_IN_DECK; ++suit)
        {
            for (Index value = 0; value < CARDS_IN_SUIT; ++value)
            {
                Card&   card = stack[tmp++];
                card.suit = suit % suits;
                card.value = value;
            }
        }
    }
}

//------------------------------------------------------------------------------
/* void */  
Deck::~Deck (void)
{
    delete[] stack;
}

//------------------------------------------------------------------------------
void   
Deck::Shuffle (void)
{
    // from 0 to top, randomly swap
    for (Index i = 0; i < top; ++i)
    {
        Index   permute = Random (0, top);
        Card    tmp = stack[i];
        stack[i] = stack[permute];
        stack[permute] = tmp;
    }
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
// END DEFINITIONS
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
#endif  //  _DECK_CPP_
