//------------------------------------------------------------------------------
//	Copyright (C) 1997-2000 Bretton Wade
//	All Rights Reserved
//------------------------------------------------------------------------------

#ifndef		_BW_CPP_SCANNER_H_
#define		_BW_CPP_SCANNER_H_

//------------------------------------------------------------------------------
//	includes
//------------------------------------------------------------------------------
#ifndef		_BW_TEXT_SCANNER_H_
#include	"text_scanner.h"
#endif	//	_BW_TEXT_SCANNER_H_

//------------------------------------------------------------------------------
//	class definitions
//------------------------------------------------------------------------------
class	CPPScanner : public TextScanner
{
	public:
		CPPScanner (void);
virtual	~CPPScanner (void);
virtual	void					BuildScanner (void);

		void					InstallPaths (void);

		void					CloseKeywordPaths (cString* pathList, uInt2 startState, uInt2 action, uInt2 blankStart, uInt2 blankStop);
		void					CloseOperatorPaths (cString* pathList, uInt2 startState, uInt2 blankStart, uInt2 blankStop);

		void					InstallPreprocessorPaths (void);

		void					SetupSpaceScanner (uInt2 state, uInt2 action);
		void					SetupNewlineScanner (uInt2 fromState, uInt2 toState, uInt2 action);
		void					SetupEscapeScanner (uInt2 state);
		void					SetupLiteral (uInt2 fromState, uInt2 literalState, cString delimiter, cString validChars, uInt2 action);

		void					InstallPaths (cString* pathList, uInt2 startState, uInt2& blankStart, uInt2& blankStop);
		uInt2					InstallPath (cString string, uInt2 startState);
		uInt2					FindPath (cString string, uInt2 startState);
	public:
		const enum
		{
			ERROR_STATE = 5,
			PP_STATE,
			PP_IDENTIFIER_STATE,
			PP_INCLUDE_READY_STATE,
			PP_I_Q_HEADER_STATE,
			PP_I_H_HEADER_STATE,
			NORMAL_STATE,
			COMMENT_LINE_STATE,
			COMMENT_BLOCK_STATE,
			COMMENT_BLOCK_END_STATE,
			PRE_INTEGER_STATE,
			INTEGER_STATE,
			OCTAL_STATE,
			PRE_HEXADECIMAL_STATE,
			HEXADECIMAL_STATE,
			INTEGER_U_SUFFIX_STATE,
			INTEGER_L_SUFFIX_STATE,
			FLOAT_DECIMAL_STATE,
			FLOAT_STATE,
			FLOAT_EXP_STATE,
			FLOAT_EXP_SIGN_STATE,
			FLOAT_EXP_FULL_STATE,
			L_STATE,
			STRING_STATE,
			CHARACTER_STATE,
			BLANK_STATE
		};
	protected:
			uInt2				myBlankState;
static		cString				nondigit;
static		cString				sign;
static		cString				digit;
static		cString				nonzero_digit;
static		cString				octal_digit;
static		cString				hexadecimal_start;
static		cString				hexadecimal_digit;
static		cString				unsigned_suffix;
static		cString				long_suffix;
static		cString				floating_suffix;
static		cString				floating_exponent;
static		cString				c_chars;
static		cString				s_chars;
static		cString				escape_chars;
static		cString				q_chars;
static		cString				h_chars;
static		cString				pp_chars;
static		cString				spaces;
static		cString				terminator;

static		cString				keywords[];
static		cString				booleans[];
static		cString				alternates[];
static		cString				operators[];
static		cString				preprocessor[];
};

//------------------------------------------------------------------------------

#endif	//	_BW_CPP_SCANNER_H_
