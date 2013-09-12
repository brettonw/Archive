//------------------------------------------------------------------------------
//	Copyright (C) 1997-2000 Bretton Wade
//	All Rights Reserved
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//	includes
//------------------------------------------------------------------------------
#include	"precompile.h"
#include	"cpp_scanner.h"
#include	"cpp_scanner_action.h"
#include	"directory.h"

//------------------------------------------------------------------------------
//	static members
//------------------------------------------------------------------------------
cString		CPPScanner::nondigit =			"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_";
cString		CPPScanner::sign =				"+-";
cString		CPPScanner::digit =				"0123456789";
cString		CPPScanner::octal_digit =		"01234567";
cString		CPPScanner::hexadecimal_start =	"xX";
cString		CPPScanner::hexadecimal_digit =	"0123456789abcdefABCDEF";
cString		CPPScanner::unsigned_suffix =	"uU";
cString		CPPScanner::long_suffix =		"lL";
cString		CPPScanner::floating_exponent =	"eE";
cString		CPPScanner::floating_suffix =	"flFL";
cString		CPPScanner::c_chars =			"\t\v abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_{}[]#()<>%:;.?*+-/^&|~!=,\"";
cString		CPPScanner::s_chars =			"\t\v abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_{}[]#()<>%:;.?*+-/^&|~!=,'";
cString		CPPScanner::escape_chars =		"\'\"\?\\abfnrtv";
cString		CPPScanner::q_chars =			"\t\v abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_{}[]#()<>%:;.?*+-/^&|~!=,\\'";
cString		CPPScanner::h_chars =			"\t\v abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_{}[]#()<%:;.?*+-/^&|~!=,\\\"'";
cString		CPPScanner::pp_chars =			"abcdefghijklmnopqrstuvwxyz";
cString		CPPScanner::spaces =			"\t\v ";
cString		CPPScanner::terminator =		"\n\r";

cString		CPPScanner::keywords[] =
{
	"asm", "do", "inline", "short", "typeid", "auto", "double", "int", 
	"signed", "typename", "bool", "dynamic_cast", "long", "sizeof", "union", 
	"break", "else", "mutable", "static", "unsigned", "case", "enum", 
	"namespace", "static_cast", "using", "catch", "explicit", "struct", 
	"virtual", "char", "extern", "operator", "switch", "void", "class", 
	"false", "private", "template", "volatile", "const", "float", "protected", 
	"this", "wchar_t", "const_cast", "for", "public", "throw", "while", 
	"continue", "friend", "register", "true", "default", "goto", 
	"reinterpret_cast", "try", "if", "return", "typedef", 0
};
cString		CPPScanner::booleans[] =
{
	"false", "true", 0
};
cString		CPPScanner::alternates[] =	//	these are text operators
{
	"and", "and_eq", "bitand", "bitor", "compl", "not", "not_eq", "or", 
	"or_eq", "xor", "xor_eq", "new", "delete", 0
};
cString		CPPScanner::operators[] =
{
	"{", "}", "[", "]", "#", "##", "(", ")", "<:", ":>", "<%", "%>", "%:", 
	"%:%:", ";", ":", "...", "?", "::", ".", ".*",	"+", "-", "*", "/", "%", 
	"^", "&", "|", "~", "!", "=", "<", ">", "+=", "-=", "*=", "/=", "%=","^=", 
	"&=", "|=", "<<", ">>", ">>=", "<<=", "==", "!=",	"<=", ">=", "&&", 
	"||", "++", "--", ",", "->*", "->",	0
};
cString		CPPScanner::preprocessor[] =
{
	"#ifndef", "#if", "#ifdef", "#endif", "#else", "#elif", "#undef", "#line", 
	"#error", "#define", "#pragma", 0
};

//------------------------------------------------------------------------------
//	class methods
//------------------------------------------------------------------------------
CPPScanner::CPPScanner
(
	void
) : TextScanner (GetDataFileName ("Scanners", "CPPScanner.stb"))
{
	myBlankState = BLANK_STATE;
	if (not myTableWasLoaded)
		BuildScanner ();
}

//------------------------------------------------------------------------------
CPPScanner::~CPPScanner
(
	void
)
{
}

