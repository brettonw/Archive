//-----------------------------------------------------------------------------
// Copyright (C) 1997-2006 Bretton Wade
// All Rights Reserved
//-----------------------------------------------------------------------------

#ifndef     _DIRECT_MUSIC_H_
#define     _DIRECT_MUSIC_H_

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
//typedef DXWrapper<IDirectMusicLoader8>          PtrToDMusicLoader;
//typedef DXWrapper<IDirectMusicPerformance8>     PtrToDMusicPerformance;
//typedef DXWrapper<IDirectMusicSegment8>         PtrToDMusicSegment;
//typedef DXWrapper<IDirectSound3DListener8>      PtrToDMusicListener;

//-----------------------------------------------------------------------------
// container types
//-----------------------------------------------------------------------------
//typedef TextMap<PtrToDMusicSegment>             DMSegmentMap;

//-----------------------------------------------------------------------------
// class definitions
//-----------------------------------------------------------------------------
class   DirectMusic : public Singleton<DirectMusic>
{
    public:
        /* void */              DirectMusic (void);
        /* void */              ~DirectMusic (void);
        void                    PlaySound (const Text& name) const;
        
    public:
static  bool                    s_bOK;

    protected:
        //PtrToDMusicLoader       m_pDMusicLoader;
        //PtrToDMusicPerformance  m_pDmusicPerformance;
        //DMSegmentMap            m_segmentMap;
};

//-----------------------------------------------------------------------------
// macros
//-----------------------------------------------------------------------------
//#define DM_TEST(condition)  DIRECT_X_SUCCEEDED(condition, DirectMusic::s_bOK, S_OK, true)

//-----------------------------------------------------------------------------

#endif  //  _DIRECT_MUSIC_H_
