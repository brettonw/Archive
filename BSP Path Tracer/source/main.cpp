//------------------------------------------------------------------------------
//	File:					initapp.cp
//	Date:					05/02/97
//	Author:				Bretton Wade
//
//	Description:	this file contains the startup and shutdown functions called
//								from the widgets
//
//------------------------------------------------------------------------------

#include	"precompile.h"
#include	"view.h"
#include	"cube.h"
#include	"bsptree_3d.h"
#include	"transform_3d.h"
#include	"control.h"
#include	"utility.h"

//------------------------------------------------------------------------------
bsptree	*world;

//------------------------------------------------------------------------------
int		main (int argc, char **argv)
{
	world = new bsptree;
	
	#if 1
	world->Insert (Cube (Scale (R(-1.0), R(-1.0), R(-1.0))), HC_OUT, HC_OUT);
	world->Insert (Cube (Scale (R(0.5), R(0.135), R(0.5)) * Translate (R(0.0), R(1.0), R(0.0)), LIGHT_INTENSITY), HC_OUT, HC_OUT);
	//world->Reduce ();
	
	//world->Insert (Cube (Scale (R(0.25), R(0.51), R(0.25)) * RotateY (R(15.0)) * Translate (R(-0.33), R(-0.5), R(-0.25))), HC_OUT, HC_OUT);
	//world->Reduce ();

	world->Insert (Cube (Scale (R(0.25), R(0.25), R(0.25)) * RotateY (R(60.0)) * Translate (R(0.5), R(-0.66), R(0.25))), HC_OUT, HC_OUT);
//	world->Reduce ();
	#else

	//	floor
	world->Insert (Cube (Scale (R( 3.0), R( 0.1), R( 3.0)) * Translate (R( 0.0), R(-1.0), R(-1.0))), HC_OUT, HC_OUT);

	//	body
	world->Insert (Cube (Scale (R( 0.10), R( 0.5), R( 0.25)) * Translate (R( 0.0), R( 0.3), R( 0.0)) * RotateY (R(15.0))), HC_OUT, HC_OUT);
	world->Insert (Cube (Scale (R( 0.35), R( 0.2), R( 0.15)) * Translate (R( 0.0), R( 0.5), R( 0.0)) * RotateY (R(15.0))), HC_OUT, HC_OUT);

	//	chest decoration
	world->Insert (Cube (Scale (R( 0.025), R( 0.3), R( 0.3)) * Translate (R( 0.0), R( 0.35), R( 0.0)) * RotateY (R(15.0))), HC_OUT, HC_OUT);

	//	shoulder decorations
	world->Insert (Cube (Scale (R( 0.25), R( 0.1), R( 0.3)) * Translate (R( 0.5), R( 0.65), R( 0.0)) * RotateZ (R( 7.5)) * RotateY (R(15.0))), HC_OUT, HC_OUT);
	world->Insert (Cube (Scale (R( 0.25), R( 0.1), R( 0.3)) * Translate (R(-0.5), R( 0.65), R( 0.0)) * RotateZ (R(-7.5)) * RotateY (R(15.0))), HC_OUT, HC_OUT);

	//	neck
	world->Insert (Cube (Scale (R( 0.05), R( 0.1), R( 0.1)) * Translate (R( 0.0), R( 0.8), R( 0.05)) * RotateY (R(15.0))), HC_OUT, HC_OUT);

	//	head
	world->Insert (Cube (Scale (R( 0.25), R( 0.15), R( 0.2)) * Translate (R( 0.0), R( 1.0), R( 0.1)) * RotateY (R(15.0))), HC_OUT, HC_OUT);

	//	eyes
	world->Insert (Cube (Scale (R( 0.05), R( 0.10), R( 0.10)) * RotateZ (R(-7.5)) * Translate (R( 0.08), R( 1.1), R( 0.25)) * RotateY (R(15.0))), HC_OUT, HC_OUT);
	world->Insert (Cube (Scale (R( 0.05), R( 0.10), R( 0.10)) * RotateZ (R( 7.5)) * Translate (R(-0.08), R( 1.1), R( 0.25)) * RotateY (R(15.0))), HC_OUT, HC_OUT);

	//	biceps
	world->Insert (Cube (Scale (R( 0.10), R( 0.20), R( 0.15)) * Translate (R( 0.65), R( 0.30), R( 0.05)) * RotateZ (R( 15.0)) * RotateY (R(15.0))), HC_OUT, HC_OUT);
	world->Insert (Cube (Scale (R( 0.10), R( 0.20), R( 0.15)) * Translate (R(-0.65), R( 0.30), R( 0.05)) * RotateZ (R(-15.0)) * RotateY (R(15.0))), HC_OUT, HC_OUT);

	//	elbows
	world->Insert (Cube (Scale (R( 0.125), R( 0.05), R( 0.175))  * Translate (R( 0.65), R( 0.10), R( 0.05)) * RotateZ (R( 15.0)) * RotateY (R(15.0))), HC_OUT, HC_OUT);
	world->Insert (Cube (Scale (R( 0.125), R( 0.05), R( 0.175)) * Translate (R(-0.65), R( 0.10), R( 0.05)) * RotateZ (R(-15.0)) * RotateY (R(15.0))), HC_OUT, HC_OUT);

	//	forearms
	world->Insert (Cube (Scale (R( 0.05), R( 0.2), R( 0.10))  * Translate (R( 0.65), R(-0.15), R( 0.05)) * RotateZ (R( 15.0)) * RotateY (R(15.0))), HC_OUT, HC_OUT);
	world->Insert (Cube (Scale (R( 0.05), R( 0.2), R( 0.10)) * Translate (R(-0.65), R(-0.15), R( 0.05)) * RotateZ (R(-15.0)) * RotateY (R(15.0))), HC_OUT, HC_OUT);

	//	hands
	world->Insert (Cube (Scale (R( 0.1), R( 0.1), R( 0.15))  * Translate (R( 0.65), R(-0.25), R( 0.05)) * RotateZ (R( 15.0)) * RotateY (R(15.0))), HC_OUT, HC_OUT);
	world->Insert (Cube (Scale (R( 0.1), R( 0.1), R( 0.15)) * Translate (R(-0.65), R(-0.25), R( 0.05)) * RotateZ (R(-15.0)) * RotateY (R(15.0))), HC_OUT, HC_OUT);

	//	leg pivot
	world->Insert (Cube (Scale (R( 0.4), R( 0.05), R( 0.05)) * Translate (R( 0.0), R(-0.125), R( 0.0)) * RotateY (R(15.0))), HC_OUT, HC_OUT);

	//	thighs
	world->Insert (Cube (Scale (R( 0.125), R( 0.25), R( 0.1)) * Translate (R( 0.25), R(-0.25), R( 0.0)) * RotateY (R(15.0))), HC_OUT, HC_OUT);
	world->Insert (Cube (Scale (R( 0.125), R( 0.25), R( 0.1)) * Translate (R(-0.25), R(-0.25), R( 0.0)) * RotateY (R(15.0))), HC_OUT, HC_OUT);

	//	knees
	world->Insert (Cube (Scale (R( 0.15), R( 0.1), R( 0.15)) * Translate (R( 0.25), R(-0.4), R( 0.0)) * RotateY (R(15.0))), HC_OUT, HC_OUT);
	world->Insert (Cube (Scale (R( 0.15), R( 0.1), R( 0.15)) * Translate (R(-0.25), R(-0.4), R( 0.0)) * RotateY (R(15.0))), HC_OUT, HC_OUT);

	//	calfs
	world->Insert (Cube (Scale (R( 0.075), R( 0.5), R( 0.075)) * Translate (R( 0.25), R(-0.4), R( 0.0)) * RotateY (R(15.0))), HC_OUT, HC_OUT);
	world->Insert (Cube (Scale (R( 0.075), R( 0.5), R( 0.075)) * Translate (R(-0.25), R(-0.4), R( 0.0)) * RotateY (R(15.0))), HC_OUT, HC_OUT);

	//	feet
	world->Insert (Cube (Scale (R( 0.15), R( 0.1), R( 0.25)) * Translate (R( 0.25), R(-0.85), R( 0.0)) * RotateY (R(15.0))), HC_OUT, HC_OUT);
	world->Insert (Cube (Scale (R( 0.15), R( 0.1), R( 0.25)) * Translate (R(-0.25), R(-0.85), R( 0.0)) * RotateY (R(15.0))), HC_OUT, HC_OUT);

	//	clean up intersections
	world->Reduce ();


	//	put the stupid light in
	point_3d		points[4];
	matrix_3d		rot = RotateY (R(45.0));
	points[0] = point_3d (R( 1.0), R( 0.25), R( 2.0)) * rot;
	points[1] = point_3d (R(-1.0), R( 0.25), R( 2.0)) * rot;
	points[2] = point_3d (R(-1.0), R( 3.0), R( 0.5)) * rot;
	points[3] = point_3d (R( 1.0), R( 3.0), R( 0.5)) * rot;
	polyptr			light (points, 4, 0, 1, 2, 3);
	light->SetEmissivity (LIGHT_INTENSITY);
	listptr			polylist;
	polylist->AddToList (light);
	world->Insert (polylist, HC_OUT, HC_OUT);

	#endif

	//	make the display window
	view v (IMAGE_SIZE, IMAGE_SIZE);
	return 0;
}


//------------------------------------------------------------------------------
