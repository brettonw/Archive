//-----------------------------------------------------------------------------
// Copyright (C) 1997-2006 Bretton Wade
// All rights reserved.
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// include files
//-----------------------------------------------------------------------------
#include    "precompile.h"
#include    "raw_text.h"
#include    "bits.h"

//-----------------------------------------------------------------------------
// class methods
//-----------------------------------------------------------------------------
/* void */
RawText::RawText (void)
{
    m_text = NewCall Character[1];
    m_text[0] = 0;
    m_length = 0;
    m_allocationSize = 1;
}

//-----------------------------------------------------------------------------
/* void */
RawText::RawText (cString text, sInt length)
{
    m_length = (length >= 0) ? static_cast<uInt> (length) : (text ? strlen (text) : 0);
    m_allocationSize = m_length + 1;
    if (not IsPowerOf2 (static_cast<sInt> (m_allocationSize)))
        m_allocationSize = static_cast<uInt> (HighestBit (static_cast<sInt> (m_allocationSize)) << 1);
    m_text = NewCall Character[m_allocationSize];
	uInt	i;
    for (i = 0; (i < m_length) and text[i]; i++)
        m_text[i] = text[i];
    m_text[i] = 0;
}

//-----------------------------------------------------------------------------
/* void */
RawText::RawText (uInt unused, cString format, ...)
{
    va_list     args;
    va_start (args, format);
    Character   buffer[32];
    m_length = vsprintf (buffer, format, args);
    va_end (args);
    m_allocationSize = m_length + 1;
    if (not IsPowerOf2 (static_cast<sInt> (m_allocationSize)))
        m_allocationSize = static_cast<uInt> (HighestBit (static_cast<sInt> (m_allocationSize)) << 1);
    m_text = NewCall Character[m_allocationSize];
    strcpy (m_text, buffer);
}

//-----------------------------------------------------------------------------
/* void */
RawText::RawText (const RawText& text)
{
    m_length = text.m_length;
    m_allocationSize = text.m_allocationSize;
    m_text = NewCall Character[m_allocationSize];
    strcpy (m_text, text.m_text);
}

//-----------------------------------------------------------------------------
/* void */
RawText::~RawText (void)
{
    delete[] m_text;
}

//-----------------------------------------------------------------------------
bool
RawText::operator == (const RawText& text) const
{
    return bool (strcmp (m_text, text.m_text) == 0);
}

//-----------------------------------------------------------------------------
bool
RawText::operator != (const RawText& text) const
{
    return bool (strcmp (m_text, text.m_text) != 0);
}

//-----------------------------------------------------------------------------
bool
RawText::operator < (const RawText& text) const
{
    return bool (strcmp (m_text, text.m_text) < 0);
}

//-----------------------------------------------------------------------------
bool
RawText::operator <= (const RawText& text) const
{
    return bool (strcmp (m_text, text.m_text) <= 0);
}

//-----------------------------------------------------------------------------
bool
RawText::operator > (const RawText& text) const
{
    return bool (strcmp (m_text, text.m_text) > 0);
}

//-----------------------------------------------------------------------------
bool
RawText::operator >= (const RawText& text) const
{
    return bool (strcmp (m_text, text.m_text) >= 0);
}

//-----------------------------------------------------------------------------
RawText*
RawText::operator + (const RawText& text) const
{
    RawText*    newRaw = NewCall RawText (m_text, static_cast<sInt> (m_length + text.m_length));
    strcpy (&(newRaw->m_text[m_length]), text.m_text);
    return newRaw;
}

//-----------------------------------------------------------------------------
cString
RawText::GetText (void) const
{
    return m_text;
}

//-----------------------------------------------------------------------------
RawText*
RawText::GetSubtext (uInt start, uInt length) const
{
    Assert (start + length <= m_length);
    return NewCall RawText (&(m_text[start]), static_cast<sInt> (length));
}

//-----------------------------------------------------------------------------
sInt1
RawText::GetChar (uInt which) const
{
    Assert (which < m_length);
    return m_text[which];
}

//-----------------------------------------------------------------------------
void
RawText::Delete (uInt start, uInt length)
{
    Assert (start + length <= m_length);
    String        copyTo = m_text + start;
    strcpy (copyTo, copyTo + length);
    m_length -= length;
}

//-----------------------------------------------------------------------------
void
RawText::Insert (cString text, uInt start, uInt length)
{
    Assert (start <= m_length);
    uInt    newLength = m_length + length,
            newSize = newLength + 1;
    if (newSize > m_allocationSize)
    {
        while (m_allocationSize < newSize)
            m_allocationSize <<= 1;
        String    newText = NewCall sInt1[m_allocationSize];
        memcpy (newText, m_text, start);
        memcpy (newText + start, text, length);
        memcpy (newText + start + length, m_text + start, m_length - start);
        delete[] m_text;
        m_text = newText;
    }
    else
    {
        memmove (m_text + start + length, m_text + start, m_length - start);
        memcpy (m_text + start, text, length);
    }
    m_length = newLength;
    m_text[m_length] = 0;
}

//-----------------------------------------------------------------------------
uInt
RawText::Length (void) const
{
    return m_length;
}

//-----------------------------------------------------------------------------
