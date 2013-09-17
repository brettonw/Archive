// ThingManager.cpp : Defines the entry point for the application.
//

#include    "precompile.h"
#include    "tools.h"
#include    "solver.h"
#include    "drawing.h"
#include    "constants.h"

//-----------------------------------------------------------------------------
// globals
//-----------------------------------------------------------------------------
ToolState       g_toolState = TS_CIRCLE;
int             g_isDown = 0;

//-----------------------------------------------------------------------------
// functions
//-----------------------------------------------------------------------------
void
DrawToolbar (void)
{
    RECT    rect = {0, WINDOW_Y_SIZE, WINDOW_X_SIZE, WINDOW_Y_SIZE + TOOLBAR_SIZE};
	FillRect (g_offscreenDC, &rect, s_cast<HBRUSH> (GetStockObject (LTGRAY_BRUSH)));
    FrameRect (g_offscreenDC, &rect, s_cast<HBRUSH> (GetStockObject (BLACK_BRUSH)));
    
    char*   text;
    if (g_toolState == TS_CIRCLE)
        text = "Draw Circle";
	else if (g_toolState == TS_BOX)
        text = "Draw Box";
	else if (g_toolState == TS_LINE)
        text = "Draw Line";
    SetBkColor (g_offscreenDC, 0x00c0c0c0);
    DrawText (g_offscreenDC, text, s_cast<int> (strlen (text)), &rect, DT_VCENTER bit_or DT_SINGLELINE bit_or DT_CENTER);
}

//-----------------------------------------------------------------------------
bool
ToolbarProc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    RECT    toolbarRect = {0, CORE_WINDOW_Y_SIZE, CORE_WINDOW_X_SIZE, CORE_WINDOW_Y_SIZE + CORE_TOOLBAR_SIZE};
	POINT   pt;
	pt.x = GET_X_LPARAM  (lParam);
	pt.y = GET_Y_LPARAM  (lParam);
	if (PtInRect (&toolbarRect, pt))
	    switch (message) 
	    {
		    case WM_RBUTTONDOWN:
		        {
		            // clear the llast addition if one is stored, else clear the whole list

		            // if this was a full clear, reset the view
			        {
			            g_viewCenter = Vector2 (0.0f, 0.0f);
			            g_zoom = 2.0f;
			        }
		        }
		        return true;
    		    
		    case WM_LBUTTONDOWN:
		        {
		            if (g_toolState == TS_CIRCLE)
		                g_toolState = TS_BOX;
		            else if (g_toolState == TS_BOX)
		                g_toolState = TS_LINE;
		            else if (g_toolState == TS_LINE)
		                g_toolState = TS_CIRCLE;
		        }
		        return true;
        }
    return false;
}

