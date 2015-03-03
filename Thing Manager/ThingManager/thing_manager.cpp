//-----------------------------------------------------------------------------
// Copyright (C) 2006 Bretton Wade, All Rights Reserved
//-----------------------------------------------------------------------------

#include    "precompile.h"
#include    "thing_manager.h"
#include    "resource.h"
#include    "tools.h"
#include    "drawing.h"
#include    "timer.h"
#include    "solver.h"
#include    "constants.h"

//-----------------------------------------------------------------------------
// defines
//-----------------------------------------------------------------------------
#define MAX_LOADSTRING 100

//-----------------------------------------------------------------------------
// control values
//-----------------------------------------------------------------------------
#define AUTO_SOLVE      1
#define VALIDATE_PAIRS  0
#define DRAW_LINKS      0
#define DRAW_BOUNDS     0
#define DRAW_ID         0

//-----------------------------------------------------------------------------
// globals
//-----------------------------------------------------------------------------
HINSTANCE       hInst;
TCHAR           szTitle[MAX_LOADSTRING];
TCHAR           szWindowClass[MAX_LOADSTRING];
bool            g_bApplicationPaused = true;
HDC				g_offscreenDC;
HBITMAP			g_offscreenBitmap;
HBITMAP			g_oldOffscreenBitmap;
HWND            g_hwnd;
bool            g_bIsDrag = false;
bool            g_bIsDrawn = false;
float           g_time = 0.0f;
float           g_time_2 = 0.0f;

Thing*          g_pSelectedThing = 0;
//-----------------------------------------------------------------------------
// functions
//-----------------------------------------------------------------------------
void
DrawPoints (HPEN pen)
{
	// set the pen
	HPEN	old_pen = static_cast<HPEN> (SelectObject (g_offscreenDC, pen));
			
	// traverse the point list
	Thing*  pThing = g_solver.m_collider.m_sentinel.GetNext ();
	Thing*  pEnd = s_cast<Thing*> (&g_solver.m_collider.m_sentinel);
	while (pThing != pEnd)
	{
	    switch (pThing->m_objectType bit_and OT_SHAPE_MASK)
	    {
	        case OT_CIRCLE:
	            if ((pThing->m_objectType bit_and OT_FIXED_POSITION) or (pThing == g_pSelectedThing))
	            {
	                HPEN	old_pen = static_cast<HPEN> (SelectObject (g_offscreenDC, GetStockObject (DC_PEN)));
	                SetDCPenColor (g_offscreenDC, 0x00aaaaff);
	                DrawCircle (g_offscreenDC, pThing->m_center, pThing->m_radius);
	                DrawCircle (g_offscreenDC, pThing->m_center, pThing->m_radius * 0.75f);
	                SelectObject (g_offscreenDC, old_pen);
	            }
	            if (pThing->m_objectType bit_and OT_INVISIBLE)
	            {
	                HPEN	old_pen = static_cast<HPEN> (SelectObject (g_offscreenDC, GetStockObject (DC_PEN)));
	                SetDCPenColor (g_offscreenDC, 0x00cccccc);
	                DrawCircle (g_offscreenDC, pThing->m_center, pThing->m_radius);
	                SelectObject (g_offscreenDC, old_pen);
	            }
	            else
	                DrawCircle (g_offscreenDC, pThing->m_center, pThing->m_radius);
	            break;
	            
	    }
	    
	    #if DRAW_BOUNDS
	        HPEN	old_pen = static_cast<HPEN> (SelectObject (g_offscreenDC, GetStockObject (DC_PEN)));
	        SetDCPenColor (g_offscreenDC, 0x00cccccc);
	        float   radius = pThing->m_radius + 1.0e-2f;
	        DrawBox (g_offscreenDC, pThing->m_center, Vector2 (1, 0), radius, radius);
	        SelectObject (g_offscreenDC, old_pen);
	    #endif

        #if DRAW_ID
        HFONT   font = CreateFont (-28 / s_cast<int> (g_zoom), 0, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, 0, FF_MODERN, "Arial");
        if (font)
        {
            HFONT   old_font = static_cast<HFONT> (SelectObject (g_offscreenDC, font));
            
            SetBkMode (g_offscreenDC, TRANSPARENT);
            
            RECT    rect;
            POINT   topleft = MapPoint (pThing->m_center + Vector2 (-pThing->m_radius, pThing->m_radius));
            POINT   bottomright = MapPoint (pThing->m_center + Vector2 (pThing->m_radius, -pThing->m_radius));
            SetRect (&rect, topleft.x, topleft.y, bottomright.x, bottomright.y);
            char    string[5];
            sprintf (string, "%02d", pThing->GetID () % 100);
            DrawText (g_offscreenDC, string, 2, &rect, DT_VCENTER bit_or DT_SINGLELINE bit_or DT_CENTER);
            
            SelectObject (g_offscreenDC, old_font);
            DeleteObject (font);
        }
        #endif
	    
	    pThing = pThing->GetNext ();
	}

	// restore the pen
	SelectObject (g_offscreenDC, old_pen);
}

