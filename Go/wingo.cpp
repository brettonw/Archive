// wingo.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "resource.h"
#include "go.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];								// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];								// The title bar text

unsigned int	g_board_size = 7,
				g_board_size_squared = g_board_size * g_board_size;
int				g_test_depth = 2,
				g_bad_move_weighting_factor = 100;
Board			g_board (g_board_size);
Stone			g_stone = BLACK_STONE;
bool			g_keep_playing = true;
int				g_pass_count = 0;

HDC				g_offscreenDC;
HBITMAP			g_offscreenBitmap;
HBITMAP			g_oldOffscreenBitmap;

//---------------------------------
float	Computer_Evaluate_Single_Move (Stone score_stone, int score, Stone stone, const Board& board, int depth, HDC dc, const RECT& clientRect)
{
	float	score_sum = 0.0f,
			score_count = 1.0e-6f;
	if (depth == 0)
	{
		for (unsigned int i = 0; i < g_board_size; i++)
			for (unsigned int j = 0; j < g_board_size; j++)
			{
				Board	test (board);
				if (test.Place_Stone (stone, i, j))
				{
					int		score_delta = test.Score_Game (score_stone) - score;
					score_sum += static_cast<float> ((score_delta < 0) ? (score_delta * g_bad_move_weighting_factor) : score_delta);
					score_count += 1.0;
				}
			}
		return score_sum / score_count;
	}
	else if (depth > 0)
	{
		unsigned int	done_so_far = 0;
		HBRUSH			red_brush = CreateSolidBrush (0x000000ff);
		RECT			progress;
		progress.left = 3;
		progress.right = progress.left + g_board_size_squared + 6;
		progress.top = clientRect.bottom - (10 * depth);
		progress.bottom = progress.top + 7;
		Rectangle (dc, progress.left, progress.top, progress.right, progress.bottom);
		progress.left += 3;
		progress.top += 2;
		progress.bottom -= 2;


		for (unsigned int i = 0; i < g_board_size; i++)
			for (unsigned int j = 0; j < g_board_size; j++)
			{
				Board	test (board);
				if (test.Place_Stone (stone, i, j))
				{
					score_sum += Computer_Evaluate_Single_Move (score_stone, score, (stone == WHITE_STONE) ? BLACK_STONE : WHITE_STONE, test, depth - 1, dc, clientRect);
					score_count += 1.0;
				}

				done_so_far++;
				progress.right = progress.left + done_so_far;
				FillRect (dc, &progress, red_brush);
			}
		DeleteObject (red_brush);
	}
	else
	{
		for (unsigned int i = 0; i < g_board_size; i++)
			for (unsigned int j = 0; j < g_board_size; j++)
			{
				Board	test (board);
				if (test.Place_Stone (stone, i, j))
				{
					score_sum += Computer_Evaluate_Single_Move (score_stone, score, (stone == WHITE_STONE) ? BLACK_STONE : WHITE_STONE, test, depth - 1, dc, clientRect);
					score_count += 1.0;
				}
			}
	}
	return score_sum / score_count;
}

//---------------------------------
void	Computer_Make_Single_Move (HDC dc, const RECT& clientRect)
{
	unsigned int	best_i,
					best_j,
					done_so_far = 0;
	float			best_score = -1.0e6;
	int				test_depth = g_test_depth;// + (static_cast<int> (g_stone) * 2) - 1;

	HBRUSH			red_brush = CreateSolidBrush (0x000000ff);

	RECT	progress;
	progress.left = 3;
	progress.right = progress.left + g_board_size_squared + 6;
	progress.top = clientRect.bottom - (10 * (test_depth + 1));
	progress.bottom = progress.top + 7;
	Rectangle (dc, progress.left, progress.top, progress.right, progress.bottom);
	progress.left += 3;
	progress.top += 2;
	progress.bottom -= 2;

	for (unsigned int i = 0; i < g_board_size; i++)
		for (unsigned int j = 0; j < g_board_size; j++)
		{
			Board	test (g_board);
			if (test.Place_Stone (g_stone, i, j))
			{
				
				float	score = Computer_Evaluate_Single_Move (g_stone, g_board.Score_Game (g_stone), (g_stone == WHITE_STONE) ? BLACK_STONE : WHITE_STONE, test, test_depth, dc, clientRect);
				if (score > best_score)
				{
					best_i = i;
					best_j = j;
					best_score = score;
				}
			}

			done_so_far++;
			progress.right = progress.left + done_so_far;
			FillRect (dc, &progress, red_brush);
		}
	g_board.Place_Stone (g_stone, best_i, best_j);
	DeleteObject (red_brush);
}

//---------------------------------
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
 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_WINGO, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow)) 
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_WINGO);

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
	wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)IDI_WINGO);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= 0;
	wcex.lpszMenuName	= (LPCSTR)IDC_WINGO;
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
						WS_OVERLAPPED bit_or WS_CAPTION bit_or WS_SYSMENU bit_or WS_MINIMIZEBOX, 
						CW_USEDEFAULT, CW_USEDEFAULT, 
						270, 360, 
						NULL, NULL, hInstance, NULL);

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
	int				wmId,		
					wmEvent;
	RECT			rect,
					clientRect;
	int				scaleFactor = 3;


	GetClientRect(hWnd, &rect);
	clientRect = rect;
	clientRect.right *= scaleFactor;
	clientRect.bottom *= scaleFactor;

	switch (message) 
	{
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

				g_board.Draw (g_offscreenDC, clientRect, g_stone);
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
		case WM_LBUTTONDOWN:
			if (g_board.Place_Stone (g_stone, LOWORD (lParam) * scaleFactor, HIWORD (lParam) * scaleFactor, clientRect))
			{
				g_stone = (g_stone == WHITE_STONE) ? BLACK_STONE : WHITE_STONE;
				g_board.Draw (g_offscreenDC, clientRect, g_stone);
				InvalidateRect (hWnd, &rect, true);
			}
			break;

		case WM_KEYDOWN:
			if (wParam == VK_ESCAPE)
			{
				g_stone = (g_stone == WHITE_STONE) ? BLACK_STONE : WHITE_STONE;
				g_board.Draw (g_offscreenDC, clientRect, g_stone);
				InvalidateRect (hWnd, &rect, true);
			}
			break;

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
				case IDM_UNDO:
					// it would be cool to have and use an undo here
				   break;
				default:
				   return DefWindowProc(hWnd, message, wParam, lParam);
			}
			break;

		case WM_PAINT:
			if (true)
			{
				PAINTSTRUCT	paint;
				BeginPaint (hWnd, &paint);
				SetStretchBltMode (paint.hdc, HALFTONE);
				StretchBlt (paint.hdc, 0, 0, rect.right, rect.bottom, g_offscreenDC, 0, 0, clientRect.right, clientRect.bottom, SRCCOPY);

				if (g_stone == WHITE_STONE)
				{
					HCURSOR	old_cursor = SetCursor (LoadCursor (0, IDC_WAIT));
					Computer_Make_Single_Move (paint.hdc, rect);
					g_stone = (g_stone == WHITE_STONE) ? BLACK_STONE : WHITE_STONE;
					g_board.Draw (g_offscreenDC, clientRect, g_stone);
					SetCursor (old_cursor);
					InvalidateRect (hWnd, &rect, true);
				}

				EndPaint(hWnd, &paint);
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