//-----------------------------------------------------------------------------
bool
DragViewProc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static Vector2  first;
    static Vector2  lastViewCenter;
    RECT                toolbarRect = {0, CORE_WINDOW_Y_SIZE, CORE_WINDOW_X_SIZE, CORE_WINDOW_Y_SIZE + CORE_TOOLBAR_SIZE};
	switch (message) 
	{
		case WM_RBUTTONDOWN:
		    {
		        POINT   pt;
		        pt.x = GET_X_LPARAM  (lParam);
		        pt.y = GET_Y_LPARAM  (lParam);
                if (not g_isDown)
		        {
		            g_isDown = WM_RBUTTONDOWN;
		            lastViewCenter = g_viewCenter;
  		            first = MapPoint (pt);
  		            SetCapture (hWnd);
  		            return true;
		        }
		    }
		    break;
		    
		case WM_MOUSEMOVE:
		    {
		        if (g_bIsDrawn and (g_isDown == WM_RBUTTONDOWN))
		        {
		            POINT       pt;
		            pt.x = GET_X_LPARAM  (lParam);
		            pt.y = GET_Y_LPARAM  (lParam);
		            g_viewCenter = lastViewCenter;
		            Vector2 second = MapPoint (pt);
		            g_viewCenter = lastViewCenter - (second - first);
	                DrawEverything ();
	                return true;
		        }
		    }
		    break;
		    
		case WM_RBUTTONUP:
		    {
		        if (g_isDown == WM_RBUTTONDOWN)
		        {
		            g_isDown = 0;
		            ReleaseCapture ();
		            return true;
		        }
		    }
		    break;
		    
		case WM_MOUSEWHEEL:
		    {
		        int zoom = GET_WHEEL_DELTA_WPARAM (wParam);
		        if (zoom < 0)
		            g_zoom = min (g_zoom * 2, 16);
		        else
		            g_zoom = max (g_zoom / 2, 1);
		    }
		    return true;
		    
    }
    return false;
}
//-----------------------------------------------------------------------------
bool
CircleToolProc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static Vector2  first;
    static float        width = c_fWidth;

	switch (message) 
	{
		case WM_LBUTTONDOWN:
		    {
		        POINT   pt;
		        pt.x = GET_X_LPARAM  (lParam);
		        pt.y = GET_Y_LPARAM  (lParam);
		        if (not g_isDown)
		        {
		            g_isDown = WM_LBUTTONDOWN;
		            g_bIsDrag = true;
		            first = MapPoint (pt);
  		            SetCapture (hWnd);
    	            DrawEverything ();
	                HPEN	old_pen = s_cast<HPEN> (SelectObject (g_offscreenDC, GetStockObject (DC_PEN)));
	                SetDCPenColor (g_offscreenDC, 0x00aaaaaa);
		            DrawCircle (g_offscreenDC, first, c_fWidth);
	                SelectObject (g_offscreenDC, old_pen);
		        }
		    }
		    return true;
		    
		case WM_MOUSEMOVE:
		    if (g_bIsDrawn and (g_isDown == WM_LBUTTONDOWN))
		    {
		        POINT   pt;
		        pt.x = GET_X_LPARAM  (lParam);
		        pt.y = GET_Y_LPARAM  (lParam);
		        Vector2 second = MapPoint (pt);
		        Vector2 delta = second - first;

	            DrawEverything ();
		        width = delta.Length ();// (&delta);
	            HPEN	old_pen = s_cast<HPEN> (SelectObject (g_offscreenDC, GetStockObject (DC_PEN)));
	            SetDCPenColor (g_offscreenDC, 0x00aaaaaa);
		        if (width > c_fMinWidth)
		            DrawCircle (g_offscreenDC, first, width);
		        else
		            DrawCircle (g_offscreenDC, first, c_fWidth);
	            SelectObject (g_offscreenDC, old_pen);
		        return true;
		    }
		    break;
		    
		case WM_LBUTTONUP:
		    if (g_isDown == WM_LBUTTONDOWN)
		    {
		        g_isDown = 0;
			    g_bIsDrag = false;
			    
                if (width < c_fMinWidth)
			        width = c_fWidth;
			        
			    Thing*  pThing = new Thing (first, width, (wParam bit_and MK_CONTROL) ? OT_FIXED_POSITION : OT_FREE);
                if (pThing)
                    g_solver.AddThing (pThing);
		     
	            width = c_fWidth;
		        ReleaseCapture ();
			}
		    break;
    }
    return false;
}
//-----------------------------------------------------------------------------
bool
BoxToolProc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static Vector2  first;
    static float        width = c_fWidth;
    static float        height = c_fWidth;

	switch (message) 
	{
		case WM_LBUTTONDOWN:
		    {
		        POINT   pt;
		        pt.x = GET_X_LPARAM  (lParam);
		        pt.y = GET_Y_LPARAM  (lParam);
		        if (not g_isDown)
		        {
		            g_isDown = WM_LBUTTONDOWN;
		            g_bIsDrag = true;
		            first = MapPoint (pt);
  		            SetCapture (hWnd);
    	            DrawEverything ();
	                HPEN	old_pen = s_cast<HPEN> (SelectObject (g_offscreenDC, GetStockObject (DC_PEN)));
	                SetDCPenColor (g_offscreenDC, 0x00aaaaaa);
		            width = height = c_fWidth;
		            DrawBox (g_offscreenDC, first, Vector2 (1, 0), width, height);
	                SelectObject (g_offscreenDC, old_pen);
		        }
		    }
		    return true;
		    
		case WM_MOUSEMOVE:
		    if (g_bIsDrawn and (g_isDown == WM_LBUTTONDOWN))
		    {
		        POINT   pt;
		        pt.x = GET_X_LPARAM  (lParam);
		        pt.y = GET_Y_LPARAM  (lParam);
		        Vector2 second = MapPoint (pt);
		        Vector2 delta = second - first;

	            DrawEverything ();
	            HPEN	old_pen = s_cast<HPEN> (SelectObject (g_offscreenDC, GetStockObject (DC_PEN)));
	            SetDCPenColor (g_offscreenDC, 0x00aaaaaa);
		        if ((fabsf (delta.x) > c_fMinBoxSize) or (fabsf (delta.y) > c_fMinBoxSize))
		        {
		            width = max (fabsf (delta.x), c_fMinBoxSize);
		            height = max (fabsf (delta.y), c_fMinBoxSize);
		        }
		        else
		            width = height = c_fWidth;
		        DrawBox (g_offscreenDC, first, Vector2 (1, 0), width, height);
	            SelectObject (g_offscreenDC, old_pen);
		        return true;
		    }
		    break;
		    
		case WM_LBUTTONUP:
		    if (g_isDown == WM_LBUTTONDOWN)
		    {
		        g_isDown = 0;
			    g_bIsDrag = false;
		        ReleaseCapture ();
		        
                /*
                if (wParam bit_and MK_CONTROL)
			        g_thing_list.push_back (Thing (first, Vector2 (1, 0), width, height, OT_FIXED_POSITION));
			    else
			        g_thing_list.push_back (Thing (first, Vector2 (1, 0), width, height));
			    */
			}
		    break;
    }
    return false;
}
//-----------------------------------------------------------------------------
bool
LineToolProc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static Vector2  first;
    static Vector2  second;
    static float        width = c_fWidth;

