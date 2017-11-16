//------------------------------------------------------------------------------
//	File:					camera.cp
//	Date:					9/4/94
//	Author:				Bretton Wade
//
//	Description:	this file contains the methods for a camera
//
//------------------------------------------------------------------------------

#include "precompile.h"
#include "utility.h"
#include "vector_3d.h"
#include "camera_3d.h"
#include "transform_3d.h"

//------------------------------------------------------------------------------
//	constructor
//------------------------------------------------------------------------------
camera::camera (const point_3d &eye, const point_3d &to, real fov)							//	default constructor
{																																								//	begin
	Look (eye, to, fov);																													//	set the camera view as requested
}																																								//	end

//------------------------------------------------------------------------------
//	Set the camera location and viewing direction
//------------------------------------------------------------------------------
void	camera::Look (const point_3d &e, const point_3d &to, real fov)						//	assign the viewing parameters
{																																								//	begin
	eye = e;																																			//	copy the eye point_3d
    real tanfov = TAN (degreesToRadians (fov * R (0.5)));												//	compute the tangent of the field of view half-angle
	real distance = R(1.0) / tanfov;																						//	compute the distance from the eye to the view plane_3d
	vector_3d	vpn = (eye - to).Normalize (),																			//	view is vector_3d from eye, to
					u = (vector_3d (R(0.0), R(1.0), R(0.0)) ^ vpn).Normalize (),					//	calculate the x' axis vector_3d
					v = vpn ^ u;																													//	calculate the y' axis vector_3d
	point_3d		vrp = eye + (vpn * -distance);																		//	compute the view reference point_3d, along the view plane_3d normal vector_3d
	viewing = ViewMatrix (u, v, vpn, vrp) * Perspective (distance);								//	set up the viewing transformation matrix_3d
	inverse = viewing.Inverse ();																									//	set up the inverse transformation matrix_3d
}																																								//	end

//------------------------------------------------------------------------------
//	compute the fov from a lens focal length and film size
//------------------------------------------------------------------------------
real	LensToFOV (int focal_length, real film_size)															//	compute the fov from a film size and lens focal length
{																																								//	begin
	real	fov = R(2.0) * ATAN2 (film_size * R(0.5) , real (focal_length));				//	compute the fov angle in radians
	return radiansToDegrees (fov);																								//	convert the fov to degrees
}																																								//	end

//------------------------------------------------------------------------------
