//------------------------------------------------------------------------------
//	File:					initapp.cp
//	Date:					9/30/96
//	Author:				Bretton Wade
//
//	Description:	this file contains the application code for the Win95
//								interface. This is not intended to be a model Win95
//								application.
//
//------------------------------------------------------------------------------

#include	"precompile.h"
#include	"view.h"
#include	"cube.h"
#include	"bsptree_3d.h"
#include	"transform_3d.h"

//------------------------------------------------------------------------------
#define	CLASS_NAME	"BSP Tree Demo"																							//	name of the window class and menu name

//------------------------------------------------------------------------------
static	MSG			gMessage;																												//	place to get a message
static	HWND		gWindow;																												//	global handle to the window structure
static	bool		gClick = FALSE;																									//	whether or not the mouse is down
static	bool		gDrawn;																													//	whether or not the view has been drawn
static	view		*gView;																													//	the world viewer
bsptree					*world;																													//	the bsp tree based world description

//------------------------------------------------------------------------------
listptr	Extrude (const point_3d* pts, int size, const vector_3d& extrude)																					//	compute a cube given a transformation
{																																								//	begin
    point_3d*   all_pts = new point_3d[size * 2];
    
    // copy the points and calculate the new points
    for (int i = 0; i < size; ++i)
    {
        all_pts[i] = pts[i];
        all_pts[size + ((size - 1) - i)] = pts[i] + extrude;
    }
    
    // build the polygons
    int         num_faces = size + 2;
	listptr	    extruded_polys;
	
	// add the cap faces
	extruded_polys->AddToList (polyptr (all_pts, size, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12));
	extruded_polys->AddToList (polyptr (all_pts, size, size, size + 1, size + 2, size + 3, size + 4, size + 5, size + 6, size + 7, size + 8, size + 9, size + 10, size + 11, size + 12));
	
	// add the extrusion faces
    for (int i = 0; i < size; ++i)
    {
        int h = (i + 1) % size;
        int j = size + ((size - 1) - i);
        int k = size + ((size - 1) - h);
	    extruded_polys->AddToList (polyptr (all_pts, 4, h, i, j, k));
	}

    // clean up
    delete[] all_pts;

	//	return the result
	return extruded_polys;
}																																								//	end

//------------------------------------------------------------------------------
void	InitializeModel (void)																										//	build the model that we want to display
{																																								//	begin
	world = new bsptree;																													//	create a new bsp tree
	
	#if 0
	//	house
	point_3d		house_pts[5];																										//	eight vertices
	house_pts[0] = point_3d (R(-1.0), R(-1.0), R(-1.0));
	house_pts[1] = point_3d (R(-1.0), R( 0.5), R(-1.0));
	house_pts[2] = point_3d (R( 0.0), R( 1.5), R(-1.0));
	house_pts[3] = point_3d (R( 1.0), R( 0.5), R(-1.0));
	house_pts[4] = point_3d (R( 1.0), R(-1.0), R(-1.0));
	world->Insert (Extrude (house_pts, 5, vector_3d (R(0.0), R(0.0), R(2.0))), HC_OUT, HC_OUT);
	
	//	chimney
	point_3d		chimney_pts[5];																										//	eight vertices
	chimney_pts[0] = point_3d (R(-1.0), R(-1.0), R(-1.0));
	chimney_pts[1] = point_3d (R(-1.0), R( 0.5), R(-1.0));
	chimney_pts[2] = point_3d (R( 0.0), R( 1.5), R(-1.0));
	chimney_pts[3] = point_3d (R( 1.0), R( 0.5), R(-1.0));
	chimney_pts[4] = point_3d (R( 1.0), R(-1.0), R(-1.0));
	//world->Insert (Extrude (chimney_pts, 5, vector_3d (R(0.0), R(0.0), R(2.0))), HC_OUT, HC_OUT);
	
    #else
	world->Insert (Cube (IDENTITY_MATRIX), HC_OUT, HC_OUT);												//	insert a basic cube
	world->Insert (																																//	cut out the cube to make a big "C"
		Cube (
			Scale (R(-1.5), R(-0.875), R(-1.5)) * 
			Translate (R(0.625), R(0.0), R(0.0))
		), 
		HC_IN, HC_SPANNING);
	world->Reduce ();																															//	strip out polygons which are no longer part of the object
	world->Insert (																																//	then add a slab
		Cube (
			Scale (R(0.75), R(1.25), R(0.125)) * 
			RotateY (R(-15.0)) * 
			Translate (R(0.5), R(0.0), R(0.0))
		), 
		HC_OUT, HC_SPANNING);
	#endif
}																																								//	end

