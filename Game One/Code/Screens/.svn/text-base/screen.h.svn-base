//-----------------------------------------------------------------------------
// Copyright (C) 1997-2006 Bretton Wade
// All Rights Reserved
//-----------------------------------------------------------------------------

#ifndef     _SCREEN_H_
#define     _SCREEN_H_

//-----------------------------------------------------------------------------
// include files
//-----------------------------------------------------------------------------
#ifndef     _INDEXED_OBJECT_H_
#include    "indexed_object.h"
#endif  //  _INDEXED_OBJECT_H_

//-----------------------------------------------------------------------------
// useful type definitions
//-----------------------------------------------------------------------------
MAKE_PTR_TO (Screen);

//-----------------------------------------------------------------------------
// class definitions
//-----------------------------------------------------------------------------
class Screen : public IndexedObject<Screen>
{
    public:
        void                GammaFade (float fValue);
        
virtual void                Prologue (void);
virtual void                Heartbeat (void);
virtual void                Epilogue (void);
        
    protected:
        enum State {PRE_PROLOGUE, PROLOGUE, EVENT, PRE_EPILOGUE, EPILOGUE};
        State               m_state;
        float               m_fTimer;
        
    protected:
        /* void */              Screen (const PtrToXMLNode& pXMLNode);
virtual /* void */              ~Screen (void) {}

};

//-----------------------------------------------------------------------------

#endif  //  _SCREEN_H_
