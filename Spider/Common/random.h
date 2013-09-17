//------------------------------------------------------------------------------
// Copyright (C) 2006 Bretton Wade, All Rights Reserved
//------------------------------------------------------------------------------

#ifndef     _RANDOM_H_
#define     _RANDOM_H_
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
// BEGIN DECLARATIONS
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

//------------------------------------------------------------------------------
// functions
//------------------------------------------------------------------------------
// return a random integer in the range (min..max]
unsigned    Random (unsigned min, unsigned max, bool secure = false);

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
// END DECLARATIONS
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
#endif //   _RANDOM_H_

#ifdef      _RANDOM_CPP_
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
// BEGIN DEFINITIONS
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

//------------------------------------------------------------------------------
// functions
//------------------------------------------------------------------------------
unsigned    Random (unsigned min, unsigned max, bool secure)
{
    // ensure well conditioned input
    if (max < min)
    {
        unsigned tmp = max;
        max = min;
        min = tmp;
    }
    
    // compute the difference between the two ranges
    unsigned    delta = max - min;
    
    // the place we'll store the number in the range (0..delta]
    unsigned    number;

    // we want to grab the old random seed, which was the last random 
    // number returned, so that we can put it back after we compute a
    // secure random number. that way we can ensure that the non-secure
    // generator continues to return the same sequence
    static  int last = 1;
    
    // check if we are doing this secure    
    if (secure)
    {
    #ifndef _DEBUG
        srand (s_cast<unsigned> (GetTickCount ()));
    #else
        srand (1);
    #endif

        // calculate a random number in the range (0..delta]
        do
            number = (rand () * delta) / RAND_MAX;                  // XXX need a uniform distributor
        while (number == delta);
        
        // restore the old seed
        srand (last);
    }
    else
    {
        // calculate a random number in the range (0..delta], grabbing
        // the value returned every time to support the secure generator
        do
        {
            last = rand ();
            number = (last * delta) / RAND_MAX;                     // XXX need a uniform distributor
        }
        while (number == delta);
    }

    // return a value in the range (min..max]
    return min + number;
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
// END DEFINITIONS
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
#endif  //  _RANDOM_CPP_
