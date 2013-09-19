//------------------------------------------------------------------------------
//	File:					view.cp
//	Date:					05/04/97
//	Author:				Bretton Wade
//
//	Description:	this file contains the class methods for a view of space
//
//------------------------------------------------------------------------------

#include	"precompile.h"
#include	"view.h"

#include <time.h>

#include	"bsptree_3d.h"
#include	"transform_3d.h"
#include	"iterator_3d.h"
#include	"utility.h"
#include	"control.h"
//#include	"fpifencode.h"
#include	<stdio.h>

//------------------------------------------------------------------------------
//	global variables
//------------------------------------------------------------------------------
extern		bsptree	*world;																												//	the world is not defined here...
listptr		gWorldList;
listptr		gLights;
char		gTime[32];

//------------------------------------------------------------------------------
//	convert a time in seconds to a hh:mm:ss string
//------------------------------------------------------------------------------
char*	TimeToString (time_t t)
{
	time_t	minutes = t / 60,
			hours = minutes / 60,
			seconds = t - (minutes * 60);
	minutes -= hours * 60;
	sprintf (gTime, "%02d:%02d:%02d", hours, minutes, seconds);
	return gTime;
}

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
	/*
	time_t	seed;
	time (&seed);
	srand (seed);
	*/
	BuildLightList ();
	if (TRACE_PARTICLES)
		TraceParticles ();
	TraceScene ("image.raw");
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
//	find a light source in a uniform distribution
//------------------------------------------------------------------------------
void		view::BuildLightList (void)
{
	int			poly_count = 0,
				light_count = 0;
	gWorldList = world->TreeToList ();																					//	convert the bsp tree to a list
	iterator	all_entry (gWorldList);																												//	list iterator
	total_power = R(0.0);
	for (polyptr poly = all_entry (); poly (); poly = all_entry ())											//	do for all the entries in the list
	{
		poly_count++;
		if (poly->Emissivity () > R(0.0))
		{
			light_count++;
			real	power = poly->Emissivity () * poly->Area ();
			total_power += power;
			gLights->AddToList (poly);
		}
	}
	std::cerr << poly_count << " polygons" << std::endl;
	std::cerr << light_count << " lights" << std::endl;
	std::cerr << "  Total power " << total_power << " Watts" << std::endl << std::endl;
}

//------------------------------------------------------------------------------
//	find a light source in a uniform distribution
//------------------------------------------------------------------------------
polyptr		view::GetLight (real& s)
{
    real		sum = R(0.0);
    iterator	iter (gLights);
	polyptr		last_poly;
	polyptr     poly;
    for (poly = iter (); poly () && (s > sum + poly->little_power); poly = iter ())											//	do for all the entries in the list
	{
		last_poly = poly;
		sum += poly->little_power;
	}
	if (poly ())
	{
		s = (s - sum) / poly->little_power;
		return poly;
	}
	else
	{
		s = (s - (sum - last_poly->little_power)) / last_poly->little_power;
		return last_poly;
	}
}

//------------------------------------------------------------------------------
//	Trace a particle through the scene
//------------------------------------------------------------------------------
void		view::TraceFirstParticle (const ray &trace, real power)											//	trace a particle
{																																								//	begin
	point_3d	ipt;																																//	the intersection point of the ray on a polygon
	polyptr		poly_hit;
	current_rays++;																														//	the polygon hit by the ray
	if (world->RayIntersection (trace, poly_hit, ipt))														//	if a polygon is intersected by the ray
	{																																							//	begin
		#if DIRECT_ILLUMINATION || (INDIRECT_ILLUMINATION && PERFORM_GATHER)
		poly_hit->AddPower (power);
		#endif
		#if INDIRECT_ILLUMINATION
		if (UnitRandom () < DIFFUSE_REFLECT)
		{
			vector_3d	reflect = poly_hit->SampleVector ();
			TraceSubsequentParticle (ray (ipt + (reflect * EPSILON), reflect), power);
		}
		#endif
	}																																							//	end
}																																								//	end

