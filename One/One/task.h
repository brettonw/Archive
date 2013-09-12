//------------------------------------------------------------------------------
//	Copyright (C) 1997-2000 Bretton Wade
//	All Rights Reserved
//------------------------------------------------------------------------------

#ifndef		_BW_TASK_H_
#define		_BW_TASK_H_

//--------------------------------------------------------------------------
//	includes
//--------------------------------------------------------------------------
#ifndef		_BW_GRAPH_OF_H_
#include	"graph_of.h"
#endif	//	_BW_GRAPH_OF_H_

//--------------------------------------------------------------------------
//	class definitions
//--------------------------------------------------------------------------
class	Task
{
	public:
							Task (const Text& name);
virtual						~Task (void);
virtual	void				Setup (GraphOf<Task*>* graph) const = 0;
virtual	void				Startup (void) const;
virtual	void				Shutdown (void) const;
		const Text&			GetName (void) const;
    	void                DoAfter (GraphOf<Task*>* graph, const Text& name) const;
	private:
		Text				myName;
};

//--------------------------------------------------------------------------
//	macros
//--------------------------------------------------------------------------
#define	DEFINE_TASK(name)													    \
	class	Task_ ## name : public Task										    \
	{																		    \
		public:																    \
	virtual					~Task_ ## name (void);							    \
	virtual	void			Setup (GraphOf<Task*>* graph) const;			    \
	virtual	void			Startup (void) const;							    \
	virtual	void			Shutdown (void) const;							    \
		private:															    \
							Task_ ## name (void);							    \
		private:															    \
	static	Task_ ## name	global;										    	\
	};																		    \
	Task_ ## name	Task_ ## name::global;									    \
	Task_ ## name::Task_ ## name (void) : Task (#name) {}					    \
	Task_ ## name::~Task_ ## name (void) {}									    \
	void	Task_ ## name::Setup (GraphOf<Task*>* graph) const

#define	DO_AFTER(name)														    \
	DoAfter (graph, #name)

#define	ON_STARTUP(name)													    \
	void	Task_ ## name::Startup (void) const

#define	ON_SHUTDOWN(name)													    \
	void	Task_ ## name::Shutdown (void) const

//--------------------------------------------------------------------------

#endif	//	_BW_TASK_H_