//------------------------------------------------------------------------------
void
CPPScanner::BuildScanner
(
	void
)
{
	//	XXX	still to do:
	//		better preprocessor state handling - more in line with the standard
	//		recognize universal character names in escape sequences
	//		test
	//	XXX	things I won't do:
	//		trigraph recognition (?) (possibly as keywords...) (does trigraph replacement occur even in strings? yes, it does in VC++ anyway, and ??? is not recognized)
	ResetScanner ();
	InstallPreprocessorPaths ();
	InstallPaths ();
	SaveStateTable (GetDataFileName ("Scanners", "CPPScanner.stb"));
}

//------------------------------------------------------------------------------
void
CPPScanner::InstallPaths
(
	void
)
{
	uInt2	keywordStart, keywordStop,
			booleanStart, booleanStop,
			operatorStart, operatorStop,
			alternateStart, alternateStop;
	InstallPaths (keywords, NORMAL_STATE, keywordStart, keywordStop);
	InstallPaths (booleans, NORMAL_STATE, booleanStart, booleanStop);
	InstallPaths (alternates, NORMAL_STATE, alternateStart, alternateStop);
	InstallPaths (operators, NORMAL_STATE, operatorStart, operatorStop);

	SetupNewlineScanner		(NORMAL_STATE, DEFAULT_STATE, CPPScannerAction::NO_ACTION																							);
	SetupSpaceScanner		(NORMAL_STATE, CPPScannerAction::NO_ACTION																											);
	OnInput					(NORMAL_STATE,				nondigit,			IDENTIFIER_STATE,			STORE_INPUT_BEFORE,		CPPScannerAction::NO_ACTION						);
	OnInput					(NORMAL_STATE,				"L",				L_STATE,					STORE_INPUT_BEFORE,		CPPScannerAction::NO_ACTION						);
	OnInput					(NORMAL_STATE,				"0",				PRE_INTEGER_STATE,			STORE_INPUT_BEFORE,		CPPScannerAction::NO_ACTION						);
	OnInput					(NORMAL_STATE,				digit,				INTEGER_STATE,				STORE_INPUT_BEFORE,		CPPScannerAction::NO_ACTION						);
	SetupLiteral			(NORMAL_STATE, STRING_STATE,    "\"", s_chars, CPPScannerAction::OUTPUT_STRING_LITERAL																);
	SetupLiteral			(NORMAL_STATE, CHARACTER_STATE, "\'", c_chars, CPPScannerAction::OUTPUT_CHARACTER_LITERAL															);

	OnInput					(IDENTIFIER_STATE,			nondigit,			IDENTIFIER_STATE,			STORE_INPUT_BEFORE,		CPPScannerAction::NO_ACTION						);
	OnInput					(IDENTIFIER_STATE,			digit,				IDENTIFIER_STATE,			STORE_INPUT_BEFORE,		CPPScannerAction::NO_ACTION						);

	SetupLiteral			(L_STATE, STRING_STATE,    "\"", s_chars, CPPScannerAction::OUTPUT_STRING_LITERAL																	);
	SetupLiteral			(L_STATE, CHARACTER_STATE, "\'", c_chars, CPPScannerAction::OUTPUT_CHARACTER_LITERAL																);

	uInt2		commentStartState = FindPath ("/", NORMAL_STATE);
	OnInput					(commentStartState,			"/",				COMMENT_LINE_STATE,			STORE_INPUT_BEFORE,		CPPScannerAction::NO_ACTION						);
	OnInput					(commentStartState,			"*",				COMMENT_BLOCK_STATE,		STORE_INPUT_BEFORE,		CPPScannerAction::NO_ACTION						);

	OnInput					(COMMENT_LINE_STATE,		terminator,			NORMAL_STATE,				STORE_INPUT_AFTER,		CPPScannerAction::OUTPUT_COMMENT				);

	OnInput					(COMMENT_BLOCK_STATE,		"*",				COMMENT_BLOCK_END_STATE,	STORE_INPUT_BEFORE,		CPPScannerAction::NO_ACTION						);
	SetupNewlineScanner		(COMMENT_BLOCK_STATE, COMMENT_BLOCK_STATE, CPPScannerAction::OUTPUT_COMMENT																			);

	OnInput					(COMMENT_BLOCK_END_STATE,	"/",				NORMAL_STATE,				STORE_INPUT_BEFORE,		CPPScannerAction::OUTPUT_COMMENT				);

	OnInput					(INTEGER_STATE,				digit,				INTEGER_STATE,				STORE_INPUT_BEFORE,		CPPScannerAction::NO_ACTION						);
	OnInput					(INTEGER_STATE,				unsigned_suffix,	INTEGER_U_SUFFIX_STATE,		STORE_INPUT_BEFORE,		CPPScannerAction::NO_ACTION						);
	OnInput					(INTEGER_STATE,				long_suffix,		INTEGER_L_SUFFIX_STATE,		STORE_INPUT_BEFORE,		CPPScannerAction::NO_ACTION						);
	OnInput					(INTEGER_STATE,				".",				FLOAT_STATE,				STORE_INPUT_BEFORE,		CPPScannerAction::NO_ACTION						);
	OnInput					(INTEGER_STATE,				floating_exponent,	FLOAT_EXP_STATE,			STORE_INPUT_BEFORE,		CPPScannerAction::NO_ACTION						);

	OnInput					(PRE_INTEGER_STATE,			octal_digit,		OCTAL_STATE,				STORE_INPUT_BEFORE,		CPPScannerAction::NO_ACTION						);
	OnInput					(PRE_INTEGER_STATE,			unsigned_suffix,	INTEGER_U_SUFFIX_STATE,		STORE_INPUT_BEFORE,		CPPScannerAction::NO_ACTION						);
	OnInput					(PRE_INTEGER_STATE,			long_suffix,		INTEGER_L_SUFFIX_STATE,		STORE_INPUT_BEFORE,		CPPScannerAction::NO_ACTION						);
	OnInput					(PRE_INTEGER_STATE,			hexadecimal_start,	PRE_HEXADECIMAL_STATE,		STORE_INPUT_BEFORE,		CPPScannerAction::NO_ACTION						);
	OnInput					(PRE_INTEGER_STATE,			".",				FLOAT_STATE,				STORE_INPUT_BEFORE,		CPPScannerAction::NO_ACTION						);

	OnInput					(OCTAL_STATE,				octal_digit,		OCTAL_STATE,				STORE_INPUT_BEFORE,		CPPScannerAction::NO_ACTION						);
	OnInput					(OCTAL_STATE,				unsigned_suffix,	INTEGER_U_SUFFIX_STATE,		STORE_INPUT_BEFORE,		CPPScannerAction::NO_ACTION						);
	OnInput					(OCTAL_STATE,				long_suffix,		INTEGER_L_SUFFIX_STATE,		STORE_INPUT_BEFORE,		CPPScannerAction::NO_ACTION						);

	OnInput					(PRE_HEXADECIMAL_STATE,		hexadecimal_digit,	HEXADECIMAL_STATE,			STORE_INPUT_BEFORE,		CPPScannerAction::NO_ACTION						);

	OnInput					(HEXADECIMAL_STATE,			hexadecimal_digit,	HEXADECIMAL_STATE,			STORE_INPUT_BEFORE,		CPPScannerAction::NO_ACTION						);
	OnInput					(HEXADECIMAL_STATE,			unsigned_suffix,	INTEGER_U_SUFFIX_STATE,		STORE_INPUT_BEFORE,		CPPScannerAction::NO_ACTION						);
	OnInput					(HEXADECIMAL_STATE,			long_suffix,		INTEGER_L_SUFFIX_STATE,		STORE_INPUT_BEFORE,		CPPScannerAction::NO_ACTION						);

	OnInput					(INTEGER_U_SUFFIX_STATE,	long_suffix,		NORMAL_STATE,				STORE_INPUT_BEFORE,		CPPScannerAction::OUTPUT_INTEGER_LITERAL		);
	OnInput					(INTEGER_L_SUFFIX_STATE,	unsigned_suffix,	NORMAL_STATE,				STORE_INPUT_BEFORE,		CPPScannerAction::OUTPUT_INTEGER_LITERAL		);

	uInt2		floatDecimalState = FindPath (".", NORMAL_STATE);
	OnInput					(floatDecimalState,			digit,				FLOAT_STATE,				STORE_INPUT_BEFORE,		CPPScannerAction::NO_ACTION						);

	OnInput					(FLOAT_STATE,				digit,				FLOAT_STATE,				STORE_INPUT_BEFORE,		CPPScannerAction::NO_ACTION						);
	OnInput					(FLOAT_STATE,				floating_exponent,	FLOAT_EXP_STATE,			STORE_INPUT_BEFORE,		CPPScannerAction::NO_ACTION						);
	OnInput					(FLOAT_STATE,				floating_suffix,	NORMAL_STATE,				STORE_INPUT_BEFORE,		CPPScannerAction::OUTPUT_FLOATING_LITERAL		);

	OnInput					(FLOAT_EXP_STATE,			digit,				FLOAT_EXP_FULL_STATE,		STORE_INPUT_BEFORE,		CPPScannerAction::NO_ACTION						);
	OnInput					(FLOAT_EXP_STATE,			sign,				FLOAT_EXP_SIGN_STATE,		STORE_INPUT_BEFORE,		CPPScannerAction::NO_ACTION						);

	OnInput					(FLOAT_EXP_SIGN_STATE,		digit,				FLOAT_EXP_FULL_STATE,		STORE_INPUT_BEFORE,		CPPScannerAction::NO_ACTION						);

	OnInput					(FLOAT_EXP_FULL_STATE,		digit,				FLOAT_EXP_FULL_STATE,		STORE_INPUT_BEFORE,		CPPScannerAction::NO_ACTION						);
	OnInput					(FLOAT_EXP_FULL_STATE,		floating_suffix,	NORMAL_STATE,				STORE_INPUT_BEFORE,		CPPScannerAction::OUTPUT_FLOATING_LITERAL		);

	CloseKeywordPaths (keywords, NORMAL_STATE, CPPScannerAction::OUTPUT_KEYWORD, keywordStart, keywordStop);
	CloseKeywordPaths (booleans, NORMAL_STATE, CPPScannerAction::OUTPUT_BOOLEAN_LITERAL, booleanStart, booleanStop);
	CloseKeywordPaths (alternates, NORMAL_STATE, CPPScannerAction::OUTPUT_ALTERNATE, alternateStart, alternateStop);
	CloseOperatorPaths (operators, NORMAL_STATE, operatorStart, operatorStop);

	OnAnyInput				(DEFAULT_STATE,									NORMAL_STATE,				STORE_INPUT_AFTER,		CPPScannerAction::NO_ACTION						);

	OnInput					(NORMAL_STATE,				"\0",				ERROR_STATE,				DONT_STORE_INPUT,		CPPScannerAction::OUTPUT_INVALID_CHAR			);
	OnAnyInput				(NORMAL_STATE,									ERROR_STATE,				STORE_INPUT_BEFORE,		CPPScannerAction::OUTPUT_INVALID_CHAR			);

	OnInput					(IDENTIFIER_STATE,			"\0",				NORMAL_STATE,				DONT_STORE_INPUT,		CPPScannerAction::OUTPUT_IDENTIFIER						);
	OnAnyInput				(IDENTIFIER_STATE,								NORMAL_STATE,				STORE_INPUT_AFTER,		CPPScannerAction::OUTPUT_IDENTIFIER				);

	OnInput					(L_STATE,					"\0",				IDENTIFIER_STATE,			DONT_STORE_INPUT,		CPPScannerAction::NO_ACTION						);
	OnAnyInput				(L_STATE,										IDENTIFIER_STATE,			STORE_INPUT_AFTER,		CPPScannerAction::NO_ACTION						);

	OnInput					(commentStartState,			"\0",				NORMAL_STATE,				DONT_STORE_INPUT,		CPPScannerAction::OUTPUT_OPERATOR				);
	OnAnyInput				(commentStartState,								NORMAL_STATE,				STORE_INPUT_AFTER,		CPPScannerAction::OUTPUT_OPERATOR				);

	OnInput					(COMMENT_LINE_STATE,		"\0",				COMMENT_LINE_STATE,			DONT_STORE_INPUT,		CPPScannerAction::OUTPUT_COMMENT						);
	OnAnyInput				(COMMENT_LINE_STATE,							COMMENT_LINE_STATE,			STORE_INPUT_BEFORE,		CPPScannerAction::NO_ACTION						);

	OnInput					(COMMENT_BLOCK_STATE,		"\0",				ERROR_STATE,				DONT_STORE_INPUT,		CPPScannerAction::NO_ACTION						);
	OnAnyInput				(COMMENT_BLOCK_STATE,							COMMENT_BLOCK_STATE,		STORE_INPUT_BEFORE,		CPPScannerAction::NO_ACTION						);

	OnInput					(COMMENT_BLOCK_STATE,		"\0",				ERROR_STATE,				DONT_STORE_INPUT,		CPPScannerAction::NO_ACTION						);
	OnAnyInput				(COMMENT_BLOCK_END_STATE,						COMMENT_BLOCK_STATE,		STORE_INPUT_AFTER,		CPPScannerAction::NO_ACTION						);

	OnAnyInput				(INTEGER_STATE,									NORMAL_STATE,				STORE_INPUT_AFTER,		CPPScannerAction::OUTPUT_INTEGER_LITERAL		);
	OnAnyInput				(PRE_INTEGER_STATE,								NORMAL_STATE,				STORE_INPUT_AFTER,		CPPScannerAction::OUTPUT_INTEGER_LITERAL		);
	OnAnyInput				(OCTAL_STATE,									NORMAL_STATE,				STORE_INPUT_AFTER,		CPPScannerAction::OUTPUT_INTEGER_LITERAL		);
	OnAnyInput				(PRE_HEXADECIMAL_STATE,							ERROR_STATE,				STORE_INPUT_AFTER,		CPPScannerAction::OUTPUT_BAD_INTEGER_LITERAL	);
	OnAnyInput				(HEXADECIMAL_STATE,								NORMAL_STATE,				STORE_INPUT_AFTER,		CPPScannerAction::OUTPUT_INTEGER_LITERAL		);
	OnAnyInput				(INTEGER_U_SUFFIX_STATE,						NORMAL_STATE,				STORE_INPUT_AFTER,		CPPScannerAction::OUTPUT_INTEGER_LITERAL		);
	OnAnyInput				(INTEGER_L_SUFFIX_STATE,						NORMAL_STATE,				STORE_INPUT_AFTER,		CPPScannerAction::OUTPUT_INTEGER_LITERAL		);
	OnAnyInput				(FLOAT_DECIMAL_STATE,							NORMAL_STATE,				STORE_INPUT_AFTER,		CPPScannerAction::OUTPUT_OPERATOR				);
	OnAnyInput				(FLOAT_STATE,									NORMAL_STATE,				STORE_INPUT_AFTER,		CPPScannerAction::OUTPUT_FLOATING_LITERAL		);
	OnAnyInput				(FLOAT_EXP_STATE,								ERROR_STATE,				STORE_INPUT_AFTER,		CPPScannerAction::OUTPUT_BAD_EXP_ERROR			);
	OnAnyInput				(FLOAT_EXP_SIGN_STATE,							ERROR_STATE,				STORE_INPUT_AFTER,		CPPScannerAction::OUTPUT_BAD_EXP_SIGN_ERROR		);
	OnAnyInput				(FLOAT_EXP_FULL_STATE,							NORMAL_STATE,				STORE_INPUT_AFTER,		CPPScannerAction::OUTPUT_FLOATING_LITERAL		);

	OnAnyInput				(ERROR_STATE,									NORMAL_STATE,				STORE_INPUT_AFTER,		CPPScannerAction::NO_ACTION						);
}

