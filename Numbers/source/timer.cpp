//------------------------------------------------------------------------------
//	Copyright ©1996-2004 Bretton Wade, All rights reserved.
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
#ifndef     _PRECOMPILE_
#include    "precompile.h"
#endif  //  _PRECOMPILE_

#ifndef     _TIMER_
#include    "timer.h"
#endif  //  _TIMER_

//------------------------------------------------------------------------------
Timer::Timer (void)
{
	my_start = 0;
}

//------------------------------------------------------------------------------
void	Timer::Start (void)
{
	QueryPerformanceCounter ((LARGE_INTEGER *) &my_start);
}

//------------------------------------------------------------------------------
double	Timer::Elapsed (void) const
{
	__int64	stop;
	QueryPerformanceCounter ((LARGE_INTEGER *) &stop);
	double	elapsed = (stop - my_start) * the_frequency;
	return elapsed;
}

//------------------------------------------------------------------------------
double	Timer::Interval (void)
{
	__int64	stop;
	QueryPerformanceCounter ((LARGE_INTEGER *) &stop);
	double	elapsed = (stop - my_start) * the_frequency;
	my_start = stop;
	return elapsed;
}

//------------------------------------------------------------------------------
void	Timer::Initialize (void)
{
	__int64	frequency;
	QueryPerformanceFrequency ((LARGE_INTEGER *) &frequency);
	the_frequency = 1.0 / frequency;
}

//------------------------------------------------------------------------------
