//-----------------------------------------------------------------------------
// Copyright (C) 2006 Bretton Wade, All Rights Reserved
//-----------------------------------------------------------------------------

#include "precompile.h"
#include "drawing.h"
#include "thing_manager.h"

//-----------------------------------------------------------------------------
// globals
//-----------------------------------------------------------------------------
float           g_zoom = 2.0f;
Vector2     g_viewCenter (0.0f, 0.0f);

//-----------------------------------------------------------------------------
// functions
//-----------------------------------------------------------------------------
void
DrawLine (HDC dc, const Vector2& A, const Vector2& B)
{
    POINT   ptA = MapPoint (A);
    POINT   ptB = MapPoint (B);
	MoveToEx (dc, ptA.x, ptA.y, 0);
	LineTo (dc, ptB.x, ptB.y);
}

//-----------------------------------------------------------------------------
void    
DrawArc (HDC dc, const Vector2& A, const Vector2& B)
{
    DrawLine (dc, A, B);
    return;
    
    // n points on a quarter circle arc
    Vector2			delta = B - A;
    float           length = delta.Length ();
	delta = delta * (1.0f / length);
    Vector2			ortho (delta.y, -delta.x);
    float           halfLength = length / 2;
    Vector2			center = A + (halfLength * delta) + (-halfLength * ortho);
    float           radius = sqrtf (2 * halfLength * halfLength) ;
    float           angle = atan2f (-delta.y, -delta.x) + (float (M_PI) / 4) + (float (M_PI) / 32);
    float           deltaAngle = ((float (M_PI) / 2) - (float (M_PI) / 16)) / 8;
    Vector2			first = center + (radius * Vector2 (cosf (angle), sinf (angle)));
    POINT           ptA = MapPoint (first);

	MoveToEx (dc, ptA.x, ptA.y, 0);
    for (Word i = 0; i < 8; ++i)
    {
        angle += deltaAngle;
        Vector2     second = center + (radius * Vector2 (cosf (angle), sinf (angle)));
        POINT           ptB = MapPoint (second);
	    LineTo (dc, ptB.x, ptB.y);
        ptA = ptB;
    }
}

//-----------------------------------------------------------------------------
void
DrawCircle (HDC dc, const Vector2& center, float radius)
{
    const float     two_pi = M_PI * 2.0f;
    float           circ = radius * two_pi;
    int             segments = max (s_cast<int> (circ / 0.025f), 8);
    float           angle = 0;
    float           deltaAngle = two_pi / s_cast<float> (segments);
    Vector2     A = center + (radius * Vector2 (cosf (angle), sinf (angle)));
    POINT           ptA = MapPoint (A);

	MoveToEx (dc, ptA.x, ptA.y, 0);
    for (int i = 0; i < segments; ++i)
    {
        angle += deltaAngle;
        Vector2     B = center + (radius * Vector2 (cosf (angle), sinf (angle)));
        POINT           ptB = MapPoint (B);
	    LineTo (dc, ptB.x, ptB.y);
        ptA = ptB;
    }
}

//-----------------------------------------------------------------------------
void    
DrawBox (HDC dc, const Vector2& center, const Vector2& xaxis, float width, float height)
{
    Vector2 yaxis (-xaxis.y, xaxis.x);
    Vector2 vecA = center + ( width * xaxis) + ( height * yaxis);
    Vector2 vecB = center + (-width * xaxis) + ( height * yaxis);// + Vector2 (-width * axis.x,  height * axis.y);
    Vector2 vecC = center + (-width * xaxis) + (-height * yaxis);// + Vector2 (-width * axis.x, -height * axis.y);
    Vector2 vecD = center + ( width * xaxis) + (-height * yaxis);// + Vector2 ( width * axis.x, -height * axis.y);
    POINT   ptA = MapPoint (vecA);
    POINT   ptB = MapPoint (vecB);
    POINT   ptC = MapPoint (vecC);
    POINT   ptD = MapPoint (vecD);
    MoveToEx (dc, ptA.x, ptA.y, 0);
    LineTo (dc, ptB.x, ptB.y);
    LineTo (dc, ptC.x, ptC.y);
    LineTo (dc, ptD.x, ptD.y);
    LineTo (dc, ptA.x, ptA.y);
}

//-----------------------------------------------------------------------------
void
DrawGrid (HPEN pen, int intervals)
{
	// set the pen
	HPEN	old_pen = s_cast<HPEN> (SelectObject (g_offscreenDC, pen));
	
	float   multiplier = (1.0f / s_cast<float> (intervals));
			
	//  calculate start so that view center changes are properly reflected in the grid display
	int     start = s_cast<int> (((g_viewCenter.x + (fmodf (g_viewCenter.x - g_zoom, multiplier))) - g_zoom) * intervals);
    int     end = start + (2 * s_cast<int> (g_zoom) * intervals) + 1;
    for (int i = start; i < end; i++)
    {
        float   fi = i * multiplier;
        Vector2 xmin (fi, g_viewCenter.y - g_zoom);
        Vector2 xmax (fi, g_viewCenter.y + g_zoom);
        DrawLine (g_offscreenDC, xmin, xmax);
    }

	start = s_cast<int> (((g_viewCenter.y + (fmodf (g_viewCenter.y - g_zoom, multiplier))) - g_zoom) * intervals);
    end = start + (2 * s_cast<int> (g_zoom) * intervals) + 1;
    for (int i = start; i < end; i++)
    {
        float   fi = i * multiplier;
        Vector2 ymin (g_viewCenter.x - g_zoom, fi);
        Vector2 ymax (g_viewCenter.x + g_zoom, fi);
        DrawLine (g_offscreenDC, ymin, ymax);
    }
	
	// restore the pen
	SelectObject (g_offscreenDC, old_pen);
}

//-----------------------------------------------------------------------------
