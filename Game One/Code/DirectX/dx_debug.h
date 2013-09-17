//-----------------------------------------------------------------------------
// Copyright (C) 1997-2006 Bretton Wade
// All Rights Reserved
//-----------------------------------------------------------------------------

#ifndef     _DX_DEBUG_H_
#define     _DX_DEBUG_H_

//-----------------------------------------------------------------------------
// include files
//-----------------------------------------------------------------------------
#ifndef     _DXERR_H_
#include    <dxerr.h>
#endif  //  _DXERR_H_

//-----------------------------------------------------------------------------
// debugging macros
//-----------------------------------------------------------------------------
#ifndef     NDEBUG
#define     DIRECT_X_SUCCEEDED(condition, successFlag, successValue, ask)       \
{                                                                               \
    HRESULT result = condition;                                                 \
    successFlag = (result == successValue);                                     \
    if (not successFlag)                                                        \
    {                                                                           \
        const char* cond = "DIRECT X FAILURE\n\t" #condition "\n\tRESULT = ";   \
        const char* error = DXGetErrorString (result);							\
        const char* file = __FILE__;                                            \
        DebugPrint ("%s(%d): %s%s\n", file, __LINE__, cond, error);             \
        extern char g_dbgBuffer[];                                              \
        sprintf (g_dbgBuffer, "%s%s\nWould you like to debug?", cond, error);   \
        if (ask)                                                                \
        {                                                                       \
            if (IDYES == MessageBox (0, g_dbgBuffer, "Direct X Fail",           \
                MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON1 | MB_TASKMODAL))     \
            {                                                                   \
                __asm { int 3 };                                                \
            }                                                                   \
        }                                                                       \
        else                                                                    \
        {                                                                       \
            __asm { int 3 };                                                    \
        }                                                                       \
    }                                                                           \
}
#else   //  NDEBUG
#define     DIRECT_X_SUCCEEDED(condition, successFlag, successValue, ask)       \
    successFlag = (condition == successValue)
#endif   //  NDEBUG

//-----------------------------------------------------------------------------

#endif  //  _DX_DEBUG_H_
