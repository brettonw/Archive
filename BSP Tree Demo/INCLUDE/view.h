//------------------------------------------------------------------------------
//	File:					view.h
//	Date:					9/18/94
//	Author:				Bretton Wade
//
//	Description:	this file contains the class definition for a view of space
//------------------------------------------------------------------------------

#include	"arcball.h"
#include	"matrix_3d.h"
#include	"polyptr_3d.h"
#include	"camera_3d.h"
#include	<ddraw.h>

#ifndef VIEW
#define VIEW

//------------------------------------------------------------------------------
//	classes
//------------------------------------------------------------------------------
class	view																																			//	view class
{																																								//	begin view definition
	private:																																			//	private interface
				point_2d			dctovdc (const POINT &p) const;														//	map screen coordinates to virtual device coordinates
				POINT					vdctodc (const point_2d &p) const;												//	map virtual device coordinates to screen coordinates
	protected:																																		//	protected interface
				camera				cam;																											//	camera_3d
				bool					cull;																											//	flag to control backface culling
				arcball				*gui;																											//	pointer to the interface controller
				matrix_3d			sum,																											//	the sum of the interface transformations
											transformation,																						//	the transformation for points
											inverse,																									//	the inverse of the current transformation matrix_3d
											viewing;																									//	the final viewing transformation
				real					xsize, ysize, aspect;																			//	parameters for mapping
				point_3d			eye;																											//	eye point_3d
				POINT					pts[128];
				HWND					window;
        LPDIRECTDRAW	ddraw;																										//	pointer to a direct draw 2 object
				LPDIRECTDRAWSURFACE	front;
				LPDIRECTDRAWSURFACE	primary;
	public:																																				//	public interface
				HDC						hdc;
static	view					*current;																									//	static value for the current view
				view (HWND window);																											//	constructor
				~view (void);																														//	destructor
				void					MoveToPt (const point_2d &pt) const;											//	move to a point_2d
				void					LineToPt (const point_2d &pt) const;											//	draw a line to a point_2d
				void					Circle (const point_2d &a, const point_2d &b) const;			//	draw a circle defined by the rectangle 'ab'
				void					CrossHair (const point_2d &p) const;											//	draw a crosshair at the specified location
				void					DrawPolygon (polyptr poly);																//	draw a polygon (transformed by the camera_3d)
				void					DrawScene (void);																					//	draw the scene that this view is for
				void					HandleClick (POINT where);																//	handle mouse down
				void					HandleDrag (POINT where);																	//	handle mouse drag
				void					Erase (void);																							//	erase the screen
				void					StartDrawing (void);																			//	lock down the gworld and set the port appropriately
				void					StopDrawing (void);																				//	unlock the gworld and reset the port
};																																							//	end view class definition

//------------------------------------------------------------------------------

#endif //VIEW
