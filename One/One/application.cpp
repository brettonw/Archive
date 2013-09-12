//------------------------------------------------------------------------------
//	Copyright (C) 1997-2000 Bretton Wade
//	All Rights Reserved
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//	includes
//------------------------------------------------------------------------------
#include	"precompile.h"
#include	"application.h"
#include	"task_registrar.h"
#include	"test_registrar.h"

//------------------------------------------------------------------------------
//	static members
//------------------------------------------------------------------------------
Application			*Application::global = 0;

//------------------------------------------------------------------------------
//	class methods
//------------------------------------------------------------------------------
void
Application::Startup
(
	HINSTANCE		instance
)
{
	assert (global == 0);
	new Application (instance);
}

//------------------------------------------------------------------------------
void
Application::Shutdown
(
	void
)
{
	assert (global);
	delete global;
	global = 0;
}

//------------------------------------------------------------------------------
bool
Application::IsValid
(
	void
)
{
	return global ? true : false;
}

//------------------------------------------------------------------------------
HINSTANCE
Application::GetInstance
(
	void
)
{
	assert (global);
	return global->myInstance;
}

//------------------------------------------------------------------------------
Application::Application
(
	HINSTANCE		instance
)
{
	global = this;
	myInstance = instance;
	TaskRegistrar::RunStartupTasks ();
	TestRegistrar::RunTests ();
}

//------------------------------------------------------------------------------
Application::~Application
(
	void
)
{
	TaskRegistrar::RunShutdownTasks ();
	global = 0;
}

//------------------------------------------------------------------------------
