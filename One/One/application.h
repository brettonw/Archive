//------------------------------------------------------------------------------
//	Copyright (C) 1997-2000 Bretton Wade
//	All Rights Reserved
//------------------------------------------------------------------------------

#ifndef		_BW_APPLICATION_H_
#define		_BW_APPLICATION_H_

//------------------------------------------------------------------------------
//	class definitions
//------------------------------------------------------------------------------
class	Application
{
	public:
static	void			Startup (HINSTANCE instance);
static	void			Shutdown (void);
static	bool			IsValid (void);
static	HINSTANCE		GetInstance (void);
	protected:
		Application (HINSTANCE instance);
		~Application (void);

    protected:
		HINSTANCE		myInstance;
static	Application*	global;
};

//------------------------------------------------------------------------------

#endif	//	_BW_APPLICATION_H_
