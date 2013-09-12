//------------------------------------------------------------------------------
//	Copyright (C) 1997-2000 Bretton Wade
//	All Rights Reserved
//------------------------------------------------------------------------------

#ifndef		_BW_STACK_OF_H_
#define		_BW_STACK_OF_H_

//--------------------------------------------------------------------------
//	includes
//--------------------------------------------------------------------------
#ifndef		_BW_RESIZABLE_VECTOR_OF_H_
#include	"resizable_vector_of.h"
#endif	//	_BW_RESIZABLE_VECTOR_OF_H_

#ifndef		_BW_EXCEPTION_H_
#include	"exception.h"
#endif	//	_BW_EXCEPTION_H_

//--------------------------------------------------------------------------
//	exception definitions
//--------------------------------------------------------------------------
DEFINE_EXCEPTION (StackUnderflow);

//--------------------------------------------------------------------------
//	class definitions
//--------------------------------------------------------------------------
template	<class aType>
class	StackOf
{
	public:
									StackOf (void);
		void						Push (const aType& item);
		aType&						Pop (void);
		aType&						Get (uInt indexFromTop = 0) const;
		uInt						GetSize (void) const;
	private:
		ResizableVectorOf<aType>	myStack;
		uInt						myIndex;
};

//--------------------------------------------------------------------------
//	template methods
//--------------------------------------------------------------------------
template	<class aType>
StackOf<aType>::StackOf (void)
{	
	myIndex = 0;
}

//--------------------------------------------------------------------------
template	<class aType>
void	StackOf<aType>::Push (const aType& item)
{
	myStack[myIndex++] = item;
}

//--------------------------------------------------------------------------
template	<class aType>
aType&	StackOf<aType>::Pop (void)
{
	if (myIndex > 0)
		return myStack[--myIndex];
	throw StackUnderflow ();
}

//--------------------------------------------------------------------------
template	<class aType>
aType&	StackOf<aType>::Get (uInt indexFromTop) const
{
	sInt	index = static_cast<sInt> (myIndex) - static_cast<sInt> (indexFromTop + 1);
	if (index >= 0)
		return myStack[static_cast<uInt> (index)];
	throw StackUnderflow ();
}

//--------------------------------------------------------------------------
template	<class aType>
uInt	StackOf<aType>::GetSize (void) const
{
	return myIndex;
}

//--------------------------------------------------------------------------

#endif	//	_BW_STACK_OF_H_
