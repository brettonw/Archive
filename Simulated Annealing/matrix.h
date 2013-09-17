#ifndef	_MATRIX_H_
#define	_MATRIX_H_

#ifndef _TUPLE_BUILDER_H_
#include "tuple_builder.h"
#endif

//------------------------------------------------------------------
namespace Space
{
	//--------------------------------------------------------------
	// template class definition
	//--------------------------------------------------------------
	template<unsigned int template_rows, unsigned int template_columns>
	class Matrix
	{
		private:
					float							my_mat[template_rows][template_columns];

		public:
					/* void */						Vector (void) {}
					/* void */						Vector (const Vector& vector);

					Vector&							operator = (const Vector& vector);
					Tuple_Builder<template_size>	operator = (float first);

					Vector							operator - (void) const;

					Vector							operator + (const Vector& vector) const;
					Vector							operator - (const Vector& vector) const;
					Vector							operator * (float scalar) const;
					Vector							operator / (float scalar) const;

					Vector&							operator += (const Vector& vector);
					Vector&							operator -= (const Vector& vector);
					Vector&							operator *= (float scalar);
					Vector&							operator /= (float scalar);

					float							operator dot (const Vector& vector) const;

					float							Get_Length_Squared (void) const;
					float							Get_Length (void) const;

					Vector							Normalized (void) const;
					Vector&							Normalize (void);

					float							operator [] (unsigned int component) const;
					float&							operator [] (unsigned int component);
	};

	//--------------------------------------------------------------
	// Cross product valid only in 3 dimensional space
	//--------------------------------------------------------------
	Vector<3>	operator cross (const Vector<3>& left, const Vector<3>& right)
	{
		// XXX not yet implemented
		return left;
	}

	//--------------------------------------------------------------
	// template class implementation
	//--------------------------------------------------------------
	template<unsigned int template_size>
	/* void */	Vector<template_size>::Vector (const Vector<template_size>& vector)
	{
		my_tuple = vector.my_tuple;
	}

	//--------------------------------------------------------------
	template<unsigned int template_size>
	Vector<template_size>&		Vector<template_size>::operator = (const Vector<template_size>& vector)
	{
		Vector<template_size>::Vector (vector);
		return *this;
	}

	//--------------------------------------------------------------
	template<unsigned int template_size>
	Tuple_Builder<template_size>	Vector<template_size>::operator = (float first)
	{
		return Tuple_Builder<template_size> (my_tuple, first);
	}

	//--------------------------------------------------------------
	template<unsigned int template_size>
	Vector<template_size>		Vector<template_size>::operator - (void) const
	{
		Vector	result;
		result = 0.0f;
		Tuple<template_size>::Sub (result, my_tuple, result);
		return result;
	}

	//--------------------------------------------------------------
	template<unsigned int template_size>
	Vector<template_size>		Vector<template_size>::operator + (const Vector<template_size>& vector) const
	{
		Vector	result;
		Tuple<template_size>::Add (my_tuple, vector.my_tuple, result);
		return result;
	}

	//--------------------------------------------------------------
	template<unsigned int template_size>
	Vector<template_size>		Vector<template_size>::operator - (const Vector<template_size>& vector) const
	{
		Vector	result;
		Tuple<template_size>::Sub (my_tuple, vector.my_tuple, result);
		return result;
	}

	//--------------------------------------------------------------
	template<unsigned int template_size>
	Vector<template_size>		Vector<template_size>::operator * (float scalar) const
	{
		Vector	result;
		Tuple<template_size>::Mul (my_tuple, scalar, result);
		return result;
	}

	//--------------------------------------------------------------
	template<unsigned int template_size>
	Vector<template_size>	operator * (float scalar, const Vector<template_size>& vector)
	{
		return vector * scalar;
	}

	//--------------------------------------------------------------
	template<unsigned int template_size>
	Vector<template_size>		Vector<template_size>::operator / (float scalar) const
	{
		Vector	result;
		Tuple<template_size>::Div (my_tuple, scalar, result);
		return result;
	}

	//--------------------------------------------------------------
	template<unsigned int template_size>
	Vector<template_size>&		Vector<template_size>::operator += (const Vector<template_size>& vector)
	{
		Tuple<template_size>::Add (my_tuple, vector.my_tuple, my_tuple);
		return *this;
	}

	//--------------------------------------------------------------
	template<unsigned int template_size>
	Vector<template_size>&		Vector<template_size>::operator -= (const Vector<template_size>& vector)
	{
		Tuple<template_size>::Sub (my_tuple, vector.my_tuple, my_tuple);
		return *this;
	}

	//--------------------------------------------------------------
	template<unsigned int template_size>
	Vector<template_size>&		Vector<template_size>::operator *= (float scalar)
	{
		Tuple<template_size>::Mul (my_tuple, scalar, my_tuple);
		return *this;
	}

	//--------------------------------------------------------------
	template<unsigned int template_size>
	Vector<template_size>&		Vector<template_size>::operator /= (float scalar)
	{
		Tuple<template_size>::Div (my_tuple, scalar, my_tuple);
		return *this;
	}

	//--------------------------------------------------------------
	template<unsigned int template_size>
	float		Vector<template_size>::operator dot (const Vector<template_size>& vector) const
	{
		return Tuple<template_size>::Dot (my_tuple, vector.my_tuple);
	}

	//--------------------------------------------------------------
	template<unsigned int template_size>
	float		Vector<template_size>::Get_Length_Squared (void) const
	{
		return Tuple<template_size>::Dot (my_tuple, my_tuple);
	}

	//--------------------------------------------------------------
	template<unsigned int template_size>
	float		Vector<template_size>::Get_Length (void) const
	{
		float	length_squared = Get_Length_Squared ();
		if (length_squared > 0.0f)
			return sqrtf (length_squared);
		return 0.0f;
	}

	//--------------------------------------------------------------
	template<unsigned int template_size>
	Vector<template_size>		Vector<template_size>::Normalized (void) const
	{
		float	length = Get_Length ();
		assert (length > 0.0f);
		Vector	result;
		Tuple::Div (my_tuple, length, result);
		return	result;
	}

	//--------------------------------------------------------------
	template<unsigned int template_size>
	Vector<template_size>&		Vector<template_size>::Normalize (void)
	{
		float	length = Get_Length ();
		assert (length > 0.0f);
		Tuple::Div (my_tuple, length, my_tuple);
		return *this;
	}

	//--------------------------------------------------------------
	template<unsigned int template_size>
	float		Vector<template_size>::operator [] (unsigned int component) const
	{
		return my_tuple[component];
	}

	//--------------------------------------------------------------
	template<unsigned int template_size>
	float&		Vector<template_size>::operator [] (unsigned int component)
	{
		return my_tuple[component];
	}

	//--------------------------------------------------------------
};

//------------------------------------------------------------------

#endif	//_MATRIX_H_