//------------------------------------------------------------------------------
void
CPPScanner::CloseKeywordPaths
(
	cString*			pathList,
	uInt2				startState,
	uInt2				action,
	uInt2				blankStart,
	uInt2				blankStop
)
{
	for (uInt2 i = 0; pathList[i]; i++)
	{
		uInt2	keywordState = FindPath (pathList[i], startState);
		OnInput			(keywordState,	nondigit,	IDENTIFIER_STATE,	STORE_INPUT_BEFORE,		CPPScannerAction::NO_ACTION			);
		OnInput			(keywordState,	digit,		IDENTIFIER_STATE,	STORE_INPUT_BEFORE,		CPPScannerAction::NO_ACTION			);
		OnAnyInput		(keywordState,				NORMAL_STATE,		STORE_INPUT_AFTER,		action								);
	}
	for (uInt2 i = blankStart; i < blankStop; i++)
	{
		OnInput			(i,				"\0",		IDENTIFIER_STATE,	DONT_STORE_INPUT,		CPPScannerAction::NO_ACTION			);
		OnAnyInput		(i,							IDENTIFIER_STATE,	STORE_INPUT_AFTER,		CPPScannerAction::NO_ACTION			);
	}
}

//------------------------------------------------------------------------------
void
CPPScanner::CloseOperatorPaths
(
	cString*			pathList,
	uInt2				startState,
	uInt2				blankStart,
	uInt2				blankStop
)
{
	for (uInt2 i = 0; pathList[i]; i++)
		OnAnyInput		(FindPath (pathList[i], startState), NORMAL_STATE, STORE_INPUT_AFTER, CPPScannerAction::OUTPUT_OPERATOR);
	for (uInt2 i = blankStart; i < blankStop; i++)
	{
		OnInput			(i,				"\0",		ERROR_STATE,	DONT_STORE_INPUT,		CPPScannerAction::OUTPUT_BAD_OPERATOR	);
		OnAnyInput		(i,							ERROR_STATE,	STORE_INPUT_BEFORE,		CPPScannerAction::OUTPUT_BAD_OPERATOR	);
	}
}

