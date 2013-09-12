//------------------------------------------------------------------------------
//	Copyright (C) 1997-2000 Bretton Wade
//	All Rights Reserved
//------------------------------------------------------------------------------

#ifndef		_BW_TEXT_H_
#define		_BW_TEXT_H_

//--------------------------------------------------------------------------
//	includes
//--------------------------------------------------------------------------
#ifndef		_BW_PTR_TO_H_
#include	"ptr_to.h"
#endif	//	_BW_PTR_TO_H_

#ifndef		_BW_RAW_TEXT_H_
#include	"raw_text.h"
#endif	//	_BW_RAW_TEXT_H_

//--------------------------------------------------------------------------
//	class definitions
//--------------------------------------------------------------------------
class	Text : private PtrTo<RawText>
{
	public:
							Text (void);
							Text (cString text, sInt length = -1);
		bool				IsUnique (void) const;
		void				MakeUnique (void);
		Text&				operator = (const Text& text);
		bool				operator == (const Text& text) const;
		bool				operator != (const Text& text) const;
		bool				operator < (const Text& text) const;
		bool				operator <= (const Text& text) const;
		bool				operator > (const Text& text) const;
		bool				operator >= (const Text& text) const;
		Text				operator + (const Text& text) const;
		Text&				operator += (const Text& text);
		bool				operator == (cString text) const;
		bool				operator != (cString text) const;
		bool				operator < (cString text) const;
		bool				operator <= (cString text) const;
		bool				operator > (cString text) const;
		bool				operator >= (cString text) const;
		Text				operator + (cString text) const;
		Text&				operator += (cString text);
							operator cString (void) const;
		cString				GetText (void) const;
		Text				GetSubtext (uInt start, uInt length) const;
		sInt1				GetChar (uInt which) const;
		void				Delete (uInt start, uInt length);
		void				Insert (cString text, uInt start, uInt length);
		uInt				Length (void) const;
		void				ToFile (const Text& filename) const;
static	Text				FromFile (const Text& filename);
	private:
							Text (RawText* text);
};

//--------------------------------------------------------------------------
//	global operators
//--------------------------------------------------------------------------
inline
Text	operator + (cString text1, const Text& text2)
{
	return Text (text1) + text2;
}

//--------------------------------------------------------------------------
//	stream operators
//--------------------------------------------------------------------------
ostream&	operator << (ostream& stream, const Text& text);

//--------------------------------------------------------------------------

#endif	//	_BW_TEXT_H_
