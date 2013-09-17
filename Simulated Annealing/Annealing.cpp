// Annealing.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "resource.h"
#include "bound.h"

#define MAX_LOADSTRING              100

using namespace Space;

#define BUCKET_SIZE                 3

#define WORLD_POPULATION            400
#define	WORLD_SIZE					(WORLD_POPULATION - (WORLD_POPULATION % BUCKET_SIZE))
#define WORLD_SPARSENESS            0.9f
#define	ANNEALING_TIME				(WORLD_SIZE * WORLD_SPARSENESS * 2 * 3)

#define	BUCKET_COUNT				(WORLD_SIZE / BUCKET_SIZE)
#define	VALIDATE_BUCKET_SIZE		assert ((WORLD_SIZE % BUCKET_COUNT) == 0)
#define	BUCKET_INDEX(point_index)	(point_index / BUCKET_SIZE)
#define POINT_INDEX(bucket_index)	(bucket_index * BUCKET_SIZE)

#define WORLD_RESOLUTION            75
#define BOX_SIZE                    (1.0f / WORLD_RESOLUTION)

#define DISPLAY_PADDING             5

Bound<2>*	array_of_boxes[WORLD_SIZE];
Bound<2>	array_of_bounds[BUCKET_COUNT];
float		schedule = 0.9f;
float		max_change = 2.0f;
float		scheduled_reduction = 1.0f / (WORLD_SIZE * ANNEALING_TIME);

float	Unit_Random (void)
{
	return rand () / static_cast<float> (SHRT_MAX);
}

unsigned int	Mod_Random (unsigned int mod)
{
	return (rand () * mod) / SHRT_MAX;
}

void	Initialize_World (void)
{
	VALIDATE_BUCKET_SIZE;
    Vector<2>           offset; offset = BOX_SIZE, BOX_SIZE;
    char                test[WORLD_RESOLUTION][WORLD_RESOLUTION] = {0};
    for (int j = 0; j < WORLD_RESOLUTION; j++)
        for (int k = 0; k < WORLD_RESOLUTION; k++)
            test[j][k] = (j ^ k) & 0x01;
	for (unsigned int i = 0; i < WORLD_SIZE; i++)
	{
		if (Unit_Random () < WORLD_SPARSENESS)
		{
            int         x = Mod_Random (WORLD_RESOLUTION);
            int         y = Mod_Random (WORLD_RESOLUTION);
            while (test[x][y])
            {
                x = Mod_Random (WORLD_RESOLUTION);
                y = Mod_Random (WORLD_RESOLUTION);
            }
            test[x][y] = 1;
            Point<2>    point1; point1 = float (x) * BOX_SIZE, float (y) * BOX_SIZE;
            Point<2>    point2 (point1 + offset);
			array_of_boxes[i] = new Bound<2> (point1, point2);
			array_of_bounds[BUCKET_INDEX(i)] += *(array_of_boxes[i]);
		}
		else
			array_of_boxes[i] = 0;
	}
}

#define USE_AREA                -1
#define USE_DIAGONAL            0
#define USE_DIAGONAL_2          1
#define USE_PERIMETER           2
#define USE_PERIMETER_2         3
#define USE_SQUARE              4
#define USE_AREA_COMBO          5
#define USE_AREA_COMBO_2        6
#define USE_AREA_COMBO_3        7
#define USE_PERIMETER_COMBO     8
#define USE_PERIMETER_COMBO_2   9
#define USE_PERIMETER_COMBO_3   10
#define USE_PERIMETER_COMBO_4   11
#define USE_MAX_X_Y             12

#define SOLID_ANGLE_METHOD  USE_DIAGONAL

