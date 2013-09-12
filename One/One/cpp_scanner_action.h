//------------------------------------------------------------------------------
//	Copyright (C) 1997-2000 Bretton Wade
//	All Rights Reserved
//------------------------------------------------------------------------------

#ifndef		_BW_CPP_SCANNER_ACTION_H_
#define		_BW_CPP_SCANNER_ACTION_H_

//------------------------------------------------------------------------------
//	includes
//------------------------------------------------------------------------------
#ifndef		_BW_TEXT_SCANNER_ACTION_H_
#include	"text_scanner_action.h"
#endif	//	_BW_TEXT_SCANNER_ACTION_H_

//------------------------------------------------------------------------------
//	class definitions
//------------------------------------------------------------------------------
class	CPPScannerAction : public TextScannerAction
{
	public:
virtual	~CPPScannerAction (void);
virtual	void					Action (cString buffer, uInt length, uInt2 action, cString nextBuffer, uInt2 nextState);
	public:
		const enum
		{
			//	non-error actions
			OUTPUT_PREPROCESSOR = LAST_TEXT_SCANNER_ACTION, //	output a preprocessor token
			OUTPUT_HEADER_NAME,						        //	output a header name
			OUTPUT_KEYWORD,							        //	output a keyword
			OUTPUT_INTEGER_LITERAL,					        //	output an integer literal
			OUTPUT_CHARACTER_LITERAL,				        //	output a character literal
			OUTPUT_FLOATING_LITERAL,				        //	output a floating literal
			OUTPUT_STRING_LITERAL,					        //	output a string literal
			OUTPUT_BOOLEAN_LITERAL,					        //	output a boolean literal
			OUTPUT_OPERATOR,						        //	output an operator or punctuator
			OUTPUT_ALTERNATE,						        //	output a string for operator (new, delete, etc.)
			OUTPUT_COMMENT,							        //	output a comment string
			//	error actions
			OUTPUT_INVALID_PREPROCESSOR,			        //	output an error because an illegal preprocessor command was found
			OUTPUT_BAD_INCLUDE,						        //	output an error because the first character after an include directive was not a < or "
			OUTPUT_BAD_HEADER_NAME,					        //	output an error because an illegal character was encountered in a header name
			OUTPUT_BAD_PREPROCESSOR,				        //	output an error because a preprocessor token was encountered in normal mode	
			OUTPUT_INVALID_CHAR,					        //	output an error because an invalid character was encountered
			OUTPUT_BAD_INTEGER_LITERAL,				        //	output an error because a malformed integer literal was encountered
			OUTPUT_BAD_EXP_ERROR,					        //	output an error because the exponent on a floating point literal is malformed
			OUTPUT_BAD_EXP_SIGN_ERROR,				        //	output an error because a digit must follow the sign of the floating point exponent
			OUTPUT_UNTERMINATED_ERROR,				        //	output an error because an unterminated string or character literal was encountered
			OUTPUT_BAD_ESCAPE,						        //	output an error because an unknown escape sequence was encountered
			OUTPUT_BAD_OPERATOR						        //	output an error because an invalid operator or punctuator was encountered
		};
};

//------------------------------------------------------------------------------

#endif	//	_BW_CPP_SCANNER_ACTION_H_