//------------------------------------------------------------------------------
void
CPPScanner::InstallPreprocessorPaths
(
	void
)
{
	uInt2	blankState = myBlankState;
	for (uInt2 i = 0; preprocessor[i]; i++)
		InstallPath (preprocessor[i], DEFAULT_STATE);
	for (uInt2 i = 0; preprocessor[i]; i++)
	{
		uInt2	ppState = FindPath (preprocessor[i], DEFAULT_STATE);
		OnAnyInput (ppState, NORMAL_STATE, STORE_INPUT_AFTER, CPPScannerAction::OUTPUT_PREPROCESSOR);
	}
	uInt2	includeState = InstallPath ("#include", DEFAULT_STATE);
	SetupSpaceScanner		(includeState, CPPScannerAction::OUTPUT_PREPROCESSOR																								);
	OnInput					(includeState,				"\"",				PP_I_Q_HEADER_STATE,		STORE_INPUT_BEFORE,		CPPScannerAction::NO_ACTION						);
	OnInput					(includeState,				"<",				PP_I_H_HEADER_STATE,		STORE_INPUT_BEFORE,		CPPScannerAction::NO_ACTION						);
	//	XXX	need some better distinction between bad #include directive (#included) and bad char (#include &yuv)
	OnAnyInput (includeState, ERROR_STATE, STORE_INPUT_AFTER, CPPScannerAction::OUTPUT_BAD_INCLUDE);

	OnInput					(PP_I_Q_HEADER_STATE,		q_chars,			PP_I_Q_HEADER_STATE,		STORE_INPUT_BEFORE,		CPPScannerAction::NO_ACTION						);
	OnInput					(PP_I_Q_HEADER_STATE,		"\"",				NORMAL_STATE,				STORE_INPUT_BEFORE,		CPPScannerAction::OUTPUT_HEADER_NAME			);
	OnAnyInput				(PP_I_Q_HEADER_STATE,							ERROR_STATE,				STORE_INPUT_AFTER,		CPPScannerAction::OUTPUT_BAD_HEADER_NAME		);

	OnInput					(PP_I_H_HEADER_STATE,		h_chars,			PP_I_H_HEADER_STATE,		STORE_INPUT_BEFORE,		CPPScannerAction::NO_ACTION						);
	OnInput					(PP_I_H_HEADER_STATE,		">",				NORMAL_STATE,				STORE_INPUT_BEFORE,		CPPScannerAction::OUTPUT_HEADER_NAME			);
	OnAnyInput				(PP_I_H_HEADER_STATE,							ERROR_STATE,				STORE_INPUT_AFTER,		CPPScannerAction::OUTPUT_BAD_HEADER_NAME		);

	for (uInt2 i = blankState; i < myBlankState; i++)
		OnAnyInput (i, ERROR_STATE, STORE_INPUT_AFTER, CPPScannerAction::OUTPUT_INVALID_PREPROCESSOR);
}

