//-----------------------------------------------------------------------------
// Copyright (C) 2002 Bretton Wade, All rights reserved
// This file may be freely transmitted for educational purposes only
//-----------------------------------------------------------------------------

#ifndef     _TIMER_H_
#define     _TIMER_H_

//-----------------------------------------------------------------------------
// class definition for a Timer, an object that acts like a stopwatch
//-----------------------------------------------------------------------------
class Timer
{
    public:
        /* void */          Timer (void);
        float               Elapsed (void) const;
		
    protected:
        __int64             myStart;
        double              myFrequency;
};

//-----------------------------------------------------------------------------

#endif  //  _TIMER_H_
