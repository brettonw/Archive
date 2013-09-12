//------------------------------------------------------------------------------
//	File:					camera.h
//	Date:					9/4/94
//	Author:				Bretton Wade
//
//	Description:	this file contains the class definition for a camera
//
//------------------------------------------------------------------------------

#include "point_3d.h"
#include "matrix_3d.h"

#ifndef	CAMERA
#define	CAMERA

//------------------------------------------------------------------------------
//	classes
//------------------------------------------------------------------------------
class	camera																																		//	camera class
{																																								//	begin camera class definition
	private:																																			//	private interface
	protected:																																		//	protected interface
			point_3d	eye;																														//	eye location
			matrix_3d	viewing, inverse;																								//	the viewing matrix_3d suite
	public:																																				//	public interface
			camera (const point_3d &eye, const point_3d &to, real fov);								//	default constructor
			void			Look (const point_3d &eye, const point_3d &to, real fov);				//	set the camera location and viewing direction
const matrix_3d	&Transform (void) const;																				//	return a reference to the transformation matrix_3d
const matrix_3d	&Inverse (void) const;																					//	return a reference to the inverse transformation matrix_3d
const	point_3d	&Eye (void) const;																							//	return a reference to the eye point_3d
};																																							//	end camera class definition

//------------------------------------------------------------------------------
//	inlines
//------------------------------------------------------------------------------
inline	const matrix_3d	&camera::Transform (void) const													//	return a reference to the transformation matrix_3d
{																																								//	begin
	return viewing;																																//	return the matrix_3d
}																																								//	end

inline	const matrix_3d	&camera::Inverse (void) const														//	return a reference to the inverse transformation matrix_3d
{																																								//	begin
	return inverse;																																//	return the matrix_3d
}																																								//	end

inline	const point_3d	&camera::Eye (void) const																//	return a reference to the eye point_3d
{																																								//	begin
	return eye;																																		//	return the point_3d
}																																								//	end

//------------------------------------------------------------------------------
//	functions
//------------------------------------------------------------------------------
real	LensToFOV (int focal_length = 50, real film_size = 43.26661531);					//	compute the fov from a film size (measured diagonally) and lens focal length

//------------------------------------------------------------------------------

#endif	//CAMERA
