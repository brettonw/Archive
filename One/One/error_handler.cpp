//------------------------------------------------------------------------------
//	Copyright (C) 1997-2000 Bretton Wade
//	All Rights Reserved
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//	includes
//------------------------------------------------------------------------------
#include	"precompile.h"
#include	"error_handler.h"
#include	"task.h"

//------------------------------------------------------------------------------
//	static members
//------------------------------------------------------------------------------
ErrorHandler	*ErrorHandler::global = 0;

//--------------------------------------------------------------------------
//	tasks
//--------------------------------------------------------------------------
DEFINE_TASK (ErrorHandler)
{
    DO_AFTER (StdErr);
}

//--------------------------------------------------------------------------
ON_STARTUP (ErrorHandler)
{
	ErrorHandler::Startup ();
	RegisterError (E_INVALIDARG);
	RegisterError (E_FAIL);
	RegisterError (E_OUTOFMEMORY);
	RegisterError (E_NOTIMPL);
	RegisterError (CLASS_E_NOAGGREGATION);
}

//--------------------------------------------------------------------------
ON_SHUTDOWN (ErrorHandler)
{
	ErrorHandler::Shutdown ();
}

//------------------------------------------------------------------------------
//	class methods
//------------------------------------------------------------------------------
void
ErrorHandler::Startup
(
	void
)
{
	assert (global == 0);
	global = new ErrorHandler;
}

//------------------------------------------------------------------------------
void
ErrorHandler::Shutdown
(
	void
)
{
	assert (global);
	delete global;
	global = 0;
}

//------------------------------------------------------------------------------
void
ErrorHandler::RegisterResponse
(
	uInt4			error,
	const Text&		name,
	ErrorResponse	response
)
{
	assert (global);
	global->PrivateRegisterResponse (error, name, response);
}

//------------------------------------------------------------------------------
void
ErrorHandler::RespondToError
(
	uInt4			error,
	const Text&		command
)
{
	assert (global);
	global->PrivateRespondToError (error, command);
}

//------------------------------------------------------------------------------
ErrorHandler::ErrorHandler
(
	void
) : myResponse (64), myDefaultResponse (ERROR_ASK, "UNKNOWN_ERROR_NAME")
{
}

//------------------------------------------------------------------------------
void
ErrorHandler::PrivateRegisterResponse
(
	uInt4			error,
	const Text&		name,
	ErrorResponse	response
)
{
	myResponse.PutItem (error, ErrorNode (response, name));
}

//------------------------------------------------------------------------------
void
ErrorHandler::PrivateRespondToError
(
	uInt4			error,
	const Text&		command
)
{
	try
	{
		myResponse.GetItem (error).Activate (error, command);
	}
	catch (KeyNotFound)
	{
		myDefaultResponse.Activate (error, command);
	}
}

//------------------------------------------------------------------------------
