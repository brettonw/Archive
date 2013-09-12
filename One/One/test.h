//------------------------------------------------------------------------------
//	Copyright (C) 1997-2000 Bretton Wade
//	All Rights Reserved
//------------------------------------------------------------------------------

#ifndef		_BW_TEST_H_
#define		_BW_TEST_H_

//--------------------------------------------------------------------------
//	includes
//--------------------------------------------------------------------------
#ifndef		_BW_GRAPH_OF_H_
#include	"graph_of.h"
#endif	//	_BW_GRAPH_OF_H_

#ifndef		_BW_TEST_REPORTER_H_
#include	"test_reporter.h"
#endif	//	_BW_TEST_REPORTER_H_

//--------------------------------------------------------------------------
//	class definitions
//--------------------------------------------------------------------------
class	Test
{
	public:
							Test (const Text& name);
virtual						~Test (void);
virtual	void				Setup (GraphOf<Test*>* graph) const = 0;
virtual	void				DoTest (void) const = 0;
		const Text&			GetName (void) const;
	    void                DoAfter (GraphOf<Test*>* graph, const Text& name) const;
	private:
		Text				myName;
};

//--------------------------------------------------------------------------
//	macros
//--------------------------------------------------------------------------
#define	DEFINE_TEST(name)													    \
	class	Test_ ## name : public Test										    \
	{																		    \
		public:																    \
	virtual					~Test_ ## name (void);							    \
	virtual	void			Setup (GraphOf<Test*>* graph) const;			    \
	virtual	void			DoTest (void) const;							    \
		private:															    \
							Test_ ## name (void);							    \
		private:															    \
	static	Test_ ## name	global;											    \
	};																		    \
	Test_ ## name	Test_ ## name::global;									    \
	Test_ ## name::Test_ ## name (void) : Test (#name) {}					    \
	Test_ ## name::~Test_ ## name (void) {}									    \
	void	Test_ ## name::Setup (GraphOf<Test*>* graph) const

#define	DO_AFTER(name)														    \
	DoAfter (graph, #name)

#define	ON_TEST(name)														    \
	void	Test_ ## name::DoTest (void) const

#define	DO_TEST_REPORT														    \
	TestReporter	__test_reporter (GetName (), _VERBOSE_TESTING);

#define	TEST_CONDITION(cond)												    \
	if (not (cond))															    \
		__test_reporter.Fail (#cond);										    \
	else																	    \
		__test_reporter.Succeed (#cond);									    \

//--------------------------------------------------------------------------

#endif	//	_BW_TEST_H_
