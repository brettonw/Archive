//------------------------------------------------------------------------------
//	Copyright (C) 1997-2000 Bretton Wade
//	All Rights Reserved
//------------------------------------------------------------------------------

#ifndef		_BW_ERROR_NODE_H_
#define		_BW_ERROR_NODE_H_

//------------------------------------------------------------------------------
//	includes
//------------------------------------------------------------------------------
#ifndef		_BW_TEXT_H_
#include	"text.h"
#endif	//	_BW_TEXT_H_

//------------------------------------------------------------------------------
//	enumerated values
//------------------------------------------------------------------------------
enum	ErrorResponse
{
	ERROR_ASK = 0,
	ERROR_QUIT = IDABORT,
	ERROR_DEBUG = IDRETRY,
	ERROR_IGNORE = IDIGNORE
};

//------------------------------------------------------------------------------
//	class definitions
//------------------------------------------------------------------------------
class	ErrorNode
{
	public:
		ErrorNode (const ErrorNode& node);
		ErrorNode (ErrorResponse response, const Text& errorName);
		~ErrorNode (void);
		void				Activate (uInt4 error, const Text& command);
	protected:
		ErrorResponse		myResponse;
		Text				myName;
};

//------------------------------------------------------------------------------

#endif	//	_BW_ERROR_NODE_H_