/*

    // line too will initially connect any two points with a line. The width of the line
    // will be set by the width of the points, and interpolated along its length
	switch (message) 
	{
		case WM_LBUTTONDOWN:
		    {
		        POINT   pt;
		        pt.x = GET_X_LPARAM  (lParam);
		        pt.y = GET_Y_LPARAM  (lParam);
		        if (not g_isDown)
		        {
		            g_isDown = WM_LBUTTONDOWN;
		            g_bIsDrag = true;
		            first = MapPoint (pt);
		            second = first;
  		            SetCapture (hWnd);
    	            DrawEverything ();
                    int     index = PointInList (first, -1);
                    if (index != -1)
                        first = g_thing_list[index].pt;
		        }
		    }
		    return true;
		    
		case WM_MOUSEMOVE:
		    if (g_bIsDrawn and (g_isDown == WM_LBUTTONDOWN))
		    {
		        POINT       pt;
		        pt.x = GET_X_LPARAM  (lParam);
		        pt.y = GET_Y_LPARAM  (lParam);
		        second = MapPoint (pt);
                int     index = PointInList (second, -1);
                if (index != -1)
                    second = g_thing_list[index].pt;
		        Vector2 delta = second - first;
		        if (D3DXVec2Length (&delta) > c_fMinWidth)
		        {
                    // draw a rubber band line
	                DrawEverything ();
	                HPEN	old_pen = s_cast<HPEN> (SelectObject (g_offscreenDC, GetStockObject (DC_PEN)));
	                SetDCPenColor (g_offscreenDC, 0x00aaaaaa);
		            DrawLine (g_offscreenDC, first, second);
	                SelectObject (g_offscreenDC, old_pen);
		        }
		        return true;
		    }
		    break;
		    
		case WM_LBUTTONUP:
		    if (g_isDown == WM_LBUTTONDOWN)
		    {
		        g_isDown = 0;
			    g_bIsDrag = false;
			    
		        ReleaseCapture ();
		        Vector2 delta = second - first;
		        float       length = D3DXVec2Length (&delta);
		        if (length > c_fMinWidth)
		        {
		            // set up the undo information
                    g_thing_bar = s_cast<int> (g_thing_list.size ());
    		        g_line_bar = s_cast<int> (g_line_list.size ());
    		        
                    // figure the point index for point A, create one if none is found
                    int indexA = PointInList (first, -1);
                    if (indexA == -1)
                    {
                        indexA = s_cast<int> (g_thing_list.size ());
			            g_thing_list.push_back (Thing (first, c_fLineWidth, (wParam bit_and MK_CONTROL) ? OT_INVISIBLE bit_or OT_FIXED_POSITION : OT_INVISIBLE));
			        }
    			    
                    // figure the point index for point B, create one if none is found
                    int indexB = PointInList (second, -1);
                    if (indexB == -1)
                    {
                        indexB = s_cast<int> (g_thing_list.size ());
			            g_thing_list.push_back (Thing (second, c_fLineWidth, (wParam bit_and MK_CONTROL) ? OT_INVISIBLE bit_or OT_FIXED_POSITION : OT_INVISIBLE));
			        }
    			    
			        // now create the line
			        g_line_list.push_back (Line (indexA, indexB, OT_DEFAULT, (wParam bit_and MK_SHIFT) ? length : 0.0f));
			    }
			    
			}
		    break;
    }
    
    */
    return false;
}

//-----------------------------------------------------------------------------
