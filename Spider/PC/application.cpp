//-----------------------------------------------------------------------------
// Copyright (C) 2006 Bretton Wade, All Rights Reserved
//-----------------------------------------------------------------------------

#include    "precompile.h"
#include    "resource.h"
#include    "screen.h"
#include    "play.h"

//-----------------------------------------------------------------------------
// defines
//-----------------------------------------------------------------------------
#define     MAX_LOADSTRING  100

//-----------------------------------------------------------------------------
// globals
//-----------------------------------------------------------------------------
HINSTANCE   g_hInst;
TCHAR       szTitle[MAX_LOADSTRING];
TCHAR       g_szClassName[MAX_LOADSTRING];
bool        g_bApplicationPaused = true;
HDC		    g_offscreenDC;
HBITMAP     g_offscreenBitmap;
HBITMAP     g_oldOffscreenBitmap;
HWND        g_hWnd;
MSG         g_currentMessage;
POINT       g_lastCursorPos;
POINT       g_cursorPos;

//-----------------------------------------------------------------------------
// forward declarations
//-----------------------------------------------------------------------------
HANDLE              FindPrevInstance (void);
ATOM                MyRegisterClass (HINSTANCE);
BOOL                InitInstance (HINSTANCE, int);
LRESULT CALLBACK    WndProc (HWND, UINT, WPARAM, LPARAM);

//-----------------------------------------------------------------------------
// functions
//-----------------------------------------------------------------------------
int 
APIENTRY wWinMain (HINSTANCE hInst, HINSTANCE hPrevInst, LPTSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInst);
	UNREFERENCED_PARAMETER(lpCmdLine);

    // load the class name and window name
    LoadString (hInst, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadString (hInst, IDC_SPIDER, g_szClassName, MAX_LOADSTRING);

	// Check to see if this application is already running
	HANDLE  hEvent = FindPrevInstance ();
	if (hEvent)
	{
	    // Perform application initialization:
	    MyRegisterClass (hInst);
	    if (not InitInstance (hInst, nCmdShow)) 
	    {
            CloseHandle (hEvent);
		    return FALSE;
	    }

	    HACCEL hAccelTable;
	    hAccelTable = LoadAccelerators (hInst, (LPCTSTR) IDC_SPIDER);

        // loop until we are done using idle processing for the inner loop
        bool    finished = false;
        while (!finished)
            if (PeekMessage (&g_currentMessage, 0, 0, 0, PM_NOREMOVE))
            {
                if (GetMessage (&g_currentMessage, 0, 0, 0) == TRUE)
                    DispatchMessage (&g_currentMessage);
                else
                    finished = true;
            }
            else if (not g_bApplicationPaused)
            {
                Heartbeat ();
                Sleep (33);
            }
            else
            {
                Sleep (1000);
            }

        // return the result of the quit message
        CloseHandle (hEvent);
        return static_cast<int> (g_currentMessage.wParam);
	}
	else
		return -1;
}

//-----------------------------------------------------------------------------
HANDLE 
FindPrevInstance (void)
{
	HANDLE hEvent;
	HWND   hwnd;

	// Create a named event
	hEvent = CreateEvent (NULL, TRUE, FALSE, g_szClassName);
	if (hEvent != NULL)
	{
		// If the event already existed, that means there's another copy of our app
		// already running
		if (GetLastError () == ERROR_ALREADY_EXISTS)
		{
        	unsigned    attempt = 0;
			do
			{
				// Just in case the other window needs to finish initialization
				Sleep (attempt ? 250 : 0);

				// Try to find the other application window
				hwnd = FindWindow (g_szClassName, NULL);
				if (hwnd != NULL)
				{
					SetForegroundWindow (hwnd);
					CloseHandle (hEvent);
					return NULL;
				}
			}
			while (++attempt < 2);  // only attempt twice

			// If we didn't find the window, the other application was probably
			// shutting down, so we'll just continue
		}
	}

	// Done
	return hEvent;
}

//-----------------------------------------------------------------------------
ATOM 
MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); 

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC) WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon (hInstance, (LPCTSTR) IDI_SPIDER);
	wcex.hCursor		= LoadCursor (NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH) (COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE (IDC_SPIDER);
	wcex.lpszClassName	= g_szClassName;
	wcex.hIconSm		= LoadIcon (wcex.hInstance, (LPCTSTR) IDI_SMALL);

	return RegisterClassEx(&wcex);
}