//------------------------------------------------------------------------------
void
CPPScanner::SetupSpaceScanner
(
	uInt2				state,
	uInt2				action
)
{
	uInt2	whitespaceState = myBlankState++;
	OnInput				(state,				spaces,				whitespaceState,	STORE_INPUT_BEFORE,		action										);
	OnInput				(whitespaceState,	spaces,				whitespaceState,	STORE_INPUT_BEFORE,		CPPScannerAction::NO_ACTION					);
	OnAnyInput			(whitespaceState,						state,				STORE_INPUT_AFTER,		CPPScannerAction::OUTPUT_WHITESPACE			);
}

//------------------------------------------------------------------------------
void
CPPScanner::SetupNewlineScanner
(
	uInt2				fromState,
	uInt2				toState,
	uInt2				action
)
{
	uInt2	newlineState1 = myBlankState++,
			newlineState2 = myBlankState++;

	OnInput				(fromState,			"\n",				newlineState1,		DONT_STORE_INPUT,		action										);
	OnInput				(fromState,			"\r",				newlineState2,		DONT_STORE_INPUT,		action										);
	OnInput				(newlineState1,		"\r",				toState,			DONT_STORE_INPUT,		CPPScannerAction::OUTPUT_NEWLINE			);
	OnAnyInput			(newlineState1,							toState,			STORE_INPUT_AFTER,		CPPScannerAction::OUTPUT_NEWLINE			);
	OnInput				(newlineState2,		"\n",				toState,			DONT_STORE_INPUT,		CPPScannerAction::OUTPUT_NEWLINE			);
	OnAnyInput			(newlineState2,							toState,			STORE_INPUT_AFTER,		CPPScannerAction::OUTPUT_NEWLINE			);
}

