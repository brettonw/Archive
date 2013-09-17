//-----------------------------------------------------------------------------
// Copyright (C) 1997-2006 Bretton Wade
// All rights reserved.
//-----------------------------------------------------------------------------

#ifndef     _RAW_TEXT_H_
#define     _RAW_TEXT_H_

//-----------------------------------------------------------------------------
// include files
//-----------------------------------------------------------------------------
#ifndef     _COUNTED_OBJECT_H_
#include    "counted_object.h"
#endif  //  _COUNTED_OBJECT_H_

//-----------------------------------------------------------------------------
// class definitions
//-----------------------------------------------------------------------------
class   RawText : public CountedObject
{
    public:
        /* void */          RawText (void);
        /* void */          RawText (cString text, sInt length = -1);
        /* void */          RawText (uInt unused, cString format, ...);
        /* void */          RawText (const RawText& text);
        /* void */          ~RawText (void);
        
        bool                operator == (const RawText& text) const;
        bool                operator != (const RawText& text) const;
        
        bool                operator < (const RawText& text) const;
        bool                operator <= (const RawText& text) const;
        bool                operator > (const RawText& text) const;
        bool                operator >= (const RawText& text) const;
        
        RawText*            operator + (const RawText& text) const;
        
        cString             GetText (void) const;
        RawText*            GetSubtext (uInt start, uInt length) const;
        Character           GetChar (uInt which) const;
        
        void                Delete (uInt start, uInt length);
        void                Insert (cString text, uInt start, uInt length);
        
        uInt                Length (void) const;
        
    private:
        String              m_text;
        uInt                m_length;
        uInt                m_allocationSize;
};

//-----------------------------------------------------------------------------

#endif  //  _RAW_TEXT_H_
