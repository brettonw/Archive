//-----------------------------------------------------------------------------
// Copyright (C) 1997-2002 Bretton Wade
// All rights reserved.
//-----------------------------------------------------------------------------

#ifndef     _TASK_H_
#define     _TASK_H_

//-----------------------------------------------------------------------------
// include files
//-----------------------------------------------------------------------------
#ifndef     _DEPENDENCY_GRAPH_H_
#include    "dependency_graph.h"
#endif  //  _DEPENDENCY_GRAPH_H_

#ifndef     _FORCE_LOAD_H_
#include    "force_load.h"
#endif  //  _FORCE_LOAD_H_

//-----------------------------------------------------------------------------
// class definitions
//-----------------------------------------------------------------------------
class Task
{
    public:
        enum Message {STARTUP, SHUTDOWN};
        
    public:
        /* void */          Task (const Text& name);
virtual /* void */          ~Task (void);

        const Text&         GetName (void) const;
        void                DoAfter (const Text& dependency) const;

virtual bool                HandleMessage (Message msg) const = 0;

    private:
        Text                m_name;
};

//-----------------------------------------------------------------------------
// macros
//-----------------------------------------------------------------------------
#define     DEFINE_TASK(name)                                                   \
    class    Task_ ## name : public Task                                        \
    {                                                                           \
        public:                                                                 \
    virtual /* void */      ~Task_ ## name (void) {}                            \
    virtual bool            HandleMessage (Task::Message msg) const;            \
        private:                                                                \
            /* void */      Task_ ## name (void);                               \
        private:                                                                \
    static    Task_ ## name s_global;                                           \
    };                                                                          \
    Task_ ## name   Task_ ## name::s_global;                                    \
    /* void */ Task_ ## name::Task_ ## name (void) : Task (#name)

#define    DO_AFTER(name)                                                       \
    DoAfter (#name)

#define    HANDLE_MESSAGE(name, messageName)                                    \
    bool    Task_ ## name::HandleMessage (Task::Message messageName) const

//-----------------------------------------------------------------------------
// inlines
//-----------------------------------------------------------------------------
#ifndef     _TASK_INL_
#include    "task.inl"
#endif  //  _TASK_INL_

//-----------------------------------------------------------------------------

#endif  //  _TASK_H_
