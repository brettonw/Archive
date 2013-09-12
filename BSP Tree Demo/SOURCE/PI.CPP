//------------------------------------------------------------------------------
//	File:					pi.cpp
//	Date:					9/30/96
//	Author:				Bretton Wade
//
//	Description:	this file contains the definitions of PI and accessories
//
//------------------------------------------------------------------------------

#include "precompile.h"

//------------------------------------------------------------------------------
#include "pi.h"

//------------------------------------------------------------------------------
real	PI;																																				//	3.14
real	OVER_PI;																																	//	0.32
real	TWO_PI;																																		//	6.28
real	OVER_TWO_PI;																															//	0.16
real	PI_OVER_TWO;																															//	1.57

//------------------------------------------------------------------------------
void	DefinePI (void)																														//	fill in the definitions of PI
{																																								//	begin
	real	PI_OVER_FOUR = ATAN (R(1.0));																						//	inverse arc tan of 1
	PI = R(4.0) * PI_OVER_FOUR;																										//	3.14
	OVER_PI = R(1.0) / PI;																												//	0.32
	TWO_PI = R(8.0) * PI_OVER_FOUR;																								//	6.28
	OVER_TWO_PI = R(1.0) / TWO_PI;																								//	0.16
	PI_OVER_TWO = R(2.0) * PI_OVER_FOUR;																					//	1.57
}																																								//	end

//------------------------------------------------------------------------------