//------------------------------------------------------------------------------
void
CPPScanner::SetupEscapeScanner
(
	uInt2				state
)
{
	uInt2	escapeState = myBlankState++,
			octalState1 = myBlankState++,
			octalState2 = myBlankState++;

	OnInput				(state,				"\\",				escapeState,		STORE_INPUT_BEFORE,		CPPScannerAction::NO_ACTION					);
	OnInput				(escapeState,		escape_chars,		state,				STORE_INPUT_BEFORE,		CPPScannerAction::NO_ACTION					);
	OnInput				(escapeState,		octal_digit,		octalState1,		STORE_INPUT_BEFORE,		CPPScannerAction::NO_ACTION					);
	OnAnyInput			(escapeState,							ERROR_STATE,		STORE_INPUT_AFTER,		CPPScannerAction::OUTPUT_BAD_ESCAPE			);
	OnInput				(octalState1,		octal_digit,		octalState2,		STORE_INPUT_BEFORE,		CPPScannerAction::NO_ACTION					);
	OnAnyInput			(octalState1,							state,				STORE_INPUT_AFTER,		CPPScannerAction::NO_ACTION					);
	OnInput				(octalState2,		octal_digit,		state,				STORE_INPUT_BEFORE,		CPPScannerAction::NO_ACTION					);
	OnAnyInput			(octalState2,							state,				STORE_INPUT_AFTER,		CPPScannerAction::NO_ACTION					);
	//	escape sequence \uXXXXXX to recognize universal character names
}

