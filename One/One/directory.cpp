//------------------------------------------------------------------------------
//	Copyright (C) 1997-2000 Bretton Wade
//	All Rights Reserved
//------------------------------------------------------------------------------

//--------------------------------------------------------------------------
//	includes
//--------------------------------------------------------------------------
#include	"precompile.h"
#include	"directory.h"

//--------------------------------------------------------------------------
//	functions
//--------------------------------------------------------------------------
Text
GetDataFileName
(
	const Text& 		subdirectory,
	const Text& 		filename
)
{
	return GetExecutableDirectory () + subdirectory + "\\" + filename;
}

//--------------------------------------------------------------------------
Text
GetExecutableDirectory
(
	void
)
{
	sInt1	executable[256];
	uInt	length = GetModuleFileName (0, executable, 256);
	assert (length > 0);
	while (executable[length - 1] != '\\')
		length--;
	executable[length] = 0;
	return Text (executable);
}

//--------------------------------------------------------------------------
