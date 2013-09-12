//------------------------------------------------------------------------------
//	Copyright (C) 1997-2000 Bretton Wade
//	All Rights Reserved
//------------------------------------------------------------------------------

#ifndef	_BW_APPLICATION_BODY_H_
#define	_BW_APPLICATION_BODY_H_

//------------------------------------------------------------------------------
//	includes
//------------------------------------------------------------------------------
#ifndef		_BW_TEXT_H_
#include	"text.h"
#endif	//	_BW_TEXT_H_

//------------------------------------------------------------------------------
//	class definitions
//------------------------------------------------------------------------------
class	ApplicationBody
{
	public:
static	void				Startup (uInt pulseRate, const Text& arguments);
static	void				Shutdown (void);
static	bool				IsValid (void);
static	sInt4				Execute (void);
static	void				Quit (void);
static	LPMSG				GetCurrentMessage (void);
	private:
		ApplicationBody (uInt pulseRate, const Text& arguments);
		~ApplicationBody (void);
	private:
		uInt				mySleepTime;
		MSG					myCurrentMessage;
static	ApplicationBody*	global;
};

//------------------------------------------------------------------------------

#endif	//	_BW_APPLICATION_BODY_H_
