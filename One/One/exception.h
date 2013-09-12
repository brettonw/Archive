//------------------------------------------------------------------------------
//	Copyright (C) 1997-2000 Bretton Wade
//	All Rights Reserved
//------------------------------------------------------------------------------

#ifndef		_BW_EXCEPTION_H_
#define		_BW_EXCEPTION_H_

//------------------------------------------------------------------------------
//	macros
//------------------------------------------------------------------------------
#define	DEFINE_EXCEPTION(type)												    \
	struct type																    \
	{																		    \
	    String  myMessage;                                                      \
		type (void)                                                             \
		    {                                                                   \
		        cerr << "Exception: " #type << endl;                            \
		        myMessage = 0;                                                  \
		    }			                                                        \
		type (cString message)                                                  \
		    {                                                                   \
		        cerr << "Exception: " #type << " (" << message << ")" <<endl;	\
		        myMessage = new Character [strlen (message) + 1];               \
		        strcpy (myMessage, message);                                    \
		    }                                                                   \
	}

//------------------------------------------------------------------------------

#endif	//	_BW_EXCEPTION_H_