template<int dimension>
float   Compute_Box_Size (const Vector<dimension>& diagonal)
{
    #if SOLID_ANGLE_METHOD == USE_AREA
	return fabsf (diagonal[X] * diagonal[Y]);

    #elif SOLID_ANGLE_METHOD == USE_DIAGONAL
	return diagonal.Get_Length_Squared ();

    #elif SOLID_ANGLE_METHOD == USE_DIAGONAL_2
	return sqrtf (diagonal.Get_Length_Squared ());

    #elif SOLID_ANGLE_METHOD == USE_PERIMETER
    return fabsf (diagonal[X]) + fabsf (diagonal[Y]) * 2.0f;

    #elif SOLID_ANGLE_METHOD == USE_PERIMETER_2
    return sqrtf (fabsf (diagonal[X] + diagonal[Y]) * 2.0f);

    #elif SOLID_ANGLE_METHOD == USE_SQUARE
    return fabsf (diagonal[X] - diagonal[Y]) + fabsf (diagonal[Y] - diagonal[X]);

    #elif SOLID_ANGLE_METHOD == USE_AREA_COMBO
    float   delta = fabsf (diagonal[X] - diagonal[Y]) + fabsf (diagonal[Y] - diagonal[X]);
    float   area = diagonal.Get_Length_Squared ();
    return delta + area;

    #elif SOLID_ANGLE_METHOD == USE_AREA_COMBO_2
    float   delta = fabsf (diagonal[X] - diagonal[Y]) + fabsf (diagonal[Y] - diagonal[X]);
    float   area = diagonal.Get_Length_Squared ();
    return (delta * delta) + area;

    #elif SOLID_ANGLE_METHOD == USE_AREA_COMBO_3
    float   delta = fabsf (diagonal[X] - diagonal[Y]) + fabsf (diagonal[Y] - diagonal[X]);
    float   area = diagonal.Get_Length_Squared ();
    return (delta * delta) + (area * area);

    #elif SOLID_ANGLE_METHOD == USE_PERIMETER_COMBO
    float   delta = fabsf (diagonal[X] - diagonal[Y]) + fabsf (diagonal[Y] - diagonal[X]);
    float   perimeter = fabsf (diagonal[X] + diagonal[Y]) * 2.0f;
    return delta + perimeter;

    #elif SOLID_ANGLE_METHOD == USE_PERIMETER_COMBO_2
    float   delta = fabsf (diagonal[X] - diagonal[Y]) + fabsf (diagonal[Y] - diagonal[X]);
    float   perimeter = fabsf (diagonal[X] + diagonal[Y]) * 2.0f;
    return (delta * delta) + perimeter;

    #elif SOLID_ANGLE_METHOD == USE_PERIMETER_COMBO_3
    float   delta = fabsf (diagonal[X] - diagonal[Y]) + fabsf (diagonal[Y] - diagonal[X]);
    float   perimeter = fabsf (diagonal[X] + diagonal[Y]) * 2.0f;
    return delta + (perimeter * perimeter);

    #elif SOLID_ANGLE_METHOD == USE_PERIMETER_COMBO_4
    float   delta = fabsf (diagonal[X] - diagonal[Y]) + fabsf (diagonal[Y] - diagonal[X]);
    float   perimeter = fabsf (diagonal[X] + diagonal[Y]) * 2.0f;
    return (delta * delta) + (perimeter * perimeter);

    #elif USE_MAX_X_Y
    return (diagonal[X] > diagonal[Y]) ? diagonal[X] : diagonal[Y];

    #endif
}

