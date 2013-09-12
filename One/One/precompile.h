//------------------------------------------------------------------------------
//	Copyright (C) 1997-2000 Bretton Wade
//	All Rights Reserved
//------------------------------------------------------------------------------

#ifndef		_BW_PRECOMPILE_H_
#define		_BW_PRECOMPILE_H_

//------------------------------------------------------------------------------
//	control macros
//------------------------------------------------------------------------------
#define		WIN32
#define		WIN32_LEAN_AND_MEAN
#define     _CRT_SECURE_NO_DEPRECATE

#define     NOMCX
#define     NOIME


//------------------------------------------------------------------------------
//	warnings control, following mmsystem headers need this
//------------------------------------------------------------------------------
//#pragma		warning (disable: 4514 4201 4100 4710 4711 4244)
#pragma		warning (disable: 4100 4291 4710 4711)

//------------------------------------------------------------------------------
//	windows includes
//------------------------------------------------------------------------------
#include	<windows.h>
#include	<windowsx.h>
#include	<wtypes.h>
#include	<winerror.h>
#include	<commdlg.h>

//------------------------------------------------------------------------------
//	ansi includes
//------------------------------------------------------------------------------
#include	<string.h>
#include	<stdlib.h>
#include	<stdio.h>
#include	<iostream>
#include	<fstream>
#include	<stdarg.h>
#include	<ctype.h>
#include	<limits.h>
#include	<malloc.h>
#include	<process.h>
#include	<new.h>

//------------------------------------------------------------------------------
//	debugging
//------------------------------------------------------------------------------
#ifdef		_DEBUG
#include	<crtdbg.h>
//#define		new		new (_NORMAL_BLOCK, __FILE__, __LINE__)
#endif	//	_DEBUG

//------------------------------------------------------------------------------
//	macros to be c++ compliant
//------------------------------------------------------------------------------
#define	alloca	_alloca
#define	or		||
#define	bitor	|
#define	and		&&
#define	bitand	&
#define	not		!
#define	compl	~
#define	xor		^

//------------------------------------------------------------------------------
//	stringizing macro
//------------------------------------------------------------------------------
#define _MAKEITASTRING(word) #word
#define MAKEITASTRING(word) _MAKEITASTRING(word)

//------------------------------------------------------------------------------
//	macro to force a crash
//------------------------------------------------------------------------------
#define MAKEITCRASH {int* ptr = 0; *ptr = 0;}

//------------------------------------------------------------------------------
//	project includes
//------------------------------------------------------------------------------
#include	"type_defines.h"
#include	"assert.h"

//------------------------------------------------------------------------------
// namespace...
//------------------------------------------------------------------------------
using namespace std;

//------------------------------------------------------------------------------
#endif	//	_BW_PRECOMPILE_H_
