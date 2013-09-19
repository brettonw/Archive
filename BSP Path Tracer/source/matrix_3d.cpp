//------------------------------------------------------------------------------
//	File:					matrix_3d.cp
//	Date:					9/2/94
//	Author:				Bretton Wade
//
//	Description:	this file contains methods for the matrix_3d class
//
//------------------------------------------------------------------------------

#include "precompile.h"
#include "utility.h"
#include "matrix_3d.h"

//------------------------------------------------------------------------------
//	global variables
//------------------------------------------------------------------------------
real	matrix_3d::identity[4][4] = {{R(1.0), R(0.0), R(0.0), R(0.0)},						//	array for identity matrix_3d
																	{R(0.0), R(1.0), R(0.0), R(0.0)},
																	{R(0.0), R(0.0), R(1.0), R(0.0)},
																	{R(0.0), R(0.0), R(0.0), R(1.0)}};

matrix_3d	IDENTITY_MATRIX (matrix_3d::identity);																//	an identity matrix_3d

//------------------------------------------------------------------------------
//	constructor
//------------------------------------------------------------------------------
matrix_3d::matrix_3d (const matrix_3d &m)																				//	copy constructor
{																																								//	begin
	for (short i = 0; i < 4; i++)																									//	loop on the rows
		for (short j = 0; j < 4; j++)																								//	loop on the columns
			mat[i][j] = m.mat[i][j];																									//	copy the matrix_3d values
}																																								//	end

//------------------------------------------------------------------------------
//	constructor
//------------------------------------------------------------------------------
matrix_3d::matrix_3d (real f[4][4])																							//	direct assignment constructor
{																																								//	begin
	for (short i = 0; i < 4; i++)																									//	loop on the rows
		for (short j = 0; j < 4; j++)																								//	loop on the columns
			mat[i][j] = f[i][j];																											//	copy the values
}																																								//	end

//------------------------------------------------------------------------------
//	assignment
//------------------------------------------------------------------------------
void	matrix_3d::operator = (const matrix_3d &m)																//	assignment operator
{																																								//	begin
	for (short i = 0; i < 4; i++)																									//	loop on the rows
		for (short j = 0; j < 4; j++)																								//	loop on the columns
			mat[i][j] = m.mat[i][j];																									//	copy the other matrx values
}																																								//	end

//------------------------------------------------------------------------------
//	multiplication
//------------------------------------------------------------------------------
matrix_3d	matrix_3d::operator * (const matrix_3d &m) const											//	multiplication operator
{																																								//	begin
	matrix_3d result;																															//	a new matrix_3d
	for (short i = 0; i < 4; i++)																									//	loop on the rows
		for (short j = 0; j < 4; j++)																								//	loop on the columns
		{																																						//	begin
			tuple_3d	r (mat[i][0], mat[i][1], mat[i][2], mat[i][3]),									//	the ith roww
						c (m.mat[0][j], m.mat[1][j], m.mat[2][j], m.mat[3][j]);							//	the jth column
			result.mat[i][j] = r | c;																									//	the inner product
		}																																						//	end
	return result;																																//	return the result
}																																								//	end

//------------------------------------------------------------------------------
//	compute cofactor of 3x3 matrix_3d reduced from entry (i, j)
//------------------------------------------------------------------------------
real	matrix_3d::Cofactor (int i, int j) const																	//	compute the cofactor for the given entry
{																																								//	begin
	int		i0, i1, i2, j0, j1, j2;																									//	indexing values
	real	det;																																		//	determinant of the sub-matrix_3d (the cofactor)
	switch (i)																																		//	switch on the desired row
	{																																							//	begin
		case 0:	i0 = 1; i1 = 2; i2 = 3; break;																			//	set appropriate indexing values
		case 1:	i0 = 0; i1 = 2; i2 = 3; break;																			//	set appropriate indexing values
		case 2:	i0 = 0; i1 = 1; i2 = 3; break;																			//	set appropriate indexing values
		case 3:	i0 = 0; i1 = 1; i2 = 2; break;																			//	set appropriate indexing values
	}																																							//	end
	switch (j)																																		//	switch on the desired column
	{																																							//	begin
		case 0:	j0 = 1; j1 = 2; j2 = 3; break;																			//	set appropriate indexing values
		case 1:	j0 = 0; j1 = 2; j2 = 3; break;																			//	set appropriate indexing values
		case 2:	j0 = 0; j1 = 1; j2 = 3; break;																			//	set appropriate indexing values
		case 3:	j0 = 0; j1 = 1; j2 = 2; break;																			//	set appropriate indexing values
	}																																							//	end
	//	compute the determinent of the resulting 3x3 matrix_3d
	det =	mat[i0][j0] * (mat[i1][j1] * mat[i2][j2] - mat[i1][j2] * mat[i2][j1]) -
				mat[i0][j1] * (mat[i1][j0] * mat[i2][j2] - mat[i1][j2] * mat[i2][j0]) +
				mat[i0][j2] * (mat[i1][j0] * mat[i2][j1] - mat[i1][j1] * mat[i2][j0]);
	return odd (i + j) ? -det : det; 																							//	if the entry index is odd, the cofactor is inverted
}																																								//	end

