//------------------------------------------------------------------------------
//	Copyright (C) 1997-2000 Bretton Wade
//	All Rights Reserved
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//	includes
//------------------------------------------------------------------------------
#include	"precompile.h"
#include	"error_node.h"

//------------------------------------------------------------------------------
//	class methods
//------------------------------------------------------------------------------
ErrorNode::ErrorNode
(
	const ErrorNode&	node
)
{
	myResponse = node.myResponse;
	myName = node.myName;
}

//------------------------------------------------------------------------------
ErrorNode::ErrorNode
(
	ErrorResponse	response,
	const Text&		name
)
{
	myResponse = response;
	myName = name;
}

//------------------------------------------------------------------------------
ErrorNode::~ErrorNode
(
	void
)
{
}

//------------------------------------------------------------------------------
void
ErrorNode::Activate
(
	uInt4			error,
	const Text&		command
)
{
	sInt1		errValue[16];
	sprintf (errValue, "0x%08x", error);
	Text		message = Text (command) + "\n" + myName + " (" + errValue + ")";
	void*		description;
	if (FormatMessage (FORMAT_MESSAGE_ALLOCATE_BUFFER bitor FORMAT_MESSAGE_FROM_SYSTEM, 0, error, MAKELANGID (LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR) &description, 0, 0))
	{
		message += Text ("\n") + String (description);
		LocalFree (description);
	}
	// XXX what should this really do?
	//DumpCore (message + "\n");
	//BW_Panic ();
	ErrorResponse	response = myResponse;
	if (response == ERROR_ASK)
		response = ErrorResponse (MessageBox (GetActiveWindow (), message, "Error", MB_ABORTRETRYIGNORE bitor MB_ICONERROR bitor MB_TASKMODAL));
	switch (response)
	{
		case ERROR_QUIT:
			exit (error);
		case ERROR_DEBUG:
			#ifdef      _DEBUG
			_CrtDbgBreak ();
			#endif  //  _DEBUG
			break;
		case ERROR_IGNORE:
			break;
	}
}

//------------------------------------------------------------------------------
