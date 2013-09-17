#ifndef	_VECTOR_H_
#define	_VECTOR_H_

#ifndef _TUPLE_BUILDER_H_
#include "tuple_builder.h"
#endif

//------------------------------------------------------------------
namespace Space
{
	//--------------------------------------------------------------
	// Vector operator names
	//--------------------------------------------------------------
	#define	dot		|
	#define	cross	^

	//--------------------------------------------------------------
	// Vector component names
	//--------------------------------------------------------------
	enum {X = 0, Y = 1, Z = 2, W = 3};
	enum {S = 0, T = 1};
	enum {U = 0, V = 1};

	//--------------------------------------------------------------
	// forward template class definition
	//--------------------------------------------------------------
	template<unsigned int dimension>
	class Point;

	//--------------------------------------------------------------
	// template class definition
	//--------------------------------------------------------------
	template<unsigned int dimension>
	class Vector
	{
		friend class Point<dimension>;

		private:
					Tuple<dimension>			my_tuple;

		public:
					/* void */					Vector (void) {}
					/* void */					Vector (const Vector<dimension>& vector);

					Vector<dimension>&			operator = (const Vector<dimension>& vector);
					Tuple_Builder<dimension>	operator = (float first);

					Vector<dimension>			operator - (void) const;

					Vector<dimension>			operator + (const Vector<dimension>& vector) const;
					Vector<dimension>			operator - (const Vector<dimension>& vector) const;
					Vector<dimension>			operator * (float scalar) const;
					Vector<dimension>			operator / (float scalar) const;

					Vector<dimension>&			operator += (const Vector<dimension>& vector);
					Vector<dimension>&			operator -= (const Vector<dimension>& vector);
					Vector<dimension>&			operator *= (float scalar);
					Vector<dimension>&			operator /= (float scalar);

					float						operator dot (const Vector<dimension>& vector) const;

					float						Get_Length_Squared (void) const;
					float						Get_Length (void) const;

					Vector<dimension>			Normalized (void) const;
					Vector<dimension>&			Normalize (void);

					float						operator [] (unsigned int component) const;
					float&						operator [] (unsigned int component);
	};

	//--------------------------------------------------------------
	// Cross product valid only in 3 dimensional space
	//--------------------------------------------------------------
	Vector<3>	operator cross (const Vector<3>& left, const Vector<3>& right)
	{
		Vector<3>	result;
		result = (left[Y] * right[Z]) - (left[Z] * right[Y]),
				 (left[Z] * right[X]) - (left[X] * right[Z]),
				 (left[X] * right[Y]) - (left[Y] * right[X]);
		return result;
	}

	//--------------------------------------------------------------
	// template class implementation
	//--------------------------------------------------------------
	template<unsigned int dimension>
	/* void */	Vector<dimension>::Vector (const Vector<dimension>& vector)
	{
		my_tuple = vector.my_tuple;
	}

	//--------------------------------------------------------------
	template<unsigned int dimension>
	Vector<dimension>&		Vector<dimension>::operator = (const Vector<dimension>& vector)
	{
		my_tuple = vector.my_tuple;
		return *this;
	}

	//--------------------------------------------------------------
	template<unsigned int dimension>
	Tuple_Builder<dimension>	Vector<dimension>::operator = (float first)
	{
		return Tuple_Builder<dimension> (my_tuple, first);
	}

	//--------------------------------------------------------------
	template<unsigned int dimension>
	Vector<dimension>		Vector<dimension>::operator - (void) const
	{
		Vector<dimension>	result;
		result = 0.0f;
		Tuple<dimension>::Sub (result.my_tuple, my_tuple, result.my_tuple);
		return result;
	}

	//--------------------------------------------------------------
	template<unsigned int dimension>
	Vector<dimension>		Vector<dimension>::operator + (const Vector<dimension>& vector) const
	{
		Vector<dimension>	result;
		Tuple<dimension>::Add (my_tuple, vector.my_tuple, result.my_tuple);
		return result;
	}

