//------------------------------------------------------------------------------
//	Copyright (C) 1997-2000 Bretton Wade
//	All Rights Reserved
//------------------------------------------------------------------------------

//--------------------------------------------------------------------------
//	includes
//--------------------------------------------------------------------------
#include	"precompile.h"
#include	"text.h"
#include	"test.h"

//--------------------------------------------------------------------------
//	class methods
//--------------------------------------------------------------------------
Text::Text
(
	void
) : PtrTo<RawText> (new RawText)
{
}

//--------------------------------------------------------------------------
Text::Text
(
	cString				text,
	sInt				length
) : PtrTo<RawText> (new RawText (text, length))
{
}

//--------------------------------------------------------------------------
bool
Text::IsUnique
(
	void
) const
{
	return PtrTo<RawText>::IsUnique ();
}

//--------------------------------------------------------------------------
void
Text::MakeUnique
(
	void
)
{
	PtrTo<RawText>::MakeUnique ();
}

//--------------------------------------------------------------------------
Text&
Text::operator =
(
	const Text&			text
)
{
	PtrTo<RawText>::operator = (text);
	return *this;
}

//--------------------------------------------------------------------------
bool
Text::operator ==
(
	const Text&			text
) const
{
	if (myPtr != text.myPtr)
		return *myPtr == *(text.myPtr);
	return true;
}

//--------------------------------------------------------------------------
bool
Text::operator !=
(
	const Text&			text
) const
{
	if (myPtr == text.myPtr)
		return false;
	return *myPtr != *(text.myPtr);
}

//--------------------------------------------------------------------------
bool
Text::operator <
(
	const Text&			text
) const
{
	if (myPtr == text.myPtr)
		return false;
	return *myPtr < *(text.myPtr);
}

//--------------------------------------------------------------------------
bool
Text::operator <=
(
	const Text&			text
) const
{
	if (myPtr != text.myPtr)
		return *myPtr <= *(text.myPtr);
	return true;
}

//--------------------------------------------------------------------------
bool
Text::operator >
(
	const Text&			text
) const
{
	if (myPtr == text.myPtr)
		return false;
	return *myPtr > *(text.myPtr);
}

//--------------------------------------------------------------------------
bool
Text::operator >=
(
	const Text&			text
) const
{
	if (myPtr != text.myPtr)
		return *myPtr >= *(text.myPtr);
	return true;
}

//--------------------------------------------------------------------------
Text
Text::operator +
(
	const Text&			text
) const
{
	return *myPtr + *(text.myPtr);
}

//--------------------------------------------------------------------------
Text&
Text::operator +=
(
	const Text&			text
)
{
	return operator = (*this + text);
}

//--------------------------------------------------------------------------
bool
Text::operator ==
(
	cString				text
) const
{
	return *myPtr == RawText (text);
}

//--------------------------------------------------------------------------
bool
Text::operator !=
(
	cString				text
) const
{
	return *myPtr != RawText (text);
}

//--------------------------------------------------------------------------
bool
Text::operator <
(
	cString				text
) const
{
	return (*myPtr < RawText (text));
}

//--------------------------------------------------------------------------
bool
Text::operator <=
(
	cString				text
) const
{
	return (*myPtr <= RawText (text));
}

//--------------------------------------------------------------------------
bool
Text::operator >
(
	cString				text
) const
{
	return (*myPtr > RawText (text));
}

//--------------------------------------------------------------------------
bool
Text::operator >=
(
	cString				text
) const
{
	return (*myPtr >= RawText (text));
}

//--------------------------------------------------------------------------
Text
Text::operator +
(
	cString				text
) const
{
	return Text (*myPtr + RawText (text));
}

//--------------------------------------------------------------------------
Text&
Text::operator +=
(
	cString				text
)
{
	return operator = (*this + text);
}

//--------------------------------------------------------------------------
Text::operator cString
(
	void
) const
{
	return myPtr->GetText ();
}

//--------------------------------------------------------------------------
cString
Text::GetText
(
	void
) const
{
	return myPtr->GetText ();
}

//--------------------------------------------------------------------------
Text
Text::GetSubtext
(
	uInt				start,
	uInt				length
) const
{
	return Text (myPtr->GetSubtext (start, length));
}

//--------------------------------------------------------------------------
sInt1
Text::GetChar
(
	uInt				which
) const
{
	return myPtr->GetChar (which);
}

//--------------------------------------------------------------------------
void
Text::Delete
(
	uInt				start,
	uInt				length
)
{
	myPtr->Delete (start, length);
}

//--------------------------------------------------------------------------
void
Text::Insert
(
	cString				text,
	uInt				start,
	uInt				length
)
{
	myPtr->Insert (text, start, length);
}

//--------------------------------------------------------------------------
uInt
Text::Length
(
	void
) const
{
	return myPtr->Length ();
}

//--------------------------------------------------------------------------
void
Text::ToFile
(
	const Text&			filename
) const
{
	myPtr->ToFile (filename);
}

//--------------------------------------------------------------------------
Text
Text::FromFile
(
	const Text&			filename
)
{
	return Text (RawText::FromFile (filename));
}

//--------------------------------------------------------------------------
Text::Text
(
	RawText*		text
) : PtrTo<RawText> (text)
{
}

//--------------------------------------------------------------------------
//	stream operators
//--------------------------------------------------------------------------
ostream&
operator <<
(
	ostream&			stream,
	const Text&			text
)
{
	stream << text.GetText ();
	return stream;
}

//--------------------------------------------------------------------------
//	tests
//--------------------------------------------------------------------------
#ifdef	_RUN_TESTS
DEFINE_TEST (Text)
{
	DO_AFTER (Bits);
	DO_AFTER (PtrTo);
}

//--------------------------------------------------------------------------
ON_TEST (Text)
{
	DO_TEST_REPORT;
	Text	text ("hello"),
			text2 = text,
			sub = text.GetSubtext (2, 2),
			A = "ABCD",
			B = "BCDE";
	TEST_CONDITION (text.Length () == 5);
	TEST_CONDITION (strcmp (text, "hello") == 0);
	TEST_CONDITION (text == text2);
	TEST_CONDITION (sub == "ll");
	TEST_CONDITION (sub > "aa");
	TEST_CONDITION (A == A);
	TEST_CONDITION (A < B);
	TEST_CONDITION (A <= B);
	TEST_CONDITION (A != B);
	TEST_CONDITION (B > A);
	TEST_CONDITION (B >= A);

	text += text2;
	TEST_CONDITION (text == "hellohello");
	TEST_CONDITION (text.Length () == 10);

	text.Delete (3, 2);
	TEST_CONDITION (text == "helhello");
	TEST_CONDITION (text.Length () == 8);

	text.Insert ("p", 3, 1);
	TEST_CONDITION (text == "helphello");
	TEST_CONDITION (text.Length () == 9);
	TEST_CONDITION (strcmp (text, "helphello") == 0);
	TEST_CONDITION (text.GetChar (3) == 'p');

	text.Delete (0, 1);
	TEST_CONDITION (text == "elphello");
	TEST_CONDITION (text.Length () == 8);

	text.Insert ("h", 0, 1);
	TEST_CONDITION (text == "helphello");
	TEST_CONDITION (text.Length () == 9);

	text.Delete (text.Length () - 1, 1);
	TEST_CONDITION (text == "helphell");
	TEST_CONDITION (text.Length () == 8);

	text.Insert ("o", text.Length (), 1);
	TEST_CONDITION (text == "helphello");
	TEST_CONDITION (text.Length () == 9);
}
#endif

//--------------------------------------------------------------------------
