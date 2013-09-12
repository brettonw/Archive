//------------------------------------------------------------------------------
//	Copyright (C) 1997-2000 Bretton Wade
//	All Rights Reserved
//------------------------------------------------------------------------------

#ifndef		_BW_RESIZABLE_VECTOR_OF_H_
#define		_BW_RESIZABLE_VECTOR_OF_H_

//--------------------------------------------------------------------------
//	class definitions
//--------------------------------------------------------------------------
template	<class aType>
class	ResizableVectorOf
{
	public:
						ResizableVectorOf (void);
						ResizableVectorOf (uInt size);
virtual					~ResizableVectorOf (void);
		aType&			operator [] (uInt index) const;
		uInt			GetSize (void) const;
		void			Resize (void);
		
	private:
		uInt			mySize;
		aType*			myArray;
};

//--------------------------------------------------------------------------
//	template methods
//--------------------------------------------------------------------------
template	<class aType>
ResizableVectorOf<aType>::ResizableVectorOf (void)
{
	mySize = 1;
	myArray = new aType[mySize];
}

//--------------------------------------------------------------------------
template	<class aType>
ResizableVectorOf<aType>::ResizableVectorOf (uInt size)
{
	mySize = 1;
	while (mySize < size)
		size <<= 1;
	myArray = new aType[mySize];
}

//--------------------------------------------------------------------------
template	<class aType>
ResizableVectorOf<aType>::~ResizableVectorOf (void)
{
	delete[] myArray;
}

//--------------------------------------------------------------------------
template	<class aType>
aType&	ResizableVectorOf<aType>::operator [] (uInt4 index) const
{
	while (index >= mySize)
		(const_cast<ResizableVectorOf<aType>*> (this))->Resize ();
	return myArray[index];
}

//--------------------------------------------------------------------------
template	<class aType>
uInt	ResizableVectorOf<aType>::GetSize (void) const
{
	return mySize;
}

//--------------------------------------------------------------------------
template	<class aType>
void	ResizableVectorOf<aType>::Resize (void)
{
	uInt	oldSize = mySize;
	aType*	tmpArray = myArray;
	mySize *= 2;
	myArray = new aType[mySize];
	for (uInt i = 0; i < oldSize; i++)
		myArray[i] = tmpArray[i];
	delete[] tmpArray;
}

//--------------------------------------------------------------------------

#endif	//	_BW_RESIZABLE_VECTOR_OF_H_
