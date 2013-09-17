//-----------------------------------------------------------------------------
// Copyright (C) 1997-2002 Bretton Wade
// All rights reserved.
//-----------------------------------------------------------------------------

#ifndef     _TASK_REGISTRAR_H_
#define     _TASK_REGISTRAR_H_

//-----------------------------------------------------------------------------
// include files
//-----------------------------------------------------------------------------
#ifndef     _SINGLETON_H_
#include    "singleton.h"
#endif  //  _SINGLETON_H_

#ifndef     _TASK_H_
#include    "task.h"
#endif  //  _TASK_H_

//-----------------------------------------------------------------------------
// class definitions
//-----------------------------------------------------------------------------
class TaskRegistrar : public Singleton <TaskRegistrar>
{
    public:
virtual /* void */              ~TaskRegistrar (void);

        void                    RegisterTask (Task* pTask);
        void                    RegisterDependency (const Text& task, const Text& dependency);

        bool                    SendMessage (Task::Message msg) const;
        bool                    SendMessageReverse (Task::Message msg) const;

static  TaskRegistrar*          GetSingleton (void);

    protected:
        DependencyGraph<Task*>  m_tasks;
};

//-----------------------------------------------------------------------------

#endif  //  _TASK_REGISTRAR_H_
