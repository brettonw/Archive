//------------------------------------------------------------------------------
//	Copyright ©1996-2004 Bretton Wade, All rights reserved.
//------------------------------------------------------------------------------

#ifndef     _TIMER_
#define     _TIMER_

//------------------------------------------------------------------------------
// A basic timer class, accuracy is that of the high performance counters.
//------------------------------------------------------------------------------
class	Timer
{
	public:
		//--------------------------------------------------------------------------------------------
		// Public Methods:
		// Start			-> Start the timer
		// Elapsed			-> Return the elapsed time in seconds since the last call to Start
		//					   or Interval
		// Interval			-> Return the elapsed time in seconds since the last call to Start
		//					   or Interval, and restart the timer
		//--------------------------------------------------------------------------------------------
		/* void */	Timer (void);
		void		Start (void);
		double		Elapsed (void) const;
		double		Interval (void);

		//--------------------------------------------------------------------------------------------
		// Static Methods:
		// Initialize		-> Called prior to using any timers
		//--------------------------------------------------------------------------------------------
		void		Initialize (void);

		//--------------------------------------------------------------------------------------------
	private:
		//--------------------------------------------------------------------------------------------
		// Private Data:
		// my_start			-> A saved performance counter value subtracted from new values
		//					   for timing
		//--------------------------------------------------------------------------------------------
		__int64		my_start;

		//--------------------------------------------------------------------------------------------
		// Static Data:
		// my_frequency		-> The frequency of the performance counter
		//--------------------------------------------------------------------------------------------
		double		the_frequency;

		//--------------------------------------------------------------------------------------------
};

//------------------------------------------------------------------------------

#endif  //  _TIMER_
