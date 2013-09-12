//------------------------------------------------------------------------------
//	Copyright (C) 1997-2000 Bretton Wade
//	All Rights Reserved
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//	includes
//------------------------------------------------------------------------------
#include	"precompile.h"
#include	"display_tokenizer.h"
#include	"text_Scanner_Action.h"

//------------------------------------------------------------------------------
//	class methods
//------------------------------------------------------------------------------
/* void */
DisplayTokenizer::DisplayTokenizer
(
	const HandleTo<Scanner>&    scanner,
	uInt                        tabStops,
	uInt                        tabSize
) : Tokenizer (scanner), mySpaceCache (new Character[tabStops]), myTabStops (tabStops), myTabSize (tabSize)
{
    for (uInt i = 0; i < tabStops; i++)
        mySpaceCache[i] = ' ';
}

//------------------------------------------------------------------------------
/* void */
DisplayTokenizer::~DisplayTokenizer
(
	void
)
{
    delete mySpaceCache;
}

//------------------------------------------------------------------------------
void
DisplayTokenizer::Action
(
	cString                     buffer,
	uInt                        length,
	uInt2                       action,
	cString                     nextBuffer,
	uInt2                       nextState
)
{
    switch (action)
    {
        case TextScannerAction::OUTPUT_WHITESPACE:
            assert (length ==1);
            switch (buffer[0])
            {
                case ' ':
                    CacheSpace ();
                    break;

                case '\t':
                    {
                        uInt    spacesToAdd = myTabSize - (myX % myTabSize);
                        for (uInt i = 0; i < spacesToAdd; i++)
                            CacheSpace ();
                    }
                    break;

                default:
                    break;
            }
            break;

        case TextScannerAction::OUTPUT_NEWLINE:
            FlushCachedSpaces ();
            AddTokenToList (buffer, length, action);
            myX = 0;
            break;
            
        default:
            FlushCachedSpaces ();
            AddTokenToList (buffer, length, action);
            myX += length;
            break;
    }
}

//------------------------------------------------------------------------------
HandleTo<ListOf<PtrTo<Token> > >
DisplayTokenizer::Tokenize
(
    const Text&                 text,
    uInt2                       startState
)
{
    // set parameters to the beginning of the line
    myX = 0;
    myCachedSpacesCount = 0;
    
    // then do the normal tokenizer thang
    return Tokenizer::Tokenize (text, startState);
}

//------------------------------------------------------------------------------
void
DisplayTokenizer::CacheSpace
(
    void
)
{
    // accumulate the space into the space cache
    myCachedSpacesCount++;
    myX++;
    
    // if the position is on a tabstop
    if (myX % myTabStops == 0)
    {
        // add the whitespace token
        AddTokenToList (mySpaceCache, myCachedSpacesCount, TextScannerAction::OUTPUT_WHITESPACE);
        
        // reset the cached space count
        myCachedSpacesCount = 0;
    }
}

//------------------------------------------------------------------------------
void
DisplayTokenizer::FlushCachedSpaces
(
    void
)
{
    // if there are cached spaces
    if (myCachedSpacesCount > 0)
    {
        // add the whitespace token
        AddTokenToList (mySpaceCache, myCachedSpacesCount, TextScannerAction::OUTPUT_WHITESPACE);
        
        // reset the cached space count
        myCachedSpacesCount = 0;
    }
}

//------------------------------------------------------------------------------
