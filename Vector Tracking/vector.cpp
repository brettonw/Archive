// vector.cpp : Defines the entry point for the application.
//

#include    "stdafx.h"
#include    "debug.h"
#include    "definitions.h"
#include    "basic_math.h"
#include    "vector.h"
#include    "timer.h"
#include    "resource.h"

#define MAX_LOADSTRING 100

using namespace TMath;

//-----------------------------------------------------------------------------
typedef float               Real32;
typedef TVector<Real32, 2>  FVec2;
typedef TPoint<Real32, 2>   FPnt2;

//-----------------------------------------------------------------------------
const int   g_iWindowSize = 800;
const float g_fWindowSize = static_cast<float> (g_iWindowSize);

//-----------------------------------------------------------------------------
const int   g_iTrackingCount = 4;
const int   g_iBackwardsStep = g_iTrackingCount - 1;
FPnt2       g_trackingPts[g_iTrackingCount];
int         g_iCurrentPt = 0;

//-----------------------------------------------------------------------------
inline
int     
NextPoint (int i)
{
    return (i + 1) % g_iTrackingCount;
}

//-----------------------------------------------------------------------------
inline  
int     
PreviousPoint (int i)
{
    return (i + g_iBackwardsStep) % g_iTrackingCount;
}

//-----------------------------------------------------------------------------
Real32
PredictNext (Real32 a, Real32 b, Real32 c, Real32 d)
{
    //DEBUG_SPEW_HERE;
    g_dbgout << "PredictNext" << std::endl;

    // compute the velocities
    Real32   vel_ab = b - a;
    Real32   vel_bc = c - b;
    Real32   vel_cd = d - c;
    g_dbgout << "Vel AB: " << vel_ab << std::endl;
    g_dbgout << "Vel BC: " << vel_bc << std::endl;
    g_dbgout << "Vel CD: " << vel_cd << std::endl;
    g_dbgout << "--" << std::endl;
    
    // compute the accelerations and jerk
    Real32  acc_abc = (vel_bc - vel_ab) * 2;
    Real32  acc_bcd = (vel_cd - vel_bc) * 2;
    //Real32  jerk = acc_bcd - acc_abc;
    g_dbgout << "Acc ABC: " << acc_abc << std::endl;
    g_dbgout << "Acc BCD: " << acc_bcd << std::endl;
    //g_dbgout << "Jerk: " << jerk << std::endl;
    g_dbgout << "--" << std::endl;
    
    // assuming a fixed time step of 1, compute the next value
    // XXX would like to incorporate jerk
    Real32  next = d + vel_cd + (0.5f * acc_bcd);
    g_dbgout << "Next: " << next << std::endl;
    g_dbgout << "--" << std::endl;
    
    return next;
}

//-----------------------------------------------------------------------------
FPnt2
PredictNextPoint (void)
{
    //DEBUG_SPEW_HERE;
    g_dbgout << "PredictNextPoint" << std::endl;
    g_dbgout << "--" << std::endl;
    int     i = NextPoint (g_iCurrentPt);
    int     j = NextPoint (i);
    int     k = NextPoint (j);
    
    // get references to the four tracking points
    FPnt2&  a = g_trackingPts[i];
    FPnt2&  b = g_trackingPts[j];
    FPnt2&  c = g_trackingPts[k];
    FPnt2&  d = g_trackingPts[g_iCurrentPt];
    g_dbgout << "A: " << a << std::endl;
    g_dbgout << "B: " << b << std::endl;
    g_dbgout << "C: " << c << std::endl;
    g_dbgout << "D: " << d << std::endl;
    g_dbgout << "--" << std::endl;
    
    Real32  nextX = PredictNext (a[0], b[0], c[0], d[0]);
    Real32  nextY = PredictNext (a[1], b[1], c[1], d[1]);
    FPnt2   next;
    next = nextX, nextY;
    return next;
}

//-----------------------------------------------------------------------------
void
DrawLine (HDC hdc, const FPnt2& a, const FPnt2& b, COLORREF color = 0, int width = 1)
{
    g_dbgout << "Draw Line from (" << a << ") to (" << b << ")" << std::endl;
    // create a pen the requested color and select it
    HPEN    pen = CreatePen (PS_SOLID, width, color);
    HPEN    oldPen = reinterpret_cast<HPEN> (SelectObject (hdc, reinterpret_cast<HGDIOBJ> (pen)));
    
    MoveToEx (hdc, static_cast<int> (RoundOff (a[0])), g_iWindowSize - static_cast<int> (RoundOff (a[1])), 0);
    LineTo (hdc, static_cast<int> (RoundOff (b[0])), g_iWindowSize - static_cast<int> (RoundOff (b[1])));

    SelectObject (hdc, reinterpret_cast<HGDIOBJ> (oldPen));
    DeleteObject (pen);
}

//-----------------------------------------------------------------------------
void
DrawLineXOR (HDC hdc, const FPnt2& a, const FPnt2& b, int width = 1)
{
    // set the hdc to xor drawing mode
    int oldMode = GetROP2 (hdc);
    SetROP2 (hdc, R2_NOTXORPEN);
    DrawLine (hdc, a, b, 0, width);
    SetROP2 (hdc, oldMode);
}

