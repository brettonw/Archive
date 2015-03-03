//-----------------------------------------------------------------------------
// Copyright (C) 2002 Bretton Wade, All rights reserved
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// include files
//-----------------------------------------------------------------------------
#include    "precompile.h"
#include    "timer.h"

//-----------------------------------------------------------------------------
// class methods
//-----------------------------------------------------------------------------
/* void */
Timer::Timer (void)
{
    // find the timer frequency
	__int64		frequency;
	QueryPerformanceFrequency (r_cast<LARGE_INTEGER*> (&frequency));
	myFrequency = 1 / s_cast<double> (frequency);

    // initialize the start value with the current "time"
	QueryPerformanceCounter (r_cast<LARGE_INTEGER*> (&myStart));
}

//-----------------------------------------------------------------------------
float
Timer::Elapsed (void) const
{
    // fetch the current "time"
	__int64		stop;
	QueryPerformanceCounter (r_cast<LARGE_INTEGER*> (&stop));

    // convert that to elapsed time in second
	double      elapsed = (stop - myStart) * myFrequency;

    // return the result
	return s_cast<float> (elapsed);
}

//-----------------------------------------------------------------------------
