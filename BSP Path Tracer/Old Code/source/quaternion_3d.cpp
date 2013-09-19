//------------------------------------------------------------------------------
//	File:					quaternion.cp
//	Date:					9/18/94
//	Author:				Bretton Wade
//
//	Description:	this file contains the methods for a quaternion
//
//------------------------------------------------------------------------------

#include "precompile.h"
#include "quaternion_3d.h"

//------------------------------------------------------------------------------
//	Transformation matrix_3d from a unit quaternion 
//------------------------------------------------------------------------------
matrix_3d	QuaternionMatrix (real x, real y, real z, real w)											//	build a transformation matrix_3d from a unit quaternion
{																																								//	begin
	matrix_3d	m;																																	//	new matrix_3d
	m (0, 0) = w*w + x*x - y*y - z*z;																							//	assign the first column
	m (0, 1) = R(2.0) * (x*y + w*z);
	m (0, 2) = R(2.0) * (x*z - w*y);
	m (0, 3) = R(0.0);
	m (1, 0) = R(2.0) * (x*y - w*z);																							//	assign the second column
	m (1, 1) = w*w - x*x + y*y - z*z;
	m (1, 2) = R(2.0) * (y*z + w*x);
	m (1, 3) = R(0.0);
	m (2, 0) = R(2.0) * (x*z + w*y);																							//	assign the third column
	m (2, 1) = R(2.0) * (y*z - w*x);
	m (2, 2) = w*w - x*x - y*y + z*z;
	m (2, 3) = R(0.0);
	m (3, 0) = R(0.0);																														//	assign the fourth column
	m (3, 1) = R(0.0);
	m (3, 2) = R(0.0);
	m (3, 3) = w*w + x*x + y*y + z*z;
	return m;																																			//	return the matrix_3d
}																																								//	end

//------------------------------------------------------------------------------
//	make a quaternion from two vectors, and return the resulting matrix_3d
//------------------------------------------------------------------------------
matrix_3d	Quaternion (const vector_3d &begin, const vector_3d &end)							//	map a quaternion to a transformation matrix_3d
{																																								//	begin
	real		w = begin | end;																											//	the quaternion values, w is cosine (theta / 2), where theta is rotation angle
	vector_3d	perp = begin ^ end;																									//	compute the vector_3d perpendicular to the begin and end vectors
	real	norm = perp.Norm ();																										//	compute the length of the perpendicular vector_3d
	if (norm < EPSILON)																														//	if the length of the perpendicular vector_3d is 0
		return IDENTITY_MATRIX;																											//	the 'begin' and 'end' vectors coincide, so return an identity transformation
//#define	HALF_SPEED_QUATERNION
#ifdef	HALF_SPEED_QUATERNION
	real theta = ACOS (w) / R(2.0);																								//	compute the half angle of the rotation between 'begin' and 'end'
	w = COS (theta);																															//	set the w coefficient to the cosine of the half angle
	perp = (perp / norm) * SIN (theta);																						//	normalize the perpendicular vector_3d, and scale it by the sine of the half angle
#endif
	return QuaternionMatrix (perp[X], perp[Y], perp[Z], w);												//	return the quaternion converted to a matrix_3d
}																																								//	end

//------------------------------------------------------------------------------
