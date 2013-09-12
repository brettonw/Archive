//------------------------------------------------------------------------------
//	Copyright (C) 1997-2000 Bretton Wade
//	All Rights Reserved
//------------------------------------------------------------------------------

#ifndef		_BW_TOKEN_H_
#define		_BW_TOKEN_H_

//------------------------------------------------------------------------------
//	includes
//------------------------------------------------------------------------------
#ifndef		_BW_TEXT_H_
#include	"text.h"
#endif	//	_BW_TEXT_H_

//------------------------------------------------------------------------------
//	class definitions
//------------------------------------------------------------------------------
class	Token : public CountedObject
{
	public:
		/* void */  Token (const Text& text, uInt2 type);
virtual	/* void */  ~Token (void);
        Text        GetText (void) const;
        uInt2       GetType (void) const;

	protected:
	    Text        myText;
	    uInt2       myType;
};

//------------------------------------------------------------------------------

#endif	//  _BW_TOKEN_H_
