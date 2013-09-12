//------------------------------------------------------------------------------
//	Copyright (C) 1997-2000 Bretton Wade
//	All Rights Reserved
//------------------------------------------------------------------------------

#ifndef		_BW_HANDLE_H_
#define		_BW_HANDLE_H_

//--------------------------------------------------------------------------
//	includes
//--------------------------------------------------------------------------
#ifndef		_BW_COUNTED_OBJECT_H_
#include	"counted_object.h"
#endif	//	_BW_COUNTED_OBJECT_H_

//--------------------------------------------------------------------------
//	class definitions
//--------------------------------------------------------------------------
class	Handle : public CountedObject
{
	public:
							Handle (void* ptr);
virtual						~Handle (void);
		void*				GetPtr (void) const;
		
	public:
		void*				operator new (size_t size);
		void				operator delete (void* ptr, size_t size);
		
	private:
		void*				myPtr;
static	void*				myFreeStore;
};

//--------------------------------------------------------------------------

#endif	//	_BW_HANDLE_H_
