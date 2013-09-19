//------------------------------------------------------------------------------
//	File:					control.h
//	Date:					06/04/97
//	Author:				Bretton Wade
//
//	Description:	this file contains the control macros needed for image generation
//
//------------------------------------------------------------------------------

#ifndef CONTROL
#define CONTROL

//------------------------------------------------------------------------------
//	macros
//------------------------------------------------------------------------------
#define	LIGHT_INTENSITY		R(5.0)
#define	DIFFUSE_REFLECT		R(0.6)
#define	PARTICLE_POWER		R(1.0e-6)
#define	UV_SAMPLES				64
#define	UV_GRID						(R(1.0) / UV_SAMPLES)
#define	UV_SCALE					(R(1.0) / (UV_SAMPLES * UV_SAMPLES))
#define	TRACE_FULL_PATH		1

//------------------------------------------------------------------------------

#endif //CONTROL
