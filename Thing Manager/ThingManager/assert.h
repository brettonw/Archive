//-----------------------------------------------------------------------------
// Copyright (C) 1997-2006 Bretton Wade
// All Rights Reserved
//-----------------------------------------------------------------------------

#ifndef     _ASSERT_H
#define     _ASSERT_H

//-----------------------------------------------------------------------------
// debugging macros
//-----------------------------------------------------------------------------
#ifndef NDEBUG
    #define Assert(condition)                                                   \
    {                                                                           \
        if (not (condition))                                                    \
        {                                                                       \
            if (IDYES == MessageBox (0, "ASSERT FAILURE: " #condition           \
                "\nWould you like to debug?\n", "Assert Failure",               \
                MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON1 | MB_TASKMODAL))     \
            __asm {int 3};                                                      \
        }                                                                       \
    }
#else
    #define Assert(condition)
#endif

//-----------------------------------------------------------------------------

#endif  //  _ASSERT_H
