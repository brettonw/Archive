//------------------------------------------------------------------------------
//	Copyright (C) 1997-2000 Bretton Wade
//	All Rights Reserved
//------------------------------------------------------------------------------

#ifndef		_BW_DISPLAY_TOKENIZER_H_
#define		_BW_DISPLAY_TOKENIZER_H_

//------------------------------------------------------------------------------
//	includes
//------------------------------------------------------------------------------
#ifndef	    _BW_TOKENIZER_H_
#include	"tokenizer.h"
#endif	//	_BW_TOKENIZER_H_

//------------------------------------------------------------------------------
//	class definitions
//------------------------------------------------------------------------------
class   DisplayTokenizer : public Tokenizer
{
	public:
		/* void */                          DisplayTokenizer (const HandleTo<Scanner>& scanner, uInt tabStops, uInt tabSize);
virtual	/* void */                          ~DisplayTokenizer (void);
virtual	void                                Action (cString buffer, uInt length, uInt2 action, cString nextBuffer, uInt2 nextState);
virtual HandleTo<ListOf<PtrTo<Token> > >    Tokenize (const Text& text, uInt2 startState = ScannerState::DEFAULT_STATE);

	protected:
	    void                                CacheSpace (void);
	    void                                FlushCachedSpaces (void);

	protected:
	    String                              mySpaceCache;
	    uInt                                myCachedSpacesCount;
	    uInt                                myTabStops;
	    uInt                                myTabSize;
	    uInt                                myX;
};

//------------------------------------------------------------------------------

#endif	//  _BW_DISPLAY_TOKENIZER_H_
