//------------------------------------------------------------------------------
//	Copyright (C) 1997-2000 Bretton Wade
//	All Rights Reserved
//------------------------------------------------------------------------------

#ifndef		_BW_TOKENIZER_H_
#define		_BW_TOKENIZER_H_

//------------------------------------------------------------------------------
//	includes
//------------------------------------------------------------------------------
#ifndef		_BW_TOKEN_H_
#include	"token.h"
#endif	//	_BW_TOKEN_H_

#ifndef		_BW_PTR_TO_H_
#include	"ptr_to.h"
#endif	//	_BW_PTR_TO_H_

#ifndef		_BW_HANDLE_TO_H_
#include	"handle_to.h"
#endif	//	_BW_HANDLE_TO_H_

#ifndef		_BW_LIST_OF_H_
#include	"list_of.h"
#endif	//	_BW_LIST_OF_H_

#ifndef		_BW_SCANNER_H_
#include	"scanner.h"
#endif	//	_BW_SCANNER_H_

//------------------------------------------------------------------------------
//	class definitions
//------------------------------------------------------------------------------
class   Tokenizer : public ScannerAction
{
	public:
		/* void */                          Tokenizer (const HandleTo<Scanner>& scanner);
virtual	/* void */                          ~Tokenizer (void);
virtual	void					            Action (cString buffer, uInt length, uInt2 action, cString nextBuffer, uInt2 nextState);
virtual HandleTo<ListOf<PtrTo<Token> > >    Tokenize (const Text& text, uInt2 startState = ScannerState::DEFAULT_STATE);
        
	protected:
	    void                                AddTokenToList (cString buffer, uInt length, uInt2 action);
        
	protected:
	    HandleTo<Scanner>                   myScanner;
	    HandleTo<ListOf<PtrTo<Token> > >    myListOfTokens;
};

//------------------------------------------------------------------------------

#endif	//  _BW_TOKENIZER_H_