//------------------------------------------------------------------------------
//	determinant of the 4x4 matrix_3d
//------------------------------------------------------------------------------
real	matrix_3d::Determinant (void) const																				//	compute the matrix_3d determinant
{																																								//	begin
	return 	mat[0][0] * Cofactor (0, 0) +																					//	multiply the value by its cofactor
					mat[0][1] * Cofactor (0, 1) +																					//	multiply the value by its cofactor and add the result to the current sum
					mat[0][2] * Cofactor (0, 2) +																					//	multiply the value by its cofactor and add the result to the current sum
					mat[0][3] * Cofactor (0, 3);																					//	multiply the value by its cofactor and add the result to the current sum
}																																								//	end

//------------------------------------------------------------------------------
//	invert the matrix_3d
//------------------------------------------------------------------------------
matrix_3d	matrix_3d::Inverse (void) const																				//	compute the matrix_3d inverse
{																																								//	begin
	matrix_3d	inverse;																														//	new matrix_3d
	real	det = Determinant ();																										//	compute the determinant
	for (short i = 0; i < 4; i++)																									//	loop on the rows
		for (short j = 0; j < 4; j++)																								//	loop on the columns
			inverse.mat[j][i] = Cofactor (i, j) / det;																//	compute the inverse value for the current entry
	return inverse;																																//	return the matrix_3d
}																																								//	end

//------------------------------------------------------------------------------
//	pre-multiplication by a tuple_3d
//------------------------------------------------------------------------------
tuple_3d	operator * (const tuple_3d &t, const matrix_3d &m)										//	multiplication operator
{																																								//	begin
	tuple_3d	c0 (m(0, 0), m(1, 0), m(2, 0), m(3, 0)),														//	build the vector_3d for the first column
				c1 (m(0, 1), m(1, 1), m(2, 1), m(3, 1)),																//	build the vector_3d for the second column
				c2 (m(0, 2), m(1, 2), m(2, 2), m(3, 2)),																//	build the vector_3d for the third column
				c3 (m(0, 3), m(1, 3), m(2, 3), m(3, 3));																//	build the vector_3d for the fourth column
	return tuple_3d (t | c0, t | c1, t | c2, t | c3);															//	return tuple_3d composed of inner products
}																																								//	end

//------------------------------------------------------------------------------
//	post-multiplication by a tuple_3d
//------------------------------------------------------------------------------
tuple_3d	operator * (const matrix_3d &m, const tuple_3d &t)										//	multiplication operator
{																																								//	begin
	tuple_3d	r0 (m(0, 0), m(0, 1), m(0, 2), m(0, 3)),														//	build the vector_3d for the first column
				r1 (m(1, 0), m(1, 1), m(1, 2), m(1, 3)),																//	build the vector_3d for the second column
				r2 (m(2, 0), m(2, 1), m(2, 2), m(2, 3)),																//	build the vector_3d for the third column
				r3 (m(3, 0), m(3, 1), m(3, 2), m(3, 3));																//	build the vector_3d for the fourth column
	return tuple_3d (t | r0, t | r1, t | r2, t | r3);															//	return tuple_3d composed of inner products
}																																								//	end

//------------------------------------------------------------------
