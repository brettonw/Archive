//------------------------------------------------------------------------------
//	Copyright (C) 1997-2000 Bretton Wade
//	All Rights Reserved
//------------------------------------------------------------------------------

#ifndef		_BW_TEST_REGISTRAR_H_
#define		_BW_TEST_REGISTRAR_H_

//--------------------------------------------------------------------------
//	includes
//--------------------------------------------------------------------------
#ifndef		_BW_TEST_H_
#include	"test.h"
#endif	//	_BW_TEST_H_

#ifndef		_BW_EXCEPTION_H_
#include	"exception.h"
#endif	//	_BW_EXCEPTION_H_

//--------------------------------------------------------------------------
//	exception definitions
//--------------------------------------------------------------------------
DEFINE_EXCEPTION (InvalidTestDependency);

//--------------------------------------------------------------------------
//	class definitions
//--------------------------------------------------------------------------
class	TestRegistrar
{
	public:
static	void			RegisterTest (Test* test);
static	void			RunTests (void);
	protected:
static	GraphOf<Test*>*	myTests;
};

//--------------------------------------------------------------------------

#endif	//	_BW_TEST_REGISTRAR_H_
