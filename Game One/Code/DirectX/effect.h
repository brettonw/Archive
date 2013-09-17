//-----------------------------------------------------------------------------
// Copyright (C) 1997-2006 Bretton Wade
// All Rights Reserved
//-----------------------------------------------------------------------------

#ifndef     _EFFECT_H_
#define     _EFFECT_H_

//-----------------------------------------------------------------------------
// include files
//-----------------------------------------------------------------------------
#ifndef     _DIRECT_3D_H_
#include    "direct_3d.h"
#endif  //  _DIRECT_3D_H_

#ifndef     _INDEXED_OBJECT_H_
#include    "indexed_object.h"
#endif  //  _INDEXED_OBJECT_H_

//-----------------------------------------------------------------------------
// wrapper types
//-----------------------------------------------------------------------------
typedef DXWrapper<ID3DXEffect>  PtrToD3DXEffect;

//-----------------------------------------------------------------------------
// useful type definitions
//-----------------------------------------------------------------------------
MAKE_PTR_TO (Effect);

//-----------------------------------------------------------------------------
// class definitions
//-----------------------------------------------------------------------------
class Effect : public IndexedObject <Effect>
{
    public:
        PtrToD3DXEffect&        GetD3DXEffect (void) { return m_pEffect; }
static  void                    Select (const PtrToEffect& pEffect);
static  void                    Clear (void);
static  PtrToEffect&            GetCurrent (void);

    protected:
    DECLARE_FACTORY (Effect, Effect);

    protected:
        PtrToD3DXEffect         m_pEffect;
static  PtrToEffect             s_pCurrent;
};

//-----------------------------------------------------------------------------

#endif  //  _EFFECT_H_
