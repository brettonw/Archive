//------------------------------------------------------------------------------
//	Copyright (C) 1997-2000 Bretton Wade
//	All Rights Reserved
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//	includes
//------------------------------------------------------------------------------
#include	"precompile.h"
#include	"application.h"
#include	"application_body.h"

//------------------------------------------------------------------------------
//	prototypes
//------------------------------------------------------------------------------
LONG WINAPI	BW_UnhandledExceptionFilter (PEXCEPTION_POINTERS exceptionPtrs);

//------------------------------------------------------------------------------
//	functions
//------------------------------------------------------------------------------
int
MyMain
(
	HINSTANCE	instance,		//	handle to the instance
	String		commandLine		//	non internationalized command line string minus name of application
)
{
	Application::Startup (instance);
	ApplicationBody::Startup (60, commandLine);
	int		result = ApplicationBody::Execute ();
	ApplicationBody::Shutdown ();
	Application::Shutdown ();
	return result;
}

//------------------------------------------------------------------------------
int
WINAPI	WinMain
(
	HINSTANCE	instance,		//	handle to the instance
	HINSTANCE	prevInstance,	//	Always NULL in a Win32 application
	String		commandLine,	//	non internationalized command line string minus name of application
	int			show			//	how to show the window
)
{
		return MyMain (instance, commandLine);
}

//------------------------------------------------------------------------------
