//-----------------------------------------------------------------------------
// Copyright (C) 1997-2006 Bretton Wade
// All Rights Reserved
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
	QueryPerformanceFrequency (reinterpret_cast<LARGE_INTEGER*> (&frequency));
	m_frequency = 1 / static_cast<float> (frequency);

    // initialize the start value with the current "time"
	QueryPerformanceCounter (reinterpret_cast<LARGE_INTEGER*> (&m_start));
}

//-----------------------------------------------------------------------------
float
Timer::Elapsed (void) const
{
    // fetch the current "time"
	__int64		stop;
	QueryPerformanceCounter (reinterpret_cast<LARGE_INTEGER*> (&stop));

    // convert that to elapsed time in second
	float	elapsed = (stop - m_start) * m_frequency;

    // return the result
	return elapsed;
}

//-----------------------------------------------------------------------------