//-----------------------------------------------------------------------------
void
DrawLinks (HPEN pen)
{
	// set the pen
	HPEN	old_pen = static_cast<HPEN> (SelectObject (g_offscreenDC, pen));
			
	// traverse the open list
	for (Word i = 0; i < g_solver.m_collider.m_openCount; ++i)
	{
        Thing*  pThingA = g_solver.m_collider.m_open[i].GetThingA ();
        Thing*  pThingB = g_solver.m_collider.m_open[i].GetThingB ();
        DrawArc (g_offscreenDC, pThingA->m_center, pThingB->m_center);
	}
    
	// restore the pen
	SelectObject (g_offscreenDC, old_pen);
}

//-----------------------------------------------------------------------------
void
DrawWorld (void)
{
	HPEN	my_pen = CreatePen (PS_SOLID, static_cast<int> (SCALE_FACTOR / g_zoom), 0x00ffdddd);
    DrawGrid (my_pen, 5);
	DeleteObject (my_pen);
	
	my_pen = CreatePen (PS_SOLID, static_cast<int> (SCALE_FACTOR / g_zoom), 0x00ffaaaa);
    DrawGrid (my_pen, 1);
	DeleteObject (my_pen);
    
    #if DRAW_LINKS
	my_pen = CreatePen (PS_SOLID, static_cast<INT> ((SCALE_FACTOR * 3) / g_zoom), 0x0022aa22);
	DrawLinks (my_pen);
	DeleteObject (my_pen);
	#endif
    
	my_pen = CreatePen (PS_SOLID, static_cast<INT> ((SCALE_FACTOR * 3) / g_zoom), 0);
	DrawPoints (my_pen);
	DeleteObject (my_pen);
}

//-----------------------------------------------------------------------------
void
DrawEverything (void)
{
    DrawWorld ();
	
	DrawToolbar ();
	
	// draw the number of corrections and the number of points
    RECT    rect = {SCALE_FACTOR * 4, WINDOW_Y_SIZE + (SCALE_FACTOR * 4), SCALE_FACTOR * (4 + 170), WINDOW_Y_SIZE + (SCALE_FACTOR * (4 + 24))};
	FillRect (g_offscreenDC, &rect, static_cast<HBRUSH> (GetStockObject (WHITE_BRUSH)));
    FrameRect (g_offscreenDC, &rect, static_cast<HBRUSH> (GetStockObject (BLACK_BRUSH)));
    
    static HFONT   font = CreateFont (-10 * SCALE_FACTOR, 0, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, 0, FIXED_PITCH bit_or FF_MODERN, "Lucida Console");
    if (font)
    {
        char    text[256];
        HFONT   old_font = static_cast<HFONT> (SelectObject (g_offscreenDC, font));
        
        SetBkColor (g_offscreenDC, 0x00ffffff);
        
        int     bottom = rect.bottom;
        rect.left += 4;
        rect.right -= 4;
        rect.top += 2;
        rect.bottom = rect.top + ((rect.bottom - rect.top) / 2);
        sprintf (text, "N: %04d/%d", g_solver.m_collider.m_boundsCount / 2, Thing::N);
        DrawText (g_offscreenDC, text, static_cast<int> (strlen (text)), &rect, DT_VCENTER bit_or DT_SINGLELINE bit_or DT_LEFT);
        sprintf (text, "T: %05.02fms", g_time);
        DrawText (g_offscreenDC, text, static_cast<int> (strlen (text)), &rect, DT_VCENTER bit_or DT_SINGLELINE bit_or DT_RIGHT);
        
        rect.top = rect.bottom;
        rect.bottom = bottom;
        sprintf (text, "O: %04d/%d", g_solver.m_collider.m_openCount, Collider::OPEN_SIZE);
        DrawText (g_offscreenDC, text, static_cast<int> (strlen (text)), &rect, DT_VCENTER bit_or DT_SINGLELINE bit_or DT_LEFT);
        sprintf (text, "C: %05d  ", g_solver.m_corrections);
        DrawText (g_offscreenDC, text, static_cast<int> (strlen (text)), &rect, DT_VCENTER bit_or DT_SINGLELINE bit_or DT_RIGHT);
        
        SelectObject (g_offscreenDC, old_font);
        //DeleteObject (font);
    }

    InvalidateRect (g_hwnd, 0, FALSE);
    g_bIsDrawn = false;
}

