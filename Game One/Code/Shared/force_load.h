//-----------------------------------------------------------------------------
// Copyright (C) 2001 Microsoft Corp.
// All rights reserved.
//-----------------------------------------------------------------------------

#ifndef     _FORCE_LOAD_H_
#define     _FORCE_LOAD_H_

//-----------------------------------------------------------------------------
// macros
//-----------------------------------------------------------------------------
#define DECLARE_FORCED_LOAD(name)					    \
	cString Force_Load_ ## name (void);                 \
        Force_Load_ ## name ()

#define DEFINE_FORCED_LOAD(name)					    \
	cString Force_Load_ ## name (void) {return #name;}

//-----------------------------------------------------------------------------

#endif  //  _FORCE_LOAD_H_
