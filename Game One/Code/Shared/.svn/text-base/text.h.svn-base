//-----------------------------------------------------------------------------
// Copyright (C) 1997-2006 Bretton Wade
// All rights reserved.
//-----------------------------------------------------------------------------

#ifndef     _TEXT_H_
#define     _TEXT_H_

//-----------------------------------------------------------------------------
// include files
//-----------------------------------------------------------------------------
#ifndef     _PTR_TO_H_
#include    "ptr_to.h"
#endif  //  _PTR_TO_H_

#ifndef     _RAW_TEXT_H_
#include    "raw_text.h"
#endif  //  _RAW_TEXT_H_

//-----------------------------------------------------------------------------
// class definitions
//-----------------------------------------------------------------------------
class Text : private PtrTo<RawText>
{
    public:
        /* void */          Text (void);
        /* void */          Text (cString text, sInt length = -1);
        /* void */          Text (float number, cString format = "% 0.03f");
        /* void */          Text (uInt number, cString format = "% u");
        /* void */          Text (sInt number, cString format = "% i");
virtual /* void */          ~Text (void);
        
        // methods for testing uniqueness and orphaning the pointer
        bool                IsUnique (void) const;
        void                MakeUnique (void);
        
        // assignment
        Text&               operator = (const Text& text);
        
        // comparison methods against text
        bool                operator == (const Text& text) const;
        bool                operator != (const Text& text) const;
        
        bool                operator < (const Text& text) const;
        bool                operator <= (const Text& text) const;
        bool                operator > (const Text& text) const;
        bool                operator >= (const Text& text) const;
        
        // addition operators against text
        Text                operator + (const Text& text) const;
        Text&               operator += (const Text& text);

        // comparison methods against strings
        bool                operator == (cString text) const;
        bool                operator != (cString text) const;
        
        bool                operator < (cString text) const;
        bool                operator <= (cString text) const;
        bool                operator > (cString text) const;
        bool                operator >= (cString text) const;
        
        // addition operators against strings
        Text                operator + (cString text) const;
        Text&               operator += (cString text);
        
        // casting operator for getting the string buffer
        /* cString */       operator cString (void) const;
        
        // methods for converting text to numbers
        float               ConvertToFloat (void) const;
        uInt                ConvertToUnsignedInt (void) const;
        sInt                ConvertToSignedInt (void) const;
        
        // methods for fetching parts of the text
        cString             GetText (void) const;
        Text                GetSubtext (uInt start, uInt length) const;
        Character           GetChar (uInt which) const;
        
        // methods for manipulating the text
        void                Delete (uInt start, uInt length);
        void                Insert (cString text, uInt start, uInt length);
        
        // return the length of the text
        uInt                Length (void) const;
        
        // read and write text buffers to and from a file
static  Text                FromFile (const Text& fileName);
static  void                ToFile (const Text& fileName, const Text& text);

        // encode and decode binary format buffers as hexadecimal text
        // text length is twice the size of the buffer
static  Text                HexEncode (const void* buffer, uInt length);
static  void                HexDecode (const Text& text, void* buffer);
        
    private:
        /* void */          Text (RawText* pRawText);
};

//-----------------------------------------------------------------------------
// inlines
//-----------------------------------------------------------------------------
#ifndef     _TEXT_INL_
#include    "text.inl"
#endif  //  _TEXT_INL_

//-----------------------------------------------------------------------------

#endif  //  _TEXT_H_
