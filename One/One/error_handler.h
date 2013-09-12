//------------------------------------------------------------------------------
//	Copyright (C) 1997-2000 Bretton Wade
//	All Rights Reserved
//------------------------------------------------------------------------------

#ifndef		_BW_ERROR_HANDLER_H_
#define		_BW_ERROR_HANDLER_H_

//------------------------------------------------------------------------------
//	includes
//------------------------------------------------------------------------------
#ifndef		_BW_HASH_TABLE_OF_H_
#include	"hash_table_of.h"
#endif	//	_BW_HASH_TABLE_OF_H_

#ifndef		_BW_ERROR_NODE_H_
#include	"error_node.h"
#endif	//	_BW_ERROR_NODE_H_

//------------------------------------------------------------------------------
//	class definitions
//------------------------------------------------------------------------------
class	ErrorHandler
{
	public:
static	void				            Startup (void);
static	void				            Shutdown (void);
static	void				            RegisterResponse (uInt4 error, const Text& name, ErrorResponse response);
static	void				            RespondToError (uInt4 error, const Text& command);
	private:
		ErrorHandler (void);
		void				            PrivateRegisterResponse (uInt4 error, const Text& name, ErrorResponse response);
		void				            PrivateRespondToError (uInt4 error, const Text& command);
	private:
static	ErrorHandler*		            global;
		HashTableOf<uInt4, ErrorNode>   myResponse;
		ErrorNode			            myDefaultResponse;
};

//------------------------------------------------------------------------------
//	macros
//------------------------------------------------------------------------------
#define	RegisterErrorResponse(err, response)									\
		ErrorHandler::RegisterResponse (uInt4 (err), #err, response)

#define	RegisterError(err)														\
		ErrorHandler::RegisterResponse (uInt4 (err), #err, ERROR_ASK)

#define	TestResult(test)														\
	{																			\
		uInt4	result = uInt4 (test);											\
		if (result != NO_ERROR)													\
			ErrorHandler::RespondToError (result, #test);						\
	}

#define	TestForFail(test, fail)													\
	{																			\
		SetLastError (NO_ERROR);												\
		uInt4	result = uInt4 (test);											\
		if (result == (fail))													\
			if ((result = GetLastError ()) != NO_ERROR)							\
				ErrorHandler::RespondToError (result, #test);					\
	}

#define	TestFailNeg(test)	TestForFail (test, ~0)
#define	TestFailZero(test)	TestForFail (test, 0)

//------------------------------------------------------------------------------

#endif	//	_BW_ERROR_HANDLER_H_
