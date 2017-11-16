//------------------------------------------------------------------------------
//	File:					matrix_3d.h
//	Date:					8/29/94
//	Author:				Bretton Wade
//
//	Description:	this file contains class declaration for a matrix_3d
//
//------------------------------------------------------------------------------

#include "tuple_3d.h"

#ifndef	MATRIX_3D
#define	MATRIX_3D

//------------------------------------------------------------------------------
//	classes
//------------------------------------------------------------------------------
class	matrix_3d																																	//	matrix_3d class
{																																								//	begin matrix_3d class definition
	private:																																			//	private interface
	protected:																																		//	protected interface
		real			mat[4][4];																												//	array representing the matrix_3d
	public:																																				//	public interface
static	real	identity[4][4];																										//	array used for constructing identity matrices
		matrix_3d (void) {}																													//	default constructor
		matrix_3d (const matrix_3d& m);																							//	copy constructor
		matrix_3d (real f[4][4]);																										//	direct assignment constructor
		void			operator = (const matrix_3d& m);																	//	assignment operator
		real			&operator () (int r, int c);																			//	function call operator
const	real		&operator () (int r, int c) const;																//	function call operator
		matrix_3d	operator * (const matrix_3d& m) const;														//	multiplication operator
		real			Cofactor (int r, int c) const;																		//	compute the cofactor for the given entry
		real			Determinant (void) const;																					//	compute the matrix_3d determinant
		matrix_3d	Inverse (void) const;																							//	compute the matrix_3d inverse
};																																							//	end matrix_3d class definition

//------------------------------------------------------------------------------
//	inlines
//------------------------------------------------------------------------------
inline	real	&matrix_3d::operator () (int r, int c)														//	function call operator
{																																								//	begin
	return (mat[r][c]);																														//	return the appropriate indexed real
}																																								//	end

inline	const	real	&matrix_3d::operator () (int r, int c) const								//	function call operator
{																																								//	begin
	return (mat[r][c]);																														//	return the appropriate indexed real
}																																								//	end

//------------------------------------------------------------------------------
//	operator functions
//------------------------------------------------------------------------------
tuple_3d	operator * (const tuple_3d& t, const matrix_3d& m);										//	pre-multiplication operator with a row vector_3d
tuple_3d	operator * (const matrix_3d& m, const tuple_3d& t);										//	post-multiplication operator with a column vector_3d

//------------------------------------------------------------------------------
//	global variables
//------------------------------------------------------------------------------
extern	matrix_3d	IDENTITY_MATRIX;																							//	an identity matrix_3d

//------------------------------------------------------------------------------

#endif	//MATRIX_3D