//------------------------------------------------------------------------------
long FAR PASCAL	WinProc (HWND window, UINT message, WPARAM wpar, LPARAM lpar)		//	window proc for the application window
{																																								//	begin
	switch (message)																															//	check the message
	{																																							//	begin
		case WM_ACTIVATE:																														//	on a window activate
			gDrawn = FALSE;																														//	clear the drawn flag
			break;																																		//	end case
		case WM_PAINT:																														//	on a window activate
			gView->StartDrawing ();																										//	open the drawing environment
			gView->DrawScene ();																											//	re-draw the scene
			gView->StopDrawing ();																										//	close the drawing environment
			break;																																		//	end case
		case WM_MOUSEMOVE:																													//	on a mouse moved
			if (gClick)																																//	if the mouse is down
			{																																					//	begin
				POINT	pt;																																//	placeholder point
				pt.x = LOWORD (lpar);																										//	fetch the x coordinate of the mouse
				pt.y = HIWORD (lpar);																										//	fetch the y coordinate of the mouse
				gView->HandleDrag (pt);																									//	drag the view according to the mouse location
			}																																					//	end
			break;																																		//	end case
		case WM_LBUTTONDOWN:																												//	if the left mouse button is clicked down
			{																																					//	begin
			ShowCursor (FALSE);																												//	hide the cursor
			gClick = TRUE;																														//	set the click flag
				POINT	pt;																																//	placeholder point
				pt.x = LOWORD (lpar);																										//	fetch the x coordinate of the mouse
				pt.y = HIWORD (lpar);																										//	fetch the y coordinate of the mouse
			gView->HandleClick (pt);																									//	start the drawing environment for a drag
			}																																					//	end
			return 0;																																	//	end case, don't let anybody else do anything with this event
		case WM_LBUTTONUP:																													//	left mouse button lifted
			gClick = FALSE;																														//	clear the click flag
			gView->StartDrawing ();																										//	open the drawing environment
			gView->DrawScene ();																											//	re-draw the scene
			gView->StopDrawing ();																										//	close the drawing environment
			ShowCursor (TRUE);																												//	show the cursor
			return 0;																																	//	end case, don't let anybody else do anything with this event
		case WM_KEYDOWN:																														//	key pressed message
			if (wpar == VK_ESCAPE)																										//	if the key is the escape key
			{																																					//	begin
				delete gView;																														//	release the view
				PostMessage (window, WM_CLOSE, 0, 0);																		//	close the window
			}																																					//	end
			break;																																		//	end case
		case WM_DESTROY:																														//	destroy the window
			PostQuitMessage (0);																											//	quit
			break;																																		//	end case
	}																																							//	end
	return DefWindowProc (window, message, wpar, lpar);														//	default window behavior
}																																								//	end

//------------------------------------------------------------------------------
static	void	Open (HINSTANCE instance, int show)																//	initialize the application
{																																								//	begin
	InitializeModel ();																														//	build the model
	WNDCLASS	wClass;																															//	window class definition
	wClass.style = CS_HREDRAW | CS_VREDRAW;																				//	style flags
	wClass.lpfnWndProc = WinProc;																									//	set the window proc
	wClass.cbClsExtra = 0;																												//	no extra data
	wClass.cbWndExtra = 0;																												//	no extra data
	wClass.hInstance = instance;																									//	the application instance
	wClass.hIcon = LoadIcon (instance, IDI_APPLICATION);													//	generic application
	wClass.hCursor = LoadCursor (0, IDC_ARROW);																		//	normal cursor
	wClass.hbrBackground = 0;																											//	no background
	wClass.lpszMenuName = CLASS_NAME;																							//	class name
	wClass.lpszClassName = CLASS_NAME;																						//	class name
	RegisterClass (&wClass);																											//	register the class
	gWindow = CreateWindowEx (WS_EX_TOPMOST, CLASS_NAME, CLASS_NAME, WS_POPUP, 0, 0, GetSystemMetrics (SM_CXSCREEN), GetSystemMetrics (SM_CYSCREEN), 0,0, instance, 0);//	create the window
	ShowWindow (gWindow, show);																										//	show the window
	gView = new view (gWindow);																										//	create the world view
	UpdateWindow (gWindow);																												//	update the window
	SetFocus (gWindow);																														//	set the window focus to my window
}																																								//	end

//------------------------------------------------------------------------------
static	void	Loop (void)																												//	the main application loop
{																																								//	begin
	bool	done = FALSE;																														//	the done flag
	while (!done)																																	//	loop until we are done
		if (PeekMessage (&gMessage, 0, 0, 0, PM_NOREMOVE))													//	if there is a message in the queue
		{
			if (GetMessage (&gMessage, gWindow, 0, 0) == TRUE)												//	if the message is not a quit message
				DispatchMessage (&gMessage);																						//	send the message to the appropriate window
			else																																			//	otherwise we want to quit
				done = TRUE;																														//	set the finished flag
		}
		else																																				//	otherwise, the application is not active
		{
			if (!gDrawn)																															//	if the interface has not been drawn
			{																																					//	begin
				gView->StartDrawing ();																									//	start the drawing environment
				gView->DrawScene ();																										//	draw the scene
				gView->StopDrawing ();																									//	close the drawing environment
				gDrawn = TRUE;																													//	set the drawn flag
			}																																					//	end
			else																																			//	otherwise
				WaitMessage ();																													//	just wait for a message
		}
}																																								//	end

//------------------------------------------------------------------------------
static	void	Close (void)																											//	close the application
{																																								//	begin
}																																								//	end

//------------------------------------------------------------------------------
int PASCAL	WinMain (HINSTANCE inst, HINSTANCE prev, LPSTR cmdLine, int show)		//	windows main
{																																								//	begin
	Open (inst, show);																														//	open the application
	Loop ();																																			//	do the main loop
	Close ();																																			//	close the application
	return TRUE;																																	//	return success
}																																								//	end

//------------------------------------------------------------------------------