void	Anneal_Step (void)
{
    float           local_schedule = powf (schedule, 12.0f);
	schedule -= scheduled_reduction;

	unsigned int	i = Mod_Random (WORLD_SIZE - 1),
					j = Mod_Random (WORLD_SIZE - 1);

	unsigned int	bucket_i = BUCKET_INDEX(i),
					bucket_j = BUCKET_INDEX(j);

	while (bucket_i == bucket_j)
	{
		j = Mod_Random (WORLD_SIZE - 1);
		bucket_j = BUCKET_INDEX(j);
	}

	unsigned int	start_i = POINT_INDEX(bucket_i),
					start_j = POINT_INDEX(bucket_j);

	Bound<2>		bound_i = array_of_bounds[bucket_i],
					bound_j = array_of_bounds[bucket_j],
					new_bound_i,
					new_bound_j;

	Vector<2>		diagonal;

	float			old_size,
					new_size;

	diagonal = bound_i.Get_Diagonal_Vector ();
	old_size = Compute_Box_Size (diagonal);

	diagonal = bound_j.Get_Diagonal_Vector ();
	old_size += Compute_Box_Size (diagonal);

	Bound<2>*		tmp = array_of_boxes[i];
	array_of_boxes[i] = array_of_boxes[j];
	array_of_boxes[j] = tmp;

	for (unsigned int a = 0; a < BUCKET_SIZE; a++)
		if (array_of_boxes[a + start_i])
			new_bound_i += *(array_of_boxes[a + start_i]);
	diagonal = new_bound_i.Get_Diagonal_Vector ();
	new_size = Compute_Box_Size (diagonal);

	for (unsigned int a = 0; a < BUCKET_SIZE; a++)
		if (array_of_boxes[a + start_j])
			new_bound_j += *(array_of_boxes[a + start_j]);
	diagonal = new_bound_j.Get_Diagonal_Vector ();
	new_size += Compute_Box_Size (diagonal);

	float			delta_size = new_size - old_size;
	if (delta_size > 0.0f)
	{
		float	size_change_ratio = delta_size / max_change;
		if (size_change_ratio > (Unit_Random () * local_schedule))
		{
			array_of_boxes[j] = array_of_boxes[i];
			array_of_boxes[i] = tmp;
			return;
		}
	}
	array_of_bounds[bucket_i] = new_bound_i;
	array_of_bounds[bucket_j] = new_bound_j;
}

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];								// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];								// The title bar text

HDC				g_offscreenDC;
HBITMAP			g_offscreenBitmap;
HBITMAP			g_oldOffscreenBitmap;

