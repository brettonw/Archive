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

//------------------------------------------------------------------------------
bsptree	*world;

//------------------------------------------------------------------------------
int		main (int argc, char **argv)
{
	world = new bsptree;
	world->Insert (Cube (Scale (R(-1.0), R(-1.0), R(-1.0))), HC_OUT, HC_OUT);
	world->Insert (Cube (Scale (R(0.5), R(0.125), R(0.5)) * Translate (R(0.0), R(1.0), R(0.0)), LIGHT_INTENSITY), HC_OUT, HC_OUT);
	world->Insert (Cube (Scale (R(0.25), R(0.5), R(0.25)) * RotateY (R(15.0)) * Translate (R(-0.25), R(-0.5), R(-0.25))), HC_OUT, HC_OUT);
	world->Reduce ();
	//	make the display window
	view v (128, 128);
}


//------------------------------------------------------------------------------
