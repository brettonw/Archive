//------------------------------------------------------------------------------
//	Copyright (C) 1997-2000 Bretton Wade
//	All Rights Reserved
//------------------------------------------------------------------------------

//--------------------------------------------------------------------------
//	includes
//--------------------------------------------------------------------------
#include	"precompile.h"
#include	"raw_text.h"
#include	"bits.h"

//--------------------------------------------------------------------------
//	class methods
//--------------------------------------------------------------------------
RawText::RawText
(
	void
)
{
	myText = new sInt1[1];
	myText[0] = 0;
	myLength = 0;
	myAllocationSize = 1;
}

//--------------------------------------------------------------------------
RawText::RawText
(
	cString				text,
	sInt				length
)
{
	myLength = (length >= 0) ? static_cast<uInt> (length) : (text ? strlen (text) : 0);
	myAllocationSize = myLength + 1;
	if (not IsPowerOf2 (static_cast<sInt> (myAllocationSize)))
		myAllocationSize = static_cast<uInt> (HighestBit (static_cast<sInt> (myAllocationSize)) << 1);
	myText = new sInt1[myAllocationSize];
	for (uInt i = 0; i < myLength; i++)
		myText[i] = text[i];
	myText[myLength] = 0;
}

//--------------------------------------------------------------------------
RawText::RawText
(
	const RawText&		text
)
{
	myLength = text.myLength;
	myAllocationSize = text.myAllocationSize;
	myText = new sInt1[myAllocationSize];
	strcpy (myText, text.myText);
}

//--------------------------------------------------------------------------
RawText::~RawText
(
	void
)
{
	delete[] myText;
}

//--------------------------------------------------------------------------
bool
RawText::operator ==
(
	const RawText&		text
) const
{
	return bool (strcmp (myText, text.myText) == 0);
}

//--------------------------------------------------------------------------
bool
RawText::operator !=
(
	const RawText&		text
) const
{
	return bool (strcmp (myText, text.myText) != 0);
}

//--------------------------------------------------------------------------
bool
RawText::operator <
(
	const RawText&		text
) const
{
	return bool (strcmp (myText, text.myText) < 0);
}

//--------------------------------------------------------------------------
bool
RawText::operator <=
(
	const RawText&		text
) const
{
	return bool (strcmp (myText, text.myText) <= 0);
}

//--------------------------------------------------------------------------
bool
RawText::operator >
(
	const RawText&		text
) const
{
	return bool (strcmp (myText, text.myText) > 0);
}

//--------------------------------------------------------------------------
bool
RawText::operator >=
(
	const RawText&		text
) const
{
	return bool (strcmp (myText, text.myText) >= 0);
}

//--------------------------------------------------------------------------
RawText*
RawText::operator +
(
	const RawText&		text
) const
{
	RawText*	newRaw = new RawText (myText, static_cast<sInt> (myLength + text.myLength));
	strcpy (&(newRaw->myText[myLength]), text.myText);
	return newRaw;
}

//--------------------------------------------------------------------------
cString
RawText::GetText
(
	void
) const
{
	return myText;
}

//--------------------------------------------------------------------------
RawText*
RawText::GetSubtext
(
	uInt				start,
	uInt				length
) const
{
	assert (start + length <= myLength);
	return new RawText (&(myText[start]), static_cast<sInt> (length));
}

//--------------------------------------------------------------------------
sInt1
RawText::GetChar
(
	uInt				which
) const
{

	assert (which < myLength);
	return myText[which];
}

//--------------------------------------------------------------------------
void
RawText::Delete
(
	uInt				start,
	uInt				length
)
{
	assert (start + length <= myLength);
	String		copyTo = myText + start;
	strcpy (copyTo, copyTo + length);
	myLength -= length;
}

//--------------------------------------------------------------------------
void
RawText::Insert
(
	cString				text,
	uInt				start,
	uInt				length
)
{
	assert (start <= myLength);
	uInt	newLength = myLength + length,
			newSize = newLength + 1;
	if (newSize > myAllocationSize)
	{
		while (myAllocationSize < newSize)
			myAllocationSize <<= 1;
		String	newText = new sInt1[myAllocationSize];
		memcpy (newText, myText, start);
		memcpy (newText + start, text, length);
		memcpy (newText + start + length, myText + start, myLength - start);
		delete[] myText;
		myText = newText;
	}
	else
	{
		memmove (myText + start + length, myText + start, myLength - start);
		memcpy (myText + start, text, length);
	}
	myLength = newLength;
	myText[myLength] = 0;
}

//--------------------------------------------------------------------------
uInt
RawText::Length
(
	void
) const
{
	return myLength;
}

//--------------------------------------------------------------------------
void
RawText::ToFile
(
	cString				filename
) const
{
	//	XXX might want to do this using streams
	FILE*	output = fopen (filename, "wb");
	if (output)
	{
		fwrite (myText, sizeof (sInt1), myLength, output);
		fclose (output);
	}
}

//--------------------------------------------------------------------------
RawText*
RawText::FromFile
(
	cString				filename
)
{
	RawText*	raw = 0;
    HANDLE      input = CreateFile (filename, FILE_READ_DATA, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
    if (input != INVALID_HANDLE_VALUE)
    {
		raw = new RawText (input);
		CloseHandle (input);
    }
    else
    {
        //raw = new RawText ("File open failed\n");
    }
    
	return raw;

	//	XXX might want to do this using streams
	/*
	RawText*	raw = 0;
	FILE*		input = fopen (filename, "rb");
	if (input)
	{
		raw = new RawText (input);
		fclose (input);
	}
	else
	{
	    char*   string = strerror (errno);
	    raw = new RawText (string);
	}
	return raw;
	*/
}

//--------------------------------------------------------------------------
RawText::RawText
(
	HANDLE				input
)
{
    myLength = GetFileSize (input, 0);
	myAllocationSize = myLength + 1;
	if (not IsPowerOf2 (myAllocationSize))
		myAllocationSize = HighestBit (myAllocationSize) << 1;
	myText = new sInt1[myAllocationSize];
	DWORD   numRead = 0;
    if (not ReadFile (input, myText, myLength, &numRead, 0))
        myLength = 0;
	myText[myLength] = 0;

    // do a quick truncation on unicode files
    // XXX would eventually like to handle unicode files
    if ((myText[0] == -1) and (myText[1] == -2))
    {
        if (MessageBox (0, "Would you like to convert unicode file to ANSI text (truncation)?", "Unicode file", MB_OKCANCEL) == IDOK)
        {
            uInt    i = 0;
            uInt    j = 2;
            while (j < myLength)
            {
                myText[i] = myText[j];
                ++i;
                j = j + 2;
            }
            myLength = i;
            myText[i] = 0;
        }
        else
        {
            myLength = 0;
            myText[0] = 0;
        }
    }
    
    /*
	//	XXX might want to do this using streams
	fseek (file, 0, SEEK_END);
	myLength = ftell (file);
	fseek (file, 0, SEEK_SET);
	myAllocationSize = myLength + 1;
	if (not IsPowerOf2 (myAllocationSize))
		myAllocationSize = HighestBit (myAllocationSize) << 1;
	myText = new sInt1[myAllocationSize];
	fread (myText, sizeof (sInt1), myLength, file);
	myText[myLength] = 0;
	*/
}

//--------------------------------------------------------------------------
