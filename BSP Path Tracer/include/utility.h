//------------------------------------------------------------------------------
//	File:					utility.h
//	Date:					2/25/95
//	Author:				Bretton Wade
//
//	Description:	this file contains common utility functions
//
//------------------------------------------------------------------------------

#ifndef	UTILITY
#define	UTILITY

//------------------------------------------------------------------------------
//	inline utilitiy functions
//------------------------------------------------------------------------------
inline	real	DegreesToRadians (real angle)																			//	convert degrees to radians
{																																								//	begin
	return angle * (PI / R(180.0));																								//	multiply the angle by the conversion factor
}																																								//	end

//------------------------------------------------------------------------------
inline	real	RadiansToDegrees (real angle)																			//	convert radians to degrees
{																																								//	begin
	return angle * (R(180.0) / PI);																								//	multiply the angle by the conversion factor
}																																								//	end

//------------------------------------------------------------------------------
/*
inline	real	abs (real v)																											//	return the absolute value
{																																								//	begin
	return (v >= R(0.0)) ? v : -v;																								//	negate v if it is less than 0
}																																								//	end
*/
//------------------------------------------------------------------------------
inline	real	sgn (real v)																											//	return the sign of the value
{																																								//	begin
	return (v > R(0.0)) ? R(1.0) : (v == R(0.0)) ? R(0.0) : R(-1.0);							//	return 1, 0, or -1 based on v
}																																								//	end

//------------------------------------------------------------------------------
inline	bool	odd (int v)																												//	return whether the number is odd
{																																								//	begin
	return bool (v & 1);																													//	if the one bit is set, the number is odd
}																																								//	end

//------------------------------------------------------------------------------
inline	real	fract (real v)																										//	return the fractional part of v
{																																								//	begin
	v = v - real (floor (v));																														//	get the fractional part
	if (v < R(0.0)) v += R(1.0);																									//	if it is a negative value, add one
	return v;																																			//	return the result
}																																								//	end

//------------------------------------------------------------------------------
inline	real	UnitRandom (void)																									//	return a random number in the range 0 - 1
{																																								//	begin
	return rand () / real (RAND_MAX);																							//	return the absolute value of the divided Random number
}																																								//	end

//------------------------------------------------------------------------------

#endif	//UTILITY
