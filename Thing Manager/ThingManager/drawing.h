//-----------------------------------------------------------------------------
// Copyright (C) 2006 Bretton Wade, All Rights Reserved
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// includes
//-----------------------------------------------------------------------------
#ifndef     _THING_MANAGER_H_
#include    "thing_manager.h"
#endif  //  _THING_MANAGER_H_

#ifndef     _VECTOR2_H_
#include    "vector2.h"
#endif  //  _VECTOR2_H_

//-----------------------------------------------------------------------------
// globals
//-----------------------------------------------------------------------------
extern float        g_zoom;
extern Vector2  g_viewCenter;

//-----------------------------------------------------------------------------
// inlines
//-----------------------------------------------------------------------------
inline
POINT
MapPoint (const Vector2& vec)
{
    // map a point in the range bottom left (-zoom, -zoom) to top right (zoom, zoom), to the range 
    // of top left (0, 0) to bottom right (WINDOW_X_SIZE, WINDOW_Y_SIZE)
    Vector2 centeredVec = vec - g_viewCenter;
    POINT       pt;
    pt.x = s_cast<LONG> (((centeredVec.x / g_zoom) + 1.0f) * (0.5f * s_cast<float> (WINDOW_X_SIZE)));
    pt.y = s_cast<LONG> ((1.0f - (centeredVec.y / g_zoom)) * (0.5f * s_cast<float> (WINDOW_Y_SIZE)));
    return pt;
}

//-----------------------------------------------------------------------------
inline
Vector2
MapPoint (const POINT& pt)
{
    // map a point in the range top left (0, 0) to bottom right (WINDOW_X_SIZE, WINDOW_Y_SIZE), to the range 
    // of bottom left (-1, -1) to top right (1, 1)
    Vector2 vec;
    vec.x = g_zoom * (((s_cast<float> (pt.x) / s_cast<float> (CORE_WINDOW_X_SIZE)) * 2.0f) - 1.0f);
    vec.y = g_zoom * (1.0f - ((s_cast<float> (pt.y) / s_cast<float> (CORE_WINDOW_X_SIZE)) * 2.0f));
    return vec + g_viewCenter;
}

//-----------------------------------------------------------------------------
// functions
//-----------------------------------------------------------------------------
void    DrawLine (HDC dc, const Vector2& A, const Vector2& B);
void    DrawArc (HDC dc, const Vector2& A, const Vector2& B);
void    DrawCircle (HDC dc, const Vector2& center, float radius);
void    DrawBox (HDC dc, const Vector2& center, const Vector2& xaxis, float width, float height);
void    DrawGrid (HPEN pen, int intervals = 5);

//-----------------------------------------------------------------------------