//------------------------------------------------------------------------------
//	Trace a particle through the scene
//------------------------------------------------------------------------------
void		view::TraceSubsequentParticle (const ray &trace, real power)											//	trace a particle
{																																								//	begin
	point_3d	ipt;																																//	the intersection point of the ray on a polygon
	polyptr		poly_hit;
	current_rays++;																														//	the polygon hit by the ray
	if (world->RayIntersection (trace, poly_hit, ipt))														//	if a polygon is intersected by the ray
	{																																							//	begin
		poly_hit->AddPower (power);
		if (UnitRandom () < DIFFUSE_REFLECT)
		{
			vector_3d	reflect = poly_hit->SampleVector ();
			TraceSubsequentParticle (ray (ipt + (reflect * EPSILON), reflect), power);
		}
	}																																							//	end
}																																								//	end

//------------------------------------------------------------------------------
//	Trace all particles through the scene
//------------------------------------------------------------------------------
void		view::TraceParticles (void)																							//	trace particles from all the light sources
{																																								//	begin
	int			total_particles = 0,
				particle_count = 0;
	iterator	entry (gLights);																												//	list iterator
	polyptr poly;
	for (poly = entry (); poly (); poly = entry ())											//	do for all the entries in the list
	{
		real	power = poly->Emissivity () * poly->Area ();
		poly->little_power = power / total_power;
		total_particles += int (power / PARTICLE_POWER);
	}
	int	max_rays = 0,
		total_rays = 0;
	time_t	start_time,
			last_time = 0,
			current_time;
	entry.Reset ();
	std::cerr << "Tracing " << total_particles << " particles..." << std::endl << "Working";
	time (&start_time);
	for (poly = entry (); poly (); poly = entry ())											//	do for all the entries in the list
	{
		real	    power = poly->Emissivity () * poly->Area ();
		int	    num_particles = int (power / PARTICLE_POWER);
		for (int i = 0; i < num_particles; i++)
		{
			current_rays = 0;
			TraceFirstParticle (ray (poly->SamplePoint (), poly->SampleVector ()), PARTICLE_POWER);
			if (current_rays > max_rays)
			    max_rays = current_rays;
			total_rays += current_rays;
			particle_count++;
		}
		    time (&current_time);
			if (current_time != last_time)
			{
				real	delta_time = real (current_time - start_time),
						completion = real (particle_count) / real (total_particles),
						total_time = delta_time / completion;
				std::cerr << "\010\010\010\010\010\010\010\010\010\010" << TimeToString (time_t (total_time - delta_time));
				last_time = current_time;
			}
	}																																							//	end
	std::cerr << "\010\010\010\010\010\010\010\010\010\010Complete. " << std::endl;
	time (&current_time);
	std::cerr << "Elapsed time: " << TimeToString (current_time - start_time) << std::endl << std::endl;
	std::cerr << std::endl << "Statistics for particles" << std::endl <<
		"------------------------" << std::endl <<
		"Maximum rays traced:       " << max_rays << std::endl <<
		"Total rays traced:         " << total_rays << std::endl <<
		"Average rays per particle: " << (real (total_rays) / real (total_particles)) << std::endl << std::endl;
}																																								//	end

//------------------------------------------------------------------------------
//	Trace a ray through the scene
//------------------------------------------------------------------------------
real		view::TraceFirstRay (const ray &trace)																	//	trace a ray from the eye point
{																																								//	begin
	real		radiosity = R(0.0);																									//	start with zero intensity
	point_3d	ipt;																																//	the intersection point of the ray on a polygon
	polyptr		poly;																																//	the polygon hit by the ray
	current_rays++;
	#if TRACE_FULL_PATH
	ray			rayToTrace (trace.Origin (), trace.Direction ());
	while (world->RayIntersection (rayToTrace, poly, ipt))																//	if a polygon is intersected by the ray
	{																																							//	begin
		#if EXPLICIT_LIGHT_SAMPLES
		radiosity += TraceLightSource (ipt, poly->Normal ()) * DIFFUSE_REFLECT;
		#else
		radiosity += poly->Emissivity ();
		#endif
		if (UnitRandom () <= DIFFUSE_REFLECT)
		{
			vector_3d	direction = poly->SampleVector ();
			rayToTrace = ray (ipt + (direction * EPSILON), direction);
			current_rays++;
		}
		else
			return radiosity;
	}																																							//	end
	#else
	if (world->RayIntersection (trace, poly, ipt))																//	if a polygon is intersected by the ray
	{																																							//	begin
		#if DIRECT_ILLUMINATION
		radiosity += poly->Emissivity ();																						//	add the emissive contribution from the hit polygon
		#endif
		#if PERFORM_GATHER
		radiosity += TraceSecondRay (ipt, poly) * DIFFUSE_REFLECT;
		//radiosity += TraceLightSource (ipt, poly->Normal ()) * DIFFUSE_REFLECT;
		#else
		radiosity += (poly->Power () / poly->Area ()) * DIFFUSE_REFLECT;							//	compute the radiance of the polygon
		#endif
	}																																							//	end
	#endif
	return radiosity;																															//	return the computed intensity
}																																								//	end