//-----------------------------------------------------------------------------
#define ROLLING_SIZE    32
#define ROLLING_MASK    (ROLLING_SIZE - 1)
void
Heartbeat (void)
{
    #if 1
    POINT   mousePt;
    GetCursorPos (&mousePt);
    ScreenToClient (g_hwnd, &mousePt);
    Vector2 mouse = MapPoint (mousePt);
    g_pSelectedThing = g_solver.m_collider.FindThing (mouse);
    #endif
    
    #if 0
        if (g_pSelectedThing)
        {
            g_solver.RemoveThing (g_pSelectedThing);
            delete g_pSelectedThing;
            g_pSelectedThing = 0;
        }
        
    #endif

    #if 0
    static  Thing*  pThing;
    static  bool    bFirstTime = true;
    if (bFirstTime)
    {
        Vector2 vec (0, 0);
        pThing = new (g_solver.GetNextThing ()) Thing (vec, c_fWidth, OT_CIRCLE bit_or OT_FIXED_POSITION);
        if (pThing)
            g_solver.AddThing (pThing);
        bFirstTime = false;
    }
    POINT   mousePT;
    GetCursorPos (&mousePT);
    ScreenToClient (g_hwnd, &mousePT);
    Vector2 mouse = MapPoint (mousePT);
    pThing->m_center = mouse;
    #endif
    
	// relax the points
	#if         AUTO_SOLVE
    static float    times[ROLLING_SIZE] = {0.0f};
    static int      times_index = 0;
    static float    time_rolling = 0.0f;
	time_rolling -= times[times_index];
	Timer   timer;
	g_solver.RelaxConstraints (1);
	times[times_index] = timer.Elapsed () * 1.0e3f;
	time_rolling += times[times_index];
	times_index = (times_index + 1) bit_and ROLLING_MASK;
	g_time = time_rolling / static_cast<float> (ROLLING_SIZE);
	#endif  //  AUTO_SOLVE
	
    // validate the open list...
    #if         VALIDATE_PAIRS
    static  Word    iCounter = 0;
    if ((++iCounter & 0x00ff) == 0)
    {
        g_solver.m_collider.ValidateOpen ();
        iCounter = 0;
    }
    #endif  //  VALIDATE_PAIRS

    // draw the points and lines and toolbar and ...
    DrawEverything ();
}

//-----------------------------------------------------------------------------
// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

//-----------------------------------------------------------------------------
MSG                 g_currentMessage;
//-----------------------------------------------------------------------------
int APIENTRY 
WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
 	// TODO: Place code here.
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_THING_MANAGER, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow)) 
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_THING_MANAGER);

    // loop until we are done using idle processing for the inner loop
    bool    finished = false;
    while (!finished)
        if (PeekMessage (&g_currentMessage, 0, 0, 0, PM_NOREMOVE))
            if (GetMessage (&g_currentMessage, 0, 0, 0) == TRUE)
                DispatchMessage (&g_currentMessage);
            else
                finished = true;
        else if ((not g_bApplicationPaused) and (not g_bIsDrag))
        {
            Heartbeat ();
        }
        else
        {
            Sleep (100);
        }

    // return the result of the quit message
    return static_cast<int> (g_currentMessage.wParam);
}

