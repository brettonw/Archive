#ifndef	BSP_PRECOMPILE
#define	BSP_PRECOMPILE

//------------------------------------------------------------------------------
//	ANSI headers
//------------------------------------------------------------------------------
#include <iostream>
#include <limits.h>
#include <float.h>
#include <math.h>
#include <stdarg.h>
#include <stdlib.h>

//------------------------------------------------------------------------------
//	defines
//------------------------------------------------------------------------------
#define	USE_FLOATS																															//	whether or not the real type is a float

//------------------------------------------------------------------------------
//	types
//------------------------------------------------------------------------------
typedef	unsigned short	ushort;																									//	abbreviation
typedef	unsigned long		ulong;																									//	abbreviation
typedef	unsigned char		uchar;																									//	abbreviation
typedef	char						*cstr;																									//	a C style string (NULL terminated)
typedef	uchar						*pstr;																									//	a pascal style string (preceded by a length byte)
#ifdef	USE_FLOATS																															//	if the real type is a float
typedef	float						real;																										//	common name for floating point type
#else																																						//	otherwise
typedef	double					real;																										//	common name for floating point type
#endif																																					//	end

//------------------------------------------------------------------------------
//	enumerations
//------------------------------------------------------------------------------
#ifndef	bool																																		//	if the compiler does nto recognize the keyword 'bool'
#define	bool	int																																//	use an int instead
#endif																																					//	end
#ifndef	TRUE																																		//	if TRUE and FALSE are not defined
#define	TRUE	1																																	//	define TRUE
#define	FALSE 0																																	//	define FALSE
#endif																																					//	end

//------------------------------------------------------------------------------
//	macros
//------------------------------------------------------------------------------
#ifdef	USE_FLOATS																															//	if the real type is a float
#define	R(num)	(num ## F)																											//	want constants to have an F after them to specify float types
#else																																						//	otherwise
#define	R(num)	(num)																														//	want constants to have an L after them to specify long double types
#endif																																					//	end

#define	TopLeft(r)	(((Point *) &(r))[0])																				//	an easy way to get a point from a rectangle
#define	BotRight(r)	(((Point *) &(r))[1])																				//	an easy way to get a point from a rectangle
#define	hiword(val)	((ulong (val) & 0xFFFF0000) >> 16)													//	make a short from the hiword of a long word
#define	loword(val)	(val & 0x0000FFFF)																					//	want only the low word of a long word

//------------------------------------------------------------------------------
//	synonyms
//------------------------------------------------------------------------------
#ifdef	USE_FLOATS																															//	if the real type is a float
#define	COS		cosf																															//	use float version of function for speed
#define	ACOS	acosf																															//	use float version of function for speed
#define	SIN		sinf																															//	use float version of function for speed
#define	ASIN	asinf																															//	use float version of function for speed
#define	TAN		tanf																																//	use float version of function for speed
#define	ATAN	atanf																															//	use float version of function for speed
#define	ATAN2	atan2f																														//	use float version of function for speed
#define	SQRT	sqrtf																															//	use float version of function for speed
#define	FABS	fabsf																															//	use float version of function for speed
#define	POW		powf																															//	use float version of function for speed
#else																																						//	otherwise, use the double versions
#define	COS		cos																																//	use normal version of function
#define	ACOS	acos																															//	use normal version of function
#define	SIN		sin																																//	use normal version of function
#define	ASIN	asin																															//	use normal version of function
#define	TAN		tan																																//	use normal version of function
#define	ATAN	atan																															//	use normal version of function
#define	ATAN2	atan2																															//	use normal version of function
#define	SQRT	sqrt																															//	use normal version of function
#define	FABS	fabs																															//	use normal version of function
#define	POW		pow																																//	use normal version of function
#endif																																					//	end real type is a float

//------------------------------------------------------------------------------
//	constants
//------------------------------------------------------------------------------
const		real	INFINITY = R(100000.0);																						//	floating point infinity value
const		real	EPSILON = R(1.0) / R(100000.0);																		//	floating point epsilon value
#ifndef	PI
const		real	PI = R(3.14159265358979323846);																		//	pi
#endif
const		real	TWO_PI = PI * R(2.0);																							//	two times pi

//------------------------------------------------------------------------------

#endif	//	BSP_PRECOMPILE