	//--------------------------------------------------------------
	template<unsigned int dimension>
	Vector<dimension>		Vector<dimension>::operator - (const Vector<dimension>& vector) const
	{
		Vector<dimension>	result;
		Tuple<dimension>::Sub (my_tuple, vector.my_tuple, result.my_tuple);
		return result;
	}

	//--------------------------------------------------------------
	template<unsigned int dimension>
	Vector<dimension>		Vector<dimension>::operator * (float scalar) const
	{
		Vector<dimension>	result;
		Tuple<dimension>::Mul (my_tuple, scalar, result.my_tuple);
		return result;
	}

	//--------------------------------------------------------------
	template<unsigned int dimension>
	Vector<dimension>	operator * (float scalar, const Vector<dimension>& vector)
	{
		return vector * scalar;
	}

	//--------------------------------------------------------------
	template<unsigned int dimension>
	Vector<dimension>		Vector<dimension>::operator / (float scalar) const
	{
		Vector<dimension>	result;
		Tuple<dimension>::Div (my_tuple, scalar, result.my_tuple);
		return result;
	}

	//--------------------------------------------------------------
	template<unsigned int dimension>
	Vector<dimension>&		Vector<dimension>::operator += (const Vector<dimension>& vector)
	{
		Tuple<dimension>::Add (my_tuple, vector.my_tuple, my_tuple);
		return *this;
	}

	//--------------------------------------------------------------
	template<unsigned int dimension>
	Vector<dimension>&		Vector<dimension>::operator -= (const Vector<dimension>& vector)
	{
		Tuple<dimension>::Sub (my_tuple, vector.my_tuple, my_tuple);
		return *this;
	}

	//--------------------------------------------------------------
	template<unsigned int dimension>
	Vector<dimension>&		Vector<dimension>::operator *= (float scalar)
	{
		Tuple<dimension>::Mul (my_tuple, scalar, my_tuple);
		return *this;
	}

	//--------------------------------------------------------------
	template<unsigned int dimension>
	Vector<dimension>&		Vector<dimension>::operator /= (float scalar)
	{
		Tuple<dimension>::Div (my_tuple, scalar, my_tuple);
		return *this;
	}

	//--------------------------------------------------------------
	template<unsigned int dimension>
	float		Vector<dimension>::operator dot (const Vector<dimension>& vector) const
	{
		return Tuple<dimension>::Dot (my_tuple, vector.my_tuple);
	}

	//--------------------------------------------------------------
	template<unsigned int dimension>
	float		Vector<dimension>::Get_Length_Squared (void) const
	{
		return Tuple<dimension>::Dot (my_tuple, my_tuple);
	}

	//--------------------------------------------------------------
	template<unsigned int dimension>
	float		Vector<dimension>::Get_Length (void) const
	{
		float	length_squared = Get_Length_Squared ();
		if (length_squared > 0.0f)
			return sqrtf (length_squared);
		return 0.0f;
	}

	//--------------------------------------------------------------
	template<unsigned int dimension>
	Vector<dimension>		Vector<dimension>::Normalized (void) const
	{
		float	length = Get_Length ();
		assert (length > 0.0f);
		Vector<dimension>	result;
		Tuple::Div (my_tuple, length, result);
		return	result;
	}

	//--------------------------------------------------------------
	template<unsigned int dimension>
	Vector<dimension>&		Vector<dimension>::Normalize (void)
	{
		float	length = Get_Length ();
		assert (length > 0.0f);
		Tuple::Div (my_tuple, length, my_tuple);
		return *this;
	}

	//--------------------------------------------------------------
	template<unsigned int dimension>
	float		Vector<dimension>::operator [] (unsigned int component) const
	{
		return my_tuple[component];
	}

	//--------------------------------------------------------------
	template<unsigned int dimension>
	float&		Vector<dimension>::operator [] (unsigned int component)
	{
		return my_tuple[component];
	}

	//--------------------------------------------------------------
};

//------------------------------------------------------------------

#endif	//_VECTOR_H_
