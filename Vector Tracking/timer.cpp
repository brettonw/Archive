//-----------------------------------------------------------------------------
// Copyright (C) 2002 Bretton Wade, All rights reserved
// This file may be freely transmitted for educational purposes only
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// include files
//-----------------------------------------------------------------------------
#include    "stdafx.h"
#include    "timer.h"

//-----------------------------------------------------------------------------
// class methods
//-----------------------------------------------------------------------------
/* void */
Timer::Timer (void)
{
    // find the timer frequency
	__int64		frequency;
	QueryPerformanceFrequency (reinterpret_cast<LARGE_INTEGER*> (&frequency));
	myFrequency = 1 / static_cast<float> (frequency);

    // initialize the start value with the current "time"
	QueryPerformanceCounter (reinterpret_cast<LARGE_INTEGER*> (&myStart));
}

//-----------------------------------------------------------------------------
float
Timer::Elapsed (void) const
{
    // fetch the current "time"
	__int64		stop;
	QueryPerformanceCounter (reinterpret_cast<LARGE_INTEGER*> (&stop));

    // convert that to elapsed time in second
	float	elapsed = (stop - myStart) * myFrequency;

    // return the result
	return elapsed;
}

//-----------------------------------------------------------------------------
