//-----------------------------------------------------------------------------
// Copyright (C) 1997-2006 Bretton Wade
// All rights reserved.
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// include files
//-----------------------------------------------------------------------------
#include    "precompile.h"
#include    "scanner_state.h"

//-----------------------------------------------------------------------------
// class methods
//-----------------------------------------------------------------------------
bool
ScannerState::Set (uInt2 input, uInt2 nextState, uInt2 storage, uInt2 action)
{
    if (m_stateInfo[input].m_nextState == INVALID_STATE)
    {
        m_stateInfo[input] (nextState, storage, action);
        return true;
    }
    return false;
}

//-----------------------------------------------------------------------------
void
ScannerState::SetAll (uInt2 nextState, uInt2 storage, uInt2 action)
{
    for (uInt2 i = 0; i < STATE_COUNT; i++)
        Set (i, nextState, storage, action);;
}

//-----------------------------------------------------------------------------
