//------------------------------------------------------------------------------
//	File:					view.cp
//	Date:					05/04/97
//	Author:				Bretton Wade
//
//	Description:	this file contains the class methods for a view of space
//
//------------------------------------------------------------------------------

#include <iostream.h>

#include	"precompile.h"
#include	"view.h"
#include	"bsptree_3d.h"
#include	"transform_3d.h"
#include	"iterator_3d.h"
#include	"utility.h"
#include	"control.h"
#include	"fpifencode.h"

//------------------------------------------------------------------------------
//	global variables
//------------------------------------------------------------------------------
extern		bsptree	*world;																												//	the world is not defined here...

//------------------------------------------------------------------------------
//	convert a screen location to the range (-1..1, -1..1)
//------------------------------------------------------------------------------
inline	point_2d	view::dctovdc (real x, real y) const													//	map floating point screen coordinates to virtual device coordinates
{																																								//	begin
	return point_2d ((x - xsize) * aspect, (y - ysize) * -aspect);								//	return the converted point_3d
}																																								//	end

//------------------------------------------------------------------------------
//	constructor
//------------------------------------------------------------------------------
view::view (int w, int h) : 																										//	view constructor
			cam (point_3d (R(0.0), R(0.0), R(4.0)), ORIGIN_3D, LensToFOV (60))				//	camera_3d constructor
{																																								//	begin
	width = w; height = h;																												//	copy the width and height
	ysize = height * R(0.5);																											//	compute the y halfsize
	xsize = width * R(0.5);																												//	compute the x halfsize
	aspect = R(1.0) / real ((width > height) ? ysize : xsize);										//	set the aspect to be the smaller of the two
	TraceParticles ();
	TraceScene ("image.flt");
}																																								//	end

//------------------------------------------------------------------------------
//	destructor
//------------------------------------------------------------------------------
view::~view (void)																															//	destructor
{																																								//	begin
}																																								//	end

//------------------------------------------------------------------------------
//	compute a phong distributed vector
//------------------------------------------------------------------------------
point_3d	view::Phong (real x, real y)																					//	return a vector from a phong distribution
{																																								//	begin
	real	root = SQRT (R(1.0) - x),
				u = ACOS (root),
				v = TWO_PI * y,
				sinu = SIN (u);
	return point_3d (sinu * COS (v), sinu * SIN (v), root);
}																																								//	end

//------------------------------------------------------------------------------
//	Trace a particle through the scene
//------------------------------------------------------------------------------
void		view::TraceParticle (const ray &trace, real power)											//	trace a particle
{																																								//	begin
	point_3d	ipt;																																//	the intersection point of the ray on a polygon
	polyptr		poly_hit;																														//	the polygon hit by the ray
	if (world->RayIntersection (trace, poly_hit, ipt))														//	if a polygon is intersected by the ray
	{																																							//	begin
		poly_hit->AddPower (power);
		if (UnitRandom () < DIFFUSE_REFLECT)
		{
			point_3d	phong = Phong (UnitRandom (), UnitRandom ());
			matrix_3d	transform = VectorMatrix (poly_hit->Plane ());
			vector_3d	reflect = phong * transform;
			TraceParticle (ray (ipt + (reflect * EPSILON), reflect), power);
		}
	}																																							//	end
}																																								//	end

//------------------------------------------------------------------------------
//	Trace all particles through the scene
//------------------------------------------------------------------------------
void		view::TraceParticles (void)																							//	trace particles from all the light sources
{																																								//	begin
	listptr	list = world->TreeToList ();																					//	convert the bsp tree to a list
	int			total_particles = 0,
					total_traced = 0,
					old_done = 100;
	iterator	entry (list);																												//	list iterator
	for (polyptr poly = entry (); poly (); poly = entry ())											//	do for all the entries in the list
		if (poly->Emissivity () > R(0.0))
		{
			real			power = poly->Emissivity () * poly->Area ();
			total_particles += int (power / PARTICLE_POWER);
		}
	cerr << "Tracing " << total_particles << " particles\n0%";
	entry.Reset ();
	for (poly = entry (); poly (); poly = entry ())											//	do for all the entries in the list
		if (poly->Emissivity () > R(0.0))
		{
			real			power = poly->Emissivity () * poly->Area ();
			int				num_particles = int (power / PARTICLE_POWER);
			matrix_3d	transform = VectorMatrix (poly->Plane ());
			for (int i = 0; i < num_particles; i++)
			{
				point_3d	phong = Phong (UnitRandom (), UnitRandom ());
				vector_3d	reflect = phong * transform;
				TraceParticle (ray (poly->Vertex (0), reflect), PARTICLE_POWER);
				int		done = (total_traced * 100) / total_particles;
				if (done != old_done)
				{
					cerr << "\010\010\010" << done << "%";
					old_done = done;
				}
				total_traced++;
			}
		}																																							//	end
	cerr << "\010\010\010100%...Complete.\n";
}																																								//	end

