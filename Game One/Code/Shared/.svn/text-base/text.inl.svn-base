//-----------------------------------------------------------------------------
// Copyright (C) 1997-2006 Bretton Wade
// All rights reserved.
//-----------------------------------------------------------------------------

#ifndef     _TEXT_INL_
#define     _TEXT_INL_

//-----------------------------------------------------------------------------
// include files
//-----------------------------------------------------------------------------
#ifndef     _TEXT_H_
#include    "text.h"
#endif  //  _TEXT_H_

//-----------------------------------------------------------------------------
// class methods
//-----------------------------------------------------------------------------
inline
/* void */
Text::Text (void) : 
    PtrTo<RawText> (NewCall RawText)
{
}

//-----------------------------------------------------------------------------
inline
/* void */
Text::Text (cString text, sInt length) :
    PtrTo<RawText> (NewCall RawText (text, length))
{
}

//-----------------------------------------------------------------------------
inline
/* void */
Text::Text (float number, cString format) :
    PtrTo<RawText> (NewCall RawText (1, format, number))
{
}

//-----------------------------------------------------------------------------
inline
/* void */
Text::Text (uInt number, cString format) :
    PtrTo<RawText> (NewCall RawText (1, format, number))
{
}

//-----------------------------------------------------------------------------
inline
/* void */
Text::Text (sInt number, cString format) :
    PtrTo<RawText> (NewCall RawText (1, format, number))
{
}

//-----------------------------------------------------------------------------
inline
bool
Text::IsUnique (void) const
{
    return PtrTo<RawText>::IsUnique ();
}

//-----------------------------------------------------------------------------
inline
void
Text::MakeUnique (void)
{
    PtrTo<RawText>::MakeUnique ();
}

//-----------------------------------------------------------------------------
inline
Text&
Text::operator = (const Text& text)
{
    PtrTo<RawText>::operator = (text);
    return *this;
}

//-----------------------------------------------------------------------------
inline
bool
Text::operator == (const Text& text) const
{
    if (m_ptr != text.m_ptr)
        return *m_ptr == *(text.m_ptr);
    return true;
}

//-----------------------------------------------------------------------------
inline
bool
Text::operator != (const Text& text) const
{
    if (m_ptr == text.m_ptr)
        return false;
    return *m_ptr != *(text.m_ptr);
}

//-----------------------------------------------------------------------------
inline
bool
Text::operator < (const Text& text) const
{
    if (m_ptr == text.m_ptr)
        return false;
    return *m_ptr < *(text.m_ptr);
}

//-----------------------------------------------------------------------------
inline
bool
Text::operator <= (const Text& text) const
{
    if (m_ptr != text.m_ptr)
        return *m_ptr <= *(text.m_ptr);
    return true;
}

//-----------------------------------------------------------------------------
inline
bool
Text::operator > (const Text& text) const
{
    if (m_ptr == text.m_ptr)
        return false;
    return *m_ptr > *(text.m_ptr);
}

//-----------------------------------------------------------------------------
inline
bool
Text::operator >= (const Text& text) const
{
    if (m_ptr != text.m_ptr)
        return *m_ptr >= *(text.m_ptr);
    return true;
}

//-----------------------------------------------------------------------------
inline
Text
Text::operator + (const Text& text) const
{
    return *m_ptr + *(text.m_ptr);
}

//-----------------------------------------------------------------------------
inline
Text&
Text::operator += (const Text& text)
{
    return operator = (*this + text);
}

//-----------------------------------------------------------------------------
inline
bool
Text::operator == (cString text) const
{
    return *m_ptr == RawText (text);
}

//-----------------------------------------------------------------------------
inline
bool
Text::operator != (cString text) const
{
    return *m_ptr != RawText (text);
}

//-----------------------------------------------------------------------------
inline
bool
Text::operator < (cString text) const
{
    return (*m_ptr < RawText (text));
}

//-----------------------------------------------------------------------------
inline
bool
Text::operator <= (cString text) const
{
    return (*m_ptr <= RawText (text));
}

//-----------------------------------------------------------------------------
inline
bool
Text::operator > (cString text) const
{
    return (*m_ptr > RawText (text));
}

//-----------------------------------------------------------------------------
inline
bool
Text::operator >= (cString text) const
{
    return (*m_ptr >= RawText (text));
}

//-----------------------------------------------------------------------------
inline
Text
Text::operator + (cString text) const
{
    return Text (*m_ptr + RawText (text));
}

//-----------------------------------------------------------------------------
inline
Text&
Text::operator += (cString text)
{
    return operator = (*this + text);
}

//-----------------------------------------------------------------------------
inline
/* cString */
Text::operator cString (void) const
{
    return m_ptr->GetText ();
}

//-----------------------------------------------------------------------------
inline
float
Text::ConvertToFloat (void) const
{
    return static_cast<float> (atof (m_ptr->GetText ()));
}

//-----------------------------------------------------------------------------
inline
uInt
Text::ConvertToUnsignedInt (void) const
{
    return atoi (m_ptr->GetText ());
}

//-----------------------------------------------------------------------------
inline
sInt
Text::ConvertToSignedInt (void) const
{
    return atoi (m_ptr->GetText ());
}

//-----------------------------------------------------------------------------
inline
cString
Text::GetText (void) const
{
    return m_ptr->GetText ();
}

//-----------------------------------------------------------------------------
inline
Text
Text::GetSubtext (uInt start, uInt length) const
{
    return Text (m_ptr->GetSubtext (start, length));
}

//-----------------------------------------------------------------------------
inline
Character
Text::GetChar (uInt which) const
{
    return m_ptr->GetChar (which);
}

//-----------------------------------------------------------------------------
inline
void
Text::Delete (uInt start, uInt length)
{
    m_ptr->Delete (start, length);
}

//-----------------------------------------------------------------------------
inline
void
Text::Insert (cString text, uInt start, uInt length)
{
    m_ptr->Insert (text, start, length);
}

//-----------------------------------------------------------------------------
inline
uInt
Text::Length (void) const
{
    return m_ptr->Length ();
}

//-----------------------------------------------------------------------------
inline
/* void */
Text::Text (RawText* pRawText) : PtrTo<RawText> (pRawText)
{
}

//-----------------------------------------------------------------------------
// global operators
//-----------------------------------------------------------------------------
inline
Text
operator + (cString text1, const Text& text2)
{
    return Text (text1) + text2;
}

//-----------------------------------------------------------------------------
//    stream operators
//-----------------------------------------------------------------------------
inline
std::ostream&
operator << (std::ostream& stream, const Text& text)
{
    return stream << text.GetText ();
}

//-----------------------------------------------------------------------------

#endif  //  _TEXT_INL_