//------------------------------------------------------------------------------
//	Trace a ray through the scene
//------------------------------------------------------------------------------
real		view::TraceLightSource (const point_3d& origin_pt, const vector_3d& normal)																	//	trace a ray from the intersection point
{																																								//	begin
	real		radiosity = R(0.0);
	iterator	entry (gLights);																												//	list iterator
	for (polyptr light_poly = entry (); light_poly (); light_poly = entry ())											//	do for all the entries in the list
	{
		point_3d	light_target_pt = light_poly->SamplePoint ();
		vector_3d	light_direction = light_target_pt - origin_pt;
		real		r = light_direction.Norm ();
		vector_3d	direction = light_direction / r;
		real	    factorA = normal | direction,
					factorB = light_poly->Normal () | -direction;
		if ((factorA > R(0.0)) && (factorB > R(0.0)))
		{
			ray			trace (origin_pt + (direction * EPSILON), direction);
			polyptr	    hit_poly;																																//	the polygon hit by the ray
			point_3d    ipt;																																//	the intersection point of the ray on a polygon
			if (world->RayIntersection (trace, hit_poly, ipt))																//	if a polygon is intersected by the ray
				if (hit_poly () == light_poly ())
					radiosity += light_poly->Emissivity () * light_poly->Area () * (factorA * factorB) / (r * r);
		}
	}
	return radiosity / PI;
}																																								//	end

//------------------------------------------------------------------------------
//	Trace a ray through the scene
//------------------------------------------------------------------------------
real		view::TraceSecondRay (const point_3d& origin_pt, const polyptr& vispoly)																	//	trace a ray from the intersection point
{																																								//	begin
	//point_3d	phong = Phong (UnitRandom (), UnitRandom ());
	//matrix_3d	transform = VectorMatrix (plane_3d (normal, origin_pt));
	//vector_3d	direction = phong * transform;
	vector_3d	direction = vispoly->SampleVector ();
	ray			trace (origin_pt + (direction * EPSILON), direction);
	polyptr		poly;																																//	the polygon hit by the ray
	point_3d    ipt;																																//	the intersection point of the ray on a polygon
	current_rays++;
	if (world->RayIntersection (trace, poly, ipt))																//	if a polygon is intersected by the ray
	{
		real	//factor = vispoly->Normal () | direction,			//	cos theta
				B;
		#if DIRECT_ILLUMINATION
		B = poly->Emissivity ();
		#else
		B = R(0.0);
		#endif
		#if INDIRECT_ILLUMINATION
		B += (poly->Power () / poly->Area ()) * DIFFUSE_REFLECT;
		#endif
		return B;// * factor;
	}
	return R(0.0);
}																																								//	end