//------------------------------------------------------------------------------
//	Trace a ray through the scene
//------------------------------------------------------------------------------
real		view::TraceFirstRay (const ray &trace)																	//	trace a ray from the eye point
{																																								//	begin
	real			intensity = R(0.0);																									//	start with zero intensity
	point_3d	ipt;																																//	the intersection point of the ray on a polygon
	polyptr		poly;																																//	the polygon hit by the ray
	if (world->RayIntersection (trace, poly, ipt))																//	if a polygon is intersected by the ray
	{																																							//	begin
		#if TRACE_FULL_PATH
		point_3d	phong = Phong (UnitRandom (), UnitRandom ());
		matrix_3d	transform = VectorMatrix (poly->Plane ());
		vector_3d	reflect = phong * transform;
		intensity = TraceSecondRay (ray (ipt + (reflect * EPSILON), reflect));
		intensity *= DIFFUSE_REFLECT;
		intensity += poly->Emissivity ();																						//	add the emissive contribution from the hit polygon
		#else
		intensity = DIFFUSE_REFLECT * (poly->Power () / poly->Area ());							//	compute the radiance of the polygon
		intensity += poly->Emissivity ();																						//	add the emissive contribution from the hit polygon
		#endif
	}																																							//	end
	return intensity;																															//	return the computed intensity
}																																								//	end

//------------------------------------------------------------------------------
//	Trace a ray through the scene
//------------------------------------------------------------------------------
real		view::TraceSecondRay (const ray &trace)																	//	trace a ray from the intersection point
{																																								//	begin
	real			intensity = R(0.0);																									//	start with zero intensity
	point_3d	ipt;																																//	the intersection point of the ray on a polygon
	polyptr		poly;																																//	the polygon hit by the ray
	if (world->RayIntersection (trace, poly, ipt))																//	if a polygon is intersected by the ray
	{
		intensity = DIFFUSE_REFLECT * (poly->Power () / poly->Area ());							//	compute the radiance of the polygon
		intensity += poly->Emissivity ();																						//	add the emissive contribution from the hit polygon
	}
	return intensity;																															//	return the computed intensity
}																																								//	end

//------------------------------------------------------------------------------
//	Ray trace the whole scene
//------------------------------------------------------------------------------
void		view::TraceScene (char *filename)																				//	ray trace the scene
{																																								//	begin
	cerr << "Tracing " << width << "x" << height << " image\n0%";
	FPIFOutputStreamToOStream stream(cout);
  FPIFEncoder 							encoder(stream);
  encoder.SetSize (width, height);
  encoder.AddStandardChannel (FPIF::Float, FPIF::Intensity);
  FPIFImageEncoder 					imageEncoder(encoder);
  FPIFFloat 								*row = new FPIFFloat[width];
  FPIFFloatChannelEncoder 	YEncoder(imageEncoder, 0);
  YEncoder.SetBuffer(row);
	for (int y = 0; y < height; y++)																							//	loop over all of the rows of pixels
	{																																							//	begin
		for (int x = 0; x < width; x++)																							//	loop over all of the pixels in the row
		{																																						//	begin
			row[x] = R(0.0);																													//	start with a zero pixel intensity
			for (int u = 0; u < UV_SAMPLES; u++)																			//	loop over all of the u sample values in the pixel
				for (int v = 0; v < UV_SAMPLES; v++)																		//	loop over all of the v sample values in the pixel
				{																																				//	begin
					point_2d	pt = dctovdc (x + UnitRandom (), y + UnitRandom ());				//	compute the location of the sample in screen space
					point_3d	pt3 = point_3d (pt[X], pt[Y], R(0.0)) * cam.Inverse ();			//	compute the location of the sample point in world space
					ray				trace (cam.Eye (), pt3 - cam.Eye ());												//	compute the ray from the eye point through the sample location
					row[x] += TraceFirstRay (trace) * UV_SCALE;														//	trace the ray from the eye point through the sample location
				}																																				//	end
			if (row[x] > R(1.0))																											//	if the intensity is greater than one
				row[x] = R(1.0);																												//	clamp it
		}																																						//	end
    YEncoder.Encode();
    imageEncoder.NextRow();
		int		done = (y * 100) / height;
		cerr << "\010\010\010" << done << "%";
	}																																							//	end
	cerr << "\010\010\010100%...Complete.\n";
  delete[] row;
}																																								//	end

//------------------------------------------------------------------------------
