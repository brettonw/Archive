//------------------------------------------------------------------------------
//	Copyright (C) 1997-2000 Bretton Wade
//	All Rights Reserved
//------------------------------------------------------------------------------

#ifndef		_BW_SCANNER_STATE_H_
#define		_BW_SCANNER_STATE_H_

//------------------------------------------------------------------------------
//	includes
//------------------------------------------------------------------------------
#ifndef		_BW_SCANNER_STATE_INFO_H_
#include	"scanner_state_info.h"
#endif	//	_BW_SCANNER_STATE_INFO_H_

//------------------------------------------------------------------------------
//	class definitions
//------------------------------------------------------------------------------
class	ScannerState
{
	public:
		ScannerState (void);
		ScannerState (const ScannerState& scannerState);
		ScannerState&		operator = (const ScannerState& scannerState);
const	ScannerStateInfo&	operator [] (uInt index);
		void				SaveScannerState (ofstream& stream);
		void				LoadScannerState (ifstream& stream);
		bool				Set (uInt2 input, uInt2 nextState, uInt2 storage, uInt2 action);
		void				SetAll (uInt2 nextState, uInt2 storage, uInt2 action);
	public:
		const enum
		{
			DEFAULT_STATE = 0,
			INVALID_STATE = 0xffff,
			STATE_COUNT = 128
		};
	private:
		ScannerStateInfo	myStateInfo[STATE_COUNT];
};

//------------------------------------------------------------------------------

#endif	//	_BW_SCANNER_STATE_H_
