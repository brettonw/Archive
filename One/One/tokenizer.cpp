//------------------------------------------------------------------------------
//	Copyright (C) 1997-2000 Bretton Wade
//	All Rights Reserved
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//	includes
//------------------------------------------------------------------------------
#include	"precompile.h"
#include	"tokenizer.h"

//------------------------------------------------------------------------------
//	class methods
//------------------------------------------------------------------------------
/* void */
Tokenizer::Tokenizer
(
	const HandleTo<Scanner>& scanner
) : myScanner (scanner)
{
}

//------------------------------------------------------------------------------
/* void */
Tokenizer::~Tokenizer
(
	void
)
{
}

//------------------------------------------------------------------------------
void
Tokenizer::Action
(
	cString                     buffer,
	uInt                        length,
	uInt2                       action,
	cString                     nextBuffer,
	uInt2                       nextState
)
{
    // add the token to the token list
    AddTokenToList (buffer, length, action);
}

//------------------------------------------------------------------------------
HandleTo<ListOf<PtrTo<Token> > >
Tokenizer::Tokenize
(
    const Text&                 text,
    uInt2                       startState
)
{
    myListOfTokens = new ListOf<PtrTo<Token> >;
    myScanner->ScanText (text, this, startState);
    return myListOfTokens;
}

//------------------------------------------------------------------------------
void
Tokenizer::AddTokenToList
(
    cString                     buffer,
    uInt                        length,
    uInt2                       action
)
{
    // build the text for the token
    String  tokenBuffer = reinterpret_cast<String> (alloca (length + 1));
    memcpy (tokenBuffer, buffer, length);
    tokenBuffer[length] = '\0';
    
    // add the token to the tail of the list
    myListOfTokens->AddToTail (new Token (tokenBuffer, action));
}

//------------------------------------------------------------------------------