//------------------------------------------------------------------------------
void
CPPScanner::SetupLiteral
(
	uInt2				fromState,
	uInt2				literalState,
	cString				delimiter,
	cString				validChars,
	uInt2				action
)
{
	OnInput				(fromState,			delimiter,			literalState,		STORE_INPUT_BEFORE,		CPPScannerAction::NO_ACTION					);
	OnInput				(literalState,		validChars,			literalState,		STORE_INPUT_BEFORE,		CPPScannerAction::NO_ACTION					);
	OnInput				(literalState,		delimiter,			fromState,			STORE_INPUT_BEFORE,		action										);
	SetupEscapeScanner	(literalState);
	OnAnyInput			(literalState,							ERROR_STATE,		STORE_INPUT_AFTER,		CPPScannerAction::OUTPUT_UNTERMINATED_ERROR	);
}

//------------------------------------------------------------------------------
void
CPPScanner::InstallPaths
(
	cString*				pathList,
	uInt2					startState,
	uInt2&					blankStart,
	uInt2&					blankStop
)
{
	blankStart = myBlankState;
	for (uInt2 i = 0; pathList[i]; i++)
		InstallPath (pathList[i], startState);
	blankStop = myBlankState;
}

//------------------------------------------------------------------------------
uInt2
CPPScanner::InstallPath
(
	cString					string,
	uInt2					startState
)
{
	assert (string);
	const	ScannerStateInfo*	info = &(myStateTable[startState][*string]);
	while ((*string) and (info->myNextState != ScannerState::INVALID_STATE))
	{
		startState = info->myNextState;
		string++;
		info = &(myStateTable[startState][*string]);
	}
	while (*string)
	{
		uInt2	nextState = myBlankState++;
		myStateTable[startState].Set (*string++, nextState, STORE_INPUT_BEFORE, CPPScannerAction::NO_ACTION);
		startState = nextState;
	}
	return startState;
}

//------------------------------------------------------------------------------
uInt2
CPPScanner::FindPath
(
	cString					string,
	uInt2					startState
)
{
	assert (string);
	const	ScannerStateInfo*	info = &(myStateTable[startState][*string]);
	while (*string)
	{
		startState = info->myNextState;
		string++;
		info = &(myStateTable[startState][*string]);
	}
	return startState;
}

//------------------------------------------------------------------------------
