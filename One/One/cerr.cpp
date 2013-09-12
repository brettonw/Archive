//------------------------------------------------------------------------------
//	Copyright (C) 1997-2000 Bretton Wade
//	All Rights Reserved
//------------------------------------------------------------------------------

//--------------------------------------------------------------------------
//	includes
//--------------------------------------------------------------------------
#include	"precompile.h"
#include	"task.h"
#include	"directory.h"

//--------------------------------------------------------------------------
//	system includes
//--------------------------------------------------------------------------
#ifndef 	_INC_FSTREAM
#include	<fstream>
#endif  //  _INC_FSTREAM

//--------------------------------------------------------------------------
//	global variables
//--------------------------------------------------------------------------
#ifdef      _DEBUG
filebuf*		gStandardErrorFile = 0;
streambuf*		gOldStandardErrorFile = 0;
#endif  //  _DEBUG

//--------------------------------------------------------------------------
//	tasks
//--------------------------------------------------------------------------
DEFINE_TASK (StdErr)
{
}

//--------------------------------------------------------------------------
ON_STARTUP (StdErr)
{
    #ifdef      _DEBUG
	gStandardErrorFile = new filebuf;
	if (gStandardErrorFile)
		if (gStandardErrorFile->open (GetDataFileName ("log", "cerr.txt"), ios::out))
		{
			gOldStandardErrorFile = cerr.rdbuf (gStandardErrorFile);
			cerr << MAKEITASTRING(_BUILD_NAME) << "\nBuild " << __DATE__ << " " << __TIME__ << endl << endl;
		}
		else
		{
			delete gStandardErrorFile;
			gStandardErrorFile = 0;
		}
	#endif  //  _DEBUG
}

//--------------------------------------------------------------------------
ON_SHUTDOWN (StdErr)
{
    #ifdef      _DEBUG
	if (gStandardErrorFile)
	{
		cerr.rdbuf (gOldStandardErrorFile);
		gStandardErrorFile->close ();
		delete gStandardErrorFile;
	}
	#endif  //  _DEBUG
}

//--------------------------------------------------------------------------
