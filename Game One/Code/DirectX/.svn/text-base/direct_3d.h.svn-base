//-----------------------------------------------------------------------------
// Copyright (C) 1997-2006 Bretton Wade
// All Rights Reserved
//-----------------------------------------------------------------------------

#ifndef     _DIRECT_3D_H_
#define     _DIRECT_3D_H_

//-----------------------------------------------------------------------------
// include files
//-----------------------------------------------------------------------------
#ifndef     _SINGLETON_H_
#include    "singleton.h"
#endif  //  _SINGLETON_H_

#ifndef     _DX_WRAPPER_H_
#include    "dx_wrapper.h"
#endif  //  _DX_WRAPPER_H_

#ifndef     _DX_DEBUG_H_
#include    "dx_debug.h"
#endif  //  _DX_DEBUG_H_

#ifndef     _XML_NODE_H_
#include    "xml_node.h"
#endif  //  _XML_NODE_H_

//-----------------------------------------------------------------------------
// wrapper types
//-----------------------------------------------------------------------------
typedef DXWrapper<IDirect3D9>                   PtrToDirect3DObj;
typedef DXWrapper<IDirect3DDevice9>             PtrToD3DDevice;
typedef DXWrapper<IDirect3DTexture9>            PtrToD3DTexture;
typedef DXWrapper<IDirect3DVertexBuffer9>       PtrToD3DVertexBuffer;
typedef DXWrapper<IDirect3DIndexBuffer9>        PtrToD3DIndexBuffer;
typedef DXWrapper<IDirect3DVertexDeclaration9>  PtrToD3DVertexDecl;

//-----------------------------------------------------------------------------
// class definitions
//-----------------------------------------------------------------------------
class   Direct3D : public Singleton<Direct3D>
{
    public:
        /* void */              Direct3D (void);
        /* void */              ~Direct3D (void);
        
        PtrToD3DDevice           operator -> (void) const { return m_pDevice; }
        PtrToD3DDevice           operator * (void) const { return m_pDevice; }
        
        const D3DDISPLAYMODE&   GetDisplayMode (void) { return m_displayMode; }
        uInt					GetRefreshRate (void) { return m_iRefreshRate; }
        float                   GetAspectRatio (void) { return m_fAspectRatio; }
        void                    TimeRefreshRate (void);

    public:
static  bool                    s_bOK;

    protected:
        PtrToD3DDevice          m_pDevice;
        
        PtrToDirect3DObj        m_pDirect3D;
        D3DDISPLAYMODE          m_displayMode;
        uInt                    m_iRefreshRate;
        float                   m_fAspectRatio;
};

//-----------------------------------------------------------------------------
// macros
//-----------------------------------------------------------------------------
#define     D3D_TEST(condition)                 DIRECT_X_SUCCEEDED(condition, Direct3D::s_bOK, D3D_OK, true)
#define     D3D_REQUIRE(condition)              DIRECT_X_SUCCEEDED(condition, Direct3D::s_bOK, D3D_OK, false)

//-----------------------------------------------------------------------------

#endif  //  _DIRECT_3D_H_
