//-----------------------------------------------------------------------------
// Copyright (C) 1997-2006 Bretton Wade
// All Rights Reserved
//-----------------------------------------------------------------------------

#ifndef     DEFINITIONS_H
#define     DEFINITIONS_H

//-----------------------------------------------------------------------------
// keywords that VC leaves out
//-----------------------------------------------------------------------------
#define     and     &&
#define     or      ||
#define     not     !
#define     xor     ^
#define     and_eq  &=
#define     bit_and &
#define     bitand  &
#define     bit_or  |
#define     bitor   |
#define     compl   ~
#define     not_eq  !=
#define     or_eq   |=
#define     xor_eq  ^=

//-----------------------------------------------------------------------------
// renamed functions
//-----------------------------------------------------------------------------
#define     StackNew    _alloca

//-----------------------------------------------------------------------------
// basic types I like to have
//-----------------------------------------------------------------------------
typedef unsigned int    uInt;
typedef          int    sInt;

typedef unsigned int    uInt4;
typedef          int    sInt4;

typedef unsigned short  uInt2;
typedef          short  sInt2;

typedef unsigned char   uInt1;
typedef          char   sInt1;

typedef char            Character;
typedef char*           String;
typedef const char*     cString;

typedef wchar_t         WideCharacter;

//-----------------------------------------------------------------------------

#endif  //  DEFINITIONS_H
