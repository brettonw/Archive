//------------------------------------------------------------------------------
//	File:					view.cp
//	Date:					9/30/96
//	Author:				Bretton Wade
//
//	Description:	this file contains the class methods for a view of space
//
//------------------------------------------------------------------------------

#include	"precompile.h"
#include	"view.h"
#include	"bsptree_3d.h"

#define WIDTH   1024
#define HEIGHT  768

//------------------------------------------------------------------------------
//	windows MoveTo define
//------------------------------------------------------------------------------
#define	MoveTo(hdc, x, y)	MoveToEx (hdc, x, y, 0)																//	because GDI sucks

//------------------------------------------------------------------------------
//	static variables
//------------------------------------------------------------------------------
view	*view::current = 0;																												//	part of the current drawing environment

//------------------------------------------------------------------------------
//	constructor
//------------------------------------------------------------------------------
view::view (HWND wind) :																												//	view constructor
transformation (matrix_3d::identity),																						//	matrix_3d constructor
inverse (matrix_3d::identity),																									//	matrix_3d constructor
cam (point_3d (R(0.0), R(0.0), R(6.0)), ORIGIN_3D, LensToFOV (60))							//	camera_3d constructor
{																																								//	begin
	gui = new arcball (ORIGIN_2D, R(0.95));																				//	make the arcball interface
	eye = cam.Eye ();																															//	assign the untransformed eye point_3d
	window = wind;																																//	save the window handle
	if (DirectDrawCreate (0, &ddraw, 0) == DD_OK)																	//	create the direct draw object
		if (ddraw->SetCooperativeLevel (window, DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN) == DD_OK)	//	make myself the exclusive owner of the direct draw object
			if (ddraw->SetDisplayMode (WIDTH,HEIGHT, 32) == DD_OK)													//	set the display mode
			{																																					//	begin
				DDSURFACEDESC	ddsd;																											//	a surface description structure for direct draw
				ddsd.dwSize = sizeof (ddsd);																						//	set the size of the surface description structure
				ddsd.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;												//	the capabilites and buffer count will be valid fields, as well as the pixel format
				ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | DDSCAPS_FLIP | DDSCAPS_COMPLEX;	//	set the caps to our double buffer design
				ddsd.dwBackBufferCount = 1;																							//	with one back buffer
				if (ddraw->CreateSurface (&ddsd, &front, 0) == DD_OK)										//	create the requested surface
				{																																				//	begin
					DDSCAPS								caps;																						//	and direct draw surface capabilities structure
					caps.dwCaps = DDSCAPS_BACKBUFFER;																			//	set the caps to a back buffer request
					front->GetAttachedSurface (&caps, &primary);													//	fetch the back surface
				}																																				//	end
				ysize = HEIGHT * R(0.5);																												//	compute the y halfsize
				xsize = WIDTH * R(0.5);																												//	compute the x halfsize
				aspect = ysize;																													//	set the aspect to be the smaller of the two
			}																																					//	end
}																																								//	end

//------------------------------------------------------------------------------
//	destructor
//------------------------------------------------------------------------------
view::~view (void)																															//	destructor
{																																								//	begin
	primary->Release ();																													//	free the surface object
	front->Release ();																														//	release the front surface
	ddraw->RestoreDisplayMode ();																									//	restore the previous display mode
	ddraw->Release ();																														//	free the direct draw object
	delete gui;																																		//	clean up the interface object
}																																								//	end

//------------------------------------------------------------------------------
//	Move the pen to a point
//------------------------------------------------------------------------------
void		view::MoveToPt (const point_2d &vp) const																//	move to a point_2d
{																																								//	begin
	POINT	p = vdctodc (vp);																												//	figure out the screen point that corresponds to the desired point
	MoveTo (hdc, p.x, p.y);																												//	move the pen there
}																																								//	end

//------------------------------------------------------------------------------
//	Draw a line to a point
//------------------------------------------------------------------------------
void		view::LineToPt (const point_2d &vp) const																//	draw a line to a point_2d
{																																								//	begin
	POINT	p = vdctodc (vp);																												//	figure out the screen point that corresponds to the desired point
	LineTo (hdc, p.x, p.y);																												//	draw a line there
}																																								//	end

