//-----------------------------------------------------------------------------
// Copyright (C) 1997-2006 Bretton Wade
// All Rights Reserved
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// include files
//-----------------------------------------------------------------------------
#include    "precompile.h"
#include    "direct_music.h"
#include    "window.h"
#include    "task.h"

//-----------------------------------------------------------------------------
// force this module to be loaded
//-----------------------------------------------------------------------------
DEFINE_FORCED_LOAD (DirectMusic);

//-----------------------------------------------------------------------------
// tasks
//-----------------------------------------------------------------------------
DEFINE_TASK (DirectMusic)
{
    DO_AFTER (Window);
}

//-----------------------------------------------------------------------------
HANDLE_MESSAGE (DirectMusic, msg)
{
    switch (msg)
    {
        case Task::STARTUP:
            NewCall DirectMusic;
            return true;
            
        case Task::SHUTDOWN:
            delete DirectMusic::GetSingleton ();
            return true;
            
        default:
            return false;
    }
}

//-----------------------------------------------------------------------------
// static class variables
//-----------------------------------------------------------------------------
bool                DirectMusic::s_bOK = true;

//-----------------------------------------------------------------------------
// class methods
//-----------------------------------------------------------------------------
/* void */
DirectMusic::DirectMusic (void)
{
/*
	// use COM to create a music loader and performance
	DM_TEST (CoCreateInstance (CLSID_DirectMusicLoader, 0, CLSCTX_INPROC, IID_IDirectMusicLoader8, reinterpret_cast<void**> (&m_pDMusicLoader)));
	DM_TEST (m_pDMusicLoader->SetSearchDirectory (GUID_DirectMusicAllTypes, L"Sound", TRUE));
	DM_TEST (CoCreateInstance (CLSID_DirectMusicPerformance, 0, CLSCTX_INPROC, IID_IDirectMusicPerformance8, reinterpret_cast<void**> (&m_pDmusicPerformance)));

    // initialize the direct x audio engine
	DM_TEST (m_pDmusicPerformance->InitAudio (0, 0, g_windowHandle, DMUS_APATH_SHARED_STEREOPLUSREVERB, 64, DMUS_AUDIOF_ALL, 0));
	
    // load the xml configuration file
    PtrToXMLNode    pXMLNode = XMLNode::FromFile ("DirectSound.xml");

	// check to see if the xml is valid
	if (pXMLNode)
	{
	    // fetch the list of sounds from the xml and check for validity
	    const XMLNodeList*          pSoundNodeList = pXMLNode->GetChildren ("Sound");
	    if (pSoundNodeList)
	    {
	        // loop over the sound list
	        XMLNodeList::const_iterator iter = pSoundNodeList->begin ();
	        XMLNodeList::const_iterator end = pSoundNodeList->end ();
	        while (iter != end)
	        {
	            // extract the current node and its parameters
	            PtrToXMLNode        pSoundNode = *iter++;
	            Text                name = pSoundNode->GetAttribute ("name");
	            Text                filename = pSoundNode->GetAttribute ("filename");
        	    
	            // convert the filename to a wide char version because direct
	            // music wants unicode filenames
	            WideCharacter       wBuffer[_MAX_PATH];
	            MultiByteToWideChar (CP_ACP, 0, filename, filename.Length () + 1, wBuffer, _MAX_PATH);
        	        
	            // create a segment for the named sound
	            PtrToDMusicSegment  pSegment;
	            DM_TEST (m_pDMusicLoader->LoadObjectFromFile (CLSID_DirectMusicSegment, IID_IDirectMusicSegment8, wBuffer, reinterpret_cast<void**> (&pSegment)));
	            DM_TEST (pSegment->Download (m_pDmusicPerformance));

                // insert the segment into our segment table
	            Assert (m_segmentMap.find (name) == m_segmentMap.end ());
	            m_segmentMap[name] = pSegment;
	        }
	    }
	}
	*/
}

//-----------------------------------------------------------------------------
/* void */
DirectMusic::~DirectMusic (void)
{
    /*
    // stop and close down the performance
    DM_TEST (m_pDmusicPerformance->Stop (0, 0, 0, 0));
    DM_TEST (m_pDmusicPerformance->CloseDown ());

    // release all of the segments
    m_segmentMap.clear ();
    */
}

//-----------------------------------------------------------------------------
void
DirectMusic::PlaySound (const Text& name) const
{
    /*
    // tell the requested segment to play itself
    DMSegmentMap::const_iterator    iter = m_segmentMap.find (name);
    Assert (iter != m_segmentMap.end ());
    if (iter != m_segmentMap.end ())
	    DM_TEST (m_pDmusicPerformance->PlaySegment (iter->second, 0, 0, 0));
	*/
}

//-----------------------------------------------------------------------------
