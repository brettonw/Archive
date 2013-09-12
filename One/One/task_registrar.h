//------------------------------------------------------------------------------
//	Copyright (C) 1997-2000 Bretton Wade
//	All Rights Reserved
//------------------------------------------------------------------------------

#ifndef		_BW_TASK_REGISTRAR_H_
#define		_BW_TASK_REGISTRAR_H_

//--------------------------------------------------------------------------
//	includes
//--------------------------------------------------------------------------
#ifndef		_BW_TASK_H_
#include	"task.h"
#endif	//	_BW_TASK_H_

//--------------------------------------------------------------------------
//	class definitions
//--------------------------------------------------------------------------
class	TaskRegistrar
{
	public:
static	void			RegisterTask (Task* task);
static	void			RunStartupTasks (void);
static	void			RunShutdownTasks (void);
	protected:
static	GraphOf<Task*>*	myTasks;
};

//--------------------------------------------------------------------------

#endif	//	_BW_TASK_REGISTRAR_H_