//------------------------------------------------------------------------------
//	Draw a circle
//------------------------------------------------------------------------------
void		view::Circle (const point_2d &a, const point_2d &b) const								//	draw a circle defined by the rectangle 'ab'
{																																								//	begin
	POINT	p1 = vdctodc (a),																												//	figure out the screen location
				p2 = vdctodc (b);																												//	figure out the screen location
	Ellipse (hdc, p1.x, p2.y, p2.x + 1, p1.y + 1);																//	fill and frame the circle
}																																								//	end

//------------------------------------------------------------------------------
//	Draw the cursor crosshair
//------------------------------------------------------------------------------
void		view::CrossHair (const point_2d &vp) const															//	draw a crosshair at the specified location
{																																								//	begin
	POINT	p = vdctodc (vp);																												//	figure out the screen location
	MoveTo (hdc, p.x - 5, p.y);																										//	move to the left
	LineTo (hdc, p.x + 6, p.y);																										//	line to the right
	MoveTo (hdc, p.x, p.y - 5);																										//	move to the top
	LineTo (hdc, p.x, p.y + 6);																										//	line to the bottom
}																																								//	end

//------------------------------------------------------------------------------
//	Draw a polygon
//------------------------------------------------------------------------------
void		view::DrawPolygon (polyptr poly)																				//	draw a polygon (transformed by the camera_3d)
{																																								//	begin
	if ((eye | poly->Plane ()) > R(0.0))																					//	if the polygon is not a backface
	{																																							//	begin
		static	vector_3d light = vector_3d (R(4.0), R(8.0), R(6.0)).Normalize ();	//	lighting vector
		real	shade = (poly->Plane () | light) * R(0.8) + R(0.2);										//	compute the lighting on this polygon
		if (shade < R(0.3)) shade = R(0.3);																					//	clamp it to the ambient factor
		if (shade > R(0.9)) shade = R(0.9);																					//	and no brighter than white
		shade *= R(255.0);																													//	multiply the shade to get a real color
		COLORREF	color = RGB (int (shade), int (shade), int (shade));							//	compute the color in RGB
		HBRUSH		brush = CreateSolidBrush (color);																	//	make a brush out of it
		HGDIOBJ		oldbrush = SelectObject (hdc, brush);															//	select the brush
		for (short i = 0; i < poly->Count (); i++)																	//	for all points
		{																																						//	begin
			point_3d	pt = poly->Vertex (i) * viewing;																//	compute the screen location
			pts[i] = vdctodc (point_2d (pt[X], pt[Y]));																//	compute the real screen location
		}																																						//	end
		Polygon (hdc, pts, poly->Count ());																					//	make a polygon fromt he closed array
		SelectObject (hdc, oldbrush);																								//	restore the old brush
		DeleteObject (brush);																												//	free up the brush
	}																																							//	end
}																																								//	end

//------------------------------------------------------------------------------
//	Draw the whole scene
//------------------------------------------------------------------------------
void		view::DrawScene (void)																									//	draw the scene that this view is for
{																																								//	begin
	viewing = transformation * cam.Transform ();																	//	compute the viewing transformation
	eye = cam.Eye () * inverse;																										//	and the eye location
	extern	bsptree	*world;																												//	the world is not defined here...
	HPEN		pen = CreatePen (PS_SOLID, 0, 0x00ffffff);														//	make a pen for the polygon outlines
	HGDIOBJ	oldpen = SelectObject (hdc, pen);																			//	select the pen
	world->Draw (eye);																														//	draw the scene
	SelectObject (hdc, oldpen);																										//	restore the old pen
	DeleteObject (pen);																														//	free up the pen
}																																								//	end

//------------------------------------------------------------------------------
//	Handle a mouse down event in the user area
//------------------------------------------------------------------------------
void		view::HandleClick (POINT where)																					//	handle mouse down/up
{																																								//	begin
	sum = transformation;																													//	copy the transformation
	gui->Click (dctovdc (where));																									//	tell the interface object about the click
	gui->Drag (dctovdc (where));																									//	tell the interface object to begin the dragging process
	StartDrawing ();																															//	prepare the offscreen drawing world
	gui->DrawBackground ();																												//	draw the interface background
	DrawScene ();																																	//	draw the scene
	gui->DrawForeground ();																												//	draw the interface foreground
	StopDrawing ();																																//	clean up the offscreen drawing world and update the screen
}																																								//	end

