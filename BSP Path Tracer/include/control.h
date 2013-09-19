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
#define	LIGHT_INTENSITY			R(5.0)	//	5.0 for cornell box
#define	DIFFUSE_REFLECT			R(0.65)
#define	PARTICLE_POWER			R(1.0e-6)// using 1.0e-5 for high quality computation

#define	ALLOWABLE_DEVIANCE		R(1.0e-4)
#define	TRACE_FULL_PATH			0	//	russian roulette path tracing
#define	PERFORM_GATHER			1	//	one hop path tracing, using a radiosity solution for information. The radiosity solution will be computed if this flag is set to 0, or if it is 1 and indirect illumination is needed
#define EXPLICIT_LIGHT_SAMPLES	1	//	whether or not to sample light sources explicitly
#define DIRECT_ILLUMINATION		1	//	whether or not to compute direct illumination, in the case of gathering only
#define INDIRECT_ILLUMINATION	1	//	whether or not to compute indirect illumination, in the case of gathering only

#define	IMAGE_SIZE				128
#define	MIN_SAMPLES				R(1.0e4)

#if TRACE_FULL_PATH
#define	TRACE_PARTICLES			0

#elif PERFORM_GATHER
#if INDIRECT_ILLUMINATION
#define	TRACE_PARTICLES			1
#else
#define	TRACE_PARTICLES			0
#endif

#else	//NOT PERFORM_GATHER
#define	TRACE_PARTICLES			1
#endif

#if TRACE_PARTICLES
#define	MAX_POLY_AREA			(R(1.0) / R(32.0))
//#define	MAX_POLY_AREA			(R(1.0) / R(8.0))
#else
#define	MAX_POLY_AREA			R(1.0e5)
#endif

//------------------------------------------------------------------------------

#endif //CONTROL
