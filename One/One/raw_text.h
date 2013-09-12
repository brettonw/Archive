//------------------------------------------------------------------------------
//	Copyright (C) 1997-2000 Bretton Wade
//	All Rights Reserved
//------------------------------------------------------------------------------

#ifndef		_BW_RAW_TEXT_H_
#define		_BW_RAW_TEXT_H_

//--------------------------------------------------------------------------
//	includes
//--------------------------------------------------------------------------
#ifndef		_BW_COUNTED_OBJECT_H_
#include	"counted_object.h"
#endif	//	_BW_COUNTED_OBJECT_H_

//--------------------------------------------------------------------------
//	class definitions
//--------------------------------------------------------------------------
class	RawText : public CountedObject
{
	public:
							RawText (void);
							RawText (cString text, sInt length = -1);
							RawText (const RawText& text);
							~RawText (void);
		bool				operator == (const RawText& text) const;
		bool				operator != (const RawText& text) const;
		bool				operator < (const RawText& text) const;
		bool				operator <= (const RawText& text) const;
		bool				operator > (const RawText& text) const;
		bool				operator >= (const RawText& text) const;
		RawText*			operator + (const RawText& text) const;
		cString				GetText (void) const;
		RawText*			GetSubtext (uInt start, uInt length) const;
		sInt1				GetChar (uInt which) const;
		void				Delete (uInt start, uInt length);
		void				Insert (cString text, uInt start, uInt length);
		uInt				Length (void) const;
		void				ToFile (cString filename) const;
static	RawText*			FromFile (cString filename);
	private:
		RawText (HANDLE input);
	private:
		String				myText;
		uInt				myLength;
		uInt				myAllocationSize;
};

//--------------------------------------------------------------------------

#endif	//	_BW_RAW_TEXT_H_
