//------------------------------------------------------------------------------
// Win95 Headers
//------------------------------------------------------------------------------
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <windowsx.h>
#undef max
#undef min

//------------------------------------------------------------------------------
// ANSI headers
//------------------------------------------------------------------------------
#include <limits.h>
#include <float.h>
#include <math.h>
#include <stdarg.h>
#include <stdlib.h>

//------------------------------------------------------------------------------
// defines
//------------------------------------------------------------------------------
//#define USE_FLOATS 

//------------------------------------------------------------------------------
// types
//------------------------------------------------------------------------------
typedef unsigned short ushort; 
typedef unsigned int uint; 
typedef unsigned long ulong; 
typedef unsigned char uchar; 
typedef char* cstr; 
typedef uchar* pstr; 
#ifdef USE_FLOATS 
typedef float real; 
#else 
typedef double real; 
#endif 

//------------------------------------------------------------------------------
// enumerations
//------------------------------------------------------------------------------
#ifndef bool 
#define bool int 
#endif 
#ifndef TRUE 
#define TRUE 1 
#define FALSE 0 
#endif 

//------------------------------------------------------------------------------
// macros
//------------------------------------------------------------------------------
#ifdef USE_FLOATS 
#define R(num) (num ## F) 
#else 
#define R(num) (num) 
#endif 

#define TopLeft(r) (((Point *) &(r))[0]) 
#define BotRight(r) (((Point *) &(r))[1]) 
#define hiword(val) ((ulong (val) & 0xFFFF0000) >> 16) 
#define loword(val) (val & 0x0000FFFF) 

//------------------------------------------------------------------------------
// synonyms
//------------------------------------------------------------------------------
#define and     &&
#define or      ||
#define not     !
#define xor     ^
#define and_eq  &=
#define bit_and & 
#define bitand  & 
#define bit_or  |
#define bitor   |
#define compl   ~
#define not_eq  !=
#define or_eq   |=
#define xor_eq  ^=

#ifdef USE_FLOATS 
#define COS  cosf 
#define ACOS acos 
#define SIN  sinf 
#define ASIN asinf 
#define TAN  tanf 
#define ATAN atanf 
#define ATAN2 atan2f 
#define SQRT sqrtf 
#define FABS fabsf 
#define POW  powf 
#define FLOOR floorf 
#else 
#define COS  cos 
#define ACOS acos 
#define SIN  sin 
#define ASIN asin 
#define TAN  tan 
#define ATAN atan 
#define ATAN2 atan2 
#define SQRT sqrt 
#define FABS fabs 
#define POW  pow 
#define FLOOR floor 
#endif 

//------------------------------------------------------------------------------
// constants
//------------------------------------------------------------------------------
//const real INFINITY = R(100000.0); 
const real EPSILON = R(1.0) / R(100000.0); 
#ifndef PI
const real PI = R(3.14159265358979323846); 
#endif
const real TWO_PI = PI * R(2.0); 

//------------------------------------------------------------------------------