//-----------------------------------------------------------------------------
void
DrawLastLine (HDC hdc)
{
    //DEBUG_SPEW_HERE;
    g_dbgout << "DrawLastLine" << std::endl;
    g_dbgout << "--" << std::endl;
    int     i = PreviousPoint (g_iCurrentPt);
    FPnt2&  b = g_trackingPts[i];
    FPnt2&  c = g_trackingPts[g_iCurrentPt];
    
    FVec2   vel_bc = c - b;
    float   vel_bc_length = vel_bc.Length ();
    if (vel_bc_length > 1)
    {
        DrawLine (hdc, b, c);
        int     j = PreviousPoint (i);
        FPnt2&  a = g_trackingPts[j];
        FVec2   vel_ab = b - a;
        vel_ab.Normalize ();
        vel_ab *= vel_bc.Length () * 0.5f;
        FVec2   acc = (vel_bc - vel_ab) * 2;
        
        float   fDivs = ceil (vel_bc_length / 5);
        //fDivs = 2;
        float   divTime = 1 / fDivs;
        float   time = divTime;
        int     divs = static_cast<int> (fDivs);
        FPnt2   lastPt = b;
        FPnt2   savePt;
        for (int k = 1; k <= divs; k++)
        {
            FPnt2   d = b + (time * vel_ab) + (0.5f * time * time * acc);
            DrawLine (hdc, lastPt, d, RGB(255, 0, 0), 2);
            savePt = lastPt;
            lastPt = d;
            time += divTime;
        }
        //b = c + ((savePt - c) * fDivs);
        //b = b + (((vel_ab * 0.5f) + vel_bc) * 0.25f);
        /*
        FPnt2   d = b + (0.5f * vel_ab) + (0.5f * 0.25f * acc);
        
        d = b + (((vel_ab * 0.5f) + vel_bc) * 0.25f);
        DrawLine (hdc, b, c);
        DrawLine (hdc, b, d, RGB(255, 0, 0), 2);
        DrawLine (hdc, d, c, RGB(255, 0, 0), 2);
        */
    }
    else
    {
        DrawLine (hdc, b, c);
    }
    g_dbgout << "--" << std::endl;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_VECTOR, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow)) 
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_VECTOR);

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) 
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); 

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)IDI_VECTOR);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= (LPCTSTR)IDC_VECTOR;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HANDLE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    HWND hWnd = 0;

    hInst = hInstance; // Store instance handle in our global variable

    RECT    windowRect = {0, 0, g_iWindowSize, g_iWindowSize};
    if (AdjustWindowRect (&windowRect, WS_OVERLAPPEDWINDOW, TRUE))
    {
        hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, windowRect.right, windowRect.bottom, NULL, NULL, hInstance, NULL);
    }

    if (!hWnd)
    {
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    SetTimer (hWnd, 0, 33, 0);

    return TRUE;
}

//
//  FUNCTION: WndProc(HWND, unsigned, WORD, LONG)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
enum
{
    NOT_TRACKING_MOUSE = 0,
    START_TRACKING_MOUSE = 1,
    TRACKING_MOUSE = 2,
    STOP_TRACKING_MOUSE = 3
};

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static  int    iTrackingMouse = NOT_TRACKING_MOUSE;
    
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message) 
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam); 
		wmEvent = HIWORD(wParam); 
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
	    {
		    hdc = BeginPaint(hWnd, &ps);
		    // TODO: Add any drawing code here...
		    FPnt2   a;
		    FPnt2   b;
		    a = 100, 200;
		    b = 200, 400;
		    DrawLineXOR (hdc, a, b);
		    DrawLineXOR (hdc, a, b);
		    EndPaint(hWnd, &ps);
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
		
    //case WM_MOUSEMOVE:
    case WM_TIMER:
        {
            // track the mouse
            POINT   WPT;
            GetCursorPos (&WPT);
            ScreenToClient (hWnd, &WPT);
            FPnt2   currentPoint;
            currentPoint = Real32 (WPT.x), Real32 (g_iWindowSize - WPT.y);
            //if (currentPoint != g_trackingPts[g_iCurrentPt])
            {
                static  FPnt2   lastPoint;
                static  FPnt2   predictedPoint;
                HDC             hdc = GetDC (hWnd);
                lastPoint = g_trackingPts[g_iCurrentPt];
                g_iCurrentPt = NextPoint (g_iCurrentPt);
                g_trackingPts[g_iCurrentPt] = currentPoint;
                
                // figure out what to draw
                switch (iTrackingMouse)
                {
                    case STOP_TRACKING_MOUSE:
                        iTrackingMouse = NOT_TRACKING_MOUSE;
                    case TRACKING_MOUSE:
                    {
                        // erase the last predicted line (Xor drawing)
                        DrawLineXOR (hdc, lastPoint, predictedPoint);
                        
                        // draw the last real line
                        DrawLastLine (hdc);
                    }
                    default:
                        break;
                }
                switch (iTrackingMouse)
                {
                    case START_TRACKING_MOUSE:
                        iTrackingMouse = TRACKING_MOUSE;
                    case TRACKING_MOUSE:
                    {
                        // draw a new predicted line
                        predictedPoint = PredictNextPoint ();
                        DrawLineXOR (hdc, currentPoint, predictedPoint);
                    }
                    default:
                        break;
                }

                ReleaseDC (hWnd, hdc);
            }
        }
        break;
        
    case WM_LBUTTONDOWN:
        {
            iTrackingMouse = START_TRACKING_MOUSE;
        }
        break;
        
    case WM_LBUTTONUP:
        {
            iTrackingMouse = STOP_TRACKING_MOUSE;
        }
        break;
        
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
		return TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) 
		{
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
		break;
	}
	return FALSE;
}