// Foward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	Initialize_World ();

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_ANNEALING, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow)) 
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_ANNEALING);

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) 
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage is only necessary if you want this code
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
	wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)IDI_ANNEALING);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= (LPCSTR)IDC_ANNEALING;
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
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow (szWindowClass, 
						szTitle, 
						WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, 
						CW_USEDEFAULT, CW_USEDEFAULT, 
						500, 550, 
						0, 0, hInstance, 0);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

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
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	TCHAR szHello[MAX_LOADSTRING];
	LoadString(hInst, IDS_HELLO, szHello, MAX_LOADSTRING);

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
			// TODO: Add any drawing code here...
			hdc = g_offscreenDC;
			RECT		rt;

      // clear the offscreen buffer
			COLORREF	color = RGB (255, 255, 255);
			HBRUSH		brush = CreateSolidBrush (color);
			GetClientRect(hWnd, &rt);
			FillRect (hdc, &rt, brush);
			DeleteObject (brush);

      // compute the size of the display grid
			int	width = (rt.right - rt.left) - (DISPLAY_PADDING * 2),
				height = (rt.bottom - rt.top) - (DISPLAY_PADDING * 2);
			if (width < height)
				height = width;
			else
				width = height;

	  // draw the grid
    {
        float   grid_size = BOX_SIZE * float (width);
        int     topleft = DISPLAY_PADDING;
        int     bottom = topleft + height;
        int     right = topleft + width;
	      HPEN    grid_pen = CreatePen (PS_SOLID, 0, 0x00dddddd),
                old_pen = static_cast<HPEN> (SelectObject (hdc, grid_pen));
	      for (int i = 0; i <= WORLD_RESOLUTION; i++)
	      {
		        int	loc = int (grid_size * float (i)) + topleft;
		        MoveToEx (hdc, loc, topleft, 0);
		        LineTo (hdc, loc, bottom);
		        MoveToEx (hdc, topleft, loc, 0);
		        LineTo (hdc, right, loc);
	      }
	      SelectObject (hdc, old_pen);
    }

      // fill the bound rectangles
			color = RGB (255, 192, 192);
			brush = CreateSolidBrush (color);
			for (int i = 0; i < BUCKET_COUNT; i++)
			{
				Point<2>	min = array_of_bounds[i].Get_Min (),
							max = array_of_bounds[i].Get_Max ();
				float	    left = (min[X] * width) + DISPLAY_PADDING,
						    top = (min[Y] * height) + DISPLAY_PADDING,
						    right = (max[X] * width) + DISPLAY_PADDING,
						    bottom = (max[Y] * height) + DISPLAY_PADDING;
                if ((int (left) <= int (right)) && (int (top) <= int (bottom)))
                {
                    RECT        rect;
                    SetRect (&rect, int (left), int (top), int (right), int (bottom));
			        FillRect (hdc, &rect, brush);
                }
			}
			DeleteObject (brush);

      // draw the boxes
			color = RGB (255, 0, 0);
			brush = CreateSolidBrush (color);
			for (int i = 0; i < WORLD_SIZE; i++)
				if (array_of_boxes[i])
				{
				    Point<2>    min = array_of_boxes[i]->Get_Min (),
							    max = array_of_boxes[i]->Get_Max ();
				    float	    left = (min[X] * width) + DISPLAY_PADDING,
						        top = (min[Y] * height) + DISPLAY_PADDING,
						        right = (max[X] * width) + DISPLAY_PADDING,
						        bottom = (max[Y] * height) + DISPLAY_PADDING;
                    RECT        rect;
                    SetRect (&rect, int (left), int (top), int (right), int (bottom));
			        FillRect (hdc, &rect, brush);
				}
			DeleteObject (brush);

      // draw the bound rectangles
			if (schedule > 0.0f)
			    color = RGB (0, 0, 255);
            else
			    color = RGB (0, 0, 0);

			HPEN		pen = static_cast<HPEN> (CreatePen (PS_SOLID, 1, color));
			HPEN		oldPen = static_cast<HPEN> (SelectObject (hdc, pen));
			for (int i = 0; i < BUCKET_COUNT; i++)
			{
				Point<2>	min = array_of_bounds[i].Get_Min (),
							max = array_of_bounds[i].Get_Max ();
				float	    left = (min[X] * width) + DISPLAY_PADDING,
						    top = (min[Y] * height) + DISPLAY_PADDING,
						    right = (max[X] * width) + DISPLAY_PADDING,
						    bottom = (max[Y] * height) + DISPLAY_PADDING;
                if ((int (left) <= int (right)) && (int (top) <= int (bottom)))
                {
				    MoveToEx (hdc, left, top, 0);
				    LineTo (hdc, left, bottom);
				    LineTo (hdc, right, bottom);
				    LineTo (hdc, right, top);
				    LineTo (hdc, left, top);
                }
			}
			SelectObject (hdc, oldPen);
			DeleteObject (pen);

			hdc = BeginPaint(hWnd, &ps);
			BitBlt (hdc, 0, 0, rt.right, rt.bottom, g_offscreenDC, 0, 0, SRCCOPY);
			EndPaint(hWnd, &ps);

			if (schedule > 0.0f)
            {
			    for (int p = 0; p < ANNEALING_TIME; p++)
				    Anneal_Step ();
				InvalidateRect (hWnd, &rt, false);
            }
			break;
		}
		case WM_CREATE:
			{
				HDC			screenDC = GetDC (hWnd);
				void*		pixels;
				BITMAPINFO	bitMapInfo;

				RECT		clientRect;
				GetClientRect(hWnd, &clientRect);
				
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

		case WM_DESTROY:
			{
				SelectObject (g_offscreenDC, g_oldOffscreenBitmap);
				DeleteObject (g_offscreenBitmap);
				DeleteDC (g_offscreenDC);
				PostQuitMessage(0);
			}
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
   }
   return 0;
}

// Mesage handler for about box.
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