//------------------------------------------------------------------------------
//	Ray trace the whole scene
//------------------------------------------------------------------------------
void		view::TraceScene (char *filename)																				//	ray trace the scene
{																																								//	begin
	int		total_pixels = width * height;
	real	*output_values = new real[total_pixels],
			**rows = new real*[height],
			*sample_counts = new real[total_pixels],
			**sample_rows = new real*[height],
			max_samples = R(0.0), min_samples = R(1.0e8),
			total_samples = R(0.0);
	for (int i = 0; i < height; i++)
	{
		rows[i] = output_values + (i * width);
		sample_rows[i] = sample_counts + (i * width);
	}
	time_t	start_time,
			current_time;
	int		max_rays = 0,
			total_rays = 0,
			pixel_counter = 0,
			pass = 1;

	std::cerr << "Tracing " << width << "x" << height << " image" << std::endl << pass;
	time (&start_time);

	for (int y = 0; y < height; y++)																							//	loop over all of the rows of pixels
	{																																							//	begin
		real	*row = rows[y],
				*sample_row = sample_rows[y];
		for (int x = 0; x < width; x++)																							//	loop over all of the pixels in the row
		{																																						//	begin
			current_rays = 0;

			// explicitly trace the first ray
			point_2d	pt = dctovdc (x + UnitRandom (), y + UnitRandom ());				//	compute the location of the sample in screen space
			point_3d	pt3 = point_3d (pt[X], pt[Y], R(0.0)) * cam.Inverse ();			//	compute the location of the sample point in world space
			ray			trace (cam.Eye (), pt3 - cam.Eye ());												//	compute the ray from the eye point through the sample location
			row[x] = TraceFirstRay (trace);														//	trace the ray from the eye point through the sample location
			sample_row[x] = R(1.0);

			// now loop until the variance drops below our allowance
			real	deviance;
			do
			{
				point_2d	pt = dctovdc (x + UnitRandom (), y + UnitRandom ());				//	compute the location of the sample in screen space
				point_3d	pt3 = point_3d (pt[X], pt[Y], R(0.0)) * cam.Inverse ();			//	compute the location of the sample point in world space
				ray			trace (cam.Eye (), pt3 - cam.Eye ());												//	compute the ray from the eye point through the sample location
				real		sample = TraceFirstRay (trace);														//	trace the ray from the eye point through the sample location
				real		expected_value = row[x] / sample_row[x];
				//deviance = FABS (sample - expected_value) / sample_row[x];
				deviance = 0;
				row[x] += sample;														
				sample_row[x] += R(1.0);
			}
			while ((deviance > ALLOWABLE_DEVIANCE) || (sample_row[x] < MIN_SAMPLES));

			if (current_rays > max_rays)
				max_rays = current_rays;
			total_rays += current_rays;

			time (&current_time);
			pixel_counter++;
			real	delta_time = real (current_time - start_time),
					total_time = (delta_time * real (total_pixels)) / real (pixel_counter);
			std::cerr << "\010\010\010\010\010\010\010\010\010\010\010\010\010\010\010\010\010\010\010\010\010\010\010\010\010\010\010\010\010\010\010\010";
			std::cerr << pixel_counter << "/" << total_pixels << " ";
			std::cerr << TimeToString (time_t (total_time - delta_time));
			std::cerr << "/" << TimeToString (time_t (total_time));
		}
	}																																							//	end
	time (&current_time);
	std::cerr << "\010\010\010\010\010\010\010\010\010\010\010\010\010\010\010\010\010\010\010\010\010\010\010\010\010\010\010\010\010\010\010\010";
	std::cerr << "Elapsed time: " << TimeToString (current_time - start_time) << "                               " << std::endl << std::endl;

	// write out the results
	char	name[255];
	sprintf (name, "_image%03d.raw", pass);
	std::cerr << "Writing " << name << "..." << std::endl;
	FILE*	output_file = fopen (name, "wb");
	total_samples = R(0.0);
	for (int i = 0; i < total_pixels; i++)
	{
		real			pixel_value = output_values[i] / sample_counts[i];
		if (pixel_value > R(1.0))
			pixel_value = R(1.0);
		unsigned short	value = unsigned short (pixel_value * 65535.0);
		fwrite (&value, sizeof (unsigned short), 1, output_file);
		if (sample_counts[i] > max_samples)
			max_samples = sample_counts[i];
		if (sample_counts[i] < min_samples)
			min_samples = sample_counts[i];
		total_samples += sample_counts[i];
	}
	fclose (output_file);
	sprintf (name, "_imagesamples%03d.raw", pass);
	std::cerr << "Writing " << name << "..." << std::endl << std::endl;
	output_file = fopen (name, "wb");

	for (int i = 0; i < total_pixels; i++)
	{
		unsigned short	value = unsigned short ((sample_counts[i] / max_samples) * 65535.0);
		fwrite (&value, sizeof (unsigned short), 1, output_file);
	}
	fclose (output_file);

	std::cerr << std::endl << "Statistics for imaging" << std::endl <<
		"------------------------" << std::endl <<
		"Maximum rays traced:       " << max_rays << std::endl <<
		"Total rays traced:         " << total_rays << std::endl <<
		"Total image samples:       " << unsigned long (total_samples) << std::endl <<
		"Average rays per path:     " << (real (total_rays) / real (total_samples)) << std::endl <<
		"Minimum samples per pixel: " << min_samples << std::endl <<
		"Maximum samples per pixel: " << max_samples << std::endl <<
		"Average samples per pixel: " << (real (total_samples) / real (total_pixels)) << std::endl << std::endl;

	// clean up the memory
	delete[] sample_rows;
	delete[] sample_counts;
	delete[] rows;
	delete[] output_values;
}																																								//	end

//------------------------------------------------------------------------------
