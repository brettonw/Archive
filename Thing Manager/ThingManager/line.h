//-----------------------------------------------------------------------------
// Copyright (C) 2006 Bretton Wade and Firaxis Games, All Rights Reserved
//-----------------------------------------------------------------------------

#ifndef     _LINE_H_
#define     _LINE_H_

//-----------------------------------------------------------------------------
// includes
//-----------------------------------------------------------------------------
#ifndef     _OBJECT_TYPE_H_
#include    "object_type.h"
#endif  //  _OBJECT_TYPE_H_

//-----------------------------------------------------------------------------
// class definition
//-----------------------------------------------------------------------------
class Line 
{
    public:
        int             iA;
        int             iB;
        short           object_type;
        float           length;
        
    public:
        /* void */      Line (int _iA, int _iB, short _type = OT_DEFAULT, float _length = 0.0f) : 
                            iA (_iA), 
                            iB (_iB), 
                            object_type (_type),
                            length (_length) {}
        void            CalculateTangentNormals (D3DXVECTOR2& normal_left, D3DXVECTOR2& normal_right) const;
};

//-----------------------------------------------------------------------------
// globals
//-----------------------------------------------------------------------------
extern vector<Line>     g_line_list;
extern int              g_line_bar;

//-----------------------------------------------------------------------------

#endif  //  _LINE_H_
