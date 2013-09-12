//------------------------------------------------------------------------------
//	File:					transform.cp
//	Date:					9/4/94
//	Author:				Bretton Wade
//
//	Description:	this file contains methods for the transformations matrices
//
//------------------------------------------------------------------------------

#include "precompile.h"
#include "utility.h"
#include "transform_3d.h"

//------------------------------------------------------------------------------
//	Transformation to rotate a point_3d about the x axis
//------------------------------------------------------------------------------
matrix_3d	RotateX (real angle)																									//	build a transformation matrix_3d
{																																								//	begin
	angle = DegreesToRadians (angle);																							//	convert degrees to radians
	real	cosine = COS (angle),																										//	compute the cosine of the angle
				sine = SIN (angle);																											//	compute the sine of the angle
	matrix_3d	m (matrix_3d::identity);																						//	new identity matrix_3d
	m (1, 1) = cosine;																														//	scale the y rotation by the cosine
	m (2, 2) = cosine;																														//	scale the z rotation by the cosine
	m (2, 1) = -sine;																															//	scale the y rotation by the -sine
	m (1, 2) = sine;																															//	scale the z rotation by the sine
	return m;																																			//	return the matrix_3d
}																																								//	end

//------------------------------------------------------------------------------
//	Transformation to rotate a point_3d about the y axis
//------------------------------------------------------------------------------
matrix_3d	RotateY (real angle)																									//	build a transformation matrix_3d
{																																								//	begin
	angle = DegreesToRadians (angle);																							//	convert degrees to radians
	real	cosine = COS (angle),																										//	compute the cosine of the angle
				sine = SIN (angle);																											//	compute the sine of the angle
	matrix_3d	m (matrix_3d::identity);																						//	new identity matrix_3d
	m (0, 0) = cosine;																														//	scale the x rotation by the cosine
	m (2, 2) = cosine;																														//	scale the z rotation by the cosine
	m (2, 0) = sine;																															//	scale the x rotation by the sine
	m (0, 2) = -sine;																															//	scale the z rotation by the -sine
	return m;																																			//	return the matrix_3d
}																																								//	end

//------------------------------------------------------------------------------
//	Transformation to rotate a point_3d about the z axis
//------------------------------------------------------------------------------
matrix_3d	RotateZ (real angle)																									//	build a transformation matrix_3d
{																																								//	begin
	angle = DegreesToRadians (angle);																							//	convert degrees to radians
	real	cosine = COS (angle),																										//	compute the cosine of the angle
			sine = SIN (angle);																												//	compute the sine of the angle
	matrix_3d	m (matrix_3d::identity);																						//	new identity matrix_3d
	m (0, 0) = cosine;																														//	scale the x rotation by the cosine
	m (1, 1) = cosine;																														//	scale the y rotation by the cosine
	m (1, 0) = -sine;																															//	scale the x rotation by the -sine
	m (0, 1) = sine;																															//	scale the y rotation by the sine
	return m;																																			//	return the matrix_3d
}																																								//	end

//------------------------------------------------------------------------------
//	Transformation to translate a point_3d by a specified amount
//------------------------------------------------------------------------------
matrix_3d	Translate (real x, real y, real z)																		//	build a transformation matrix_3d
{																																								//	begin
	matrix_3d	m (matrix_3d::identity);																						//	new identity matrix_3d
	m (3, 0) = x;																																	//	translate the x axis
	m (3, 1) = y;																																	//	translate the y axis
	m (3, 2) = z;																																	//	translate the z axis
	return m;																																			//	return the matrix_3d
}																																								//	end

//------------------------------------------------------------------------------
//	Transformation to scale a point_3d by a specified amount
//------------------------------------------------------------------------------
matrix_3d	Scale (real x, real y, real z)																				//	build a transformation matrix_3d
{																																								//	begin
	matrix_3d	m (matrix_3d::identity);																						//	new identity matrix_3d
	m (0, 0) = x;																																	//	scale the x axis
	m (1, 1) = y;																																	//	scale the y axis
	m (2, 2) = z;																																	//	scale the z axis
	return m;																																			//	return the matrix_3d
}																																								//	end

//------------------------------------------------------------------------------
//	Transformation to produce a perspective projection
//------------------------------------------------------------------------------
matrix_3d	Perspective (real distance)																						//	build a transformation matrix_3d
{																																								//	begin
	matrix_3d	m (matrix_3d::identity);																						//	new identity matrix_3d
	m (2, 3) = R(-1.0) / distance;																								//	set the perspective factor to 1.0 / distance to the picture plane_3d
	return m;																																			//	return the matrix_3d
}																																								//	end

//------------------------------------------------------------------------------
//	Transformation for rotating to an arbitrary normalized basis
//------------------------------------------------------------------------------
matrix_3d	VectorMatrix (tuple_3d &n)																						//	build a vector_3d rotation matrix_3d
{																																								//	begin
	vector_3d	t (n),																															//	copy it
						w (n);
	t[w.MinorAxis ()] = R(1.0);																										//	set the minor axis to 1.0
	vector_3d	u = (t ^ w).Normalize (),																						//	compute a normal perpendicular vector_3d to w and t
					v = w ^ u;																														//	compute a normal perpendicular vector_3d to w and u
	matrix_3d	m (matrix_3d::identity);																						//	new identity matrix_3d
	m (0, 0) = u[X];																															//	assign the first row
	m (0, 1) = u[Y];
	m (0, 2) = u[Z];
	
	m (1, 0) = v[X];																															//	assign the second row
	m (1, 1) = v[Y];
	m (1, 2) = v[Z];
	
	m (2, 0) = w[X];																															//	assign the third row
	m (2, 1) = w[Y];
	m (2, 2) = w[Z];
	return m;																																			//	return the matrix_3d
}																																								//	end

//------------------------------------------------------------------------------
//	Transformation for rotating to an arbitrary basis at an arbitrary location
//------------------------------------------------------------------------------
matrix_3d	ViewMatrix (const vector_3d &u, const vector_3d &v, const vector_3d &n, const point_3d &r)	//	build a viewing matrix_3d from a vector_3d set
{																																								//	begin
	matrix_3d	m;																																	//	new matrix_3d
	m (0, 0) = u[X];																															//	assign the first column
	m (1, 0) = u[Y];
	m (2, 0) = u[Z];
	m (3, 0) = -(r | u);
	
	m (0, 1) = v[X];																															//	assign the second column
	m (1, 1) = v[Y];
	m (2, 1) = v[Z];
	m (3, 1) = -(r | v);
	
	m (0, 2) = n[X];																															//	assign the third column
	m (1, 2) = n[Y];
	m (2, 2) = n[Z];
	m (3, 2) = -(r | n);
	
	m (0, 3) = R(0.0);																														//	assign the fourth column
	m (1, 3) = R(0.0);
	m (2, 3) = R(0.0);
	m (3, 3) = R(1.0);
	return m;																																			//	return the matrix_3d
}																																								//	end

//------------------------------------------------------------------------------