//-----------------------------------------------------------------------------
ATOM 
MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); 

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)IDI_THING_MANAGER);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= 0;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);

	return RegisterClassEx(&wcex);
}

//-----------------------------------------------------------------------------
BOOL 
InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // Store instance handle in our global variable

    RECT    winRect;
    winRect.left = 0;
    winRect.top = 0;
    winRect.right = CORE_WINDOW_X_SIZE;
    winRect.bottom = CORE_WINDOW_Y_SIZE + CORE_TOOLBAR_SIZE;
    
    AdjustWindowRect (&winRect, WS_OVERLAPPED bit_or WS_CAPTION bit_or WS_SYSMENU bit_or WS_MINIMIZEBOX, FALSE);
    g_hwnd = CreateWindow ( szWindowClass, 
                            szTitle, 
                            WS_OVERLAPPED bit_or WS_CAPTION bit_or WS_SYSMENU bit_or WS_MINIMIZEBOX,
                            CW_USEDEFAULT, CW_USEDEFAULT, 
                            winRect.right - winRect.left, winRect.bottom - winRect.top, 
                            NULL, NULL, 
                            hInstance, 
                            NULL);

    if (!g_hwnd)
        return FALSE;

    ShowWindow(g_hwnd, nCmdShow);
    UpdateWindow(g_hwnd);
    
    // set up the random number seed
    #ifdef      NDEBUG
    Word    tc = GetTickCount ();
    srand (tc);
    DebugPrint ("RandomSeed %d\n", tc);
    #endif  //  NDEBUG

    return TRUE;
}