//------------------------------------------------------------------------------
//	Handle a mouse drag event in the user area
//------------------------------------------------------------------------------
void	view::HandleDrag (POINT where)																						//	handle mouse drag
{																																								//	begin
	transformation = sum * gui->Drag (dctovdc (where));														//	set up the current transformation
	inverse = transformation.Inverse ();																					//	compute its inverse
	StartDrawing ();																															//	prepare the offscreen drawing world
	gui->DrawBackground ();																												//	draw the interface background
	DrawScene ();																																	//	draw the scene
	gui->DrawForeground ();																												//	draw the interface foreground
	StopDrawing ();																																//	clean up the offscreen drawing world and update the screen
}																																								//	end

//------------------------------------------------------------------------------
//	convert a screen location to the range (-1..1, -1..1)
//------------------------------------------------------------------------------
point_2d		view::dctovdc (const POINT &p) const																//	map screen coordinates to virtual device coordinates
{																																								//	begin
	return point_2d ((p.x - xsize) / aspect, (p.y - ysize) / -aspect);						//	return the converted point_3d
}																																								//	end

//------------------------------------------------------------------------------
//	convert a point in the range (-1..1, -1..1) to a screen coordinate
//------------------------------------------------------------------------------
POINT		view::vdctodc (const point_2d &p) const																	//	map virtual device coordinates to screen coordinates
{																																								//	begin
	POINT	pt;																																			//	storage for the return value
	pt.x = (short) ((p[X] * aspect) + xsize);																			//	convert the x coordinate to screen coordinates
	pt.y = (short) ((p[Y] * -aspect) + ysize);																		//	convert the y coordinate to screen coordinates
	return pt;																																		//	return the converted point_3d
}																																								//	end

//------------------------------------------------------------------------------
//	Erase the display
//------------------------------------------------------------------------------
void	view::Erase (void)																												//	erase the screen
{																																								//	begin
	RECT		bound = {0, 0, WIDTH, HEIGHT};																							//	rectangle to specify the erasable area
	DDBLTFX	fx;																																		//	blit effects structure
	fx.dwSize = sizeof (fx);																											//	set the size of the blit effects structure
	fx.dwFillColor = 0;																														//	set the fill color to black
	bool		trying = TRUE;																												//	variable indicating try state
	while (trying)																																//	while we are still trying to get the DC
		switch (primary->Blt (&bound, 0, 0, DDBLT_COLORFILL | DDBLT_WAIT, &fx))			//	check the result of trying to erase the surfaces
		{																																						//	begin
			case DDERR_SURFACELOST:																										//	if the surface was lost (due to context switches)
				front->Restore ();																											//	restore the primary surface
				primary->Blt (&bound, 0, 0, DDBLT_COLORFILL | DDBLT_WAIT, &fx);					//	erase the surface
			case DD_OK:																																//	or if the result is anything else
				trying = FALSE;																													//	stop looping
				break;																																	//	end case
		}																																						//	end
}																																								//	end

//------------------------------------------------------------------------------
//	start drawing to the gworld
//------------------------------------------------------------------------------
void		view::StartDrawing (void)																								//	lock down the gworld and set the port appropriately
{																																								//	begin
	current = this;																																//	set the current drawing world
	Erase ();																																			//	eraset he surface
	primary->GetDC (&hdc);																												//	test the result of trying to get the DC
}																																								//	end

//------------------------------------------------------------------------------
//	stop drawing to the gworld
//------------------------------------------------------------------------------
void		view::StopDrawing (void)																								//	unlock the gworld and reset the port
{																																								//	begin
	current = 0;																																	//	make sure the current drawing world is empty
	primary->ReleaseDC (hdc);																											//	release the display context
	front->Flip (0, 1);																														//	flip the surfaces in direct draw
}																																								//	end

//------------------------------------------------------------------------------
