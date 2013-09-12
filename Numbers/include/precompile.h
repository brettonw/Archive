//------------------------------------------------------------------------------
//	Copyright ©1996-2004 Bretton Wade, All rights reserved.
//------------------------------------------------------------------------------

#ifndef     _PRECOMPILE_
#define     _PRECOMPILE_

//------------------------------------------------------------------------------
//	Control Macros for Windows Headers
//------------------------------------------------------------------------------
#define	    WIN32_LEAN_AND_MEAN

//------------------------------------------------------------------------------
//	Win95 Headers
//------------------------------------------------------------------------------
#include    <windows.h>
#include    <windowsx.h>
#pragma warning( disable : 4244 )

//------------------------------------------------------------------------------
//	ANSI Headers
//------------------------------------------------------------------------------
#include    <stdio.h>
#include    <math.h>
#include    <float.h>
#include    <limits.h>
#include    <stdlib.h>
#include    <stdarg.h>

//------------------------------------------------------------------------------
//	c++ keywords that VCC doesn't properly support
//------------------------------------------------------------------------------
#define	and				&&
#define	or				||
#define	not				!
#define	bit_and			&
#define	bit_or			|
#define	exclusive_or	^
#define	complement		~

//------------------------------------------------------------------------------
//	basic types
//------------------------------------------------------------------------------
typedef	char			    sInt1;
typedef	short			    sInt2;
typedef	int				    sInt4;
typedef	__int64			    sInt8;
typedef	unsigned char	    uInt1;
typedef	unsigned short	    uInt2;
typedef	unsigned int	    uInt4;
typedef unsigned __int64    uInt8;

//------------------------------------------------------------------------------
//	pointer types
//------------------------------------------------------------------------------
typedef	sInt1*			    sPtr1;
typedef	sInt2*			    sPtr2;
typedef	sInt4*			    sPtr4;
typedef	sInt8*			    sPtr8;
typedef	uInt1*			    uPtr1;
typedef	uInt2*			    uPtr2;
typedef	uInt4*			    uPtr4;
typedef	uInt8*			    uPtr8;

//------------------------------------------------------------------------------
//	string types
//------------------------------------------------------------------------------
typedef	char*			cstr;
typedef	const char*		const_cstr;

//------------------------------------------------------------------------------

#endif  //  _PRECOMPILE_