//-----------------------------------------------------------------------------
LRESULT CALLBACK 
WndProc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (ToolbarProc (hWnd, message, wParam, lParam))
        return 0;
        
    if (DragViewProc (hWnd, message, wParam, lParam))
        return 0;
    
    bool    bTool;
    switch (g_toolState)
    {
        case TS_CIRCLE:
            bTool = CircleToolProc (hWnd, message, wParam, lParam);
            break;
        
        case TS_BOX:
            bTool = BoxToolProc (hWnd, message, wParam, lParam);
            break;
            
        case TS_LINE:
            bTool = LineToolProc (hWnd, message, wParam, lParam);
            break;
            
        default:
            bTool = false;
            break;
    }
    if (bTool)
        return 0;
    
	RECT			    rect;
	RECT			    clientRect;
	GetClientRect(hWnd, &rect);
	clientRect = rect;
	clientRect.right *= SCALE_FACTOR;
	clientRect.bottom *= SCALE_FACTOR;
    static bool         shiftKeyDown = false;
    static bool         ctrlKeyDown = false;

	switch (message) 
	{
		case WM_KEYUP:
		    {
		        switch (wParam)
		        {
		            case VK_SHIFT:
		                shiftKeyDown = false;
		                break;
		            
		            case VK_CONTROL:
		                ctrlKeyDown = false;
		                break;
		        }
		    }
		    break;
		case WM_KEYDOWN:
		    {
		        switch (wParam)
		        {
		            case VK_DOWN:
		                g_zoom = min (floorf ((g_zoom * 2.0f) + 0.5f), 16.0f);
		                break;
		                
		            case VK_UP:
		                g_zoom = max (floorf ((g_zoom * 0.5f) + 0.5f), 1.0f);
		                break;
		                
		            case VK_SHIFT:
		                shiftKeyDown = true;
		                break;
		            
		            case VK_CONTROL:
		                ctrlKeyDown = true;
		                break;
		            
	#ifndef  AUTO_SOLVE
		            case VK_SPACE:
		                g_solver.RelaxConstraints ();
		                break;
	#endif            
		            
		            case VK_ESCAPE:
						g_solver.m_collider.ValidateOpen ();
		                break;

		            case VK_DELETE:
		                {
		                    if (g_solver.m_collider.m_boundsCount)
		                    {
		                        if (not g_pSelectedThing)
		                        {
		                            do
		                            {
		                                Word    id = rand () % Thing::N;
		                                g_pSelectedThing = Thing::GetThing (id);
		                            }
		                            while (not g_pSelectedThing->IsValid ());
	                            }
                                g_solver.RemoveThing (g_pSelectedThing);
                                delete g_pSelectedThing;
                                g_pSelectedThing = 0;
                            }
		                }
		                break;
		            
		            default:
		                {
		                    TranslateMessage (&g_currentMessage);
		                
                            float       x = ((static_cast<float> (rand ()) / static_cast<float> (SHRT_MAX)) * 2.0f) - 1.0f;
                            float       y = ((static_cast<float> (rand ()) / static_cast<float> (SHRT_MAX)) * 2.0f) - 1.0f;
                            float       radius = shiftKeyDown ? ((static_cast<float> (rand ()) / static_cast<float> (SHRT_MAX)) * 2.0f * c_fWidth) + c_fMinWidth : c_fWidth;
                            Vector2 vec  = Vector2 (x, y) * 0.25f;
			                Thing*  pThing = new Thing (vec, radius);
			                if (pThing)
			                    g_solver.AddThing (pThing);
			            }
			            break;
			    }
		    }
		    break;
		    
		case WM_CREATE:
			{
				HDC			screenDC = GetDC (hWnd);
				void*		pixels;
				BITMAPINFO	bitMapInfo;
				
				g_offscreenDC = CreateCompatibleDC (screenDC);

				bitMapInfo.bmiHeader.biSize = sizeof (BITMAPINFOHEADER);
				bitMapInfo.bmiHeader.biWidth = clientRect.right;
				bitMapInfo.bmiHeader.biHeight = clientRect.bottom;
				bitMapInfo.bmiHeader.biPlanes = 1;
				bitMapInfo.bmiHeader.biBitCount = 32;
				bitMapInfo.bmiHeader.biCompression = BI_RGB;
				bitMapInfo.bmiHeader.biSizeImage = 0;
				bitMapInfo.bmiHeader.biXPelsPerMeter = GetDeviceCaps (screenDC, LOGPIXELSX) / 72;
				bitMapInfo.bmiHeader.biYPelsPerMeter = GetDeviceCaps (screenDC, LOGPIXELSY) / 72;
				bitMapInfo.bmiHeader.biClrUsed = 0;
				bitMapInfo.bmiHeader.biClrImportant = 0;

				g_offscreenBitmap = CreateDIBSection (g_offscreenDC, &bitMapInfo, DIB_RGB_COLORS, &pixels, 0, 0);
				g_oldOffscreenBitmap = static_cast<HBITMAP> (SelectObject (g_offscreenDC, g_offscreenBitmap));
				ReleaseDC (hWnd, screenDC);
			}
			break;

        case WM_ACTIVATE:
            {
                // pause the app if we are deactivated
                g_bApplicationPaused = (LOWORD (wParam) == WA_INACTIVE);
            }
			break;

		case WM_DESTROY:
			{
				SelectObject (g_offscreenDC, g_oldOffscreenBitmap);
				DeleteObject (g_offscreenBitmap);
				DeleteDC (g_offscreenDC);
				PostQuitMessage(0);
			}
			break;
			
	    case WM_PAINT:
			{
				PAINTSTRUCT	paint;
				BeginPaint (hWnd, &paint);
				SetStretchBltMode (paint.hdc, HALFTONE);
				StretchBlt (paint.hdc, 0, 0, rect.right, rect.bottom, g_offscreenDC, 0, 0, clientRect.right, clientRect.bottom, SRCCOPY);
				EndPaint(hWnd, &paint);
				
				// set the drawn flag
				g_bIsDrawn = true;
				
				// erase the back plane
	            FillRect (g_offscreenDC, &clientRect, static_cast<HBRUSH> (GetStockObject (WHITE_BRUSH)));
			}
		    return 0;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

//-----------------------------------------------------------------------------
