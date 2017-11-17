#include	"precompile.h"
#include	"view.h"
#include	"cube.h"
#include	"bsptree_3d.h"
#include	"transform_3d.h"

#define	CLASS_NAME	"BSP Tree Demo"

static	MSG			gMessage;
static	HWND		gWindow;
static	bool		gClick = FALSE;
static	bool		gDrawn;
static  BspTree     gWorld;

//------------------------------------------------------------------------------
PtrToPolygonList_3d	Extrude(const point_3d* pts, int size, const vector_3d& extrude)
{
	point_3d*   all_pts = new point_3d[size * 2];


	for (int i = 0; i < size; ++i)
	{
		all_pts[i] = pts[i];
		all_pts[size + ((size - 1) - i)] = pts[i] + extrude;
	}


	int         num_faces = size + 2;
    PtrToPolygonList_3d extruded_polys;


	extruded_polys->addToList(new Polygon_3d (all_pts, (uint) size, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12));
	extruded_polys->addToList(new Polygon_3d (all_pts, (uint) size, size, size + 1, size + 2, size + 3, size + 4, size + 5, size + 6, size + 7, size + 8, size + 9, size + 10, size + 11, size + 12));


	for (int i = 0; i < size; ++i)
	{
		int h = (i + 1) % size;
		int j = size + ((size - 1) - i);
		int k = size + ((size - 1) - h);
		extruded_polys->addToList(new Polygon_3d (all_pts, 4, h, i, j, k));
	}


	delete[] all_pts;


	return extruded_polys;
}


void	InitializeModel(void)
{
	//gWorld = BspTree;

#if 0

	point_3d		house_pts[5];
	house_pts[0] = point_3d(R(-1.0), R(-1.0), R(-1.0));
	house_pts[1] = point_3d(R(-1.0), R(0.5), R(-1.0));
	house_pts[2] = point_3d(R(0.0), R(1.5), R(-1.0));
	house_pts[3] = point_3d(R(1.0), R(0.5), R(-1.0));
	house_pts[4] = point_3d(R(1.0), R(-1.0), R(-1.0));
	world->Insert(Extrude(house_pts, 5, vector_3d(R(0.0), R(0.0), R(2.0))), HC_OUT, HC_OUT);


	point_3d		chimney_pts[5];
	chimney_pts[0] = point_3d(R(-1.0), R(-1.0), R(-1.0));
	chimney_pts[1] = point_3d(R(-1.0), R(0.5), R(-1.0));
	chimney_pts[2] = point_3d(R(0.0), R(1.5), R(-1.0));
	chimney_pts[3] = point_3d(R(1.0), R(0.5), R(-1.0));
	chimney_pts[4] = point_3d(R(1.0), R(-1.0), R(-1.0));


#else
	gWorld.insert(Cube(IDENTITY_MATRIX), HC_OUT, HC_OUT);
    gWorld.insert(
		Cube(
			Scale(R(-1.5), R(-0.875), R(-1.5)) *
			Translate(R(0.625), R(0.0), R(0.0))
		),
		HC_IN, HC_SPANNING);
    gWorld.reduce();
    gWorld.insert(
		Cube(
			Scale(R(0.75), R(1.25), R(0.125)) *
			RotateY(R(-15.0)) *
			Translate(R(0.5), R(0.0), R(0.0))
		),
		HC_OUT, HC_SPANNING);
#endif
}


HDC				g_offscreenDC;
HBITMAP			g_offscreenBitmap;
HBITMAP			g_oldOffscreenBitmap;

long FAR PASCAL	WinProc(HWND window, UINT message, WPARAM wpar, LPARAM lpar)
{
	switch (message)
	{
	case WM_CREATE:
		break;
	case WM_ACTIVATE:
		gDrawn = FALSE;
		break;
	case WM_PAINT:
		gView->swapOffscreen();
		break;
	case WM_MOUSEMOVE:
		if (gClick)
		{
			POINT	pt;
			pt.x = LOWORD(lpar);
			pt.y = HIWORD(lpar);
            gView->eraseOffscreen();
			gView->handleDrag(pt, gWorld);
		}
		break;
	case WM_LBUTTONDOWN:
	{
		ShowCursor(FALSE);
		gClick = TRUE;
		POINT	pt;
		pt.x = LOWORD(lpar);
		pt.y = HIWORD(lpar);
        gView->eraseOffscreen ();
        gView->handleClick(pt, gWorld);
	}
	return 0;
	case WM_LBUTTONUP:
		ShowCursor(TRUE);
		gClick = FALSE;
        gView->eraseOffscreen ();
        gView->drawScene(gWorld);
		return 0;
	case WM_KEYDOWN:
		if (wpar == VK_ESCAPE)
		{
			delete gView;
			PostMessage(window, WM_CLOSE, 0, 0);
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(window, message, wpar, lpar);
}


static	void	Open(HINSTANCE instance, int show)
{
	InitializeModel();
	WNDCLASS	wClass;
	wClass.style = CS_BYTEALIGNCLIENT | CS_OWNDC;
	wClass.lpfnWndProc = WinProc;
	wClass.cbClsExtra = 0;
	wClass.cbWndExtra = 0;
	wClass.hInstance = instance;
	wClass.hIcon = LoadIcon(instance, IDI_APPLICATION);
	wClass.hCursor = LoadCursor(0, IDC_ARROW);
	wClass.hbrBackground = 0;
	wClass.lpszMenuName = CLASS_NAME;
	wClass.lpszClassName = CLASS_NAME;
	RegisterClass(&wClass);
	gWindow = CreateWindowEx(WS_EX_TOPMOST, CLASS_NAME, CLASS_NAME, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, CW_USEDEFAULT, CW_USEDEFAULT, WIDTH, HEIGHT, 0, 0, instance, 0);//	create the window
	ShowWindow(gWindow, show);
	gView = new View(gWindow);
	UpdateWindow(gWindow);
	SetFocus(gWindow);
}


static	void	Loop(void)
{
	bool	done = FALSE;
	while (!done)
		if (PeekMessage(&gMessage, 0, 0, 0, PM_NOREMOVE))
		{
			if (GetMessage(&gMessage, gWindow, 0, 0) == TRUE)
				DispatchMessage(&gMessage);
			else
				done = TRUE;
		}
		else
		{
			if (!gDrawn)
			{
                gView->eraseOffscreen();
				gView->drawScene(gWorld);
				gDrawn = TRUE;
			}
			else
				WaitMessage();
		}
}


static	void	Close(void)
{
}


int PASCAL	WinMain(HINSTANCE inst, HINSTANCE prev, LPSTR cmdLine, int show)
{
	Open(inst, show);
	Loop();
	Close();
	return TRUE;
}


