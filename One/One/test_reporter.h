//------------------------------------------------------------------------------
//	Copyright (C) 1997-2000 Bretton Wade
//	All Rights Reserved
//------------------------------------------------------------------------------

#ifndef		_BW_TEST_REPORTER_H_
#define		_BW_TEST_REPORTER_H_

//--------------------------------------------------------------------------
//	includes
//--------------------------------------------------------------------------
#ifndef		_BW_TEXT_H_
#include	"text.h"
#endif	//	_BW_TEXT_H_

//--------------------------------------------------------------------------
//	class definitions
//--------------------------------------------------------------------------
class	TestReporter
{
	public:
							TestReporter (const Text& name, bool verbose);
							~TestReporter (void);
		void				Fail (const Text& test);
		void				Succeed (const Text& test);
	private:
		bool				myVerbose;
};

//--------------------------------------------------------------------------

#endif	//	_BW_TEST_REPORTER_H_
