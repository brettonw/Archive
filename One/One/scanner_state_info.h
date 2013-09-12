//------------------------------------------------------------------------------
//	Copyright (C) 1997-2000 Bretton Wade
//	All Rights Reserved
//------------------------------------------------------------------------------

#ifndef		_BW_SCANNER_STATE_INFO_H_
#define		_BW_SCANNER_STATE_INFO_H_

//------------------------------------------------------------------------------
//	class definitions
//------------------------------------------------------------------------------
class	ScannerStateInfo
{
	public:
		ScannerStateInfo (void);
		ScannerStateInfo (uInt2 nextState, uInt2 storage, uInt2 action);
		void			operator () (uInt2 nextState, uInt2 storage, uInt2 action);
	public:
		uInt2			myNextState;
		uInt2			myStorage:2;
		uInt2			myAction:14;
};

//------------------------------------------------------------------------------

#endif	//	_BW_SCANNER_STATE_INFO_H_
