//------------------------------------------------------------------------------
//	Copyright (C) 1997-2000 Bretton Wade
//	All Rights Reserved
//------------------------------------------------------------------------------

#ifndef		_BW_SCANNER_ACTION_H_
#define		_BW_SCANNER_ACTION_H_

//------------------------------------------------------------------------------
//	class definitions
//------------------------------------------------------------------------------
class	ScannerAction
{
	public:
virtual	~ScannerAction (void);
virtual	void					Action (cString buffer, uInt length, uInt2 action, cString nextBuffer, uInt2 nextState) = 0;
	public:
		const enum
		{
			NO_ACTION = 0,					//	perform no action on an input state
		};
};

//------------------------------------------------------------------------------

#endif	//	_BW_SCANNER_ACTION_H_
