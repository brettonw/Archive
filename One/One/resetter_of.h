//------------------------------------------------------------------------------
//	Copyright (C) 1997-2000 Bretton Wade
//	All Rights Reserved
//------------------------------------------------------------------------------

#ifndef		_BW_RESETTER_OF_H_
#define		_BW_RESETTER_OF_H_

//--------------------------------------------------------------------------
//	class definitions
//--------------------------------------------------------------------------
template	<class aType>
class	ResetterOf
{
	public:
							ResetterOf (aType& target);
							ResetterOf (aType* target);
							~ResetterOf (void);
		void				Cancel (void);
	protected:
		aType*				myTarget;
		aType				myBackup;
};

//--------------------------------------------------------------------------
//	template methods
//--------------------------------------------------------------------------
template	<class aType>
ResetterOf<aType>::ResetterOf (aType& target) : myBackup (target)
{
	myTarget = &target;
}

//--------------------------------------------------------------------------
template	<class aType>
ResetterOf<aType>::ResetterOf (aType* target) : myBackup (*target)
{
	assert (target);
	myTarget = target;
}

//--------------------------------------------------------------------------
template	<class aType>
ResetterOf<aType>::~ResetterOf (void)
{
	if (myTarget)
		*myTarget = myBackup;
}

//--------------------------------------------------------------------------
template	<class aType>
void	ResetterOf<aType>::Cancel (void)
{
	myTarget = 0;
}

//--------------------------------------------------------------------------

#endif	//	_BW_RESETTER_OF_H_