//-----------------------------------------------------------------------------
BOOL 
InitInstance(HINSTANCE hInst, int nCmdShow)
{
    g_hInst = hInst; // Store instance handle in our global variable

    RECT     winRect;
    SetRect (&winRect, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    AdjustWindowRect (&winRect, WS_OVERLAPPED bit_or WS_CAPTION bit_or WS_SYSMENU bit_or WS_MINIMIZEBOX, TRUE);
    g_hWnd = CreateWindow ( g_szClassName, 
                            szTitle, 
                            WS_OVERLAPPED bit_or WS_CAPTION bit_or WS_SYSMENU bit_or WS_MINIMIZEBOX,
                            CW_USEDEFAULT, CW_USEDEFAULT, 
                            winRect.right - winRect.left, winRect.bottom - winRect.top, 
                            NULL, NULL, 
                            hInst, 
                            NULL);

    if (!g_hWnd)
        return FALSE;

    ShowWindow (g_hWnd, nCmdShow);
    
    Card::LoadBitmaps ();
    Spider::LoadBitmaps ();
    NewGame ();

    UpdateWindow (g_hWnd);

    return TRUE;
}

//-----------------------------------------------------------------------------
LRESULT 
CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	RECT			    clientRect;
	GetClientRect(hWnd, &clientRect);
	
	switch (message)
	{
	    case WM_COMMAND:
	        {
		        int wmId    = LOWORD(wParam);
		        int wmEvent = HIWORD(wParam);
		        // Parse the menu selections:
		        switch (wmId)
		        {
		            case ID_MOVE_SUGGEST:
		                break;
		            case ID_MOVE_UNDO:
		                Undo ();
		                break;
		            case ID_MOVE_REDO:
		                Redo ();
		                break;
			        case ID_GAME_RESET:
			            ResetGame ();
			            break;
			        case ID_GAME_NEW:
			            NewGame ();
			            break;
			        case ID_SUITS_1:
			            SetSuits (SINGLE_SUIT);
			            break;
			        case ID_SUITS_2:
			            SetSuits (DOUBLE_SUIT);
			            break;
			        case ID_SUITS_4:
			            SetSuits (QUAD_SUIT);
			            break;
		            default:
			            return DefWindowProc(hWnd, message, wParam, lParam);
		        }
		    }
		    break;
		    
	    case WM_KEYDOWN:
	        {
	            TranslateMessage (&g_currentMessage);
	        }
	        break;
	        
	    case WM_CHAR:
	        {
	            switch (wParam)
	            {
	                case 'z':
	                    Undo ();
	                    break;
	                case 'y':
	                    Redo ();
	                    break;
	                case 'q':
	                    DestroyWindow (g_hWnd);
	                    break;
	            }
		    }
		    break;
		    
	    case WM_LBUTTONDOWN:
	        {
	            SetCapture (hWnd);
                g_cursorPos.x = GET_X_LPARAM  (lParam);
                g_cursorPos.y = GET_Y_LPARAM  (lParam);
                g_lastCursorPos = g_cursorPos;
	            ClickDown (&g_cursorPos);
	        }
	        break;
	        
	    case WM_LBUTTONUP:
	        {
                g_cursorPos.x = GET_X_LPARAM  (lParam);
                g_cursorPos.y = GET_Y_LPARAM  (lParam);
	            ClickUp (&g_cursorPos);
	            ReleaseCapture ();
	        }
	        break;
	        
	    case WM_MOUSEMOVE:
	        {
                g_cursorPos.x = GET_X_LPARAM  (lParam);
                g_cursorPos.y = GET_Y_LPARAM  (lParam);
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
				BitBlt (paint.hdc, paint.rcPaint.left, paint.rcPaint.top, paint.rcPaint.right - paint.rcPaint.left, paint.rcPaint.bottom - paint.rcPaint.top, g_offscreenDC, paint.rcPaint.left, paint.rcPaint.top, SRCCOPY);
				EndPaint(hWnd, &paint);
			}
		    return 0;
		    
	    default:
		    return DefWindowProc (hWnd, message, wParam, lParam);
	}
	return 0;
}

//-----------------------------------------------------------------------------
